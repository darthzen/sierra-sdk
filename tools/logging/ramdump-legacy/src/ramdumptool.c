/*************
 *
 * Filename:    RamDumptool.c
 *
 * Purpose:     RAM Dump tool
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#define _SVID_SOURCE
#define _XOPEN_SOURCE 500
#define _XOPEN_SOURCE_EXTENDED

#include "common.h"
#include <time.h>
#include <syslog.h>
#include <unistd.h>
#include <pthread.h>

/****************************************************************
*                       DEFINES
****************************************************************/
#define OPTION_LEN                      5
#define ENTER_KEY                       0x0A
#define TIMER_2_MILLISEC                1000 * 2
#define TIMER_4_MILLISEC                1000 * 4

/* Serial Buffer Size */
#define MAX_READ_BUFFER_LEN             4096
#define MAX_WRITE_BUFFER_LEN            1024

/* Commands */
#define CMD_ACK                         0x02
#define CMD_NAK                         0x03
#define CMD_PREQ                        0x07
#define CMD_PARAMS                      0x08
#define CMD_RESET                       0x0A
#define CMD_MEM_DEBUG_QUERY             0x10
#define CMD_MEM_DEBUG_INFO              0x11
#define CMD_MEM_READ_REQ                0x12
#define CMD_MEM_READ_RESP               0x13
#define CMD_MEM_UNFRAMED_READ_REQ       0x14
#define CMD_MEM_UNFRAMED_READ_RESP      0x15

/* Memory Info */
#define MAX_MEM_REGIONS                 12
#define MAX_REGION_DESCR_LEN            100
#define MAX_FILENAME_LEN                25

#define MAX_CONSECUTIVE_NAKS            10

#define MAX_MEM_READ_LEN                0x07F8
#define MAX_MEM_READ_PKT_LEN            0x0B

#define MAX_MEM_UNFRAMED_READ_LEN       0x0FF0
#define MAX_MEM_UNFRAMED_READ_PKT_LEN   0x10

/****************************************************************
*                       DATA STRUCTURE
****************************************************************/
/* Ram Dump tool state machine */
enum eRamDumpState
{
    STATE_DEVICE_DETECTION,
    STATE_GET_PARAMS,
    STATE_GET_REGIONS,
    STATE_GET_CHUNK,
    STATE_CLEANUP,
    STATE_MAX
};

/* Application Error enumeration */
enum eRamDumpStatus
{
    eRAM_DUMP_STATUS_OK,
    eRAM_DUMP_STATUS_FAIL_GENERIC,
    eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND,
    eRAM_DUMP_STATUS_FAIL_INV_PARAM,
    eRAM_DUMP_STATUS_FAIL_INV_STATE,
    eRAM_DUMP_STATUS_FAIL_CRC,
    eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM,
    eRAM_DUMP_STATUS_FAIL_NO_RESPONSE,
    eRAM_DUMP_STATUS_FAIL_RETRIES,
    eRAM_DUMP_STATUS_STATUS_MAX
};

/* Serial Communication Information */
typedef struct
{
    uint8  read[MAX_READ_BUFFER_LEN];
    uint8  write[MAX_WRITE_BUFFER_LEN];
    uint16 readLen;
    uint16 writeLen;
} SerialReadWriteInfo_s;

/* Memory region Information */
typedef struct
{
    uint8  savePref;
    uint8  description[MAX_REGION_DESCR_LEN];
    uint8  fileName[MAX_FILENAME_LEN];
    uint32 baseAddress;
    uint32 length;
    uint32 nextRead;
} MemoryRegionInfo_s;

/* Memory Dump Information */
typedef struct
{
    uint8  currMemRegion;
    uint8  maxMemRegion;
    uint8  protocolVer;
    uint8  protocolMin;
    uint16 maxWriteSize;
    uint32 totalMemorySize;
    uint32 totalMemoryRead;
    MemoryRegionInfo_s MemRegions[MAX_MEM_REGIONS];
} MemoryDumpInfo_s;

/* RAM Dump status */
typedef struct
{
    enum eRamDumpState state;
    uint8              progress;
    union
    {
      MemoryDumpInfo_s MemDumpInfo;
    } data;
} RamDumpStatus_s;

/****************************************************************
*                    GLOBAL DATA
****************************************************************/
/* Canned Messages */
static const uint8 CMD_NOOP_PKT[]           = { 0x7E, 0x06, 0x4E, 0x95, 0x7E };
static const uint8 CMD_PREQ_PKT[]           = { 0x7E, 0x07, 0xC7, 0x84, 0x7E };
static const uint8 CMD_MEMDEBUG_QUERY_PKT[] = { 0x7E, 0x10, 0xF9, 0xE0, 0x7E };
static const uint8 CMD_RESET_PKT[]          = { 0x7E, 0x0A, 0x22, 0x5F, 0x7E };

static int                   RAMDumpCaptured = FALSE;
static SerialReadWriteInfo_s sSerialInfo;
static MemoryDumpInfo_s      MemDumpInfo;
static RamDumpStatus_s       sRamDumpProgress;
static int                   bForceSlow = FALSE;

/****************************************************************
*                       FUNCTIONS
****************************************************************/
/*
 * Name:     DoProgressUpdate
 *
 * Purpose:  This function updated the RAM Dump capturing status and
 *           information received from the modem.
 *
 * Parms:    pRamDumpProgress [IN] - Pointer to structure containing RAM Dump
 *                                   tool progress.
 *
 * Return:   None
 *
 * Notes:    None
 */
void DoProgressUpdate( RamDumpStatus_s *pRamDumpProgress )
{
    static int updateOnlyOnce = TRUE;
    int        idx = 0;

    switch( pRamDumpProgress->state )
    {
        case STATE_DEVICE_DETECTION:
            break;

        case STATE_GET_PARAMS:
            fprintf( stderr, "Querying device parameters...\n");
            break;

        case STATE_GET_REGIONS:
            fprintf( stderr, "Querying Memory Debug Info...\n");
            for( idx = 0;
                 idx < pRamDumpProgress->data.MemDumpInfo.maxMemRegion;
                 idx++)
            {
                fprintf( stderr, "%d: %s [Base:0x%lx, Length:0x%lx]\n",
                idx + 1,
                pRamDumpProgress->data.MemDumpInfo.MemRegions[idx].description,
                pRamDumpProgress->data.MemDumpInfo.MemRegions[idx].baseAddress,
                pRamDumpProgress->data.MemDumpInfo.MemRegions[idx].length );
            }
            break;

        case STATE_GET_CHUNK:
            if( TRUE == updateOnlyOnce )
            {
                fprintf( stderr, "Reading memory...\n" );
                updateOnlyOnce = FALSE;
            }
            fprintf( stderr, "\r" );
            fprintf( stderr, "%d%% Complete", pRamDumpProgress->progress );
            break;

        case STATE_CLEANUP:
            fprintf( stderr, "\r" );
            fprintf( stderr, "100%% Complete\n" );
            break;

        default:
            break;
    }
}

/*
 * Name:     EscapePkt
 *
 * Purpose:  Add Escape characters where needed to an outgoing packet
 *
 * Parms:    None
 *
 * Return:   TRUE/FALSE - False indicates a major failure
 *
 * Notes:    Assumes that sSerialInfo.write and sSerialInfo.writeLen are
 *           set correctly
 */
int EscapePkt()
{
    uint16 idx1 = 0, idx2 = 0;

    /* Always skip the first and last characters */
    for( idx1 = 1; idx1 < sSerialInfo.writeLen - 1; idx1++ )
    {
        if( ( sSerialInfo.write[idx1] == ASYNC_HDLC_FLAG ) ||
            ( sSerialInfo.write[idx1] == ASYNC_HDLC_ESC ) )
       {
            for ( idx2 = sSerialInfo.writeLen; idx2 > idx1 + 1; idx2-- )
            {
                sSerialInfo.write[idx2] = sSerialInfo.write[idx2 - 1];
            }
            sSerialInfo.write[idx1 + 1] = sSerialInfo.write[idx1] ^
                                                           ASYNC_HDLC_ESC_MASK;
            sSerialInfo.write[idx1] = ASYNC_HDLC_ESC;
            sSerialInfo.writeLen++;
        }
    }
    return TRUE;
}

/*
 *
 * Name:     SendNextReadReq
 *
 * Purpose:  Send the next memory dump read request
 *
 * Parms:    None
 *
 * Return:   TRUE/FALSE - False indicates that no more reads are required
 *
 * Notes:    None
 */
int SendNextReadReq()
{
    uint32           endAddress;
    uint32           readLen;
    uint16           crc;
    MemoryRegionInfo_s *pMemRegion = NULL;

    pMemRegion = &MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion];

    /* Build the read request packet in the write buffer */
    sSerialInfo.write[0] = ASYNC_HDLC_FLAG;
    sSerialInfo.write[1] = CMD_MEM_READ_REQ;

    /* Start Address - 4 Bytes */
    sSerialInfo.write[2] = (uint8)( pMemRegion->nextRead >> 24 ) & 0xFF;
    sSerialInfo.write[3] = (uint8)( pMemRegion->nextRead >> 16 ) & 0xFF;
    sSerialInfo.write[4] = (uint8)( pMemRegion->nextRead >> 8 ) & 0xFF;
    sSerialInfo.write[5] = (uint8)( pMemRegion->nextRead & 0xFF );

    /* Read Size - 2 Bytes */
    endAddress = pMemRegion->length + pMemRegion->baseAddress;
    if ( MAX_MEM_READ_LEN < ( endAddress - pMemRegion->nextRead ) )
    {
        readLen = MAX_MEM_READ_LEN;
    }
    else
    {
        readLen = endAddress - pMemRegion->nextRead;
    }

    sSerialInfo.write[6] = (uint8)( readLen >> 8 ) & 0xFF;
    sSerialInfo.write[7] = (uint8)( readLen & 0xFF );

    /* Calculate the CRC before escaping the data */
    crc = dmcrc( sSerialInfo.write, MAX_MEM_READ_PKT_LEN );

    sSerialInfo.write[8] = (uint8)( crc & 0xFF );
    sSerialInfo.write[9] = (uint8)( ( crc >> 8 ) & 0xFF );
    sSerialInfo.write[10] = ASYNC_HDLC_FLAG;
    sSerialInfo.writeLen = 11;

    /* Escape the packet. Results are stored in sSerialInfo */
    EscapePkt();

    WriteOnDMPort( (char *)sSerialInfo.write, sSerialInfo.writeLen );
    return TRUE;
}

/*
 * Name:     SendNextUnframedReadReq
 *
 * Purpose:  Send the next memory dump unframed read request
 *
 * Parms:    None
 *
 * Return:   TRUE/FALSE - False indicates that no more reads are required
 *
 * Notes:    None
 */
int SendNextUnframedReadReq()
{
    uint32           endAddress;
    uint32           readLen;
    uint16           crc;
    MemoryRegionInfo_s *pMemRegion = NULL;

    pMemRegion = &MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion];

    /* Build the read request packet in the write buffer */
    sSerialInfo.write[0] = ASYNC_HDLC_FLAG;
    sSerialInfo.write[1] = CMD_MEM_UNFRAMED_READ_REQ;

    /* Padding - QCT uses these values, Using these values helped double check
     * CRC values, since they match QCT
     */
    sSerialInfo.write[2] = 0x12;
    sSerialInfo.write[3] = 0x56;
    sSerialInfo.write[4] = 0x34;

    /* Start Address - 4 Bytes */
    sSerialInfo.write[5] = (uint8)( pMemRegion->nextRead >> 24 ) & 0xFF;
    sSerialInfo.write[6] = (uint8)( pMemRegion->nextRead >> 16 ) & 0xFF;
    sSerialInfo.write[7] = (uint8)( pMemRegion->nextRead >> 8 ) & 0xFF;
    sSerialInfo.write[8] = (uint8)( pMemRegion->nextRead & 0xFF );

    /* Read Size - 2 Bytes */
    endAddress = pMemRegion->length + pMemRegion->baseAddress;
    if ( MAX_MEM_UNFRAMED_READ_LEN < ( endAddress - pMemRegion->nextRead ) )
    {
        readLen = MAX_MEM_UNFRAMED_READ_LEN;
    }
    else
    {
        readLen = endAddress - pMemRegion->nextRead;
    }

    sSerialInfo.write[9]  = (uint8)( readLen >> 24 ) & 0xFF;
    sSerialInfo.write[10] = (uint8)( readLen >> 16 ) & 0xFF;
    sSerialInfo.write[11] = (uint8)( readLen >> 8) & 0xFF;
    sSerialInfo.write[12] = (uint8)( readLen & 0xFF );

    /* Calculate the CRC before escaping the data */
    crc = dmcrc( sSerialInfo.write, MAX_MEM_UNFRAMED_READ_PKT_LEN );

    sSerialInfo.write[13] = (uint8)( crc & 0xFF );
    sSerialInfo.write[14] = (uint8)( ( crc >> 8 ) & 0xFF );
    sSerialInfo.write[15] = ASYNC_HDLC_FLAG;
    sSerialInfo.writeLen = 16;

    /* Escape the packet. Results are stored in sSerialInfo */
    EscapePkt();

    WriteOnDMPort( (char *)sSerialInfo.write, sSerialInfo.writeLen );
    return TRUE;
}

/*
 * Name:     ProcessCmdMemUnframedReadResp
 *
 * Purpose:  Process an unframed memory read response packet
 *
 * Parms:    None
 *
 * Return:   TRUE  - If response is successfully read and stored in log file.
 *           FALSE - Failed to read response or store it in log file.
 *
 * Notes:    None
 */
int ProcessCmdMemUnframedReadResp()
{
    uint32           endAddress = 0, pktAddress = 0;
    uint16           pktLen = 0, idx = 0;
    uint8            *pData = NULL;
    MemoryRegionInfo_s *pMemRegion = NULL;
    static FILE      *pFile;

    pMemRegion = &MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion];

    /* Determine the end address for this memory region */
    endAddress = pMemRegion->length + pMemRegion->baseAddress;

    /* Get the start address for this packet */
    pktAddress  = sSerialInfo.read[4] << 24;
    pktAddress |= sSerialInfo.read[5] << 16;
    pktAddress |= sSerialInfo.read[6] << 8;
    pktAddress |= sSerialInfo.read[7];

    pktLen  = sSerialInfo.read[8] << 24;
    pktLen |= sSerialInfo.read[9] << 16;
    pktLen |= sSerialInfo.read[10] << 8;
    pktLen |= sSerialInfo.read[11];


    /* Open the file, if this is the first read */
    if ( pktAddress == pMemRegion->baseAddress )
    {
        pFile = fopen( (const char*)pMemRegion->fileName, "wb" );
        if ( NULL == pFile )
        {
            fprintf( stderr, "\nFailed to open Log file : %s!!\n",
                             pMemRegion->fileName );
            return FALSE;
        }
    }

    /* Write the packet to the appropriate file */
    if ( NULL != pFile )
    {
        pData = &sSerialInfo.read[12];

        if ( MAX_READ_BUFFER_LEN < ( pktLen + 12 ) )
        {
            /* Bad data - try this one again */
            return FALSE;
        }
        for ( idx = 0; idx < pktLen; idx++ )
        {
            fputc( *pData++, pFile );
        }
        MemDumpInfo.totalMemoryRead += pktLen;
    }
    else
    {
        fprintf( stderr, "Output file %s is not opened!!\n",
                         pMemRegion->fileName );
        return FALSE;
    }

    /* Close the file if this is the last read */
    if ( ( pktAddress + pktLen ) >= endAddress )
    {
        fclose( pFile );
        pFile = NULL;

        /* Increment the memory region index, and reset the read pointer */
        MemDumpInfo.currMemRegion++;
        if ( MemDumpInfo.currMemRegion < MemDumpInfo.maxMemRegion )
        {
            MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion].nextRead =
               MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion].baseAddress;
        }
    }
    else
    {
        /* Increment the Read index */
        pMemRegion->nextRead += pktLen;
    }
    return TRUE;
}

/*
 * Name:     ProcessCmdMemReadResp
 *
 * Purpose:  Process a memory read response packet
 *
 * Parms:    None
 *
 * Return:   TRUE  - If response is successfully read and stored in log file.
 *           FALSE - Failed to read response or store it in log file.
 *
 * Notes:    None
 */
int ProcessCmdMemReadResp()
{
    uint32           endAddress = 0, pktAddress = 0;
    uint16           pktLen = 0, idx = 0;
    uint8            *pData = NULL;
    MemoryRegionInfo_s *pMemRegion = NULL;
    static FILE      *pFile;

    pMemRegion = &MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion];

    /* Determine the end address for this memory region */
    endAddress = pMemRegion->length + pMemRegion->baseAddress;

    /* Get the start address for this packet */
    pktAddress  = sSerialInfo.read[1] << 24;
    pktAddress |= sSerialInfo.read[2] << 16;
    pktAddress |= sSerialInfo.read[3] << 8;
    pktAddress |= sSerialInfo.read[4];

    pktLen  = sSerialInfo.read[5] << 8;
    pktLen |= sSerialInfo.read[6];

    /* Open the file, if this is the first read */
    if ( pktAddress == pMemRegion->baseAddress )
    {
        pFile = fopen( (const char*)pMemRegion->fileName, "wb" );
        if ( NULL == pFile )
        {
            fprintf( stderr, "\nFailed to open Log file : %s!!\n",
                             pMemRegion->fileName );
            return FALSE;
        }
    }

    /* Write the packet to the appropriate file */
    if ( NULL != pFile )
    {
        pData = &sSerialInfo.read[7];
        for ( idx=0; idx < pktLen ; idx++ )
        {
            fputc( *pData++, pFile );
        }
        MemDumpInfo.totalMemoryRead += pktLen;
    }
    else
    {
        fprintf( stderr, "Output file %s is not opened!!\n",
                         pMemRegion->fileName );
        return FALSE;
    }

    /* Close the file if this is the last read */
    if ( ( pktAddress + pktLen ) >= endAddress )
    {
        fclose( pFile );
        pFile = NULL;

        /* Increment the memory region index, and reset the read pointer */
        MemDumpInfo.currMemRegion++;
        if ( MemDumpInfo.currMemRegion < MemDumpInfo.maxMemRegion )
        {
            MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion].nextRead =
               MemDumpInfo.MemRegions[MemDumpInfo.currMemRegion].baseAddress;
        }
    }
    else
    {
        /* Increment the Read index */
        pMemRegion->nextRead += pktLen;
    }
    return TRUE;
}

/*
 * Name:     UnEscapePkt
 *
 * Purpose:  Remove Escape characters from a received packet
 *
 * Parms:    None
 *
 * Return:   None
 *
 * Notes:    Assumes that sSerialInfo.read and sSerialInfo.readLen are
 *           set correctly.
 */
void UnEscapePkt()
{
    uint16 len = 0;

    /* Download protocol begins with a HDLC End character, so start unescape
     * process on next byte and then add the byte back in the return length
     */
    len = dmunescape( &sSerialInfo.read[1], sSerialInfo.readLen - 1 );
    sSerialInfo.readLen = len + 1;
}

/*
 * Name:     RamDumpCmdAck
 *
 * Purpose:  Check whether the device is detected or not and update the state
 *           of RAM Dump tool accordingly.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdAck(
    enum eRamDumpState *pRamDumpState )
{
    if( STATE_DEVICE_DETECTION == *pRamDumpState )
    {
        /* Device detected. Get the memory parameters */
        *pRamDumpState = STATE_GET_PARAMS;
    }
    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     RamDumpCmdParams
 *
 * Purpose:  Extract the protocol information received from the modem and
 *           update the state of RAM Dump tool accordingly.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *           pCmdBuf       [IN]     - Pointer to the packet received from
 *                                    the modem
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdParams(
    enum eRamDumpState *pRamDumpState,
    uint8              *pCmdBuf )
{
    if( STATE_GET_PARAMS == *pRamDumpState )
    {
        /* Extract the important info */
        MemDumpInfo.protocolVer  = *pCmdBuf++;
        MemDumpInfo.protocolMin  = *pCmdBuf++;
        MemDumpInfo.maxWriteSize = *pCmdBuf;

        /* Transition to get the memory regions */
        *pRamDumpState = STATE_GET_REGIONS;
        MemDumpInfo.currMemRegion = 0;
        MemDumpInfo.maxMemRegion  = 0;
    }
    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     RamDumpCmdMemReadResp
 *
 * Purpose:  Process the memory read command's response from modem.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdMemReadResp(
    enum eRamDumpState *pRamDumpState )
{
    if( STATE_GET_CHUNK == *pRamDumpState )
    {
        if( FALSE == ProcessCmdMemReadResp() )
        {
            fprintf( stderr, "%s: Fatal error occured,exiting!\n", __func__ );
            return eRAM_DUMP_STATUS_FAIL_GENERIC;
        }

        if ( MemDumpInfo.currMemRegion >=
             MemDumpInfo.maxMemRegion )
        {
            /* We're Done! */
            *pRamDumpState = STATE_CLEANUP;
        }
    }
    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     RamDumpCmdMemUnframedReadResp
 *
 * Purpose:  Process the memory unframed read command's response from modem.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdMemUnframedReadResp(
    enum eRamDumpState *pRamDumpState )
{
    if( STATE_GET_CHUNK == *pRamDumpState )
    {
        if( FALSE == ProcessCmdMemUnframedReadResp() )
        {
            fprintf( stderr, "Fatal error occured,exiting!\n");
            return eRAM_DUMP_STATUS_FAIL_GENERIC;
        }

        if ( MemDumpInfo.currMemRegion >=
             MemDumpInfo.maxMemRegion )
        {
            /* We're Done! */
            *pRamDumpState = STATE_CLEANUP;
        }
    }

    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     RamDumpCmdMemDebugInfo
 *
 * Purpose:  Extract the memory debug information from the packet received from
 *           the modem.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *           pCmdBuf       [IN]     - Pointer to the packet received from
 *                                    the modem
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdMemDebugInfo(
    enum eRamDumpState *pRamDumpState,
    uint8              *pCmdBuf )
{
    uint8 *pCmdEnd = NULL;

    if( STATE_GET_REGIONS == *pRamDumpState )
    {
        /* Get memory info len */
        uint16 memInfoLen = (uint16)( ( *pCmdBuf++ << 8 ) & 0xFF00 );
        memInfoLen += ( (uint16)*pCmdBuf++ & 0xFF );
        pCmdEnd = pCmdBuf + memInfoLen;

        MemoryRegionInfo_s *pMemRegionInfo = &MemDumpInfo.MemRegions[0];

        /* For each memory region: */
        while (1)
        {
            pMemRegionInfo->savePref = *pCmdBuf++;
            pMemRegionInfo->baseAddress = *pCmdBuf++ << 24;
            pMemRegionInfo->baseAddress += *pCmdBuf++ << 16;
            pMemRegionInfo->baseAddress += *pCmdBuf++ << 8;
            pMemRegionInfo->baseAddress += *pCmdBuf++;

            pMemRegionInfo->length = *pCmdBuf++ << 24;
            pMemRegionInfo->length += *pCmdBuf++ << 16;
            pMemRegionInfo->length += *pCmdBuf++ << 8;
            pMemRegionInfo->length += *pCmdBuf++;

            uint16 idx = 0;
            while ( 0x00 != *pCmdBuf )
            {
                pMemRegionInfo->description[idx++] =*pCmdBuf++;
            }

            pCmdBuf++;
            idx = 0;
            while ( 0x00 != *pCmdBuf )
            {
                pMemRegionInfo->fileName[idx++] = *pCmdBuf++;
            }

            pCmdBuf++;
            pMemRegionInfo++;
            MemDumpInfo.maxMemRegion++;

            if ( pCmdBuf >= pCmdEnd )
            {
                break;
            }
        }

        /* Calculate total memory size to be logged to track
         * percentage completion.
         */
        int idx;
        for ( MemDumpInfo.totalMemorySize = 0, idx = 0;
              idx < MemDumpInfo.maxMemRegion;
              idx++)
        {
            MemDumpInfo.totalMemorySize += MemDumpInfo.
                                        MemRegions[idx].length;
        }

        /* Transition to the memory dump */
        MemDumpInfo.currMemRegion = 0;
        MemDumpInfo.MemRegions[0].nextRead =
                        MemDumpInfo.MemRegions[0].baseAddress;
        *pRamDumpState = STATE_GET_CHUNK;
        memcpy( (void *)&( sRamDumpProgress.data.MemDumpInfo ),
                (void *)&( MemDumpInfo ),
                sizeof( MemoryDumpInfo_s ) );
        DoProgressUpdate( ( void *)&sRamDumpProgress );
    }

    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     RamDumpCmdNak
 *
 * Purpose:  Process the NAK response received from the modem.
 *
 * Parms:    pTotalConsecutiveNaks [IN/OUT] - Total NAK so far received from
 *                                            the modem
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus RamDumpCmdNak(
    uint16 *pTotalConsecutiveNaks )
{
    ( *pTotalConsecutiveNaks )++;

    if ( MAX_CONSECUTIVE_NAKS < *pTotalConsecutiveNaks )
    {
        CloseDMPort();
        return eRAM_DUMP_STATUS_FAIL_RETRIES;
    }

    /* Try Again, first */
    WriteOnDMPort( (char *)sSerialInfo.write,
                   sSerialInfo.writeLen );

    return eRAM_DUMP_STATUS_OK;
}

/*
 * Name:     StartRamDump
 *
 * Purpose:  Starts the Ram Dump gathering protocol.
 *
 * Params:   dmport    - /dev/ttyUSBx device file to be opened for communication
 *                       with modem.
 *
 *           HSEnabled - Use High Speed protocol (TRUE/FALSE)
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 */
static enum eRamDumpStatus StartRamDump(
    const char *dmport,
    int HSEnabled )
{
    enum eRamDumpState  RamDumpState = STATE_DEVICE_DETECTION;
    enum eRamDumpStatus status = eRAM_DUMP_STATUS_OK;
    int                 sleepVal = 0;


    /* Open the port entered by the user */
    if( FALSE == OpenDMPort( dmport ) )
    {
        return eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND;
    }

    fprintf( stderr, "Initiating RAM Dump capture...\n" );

    /* Send the NO-OP command as a ping test to the device */
    WriteOnDMPort( (char *)CMD_NOOP_PKT, sizeof(CMD_NOOP_PKT) );

    /* Track the number of consecutive naks to look for protocol errors */
    uint16 totalConsecutiveNaks = 0;

    /* time tracking variables */
    time_t startTime, currTime, lastReadTime;
    time( &startTime );
    time( &lastReadTime );

    while(1)
    {

        /* Wait for a few millisecond. No timer exist for reading and writing to
         * Com port. Thus explicitly wait for few millisecond. Does not work
         * with less than 2 milliseconds sleep.
         */
        if( HSEnabled )
          sleepVal = TIMER_2_MILLISEC;
        else
          sleepVal = TIMER_4_MILLISEC;

        usleep( sleepVal );

        /* Get the response from the device */
        sSerialInfo.readLen = ReadFromDMPort( (char *)sSerialInfo.read,
                                              MAX_READ_BUFFER_LEN );

        /* If the data is received */
        if ( 0 != sSerialInfo.readLen )
        {
            time( &lastReadTime );

            /* If the first byte is the Async HDLC ESC character, the command
             * type is the next byte.
             */
            uint8 *pCmdBuf = NULL;

            pCmdBuf = ASYNC_HDLC_FLAG == sSerialInfo.read[0]
            ? &sSerialInfo.read[1]
            : &sSerialInfo.read[0];

            /* High speed memory debugging does not use HDLC framing */
            if ( CMD_MEM_UNFRAMED_READ_RESP != *pCmdBuf )
            {
                UnEscapePkt();

                /* First validate the packet */
                uint16 calculatedCRC = dmcrc( sSerialInfo.read, sSerialInfo.readLen );
                uint16 receivedCRC = sSerialInfo.read[sSerialInfo.readLen-3] +
                              ( sSerialInfo.read[sSerialInfo.readLen-2] << 8 );

                if ( calculatedCRC != receivedCRC )
                {
#ifdef RAMDUMP_DBG
                    fprintf( stderr, "Checksum Error, Type %d, Length %d\n",
                                    sSerialInfo.read[0], sSerialInfo.readLen );
                    fprintf( stderr, "calculatedCRC = 0x%x, ReceivedCrc = 0x%x\n",
                                    calculatedCRC, receivedCRC );
#endif

                    /* Try Again, first */
                    WriteOnDMPort( (char *)sSerialInfo.write,
                                   sSerialInfo.writeLen);
                    continue;
                }
            }

            if ( CMD_NAK != *pCmdBuf )
            {
                totalConsecutiveNaks = 0;
            }

            int rc;
            switch( *pCmdBuf++ )
            {
                case CMD_ACK:
                    (void)RamDumpCmdAck( &RamDumpState );
                    break;

                case CMD_PARAMS:
                    (void)RamDumpCmdParams( &RamDumpState, pCmdBuf );
                    break;

                case CMD_MEM_DEBUG_INFO:
                    (void)RamDumpCmdMemDebugInfo(&RamDumpState, pCmdBuf );
                    break;

                case CMD_MEM_READ_RESP:
                    if( eRAM_DUMP_STATUS_OK !=
                        ( rc = RamDumpCmdMemReadResp( &RamDumpState) ) )
                    {
                        return rc;
                    }
                    break;

                case CMD_MEM_UNFRAMED_READ_RESP:
                    if( eRAM_DUMP_STATUS_OK !=
                        ( rc = RamDumpCmdMemUnframedReadResp( &RamDumpState) ) )
                    {
                        return rc;
                    }
                    break;

                case CMD_NAK:
                default: /* Count unrecognized CMD responses as NAKS */
                    if( eRAM_DUMP_STATUS_OK !=
                        ( rc = RamDumpCmdNak( &totalConsecutiveNaks) ) )
                    {
                        return rc;
                    }
                    continue;
                    break;
            }
        }
        else
        {
            /* Every time we reach this point, it means a packet failed to
             * generate a response.
             */
            time( &currTime );

            /* Timeout after continued failure */
            int timeout = 5;
            if ( ( currTime - lastReadTime ) > timeout )
            {
                fprintf( stderr, "\nNo communication received for %d seconds."\
                                 " Aborting!!\n", timeout );

                return eRAM_DUMP_STATUS_FAIL_NO_RESPONSE;
            }
        }

        sRamDumpProgress.state = RamDumpState;

        /* Next Step */
        switch( RamDumpState )
        {
            case STATE_DEVICE_DETECTION:
                /* Every time we reach this point,it means a NOOP packet failed
                 * to generate a response.
                 */
                time( &currTime );

                /* Timeout after 10 seconds of failure */
                if ( ( currTime - startTime ) > 10 )
                {
                    return eRAM_DUMP_STATUS_FAIL_NO_RESPONSE;
                }

                /* Send the NO-OP command as a ping test to the device */
                WriteOnDMPort( (char *)CMD_NOOP_PKT, sizeof( CMD_NOOP_PKT ) );
                sRamDumpProgress.progress = 0;
                DoProgressUpdate( &sRamDumpProgress );
                break;

            case STATE_GET_PARAMS:
                /* Send the NO-OP command as a ping test to the device */
                WriteOnDMPort( (char *)CMD_PREQ_PKT, sizeof( CMD_PREQ_PKT ) );
                sRamDumpProgress.progress = 0;
                DoProgressUpdate( &sRamDumpProgress );
                break;

            case STATE_GET_REGIONS:
                WriteOnDMPort( (char *)CMD_MEMDEBUG_QUERY_PKT,
                               sizeof( CMD_MEMDEBUG_QUERY_PKT ) );
                sRamDumpProgress.progress = 0;
                MemDumpInfo.totalMemoryRead = 0;
                /* Progress Update will be sent after reading the memory info */
                break;

            case STATE_GET_CHUNK:
                if ( TRUE == HSEnabled )
                {
                    SendNextUnframedReadReq();
                }
                else
                {
                    SendNextReadReq();
                }
                sRamDumpProgress.progress =
                (uint8)( ( (MemDumpInfo.totalMemoryRead >> 8 ) * 100 ) /
                                       ( MemDumpInfo.totalMemorySize >> 8 ) );
                DoProgressUpdate( &sRamDumpProgress );
                break;

            case STATE_CLEANUP:
                CloseDMPort();
                sRamDumpProgress.progress = 100;
                DoProgressUpdate( &sRamDumpProgress );
                return eRAM_DUMP_STATUS_OK;

            default:
                status = eRAM_DUMP_STATUS_FAIL_INV_STATE;
                sRamDumpProgress.progress = 100;
                DoProgressUpdate( &sRamDumpProgress );
                break;
        }

        if ( eRAM_DUMP_STATUS_OK != status )
        {
            /* An unfixable error has occurred, abort the memory dump */
            break;
        }
    } /* While(1) */
    return status;
}

/*
 * Name:     RAMDumpThread
 *
 * Purpose:  RAM Dump Capturing Thread created by main.
 *
 * Params:   ptr [IN] - RAM Dump port
 *
 * Return:   None
 *
 * Notes:    None
 */
static void* RAMDumpThread( void *ptr )
{
    enum eRamDumpStatus status;
    const char *dmport = (const char *)ptr;

    /* Start Capturing the RAM Dump */
    if( !bForceSlow )
        status = StartRamDump( dmport, TRUE );

    /* First check if we should retry without High Speed protocol enabled */
    if ( eRAM_DUMP_STATUS_FAIL_RETRIES == status || bForceSlow )
    {
        /* Retry without using the HS protocol */
        status = StartRamDump( dmport, FALSE );
    }

    /* check RAM Dump status */
    switch ( status )
    {
        case eRAM_DUMP_STATUS_OK:
        /* We've completed the session */
        fprintf( stderr, "RAM Dump capture SUCCESS!!\n" );
        break;

        case eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND:
            /* Port not detected */
            fprintf( stderr, "Unable to find %s\n", dmport );
            break;

        case eRAM_DUMP_STATUS_FAIL_NO_RESPONSE:
            /* Port detected and opened, but no DM response */
            fprintf( stderr, "Modem not responding, please try again\n" );
            break;

        case eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM:
            /* Generic detected, but not available to be opened */
            fprintf( stderr, "Modem COM port(s) in use.  Shutdown all modem"\
                             "applications and try again.\n");
            break;

        case eRAM_DUMP_STATUS_FAIL_INV_PARAM:
        case eRAM_DUMP_STATUS_FAIL_INV_STATE:
        case eRAM_DUMP_STATUS_FAIL_CRC:
        case eRAM_DUMP_STATUS_FAIL_RETRIES:
            fprintf( stderr, "Protocol Error: %d\n", status );
            break;

        case eRAM_DUMP_STATUS_FAIL_GENERIC:
        default:
            fprintf( stderr, "Failed\n");
            break;
    }

    /* Close the serial port if remained opened in case an error is
     * encountered and mark the RAM Dump completion flag as TRUE.
     */
    CloseDMPort();
    RAMDumpCaptured = TRUE;
    return NULL;
}

/*
 * Name:     usage
 *
 * Purpose:  Prints the format for using RAM Dump tool from command line.
 *
 * Params:   pProgname [IN] - Name of the program to be printed
 *
 * Return:   None
 *
 * Notes:    None
 */
static void usage( const char *pProgname )
{
    fprintf( stderr, "%s: %s /dev/ttyUSB<digit> [-s]\n",
             __func__,
             pProgname );
    fprintf( stderr,
             "-s  slow mode operation( tool defaults to high-speed mode )\n" );
}

/*
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Params:   argc   - number of arguments
 *           argv   - argument string vector
 *
 * Return:   EXIT_SUCCESS on success, EXIT_FAILURE on failure
 *
 * Notes:    None
 */
int main( int argc, const char *argv[] )
{
    /* if port not provided, print usage */
    if( argc <= 1 )
    {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* Create the RAM Dump thread */
    pthread_t thread;
    int rc;
    if( ( 3 == argc ) && ( strcmp(argv[2], "-s") == 0 ) )
    {
        fprintf( stderr, "Starting in slow mode\n");
        bForceSlow = TRUE;
    }

    if( 0 != (rc = pthread_create( &thread, NULL, RAMDumpThread, (char *)argv[1] ) ) )
    {
        fprintf(stderr, "RAM Dump thread creation failed: %d\n", rc);
        exit( EXIT_FAILURE );
    }
    else
    {
        /* Wait for RAM Dump thread to terminate */
        if( 0 != (rc = pthread_join(thread, NULL) ) )
        {
            fprintf(stderr, "failed to join RAM Dump thread: %d\n", rc);
            exit( EXIT_FAILURE );
        }
    }

    fprintf( stderr, "Exit Application!\n");
    exit( EXIT_SUCCESS );
}
