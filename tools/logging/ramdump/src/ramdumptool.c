/************
 *
 * Filename:    RamDumptool.c
 *
 * Purpose:     RAM Dump tool
 *
 * Copyright: (c) 2011 Sierra Wireless Inc., all rights reserved
 *
 ************/
#include "common.h"
#ifdef RAMDUMP_WIN32
  #include "registry_windows.h"
#endif
#include <stdio.h>
#include <time.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <endian.h>

/************
 *                       DEFINES
 ************/
#ifndef VERSION_TAG
  #define VERSION_TAG   "version undefined"
#endif

#define MAX_COMPORT_LEN                 20
#define OPTION_LEN                      5
#define ENTER_KEY                       0x0A
#define TIMER_1_MILLISEC                1000
#define TIMER_2_MILLISEC                1000 * 2
#define TIMER_3_MILLISEC                1000 * 3
#define TIMER_4_MILLISEC                1000 * 4

/* Serial Buffer Size */
#define MAX_READ_BUFFER_LEN             4096
#define MAX_WRITE_BUFFER_LEN            1024

/* Commands */
#define DMSS_CMDACK                         0x02
#define DMSS_CMDNAK                         0x03
#define DMSS_CMDPREQ                        0x07
#define DMSS_CMDPARAMS                      0x08
#define DMSS_CMDRESET                       0x0A
#define DMSS_CMDMEM_DEBUG_QUERY             0x10
#define DMSS_CMDMEM_DEBUG_INFO              0x11
#define DMSS_CMDMEM_READ_REQ                0x12
#define DMSS_CMDMEM_READ_RESP               0x13
#define DMSS_CMDMEM_UNFRAMED_READ_REQ       0x14
#define DMSS_CMDMEM_UNFRAMED_READ_RESP      0x15

/* Memory Info */
#define MAX_MEM_REGIONS                 20
#define MAX_REGION_DESCR_LEN            100
#define MAX_FILENAME_LEN                25

#define MAX_CONSECUTIVE_NAKS            10

#define MAX_MEM_READ_LEN                0x07F8
#define MAX_MEM_READ_PKT_LEN            0x0B

#define MAX_MEM_UNFRAMED_READ_LEN       0x0FF0
#define MAX_MEM_UNFRAMED_READ_PKT_LEN   0x10

/* Define for Sahara protocol */
#define SAHARA_CURRENT_VERSION         2
#define SAHARA_COMPATIBLE_VERSION      1
#define SAHARA_MEM_REGION_NAME_SIZE    20 /* This is found by experiment */

/* In boot_images/core/bsp/bootloaders/sbl1/build/mdm9x35.scons
 * QCT defines SAHARA_MAX_MEMORY_DATA_SIZE_IN_BYTES = 0x1000
 *     and     SAHARA_MAX_PACKET_SIZE_IN_BYTES = 0x400
 */
#ifdef RAMDUMP_WIN32
  #define SAHARA_MAX_MEM_READ_LEN      0x1000
#else
  /* On Linux, requesting a 0x1000 read len somehow only receive 0xFFF */
  #define SAHARA_MAX_MEM_READ_LEN      0x0FF0
#endif

/* Define options for the command line */
#define PORT_OPT      "-c"
#define PATH_OPT      "-o"
#define NORESET_OPT   "-noreset"
#define HELP_OPT      "-h"
#define DEVICE_OPT    "-d"
#define SLOW_OPT      "-s"  /* Slow speed for DMSS protocol only */

#define DEVICE_9X30   "9x30"
#define DEVICE_9X15   "9x15"


#ifdef __PPC__
#include <stdint.h>
#define htole32(x) ({                   \
    uint32_t tmp = (x);                 \
                                        \
    (uint32_t)(                         \
        (tmp & 0xff) << 24 |            \
        (tmp & 0xff00) << 8 |           \
        (tmp & 0xff0000) >> 8 |         \
        (tmp & 0xff000000) >> 24);      \
})
#endif

#ifdef __ARM_EABI__
#warning "assume little endian on ARM, please update otherwise"
#define htole32(x) x
#endif

/************
 *                       DATA STRUCTURE
 ************/
enum eProtocol
{
  PROTOCOL_AUTO,                /* should be default when protocol detection
                                 * is supported */
  PROTOCOL_DMSS,
  PROTOCOL_SAHARA,
  PROTOCOL_MAX                  /* Indicates invalid protocol */
};

/* Ram Dump tool state machine */
enum eRamDumpState
{
  STATE_DMSS_DEVICE_DETECTION,
  STATE_DMSS_GET_PARAMS,
  STATE_DMSS_GET_REGIONS,
  STATE_DMSS_GET_CHUNK,
  STATE_DMSS_CLEANUP,

  STATE_SAHARA_WAIT_HELLO,
  STATE_SAHARA_WAIT_COMMAND,
  STATE_SAHARA_WAIT_MEMORY_TABLE,
  STATE_SAHARA_WAIT_MEMORY_REGION,
  STATE_SAHARA_CLEANUP,

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
  eRAM_DUMP_STATUS_FAIL_PORT_BUSY_CNS,
  eRAM_DUMP_STATUS_FAIL_NO_RESPONSE,
  eRAM_DUMP_STATUS_FAIL_RETRIES,
  eRAM_DUMP_STATUS_STATUS_MAX
};

/* Serial Communication Information */
typedef struct
{
  uint8 read[MAX_READ_BUFFER_LEN];
  uint8 write[MAX_WRITE_BUFFER_LEN];
  uint16 readLen;
  uint16 writeLen;
} SerialReadWriteInfo_s;

/* dmss Memory region Information */
typedef struct
{
  uint8 savePref;
  uint8 description[MAX_REGION_DESCR_LEN];
  uint8 fileName[MAX_FILENAME_LEN];
  uint32 baseAddress;
  uint32 length;
  uint32 nextRead;
} dmss_MemoryRegionInfo_s;

/* dmss Memory Dump Information */
typedef struct
{
  uint8 currMemRegion;
  uint8 maxMemRegion;
  uint32 protocolVer;
  uint32 protocolMin;
  uint32 maxWriteSize;
  uint32 totalMemorySize;
  uint32 totalMemoryRead;
  dmss_MemoryRegionInfo_s MemRegions[MAX_MEM_REGIONS];
} dmss_MemoryDumpInfo_s;

/* Define commands used in Sahara command packet */
typedef enum{
/* Command name                   Value       Sent by */
  SAHARA_CMD_INVALID             = 0x00,
  SAHARA_CMD_HELLO               = 0x01,    /* Target */
  SAHARA_CMD_HELLO_RESP          = 0x02,    /* Host */
  SAHARA_CMD_READ_DATA           = 0x03,    /* Target */
  SAHARA_CMD_END_TRANSFER_IMAGE  = 0x04,    /* Target */
  SAHARA_CMD_DONE                = 0x05,    /* Host */
  SAHARA_CMD_DONE_RESP           = 0x06,    /* Target */
  SAHARA_CMD_RESET               = 0x07,    /* Host */
  SAHARA_CMD_RESET_RESP          = 0x08,    /* Target */
  SAHARA_CMD_MEMORY_DEBUG        = 0x09,    /* Target */
  SAHARA_CMD_MEMORY_READ         = 0x0A,    /* Host */
  SAHARA_CMD_CMD_READY           = 0x0B,    /* Target */
  SAHARA_CMD_CMD_SWITCH          = 0x0C,    /* Host */
  SAHARA_CMD_CMD_EXECUTE         = 0x0D,    /* Host */
  SAHARA_CMD_CMD_EXECUTE_RESP    = 0x0E,    /* Target */
  SAHARA_CMD_CMD_EXECUTE_DATA    = 0x0F     /* Host */
} SaharaCommand_e;

enum
{
  LEN_SAHARA_CMD_HELLO                     = 0x30,
  LEN_SAHARA_CMD_HELLO_RESPONSE            = 0x30,
  LEN_SAHARA_CMD_READ_DATA                 = 0x14,
  LEN_SAHARA_CMD_END_IMAGE_TRANSFER        = 0x10,
  LEN_SAHARA_CMD_DONE                      = 0x08,
  LEN_SAHARA_CMD_DONE_RESPONSE             = 0x0C,
  LEN_SAHARA_CMD_RESET                     = 0x08,
  LEN_SAHARA_CMD_RESET_RESPONSE            = 0x08,
  LEN_SAHARA_CMD_MEMORY_DEBUG              = 0x10,
  LEN_SAHARA_CMD_MEMORY_READ               = 0x10,
  LEN_SAHARA_CMD_COMMAND_READY             = 0x08,
  LEN_SAHARA_CMD_COMMAND_SWITCH_MODE       = 0x0C,
  LEN_SAHARA_CMD_COMMAND_EXECUTE           = 0x0C,
  LEN_SAHARA_CMD_COMMAND_EXECUTE_RESPONSE  = 0x10,
  LEN_SAHARA_CMD_COMMAND_EXECUTE_DATA      = 0x0C
};

#define SAHARA_MODE_MEMORY_DEBUG      0x02

/* Structures defined for command packet */
typedef struct _PACKED hello_pkt_s
{
  uint32 command;
  uint32 len;
  uint32 version;
  uint32 version_compatible;
  uint32 cmd_pkt_len;
  uint32 mode;
  uint32 reserved1;
  uint32 reserved2;
  uint32 reserved3;
  uint32 reserved4;
  uint32 reserved5;
  uint32 reserved6;
} hello_pkt_t;

typedef struct _PACKED hello_resp_pkt_s
{
  uint32 command;
  uint32 len;
  uint32 version;
  uint32 version_compatible;
  uint32 status;
  uint32 mode;
  uint32 reserved1;
  uint32 reserved2;
  uint32 reserved3;
  uint32 reserved4;
  uint32 reserved5;
  uint32 reserved6;
} hello_resp_pkt_t;

typedef struct _PACKED memory_debug_pkt_s
{
  uint32 command;
  uint32 length;
  uint32 table_addr;
  uint32 table_len;
} memory_debug_pkt_t;

typedef struct _PACKED memory_read_pkt_s
{
  uint32 command;
  uint32 len;
  uint32 mem_addr;
  uint32 mem_len;
} memory_read_pkt_t;

typedef struct _PACKED cmd_switch_mode_pkt_s
{
  uint32 command;
  uint32 len;
  uint32 mode;
} cmd_switch_mode_pkt_t;

typedef struct _PACKED cmd_reset_pkt_s
{
  uint32  command;
  uint32  len;
} cmd_reset_pkt_t;

/* Sahara memory region information
 * NOTE: When calculating size of this struct for parsing the raw memory table,
 *       the size should be: sizeof(sahara_memory_region_t) - sizof(uint32) 
 */
typedef struct _PACKED memory_region_s
{
  uint32 save_pref; /* not sure what this field is used for? */
  uint32 addr;
  uint32 len;
  uint8  description[SAHARA_MEM_REGION_NAME_SIZE];
  uint8  filename[SAHARA_MEM_REGION_NAME_SIZE];
  uint32 next_read;    /* This is only used to keep track of memory read and is
                        * not counted when parsing memory region info w/ memcpy */
} sahara_memory_region_t;

/* Sahara memory dump information */
typedef struct
{
  uint8                   current_region;
  uint8                   max_num_region;
  uint8                   protocolVer;
  uint8                   protocolMin;
  uint16                  maxWriteSize;
  uint32                  total_mem_size;
  uint32                  total_mem_read;
  sahara_memory_region_t  mem_region[MAX_MEM_REGIONS];
} sahara_memdump_info_t;


/* RAM Dump status */
typedef struct
{
  enum eRamDumpState state;
  uint8 progress;
  union
  {
    dmss_MemoryDumpInfo_s dmss_MemDumpInfo;
    sahara_memdump_info_t sahara_MemDumpInfo;
  } data;
} SwiMemDebugProgressT;

/************
 *                    GLOBAL DATA
 ************/
/* Canned Messages */
static const uint8 DMSS_CMDNOOP_PKT[] = { 0x7E, 0x06, 0x4E, 0x95, 0x7E };
static const uint8 DMSS_CMDPREQ_PKT[] = { 0x7E, 0x07, 0xC7, 0x84, 0x7E };
static const uint8 DMSS_CMDMEMDEBUG_QUERY_PKT[] = { 0x7E, 0x10, 0xF9, 0xE0, 0x7E };
static const uint8 DMSS_CMDRESET_PKT[] = { 0x7E, 0x0A, 0x22, 0x5F, 0x7E };

static int RAMDumpCaptured = FALSE;
static SerialReadWriteInfo_s sSerialInfo;
static dmss_MemoryDumpInfo_s dmss_MemDumpInfo;
static SwiMemDebugProgressT sRamDumpProgress;
static int bForceSlow = FALSE;

/* ramdump options */
static char           *memdump_path = NULL;
static int            memdump_comport;
static bool           memdump_noreset;
static enum eProtocol memdump_protocol;

/* buffer used for executing shell commands */
#define SHELL_CMD_MAX_LEN     512
static char shell_cmd[SHELL_CMD_MAX_LEN] = {0};
/* help string */
static const char help_str[] = "\
  -d <device>\n\
       Optionally specify device type. i.e -d 9x30.\n\
  -c <comport>\n\
       Optional parameter to specify the DM port, example: -c 3 for COM3 on Windows\n\
       or -c 0 for /dev/ttyUSB0 on Linux\n\
  -o <output_directory>\n\
       Optional parameter to specify where to store the crash dump.\n\
       If this is not specified, '<current dir>/MemDumpOut' will be used as default \n\
  -noreset\n\
       Optional Parameter to not reset the device after collecting a crash dump.\n\
  -s\n\
       Force slow speed for DMSS protocol. Will have no effect on 9x30 device\n\
  -h\n\
       Display this help menu\n";

/************
 *                       FUNCTIONS
 ************/

/*************
*
* Name:     print_packet
* 
* Purpose:  Debugging function to print out content of a buffer
*
* Parms:    buf [IN] - pointer to the buffer
*           len [IN] - length of the buffer
*
* Return:   None
*
**************/
void print_packet(uint8 *buf, int len)
{
#ifdef SWI_DEBUG
  int i;
  for (i=0; i<len; i++)
  {
    SWI_PRINTF("[%02X]", buf[i]);
  }
  SWI_PRINTF("\n");
#endif
}

/*************
*
* Name:     Ramdump_OpenDMPort
* 
* Purpose:  Find a serial port with a valid device running in Download mode.
*
* Parms:    portnum - Port number to open, -1 for auto detect
*
* Return:   eRAM_DUMP_STATUS_OK if port is found and opened successfuly
*
* Abort:    none
*
* Notes:    none
*
**************/
enum eRamDumpStatus Ramdump_OpenDMPort(int portnum)
{
  char ComPort[MAX_COMPORT_LEN] = {0};  
  enum eRamDumpStatus status = eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND;
  bool rc = FALSE;

#ifdef RAMDUMP_WIN32
  enum eRamDumpStatus busystatus = eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND;
  typedef struct
  {
    enum eRamDumpStatus busyStatus;
    char *name;
  } SwiMemDebugPortInfo;

  /* Known port names for Autodetection */
  SwiMemDebugPortInfo autoPorts[] = 
  {
    { eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM,  (char *)"QDLoader" },
    { eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM,  (char *)"Sierra Wireless DM Port" },
    { eRAM_DUMP_STATUS_FAIL_PORT_BUSY_CNS, (char *)"CNS" },
    /* Mark the end of table */
    { eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM,  NULL }
  };

  SwiMemDebugPortInfo *pPortInfo = autoPorts;

  if (portnum == -1)
  {
    /* Autodetection */
    while (pPortInfo->name != NULL)
    {
      if (registry_read_swidiag(pPortInfo->name, ComPort, MAX_COMPORT_LEN))
      {
        /* Store the busy status to be returned later, in case the port can't be opened */
        busystatus = pPortInfo->busyStatus;
        status = eRAM_DUMP_STATUS_OK;
        break;
      }
      pPortInfo++;
    }
  }
  else 
  {
    /* Port number has been provided */
    snprintf(ComPort, MAX_COMPORT_LEN, "\\\\.\\COM%d", portnum);
    ComPort[MAX_COMPORT_LEN - 1] = '\0'; /*make sure the string is terminated.*/
    status = eRAM_DUMP_STATUS_OK;
  }

  if (status == eRAM_DUMP_STATUS_OK)
  {
    rc = serialOpen(ComPort);
  }
#else /* Is a Linux app */
  int i;
  struct stat st;
  if (portnum == -1)
  {
    /* Search for a sierra device amongst /dev/ttyUSB* */
    for (i=0; i<=127; i++)
    {
      memset(shell_cmd, 0, SHELL_CMD_MAX_LEN);
      sprintf(shell_cmd, "udevadm info --query=\"property\" --name=/dev/ttyUSB%d 2>err | grep -i sierra 1>log", i);

      if (-1 != system(shell_cmd))
      {            
        if (stat("err", &st)!=-1 && (st.st_size > 0))
        {
          SWI_PRINT_INFO("no device in /dev/ttyUSB%d\n",i);
        }
        else if ((-1 != stat("log", &st)) && (st.st_size > 0))
        {
          SWI_PRINT_INFO("sierra device in /dev/ttyUSB%d\n",i);
          portnum = i;
          break;
        }
      }
    }
    /* clean up */
    system("rm -f err log");

    if (-1 == portnum)
    {
      SWI_PRINTF("DM port not detected\n");
      return eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND;
    }
  }

  snprintf(ComPort, MAX_COMPORT_LEN, "/dev/ttyUSB%d", portnum);
  rc = OpenDMPort(ComPort);
#endif

  if (TRUE == rc)
  {
    status = eRAM_DUMP_STATUS_OK;
  }
  else
  {
    status = eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND;
  }
  return status;
}

/*************
*
* Name:     Ramdump_CloseDMPort
* 
* Purpose:  Wrapper function for closing DM port
*
* Parms:    None
*
* Return:   None
*
* Abort:    none
*
* Notes:    none
*
**************/
void Ramdump_CloseDMPort()
{
  #ifdef RAMDUMP_WIN32
  serialClose();
  #else
  CloseDMPort();
  #endif
}

/*************
*
* Name:     Ramdump_WriteOnDMPort
* 
* Purpose:  Wrapper function for writing to DM port
*
* Parms:    pBuf [IN] - pointer to data to write
*           len  [IN] - length of data to write
*
* Return:   None
*
* Abort:    none
*
* Notes:    none
*
**************/
void Ramdump_WriteOnDMPort(
  char *pBuf,
  int  len 
)
{
  #ifdef RAMDUMP_WIN32
  serialWrite(pBuf, len);
  #else
  WriteOnDMPort(pBuf, len);
  #endif
}

/*************
*
* Name:     Ramdump_ReadFromDMPort
* 
* Purpose:  Wrapper function for reading from DM port
*
* Parms:    pBuf [IN/OUT] - storage for data read
*           len  [IN]     - number of bytes to be read
*
* Return:   Number of bytes read
*
* Abort:    none
*
* Notes:    none
*
**************/
int Ramdump_ReadFromDMPort(
  char *pBuf,
  int  len 
)
{
  int ret = 0;
  #ifdef RAMDUMP_WIN32
  ret = serialRead(pBuf, len);
  #else
  ret = ReadFromDMPort(pBuf, len);
  #endif
  return ret;
}

/************ 
 * Name:     DoProgressUpdate
 *
 * Purpose:  This function updated the RAM Dump capturing status and
 *           information received from the modem.
 *
 * Parms:    pRamDumpProgress [IN] - Pointer to structure containing RAM Dump
 *                                   progress.
 *
 * Return:   None
 *
 * Notes:    None
 ************/
void DoProgressUpdate(
  SwiMemDebugProgressT * pRamDumpProgress)
{
  static int updateOnlyOnce = TRUE;
  int idx = 0;

  switch (pRamDumpProgress->state)
  {
    case STATE_DMSS_DEVICE_DETECTION:
    case STATE_SAHARA_WAIT_HELLO:
      break;

    case STATE_DMSS_GET_PARAMS:
    case STATE_SAHARA_WAIT_COMMAND:
      printf( "Querying device parameters...\n");
      break;

    case STATE_DMSS_GET_REGIONS:
    case STATE_SAHARA_WAIT_MEMORY_TABLE:
      printf( "Querying Memory Debug Info...\n");
      if (PROTOCOL_DMSS == memdump_protocol)
      {
        for (idx = 0; idx < pRamDumpProgress->data.dmss_MemDumpInfo.maxMemRegion; idx++)
        {
          printf( "%d: %s\n [Base:0x%08X, Length:0x%08X]\n",
                  idx + 1,
                  pRamDumpProgress->data.dmss_MemDumpInfo.MemRegions[idx].description,
                  (unsigned int)pRamDumpProgress->data.dmss_MemDumpInfo.MemRegions[idx].baseAddress,
                  (unsigned int)pRamDumpProgress->data.dmss_MemDumpInfo.MemRegions[idx].length);
        }
      }
      else if (PROTOCOL_SAHARA == memdump_protocol)
      {
        for (idx = 0; idx <= pRamDumpProgress->data.sahara_MemDumpInfo.max_num_region; idx++)
        {
          printf( "%d: %s\n [Base:0x%08X, Length:0x%08X]\n",
                  idx+1,
                  pRamDumpProgress->data.sahara_MemDumpInfo.mem_region[idx].description,
                  (unsigned int)pRamDumpProgress->data.sahara_MemDumpInfo.mem_region[idx].addr,
                  (unsigned int)pRamDumpProgress->data.sahara_MemDumpInfo.mem_region[idx].len);
        }
      }
      break;

    case STATE_DMSS_GET_CHUNK:
    case STATE_SAHARA_WAIT_MEMORY_REGION:
      if (TRUE == updateOnlyOnce)
      {
        printf( "Reading memory...\n");
        updateOnlyOnce = FALSE;
      }
      fprintf(stderr, "\r");
      fprintf(stderr, "%d%% Complete", pRamDumpProgress->progress);
      break;

    case STATE_DMSS_CLEANUP:
    case STATE_SAHARA_CLEANUP:
      printf( "\r");
      printf( "100%% Complete\n");
      break;

    default:
      break;
  }
}

/************
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
 ************/
int EscapePkt(
  )
{
  uint16 idx1 = 0, idx2 = 0;

  /* Always skip the first and last characters */
  for (idx1 = 1; idx1 < sSerialInfo.writeLen - 1; idx1++)
  {
    if ((sSerialInfo.write[idx1] == ASYNC_HDLC_FLAG) || (sSerialInfo.write[idx1] == ASYNC_HDLC_ESC))
    {
      for (idx2 = sSerialInfo.writeLen; idx2 > idx1 + 1; idx2--)
      {
        sSerialInfo.write[idx2] = sSerialInfo.write[idx2 - 1];
      }
      sSerialInfo.write[idx1 + 1] = sSerialInfo.write[idx1] ^ ASYNC_HDLC_ESC_MASK;
      sSerialInfo.write[idx1] = ASYNC_HDLC_ESC;
      sSerialInfo.writeLen++;
    }
  }
  return TRUE;
}

/************
 * Name:     dmss_SendNextReadReq
 *
 * Purpose:  Send the next memory dump read request
 *
 * Parms:    None
 *
 * Return:   TRUE/FALSE - False indicates that no more reads are required
 *
 * Notes:    None
 ************/
int dmss_SendNextReadReq(
  )
{
  uint32 endAddress;
  uint32 readLen;
  uint16 crc;
  dmss_MemoryRegionInfo_s *pMemRegion = NULL;

  pMemRegion = &dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion];

  /* Build the read request packet in the write buffer */
  sSerialInfo.write[0] = ASYNC_HDLC_FLAG;
  sSerialInfo.write[1] = DMSS_CMDMEM_READ_REQ;

  /* Start Address - 4 Bytes */
  sSerialInfo.write[2] = (uint8)(pMemRegion->nextRead >> 24) & 0xFF;
  sSerialInfo.write[3] = (uint8)(pMemRegion->nextRead >> 16) & 0xFF;
  sSerialInfo.write[4] = (uint8)(pMemRegion->nextRead >> 8) & 0xFF;
  sSerialInfo.write[5] = (uint8)(pMemRegion->nextRead & 0xFF);

  /* Read Size - 2 Bytes */
  endAddress = pMemRegion->length + pMemRegion->baseAddress;
  if (MAX_MEM_READ_LEN < (endAddress - pMemRegion->nextRead))
  {
    readLen = MAX_MEM_READ_LEN;
  }
  else
  {
    readLen = endAddress - pMemRegion->nextRead;
  }

  sSerialInfo.write[6] = (uint8)(readLen >> 8) & 0xFF;
  sSerialInfo.write[7] = (uint8)(readLen & 0xFF);

  /* Calculate the CRC before escaping the data */
  crc = dmcrc(sSerialInfo.write, MAX_MEM_READ_PKT_LEN);

  sSerialInfo.write[8] = (uint8)(crc & 0xFF);
  sSerialInfo.write[9] = (uint8)((crc >> 8) & 0xFF);
  sSerialInfo.write[10] = ASYNC_HDLC_FLAG;
  sSerialInfo.writeLen = 11;

  /* Escape the packet. Results are stored in sSerialInfo */
  EscapePkt();

  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
  return TRUE;
}

/************
 * Name:     dmss_SendNextUnframedReadReq
 *
 * Purpose:  Send the next memory dump unframed read request
 *
 * Parms:    None
 *
 * Return:   TRUE/FALSE - False indicates that no more reads are required
 *
 * Notes:    None
 ************/
int dmss_SendNextUnframedReadReq(
  )
{
  uint32 endAddress;
  uint32 readLen;
  uint16 crc;
  dmss_MemoryRegionInfo_s *pMemRegion = NULL;

  pMemRegion = &dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion];

  /* Build the read request packet in the write buffer */
  sSerialInfo.write[0] = ASYNC_HDLC_FLAG;
  sSerialInfo.write[1] = DMSS_CMDMEM_UNFRAMED_READ_REQ;

  /* Padding - QCT uses these values, Using these values helped double check
   * CRC values, since they match QCT */
  sSerialInfo.write[2] = 0x12;
  sSerialInfo.write[3] = 0x56;
  sSerialInfo.write[4] = 0x34;

  /* Start Address - 4 Bytes */
  sSerialInfo.write[5] = (uint8)(pMemRegion->nextRead >> 24) & 0xFF;
  sSerialInfo.write[6] = (uint8)(pMemRegion->nextRead >> 16) & 0xFF;
  sSerialInfo.write[7] = (uint8)(pMemRegion->nextRead >> 8) & 0xFF;
  sSerialInfo.write[8] = (uint8)(pMemRegion->nextRead & 0xFF);

  /* Read Size - 2 Bytes */
  endAddress = pMemRegion->length + pMemRegion->baseAddress;
  if (MAX_MEM_UNFRAMED_READ_LEN < (endAddress - pMemRegion->nextRead))
  {
    readLen = MAX_MEM_UNFRAMED_READ_LEN;
  }
  else
  {
    readLen = endAddress - pMemRegion->nextRead;
  }

  sSerialInfo.write[9] = (uint8)(readLen >> 24) & 0xFF;
  sSerialInfo.write[10] = (uint8)(readLen >> 16) & 0xFF;
  sSerialInfo.write[11] = (uint8)(readLen >> 8) & 0xFF;
  sSerialInfo.write[12] = (uint8)(readLen & 0xFF);

  /* Calculate the CRC before escaping the data */
  crc = dmcrc(sSerialInfo.write, MAX_MEM_UNFRAMED_READ_PKT_LEN);

  sSerialInfo.write[13] = (uint8)(crc & 0xFF);
  sSerialInfo.write[14] = (uint8)((crc >> 8) & 0xFF);
  sSerialInfo.write[15] = ASYNC_HDLC_FLAG;
  sSerialInfo.writeLen = 16;

  /* Escape the packet. Results are stored in sSerialInfo */
  EscapePkt();

  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
  return TRUE;
}

/************
 * Name:     dmss_ProcessCmdMemUnframedReadResp
 *
 * Purpose:  Process an unframed memory read response packet
 *
 * Parms:    None
 *
 * Return:   TRUE  - If response is successfully read and stored in log file.
 *           FALSE - Failed to read response or store it in log file.
 *
 * Notes:    None
 ************/
int dmss_ProcessCmdMemUnframedReadResp(
  )
{
  uint32 endAddress = 0, pktAddress = 0;
  uint16 pktLen = 0, idx = 0;
  uint8 *pData = NULL;
  dmss_MemoryRegionInfo_s *pMemRegion = NULL;
  static FILE *pFile;
  char filename[MAX_PATH_NAME] = {0};

  pMemRegion = &dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion];

  /* Determine the end address for this memory region */
  endAddress = pMemRegion->length + pMemRegion->baseAddress;

  /* Get the start address for this packet */
  pktAddress = sSerialInfo.read[4] << 24;
  pktAddress |= sSerialInfo.read[5] << 16;
  pktAddress |= sSerialInfo.read[6] << 8;
  pktAddress |= sSerialInfo.read[7];

  pktLen = sSerialInfo.read[8] << 24;
  pktLen |= sSerialInfo.read[9] << 16;
  pktLen |= sSerialInfo.read[10] << 8;
  pktLen |= sSerialInfo.read[11];

  /* Open the file, if this is the first read */
  if (pktAddress == pMemRegion->baseAddress)
  {
    /* Check if output path is specified */
    if(memdump_path != NULL)
    {
      strcpy(filename, memdump_path);
      strcat(filename,"/");
      strncat(filename, (const char *)pMemRegion->fileName, MAX_PATH_NAME - strlen(filename) - 1);
      pFile = fopen((const char *)filename, "wb");
    }
    else
    {
      pFile = fopen((const char *)pMemRegion->fileName, "wb");
    }
    if (NULL == pFile)
    {
      fprintf(stderr, "\nFailed to open Log file : %s!!\n", pMemRegion->fileName);
      return FALSE;
    }
  }

  /* Write the packet to the appropriate file */
  if (NULL != pFile)
  {
    pData = &sSerialInfo.read[12];

    if (MAX_READ_BUFFER_LEN < (pktLen + 12))
    {
      /* Bad data - try this one again */
      fprintf(stderr,"\nBad data. Try again\n");
      return FALSE;
    }
    for (idx = 0; idx < pktLen; idx++)
    {
      fputc(*pData++, pFile);
    }
    dmss_MemDumpInfo.totalMemoryRead += pktLen;
  }
  else
  {
    fprintf(stderr, "\nOutput file %s is not opened!!\n", pMemRegion->fileName);
    return FALSE;
  }

  /* Close the file if this is the last read */
  if ((pktAddress + pktLen) >= endAddress)
  {
    fclose(pFile);
    pFile = NULL;

    /* Increment the memory region index, and reset the read pointer */
    dmss_MemDumpInfo.currMemRegion++;
    if (dmss_MemDumpInfo.currMemRegion < dmss_MemDumpInfo.maxMemRegion)
    {
      dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion].nextRead =
        dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion].baseAddress;
    }
  }
  else
  {
    /* Increment the Read index */
    pMemRegion->nextRead += pktLen;
  }
  return TRUE;
}

/************
 * Name:     dmss_ProcessCmdMemReadResp
 *
 * Purpose:  Process a memory read response packet
 *
 * Parms:    None
 *
 * Return:   TRUE  - If response is successfully read and stored in log file.
 *           FALSE - Failed to read response or store it in log file.
 *
 * Notes:    None
 ************/
int dmss_ProcessCmdMemReadResp(
  )
{
  uint32 endAddress = 0, pktAddress = 0;
  uint16 pktLen = 0, idx = 0;
  uint8 *pData = NULL;
  dmss_MemoryRegionInfo_s *pMemRegion = NULL;
  static FILE *pFile;
  char filename[MAX_PATH_NAME] = {0};

  pMemRegion = &dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion];

  /* Determine the end address for this memory region */
  endAddress = pMemRegion->length + pMemRegion->baseAddress;

  /* Get the start address for this packet */
  pktAddress = sSerialInfo.read[1] << 24;
  pktAddress |= sSerialInfo.read[2] << 16;
  pktAddress |= sSerialInfo.read[3] << 8;
  pktAddress |= sSerialInfo.read[4];

  pktLen = sSerialInfo.read[5] << 8;
  pktLen |= sSerialInfo.read[6];

  /* Open the file, if this is the first read */
  if (pktAddress == pMemRegion->baseAddress)
  {
    /* Check if output path is specified */
    if(memdump_path != NULL)
    {
      strcpy(filename, memdump_path);
      strcat(filename,"/");
      strncat(filename, (const char *)pMemRegion->fileName, MAX_PATH_NAME - strlen(filename) - 1);
      pFile = fopen((const char *)filename, "wb");
    }
    else
    {
      pFile = fopen((const char *)pMemRegion->fileName, "wb");
    }
    if (NULL == pFile)
    {
      fprintf(stderr, "\nFailed to open Log file : %s!!\n", pMemRegion->fileName);
      return FALSE;
    }
  }

  /* Write the packet to the appropriate file */
  if (NULL != pFile)
  {
    pData = &sSerialInfo.read[7];
    for (idx = 0; idx < pktLen; idx++)
    {
      fputc(*pData++, pFile);
    }
    dmss_MemDumpInfo.totalMemoryRead += pktLen;
  }
  else
  {
    fprintf(stderr, "Output file %s is not opened!!\n", pMemRegion->fileName);
    return FALSE;
  }

  /* Close the file if this is the last read */
  if ((pktAddress + pktLen) >= endAddress)
  {
    fclose(pFile);
    pFile = NULL;

    /* Increment the memory region index, and reset the read pointer */
    dmss_MemDumpInfo.currMemRegion++;
    if (dmss_MemDumpInfo.currMemRegion < dmss_MemDumpInfo.maxMemRegion)
    {
      dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion].nextRead =
        dmss_MemDumpInfo.MemRegions[dmss_MemDumpInfo.currMemRegion].baseAddress;
    }
  }
  else
  {
    /* Increment the Read index */
    pMemRegion->nextRead += pktLen;
  }
  return TRUE;
}

/************
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
 ************/
void UnEscapePkt(
  )
{
  uint16 len = 0;

  /* Download protocol begins with a HDLC End character, so start unescape
   * process on next byte and then add the byte back in the return length */
  len = dmunescape(&sSerialInfo.read[1], sSerialInfo.readLen - 1);
  sSerialInfo.readLen = len + 1;
}

/************
 * Name:     dmss_RamDumpCmdAck
 *
 * Purpose:  Check whether the device is detected or not and update the state
 *           of RAM Dump tool accordingly.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdAck(
  enum eRamDumpState *pRamDumpState)
{
  if (STATE_DMSS_DEVICE_DETECTION == *pRamDumpState)
  {
    /* Device detected. Get the memory parameters */
    *pRamDumpState = STATE_DMSS_GET_PARAMS;
  }
  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_RamDumpCmdParams
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
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdParams(
  enum eRamDumpState *pRamDumpState,
  uint8 * pCmdBuf)
{
  if (STATE_DMSS_GET_PARAMS == *pRamDumpState)
  {
    /* Extract the important info */
    dmss_MemDumpInfo.protocolVer = *pCmdBuf++;
    dmss_MemDumpInfo.protocolMin = *pCmdBuf++;
    dmss_MemDumpInfo.maxWriteSize = *pCmdBuf;

    /* Transition to get the memory regions */
    *pRamDumpState = STATE_DMSS_GET_REGIONS;
    dmss_MemDumpInfo.currMemRegion = 0;
    dmss_MemDumpInfo.maxMemRegion = 0;
  }
  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_RamDumpCmdMemReadResp
 *
 * Purpose:  Process the memory read command's response from modem.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdMemReadResp(
  enum eRamDumpState *pRamDumpState)
{
  if (STATE_DMSS_GET_CHUNK == *pRamDumpState)
  {
    if (FALSE == dmss_ProcessCmdMemReadResp())
    {
      fprintf(stderr, "\n%s: Fatal error occured,exiting!\n", __func__);
      return eRAM_DUMP_STATUS_FAIL_GENERIC;
    }

    if (dmss_MemDumpInfo.currMemRegion >= dmss_MemDumpInfo.maxMemRegion)
    {
      /* We're Done! */
      *pRamDumpState = STATE_DMSS_CLEANUP;
    }
  }
  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_RamDumpCmdMemUnframedReadResp
 *
 * Purpose:  Process the memory unframed read command's response from modem.
 *
 * Parms:    pRamDumpState [IN/OUT] - Current state of RAM Dump tool
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdMemUnframedReadResp(
  enum eRamDumpState *pRamDumpState)
{
  if (STATE_DMSS_GET_CHUNK == *pRamDumpState)
  {
    if (FALSE == dmss_ProcessCmdMemUnframedReadResp())
    {
      fprintf(stderr, "\nFatal error occured,exiting!\n");
      return eRAM_DUMP_STATUS_FAIL_GENERIC;
    }

    if (dmss_MemDumpInfo.currMemRegion >= dmss_MemDumpInfo.maxMemRegion)
    {
      /* We're Done! */
      *pRamDumpState = STATE_DMSS_CLEANUP;
    }
  }

  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_RamDumpCmdMemDebugInfo
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
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdMemDebugInfo(
  enum eRamDumpState *pRamDumpState,
  uint8 * pCmdBuf)
{
  uint8 *pCmdEnd = NULL;

  if (STATE_DMSS_GET_REGIONS == *pRamDumpState)
  {
    /* Get memory info len */
    uint16 memInfoLen = (uint16)((*pCmdBuf++ << 8) & 0xFF00);
    memInfoLen += ((uint16)* pCmdBuf++ & 0xFF);
    pCmdEnd = pCmdBuf + memInfoLen;

    dmss_MemoryRegionInfo_s *pMemRegionInfo = &dmss_MemDumpInfo.MemRegions[0];

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
      while (0x00 != *pCmdBuf)
      {
        pMemRegionInfo->description[idx++] = *pCmdBuf++;
      }

      pCmdBuf++;
      idx = 0;
      while (0x00 != *pCmdBuf)
      {
        pMemRegionInfo->fileName[idx++] = *pCmdBuf++;
      }

      pCmdBuf++;
      pMemRegionInfo++;
      dmss_MemDumpInfo.maxMemRegion++;

      if (pCmdBuf >= pCmdEnd)
      {
        break;
      }
    }

    /* Calculate total memory size to be logged to track percentage
     * completion. */
    int idx;
    for (dmss_MemDumpInfo.totalMemorySize = 0, idx = 0; idx < dmss_MemDumpInfo.maxMemRegion; idx++)
    {
      dmss_MemDumpInfo.totalMemorySize += dmss_MemDumpInfo.MemRegions[idx].length;
    }

    /* Transition to the memory dump */
    dmss_MemDumpInfo.currMemRegion = 0;
    dmss_MemDumpInfo.MemRegions[0].nextRead = dmss_MemDumpInfo.MemRegions[0].baseAddress;
    *pRamDumpState = STATE_DMSS_GET_CHUNK;
    memcpy((void *)&(sRamDumpProgress.data.dmss_MemDumpInfo),
           (void *)&(dmss_MemDumpInfo), sizeof(dmss_MemoryDumpInfo_s));
    DoProgressUpdate(&sRamDumpProgress);
  }

  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_RamDumpCmdNak
 *
 * Purpose:  Process the NAK response received from the modem.
 *
 * Parms:    pTotalConsecutiveNaks [IN/OUT] - Total NAK so far received from
 *                                            the modem
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 ************/
static enum eRamDumpStatus dmss_RamDumpCmdNak(
  uint16 * pTotalConsecutiveNaks)
{
  (*pTotalConsecutiveNaks)++;

  if (MAX_CONSECUTIVE_NAKS < *pTotalConsecutiveNaks)
  {
    Ramdump_CloseDMPort();
    return eRAM_DUMP_STATUS_FAIL_RETRIES;
  }

  /* Try Again, first */
  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);

  return eRAM_DUMP_STATUS_OK;
}

/************
 * Name:     dmss_StartRamDump
 *
 * Purpose:  Starts the Ram Dump gathering with DMSS protocol.
 *
 * Params:   HSEnabled - Use High Speed protocol (TRUE/FALSE)
 *
 * Return:   eRamDumpStatus - Enum indicating success or failure cause
 *
 * Notes:    None
 ************/
static enum eRamDumpStatus dmss_StartRamDump(
  int HSEnabled)
{
#define MAX_CONSECUTIVE_CS_ERROR_COUNT 1000
  uint32 consecutive_cs_error = 0;
  enum eRamDumpState RamDumpState = STATE_DMSS_DEVICE_DETECTION;
  enum eRamDumpStatus status = eRAM_DUMP_STATUS_OK;
  int sleepVal = 0;


  /* Open the port entered by the user */
  status = Ramdump_OpenDMPort(memdump_comport);
  if (eRAM_DUMP_STATUS_OK != status)
  {
    return status;
  }

  /* Send the NO-OP command as a ping test to the device */
  Ramdump_WriteOnDMPort((char *)DMSS_CMDNOOP_PKT, sizeof(DMSS_CMDNOOP_PKT));

  /* Track the number of consecutive naks to look for protocol errors */
  uint16 totalConsecutiveNaks = 0;

  /* time tracking variables */
  time_t startTime, currTime, lastReadTime;
  time(&startTime);
  time(&lastReadTime);

  while (1)
  {
    #ifndef RAMDUMP_WIN32
    /* Wait for a few millisecond. No timer exist for reading and writing to
     * Linux Com port. Thus explicitly wait for few millisecond. Does not work 
     * with less than 2 milliseconds sleep. */
    if (HSEnabled)
      /* 2 ms seems too fast, high frequency of encountering bad data ... */
      sleepVal = TIMER_3_MILLISEC;
    else
      sleepVal = TIMER_4_MILLISEC;

    usleep(sleepVal);
    #endif

    /* Get the response from the device */
    memset(sSerialInfo.read, 0, MAX_READ_BUFFER_LEN);
    sSerialInfo.readLen = Ramdump_ReadFromDMPort((char *)sSerialInfo.read, MAX_READ_BUFFER_LEN);

    /* If the data is received */
    if (0 != sSerialInfo.readLen)
    {
      #ifdef SWI_DEBUG_INFO
      if (sSerialInfo.readLen < 40)
      {
        print_packet(sSerialInfo.read, sSerialInfo.readLen);
      }
      #endif
      time(&lastReadTime);

      /* If the first byte is the Async HDLC ESC character, the command type
       * is the next byte. */
      uint8 *pCmdBuf = NULL;

      pCmdBuf = ASYNC_HDLC_FLAG == sSerialInfo.read[0]
        ? &sSerialInfo.read[1] : &sSerialInfo.read[0];

      /* High speed memory debugging does not use HDLC framing */
      if (DMSS_CMDMEM_UNFRAMED_READ_RESP != *pCmdBuf)
      {
        SWI_PRINT_INFO("\n*pCmdBuf = %X\n", *pCmdBuf);
        UnEscapePkt();

        /* First validate the packet */
        uint16 calculatedCRC = dmcrc(sSerialInfo.read, sSerialInfo.readLen);
        uint16 receivedCRC = sSerialInfo.read[sSerialInfo.readLen - 3] +
          (sSerialInfo.read[sSerialInfo.readLen - 2] << 8);

        if (calculatedCRC != receivedCRC)
        {
          #ifdef RAMDUMP_DBG
          fprintf(stderr, "\nChecksum Error, Type %X, Length %d, Count %d\n",
                  sSerialInfo.read[0], sSerialInfo.readLen, consecutive_cs_error);
          fprintf(stderr, "calculatedCRC = 0x%x, ReceivedCrc = 0x%x\n", calculatedCRC, receivedCRC);
          print_packet(sSerialInfo.read, sSerialInfo.readLen);
          #endif
          consecutive_cs_error++;
          if (consecutive_cs_error > MAX_CONSECUTIVE_CS_ERROR_COUNT)
          {
            /* Abort if there are too many checksum errors */
            return eRAM_DUMP_STATUS_FAIL_CRC;
          }
          /* Try Again, first */
          Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
          continue;
        }
        else
        {
          SWI_PRINT_INFO("\nReset checksum error count\n");
          consecutive_cs_error = 0;
        }
      }

      if (DMSS_CMDNAK != *pCmdBuf)
      {
        totalConsecutiveNaks = 0;
      }

      enum eRamDumpStatus rc;
      switch (*pCmdBuf++)
      {
        case DMSS_CMDACK:
          (void)dmss_RamDumpCmdAck(&RamDumpState);
          break;

        case DMSS_CMDPARAMS:
          (void)dmss_RamDumpCmdParams(&RamDumpState, pCmdBuf);
          break;

        case DMSS_CMDMEM_DEBUG_INFO:
          (void)dmss_RamDumpCmdMemDebugInfo(&RamDumpState, pCmdBuf);
          break;

        case DMSS_CMDMEM_READ_RESP:
          if (eRAM_DUMP_STATUS_OK != (rc = dmss_RamDumpCmdMemReadResp(&RamDumpState)))
          {
            return rc;
          }
          break;

        case DMSS_CMDMEM_UNFRAMED_READ_RESP:
          if (eRAM_DUMP_STATUS_OK != (rc = dmss_RamDumpCmdMemUnframedReadResp(&RamDumpState)))
          {
            return rc;
          }
          break;

        case DMSS_CMDNAK:
        default:               /* Count unrecognized CMD responses as NAKS */
          if (eRAM_DUMP_STATUS_OK != (rc = dmss_RamDumpCmdNak(&totalConsecutiveNaks)))
          {
            return rc;
          }
          continue;
          break;
      }
    }
    else
    {
      /* Every time we reach this point, it means a packet failed to generate 
       * a response. */
      time(&currTime);

      /* Timeout after continued failure */
      int timeout = 5;
      if ((currTime - lastReadTime) > timeout)
      {
        printf( "\nNo communication received for %d seconds." " Aborting!!\n", timeout);

        return eRAM_DUMP_STATUS_FAIL_NO_RESPONSE;
      }
    }

    sRamDumpProgress.state = RamDumpState;

    /* Next Step */
    switch (RamDumpState)
    {
      case STATE_DMSS_DEVICE_DETECTION:
        /* Every time we reach this point,it means a NOOP packet failed to
         * generate a response. */
        time(&currTime);

        /* Timeout after 10 seconds of failure */
        if ((currTime - startTime) > 10)
        {
          return eRAM_DUMP_STATUS_FAIL_NO_RESPONSE;
        }

        /* Send the NO-OP command as a ping test to the device */
        Ramdump_WriteOnDMPort((char *)DMSS_CMDNOOP_PKT, sizeof(DMSS_CMDNOOP_PKT));
        sRamDumpProgress.progress = 0;
        DoProgressUpdate(&sRamDumpProgress);
        break;

      case STATE_DMSS_GET_PARAMS:
        /* Send the NO-OP command as a ping test to the device */
        Ramdump_WriteOnDMPort((char *)DMSS_CMDPREQ_PKT, sizeof(DMSS_CMDPREQ_PKT));
        sRamDumpProgress.progress = 0;
        DoProgressUpdate(&sRamDumpProgress);
        break;

      case STATE_DMSS_GET_REGIONS:
        Ramdump_WriteOnDMPort((char *)DMSS_CMDMEMDEBUG_QUERY_PKT, sizeof(DMSS_CMDMEMDEBUG_QUERY_PKT));
        sRamDumpProgress.progress = 0;
        dmss_MemDumpInfo.totalMemoryRead = 0;
        /* Progress Update will be sent after reading the memory info */
        break;

      case STATE_DMSS_GET_CHUNK:
        if (TRUE == HSEnabled)
        {
          dmss_SendNextUnframedReadReq();
        }
        else
        {
          dmss_SendNextReadReq();
        }
        sRamDumpProgress.progress =
          (uint8)(((dmss_MemDumpInfo.totalMemoryRead >> 8) * 100) / (dmss_MemDumpInfo.totalMemorySize >> 8));
        DoProgressUpdate(&sRamDumpProgress);
        break;

      case STATE_DMSS_CLEANUP:
        sRamDumpProgress.progress = 100;
        DoProgressUpdate(&sRamDumpProgress);
        /* Reset the device? */
        if (!memdump_noreset)
        {
          printf( "\nReseting Device ...\n");
          Ramdump_WriteOnDMPort((char *)DMSS_CMDRESET_PKT, sizeof(DMSS_CMDRESET_PKT));
        }
        return eRAM_DUMP_STATUS_OK;

      default:
        status = eRAM_DUMP_STATUS_FAIL_INV_STATE;
        printf( "\nUnknown error. Aborting ram dump\n");
        sRamDumpProgress.progress = 100;
        DoProgressUpdate(&sRamDumpProgress);
        break;
    }

    if (eRAM_DUMP_STATUS_OK != status)
    {
      /* An unfixable error has occurred, abort the memory dump */
      break;
    }
  }                             /* While(1) */
  return status;
}


/**********************
 * Sahara Protocol
 **********************/
  sahara_memdump_info_t SaharaMemInfo;
  SwiMemDebugProgressT SaharaProgress;
  bool mStopFlag;

/* Functions */

/***********
* Name:     get4bytesField
*
* Purpose:  This function is used to get the value of a 4 bytes field in the
*           command packet
* Params:   buf   -   pointer to the starting address of the 4 bytes field
*
* Notes:    The 4 bytes long data sent from the target may be in little
*           endian format, hence this is needed to read them correctly
*
************/
static uint32 get4bytesField(uint8 *buf)
{
  uint32 value;

  value = (uint32)*buf;

  return value;
}

/***********
* Name:     sahara_send_hello_resp_pkt
* 
* Purpose:  This function will send the hello response packet to initiate memory dump
*
* Parms:    None
*
* Return:   Expected length of the next packet received (memory debug)
*
* Abort:    none
*
* Notes:    none
*
************/

int sahara_send_hello_resp_pkt()
{
  hello_resp_pkt_t hello_resp_packet = {0};
  hello_resp_pkt_t *hello_resp_packetp = &hello_resp_packet;

  memset(&hello_resp_packet, 0, sizeof(hello_resp_packet));
  hello_resp_packet.command = htole32(SAHARA_CMD_HELLO_RESP);
  hello_resp_packet.len = htole32(LEN_SAHARA_CMD_HELLO_RESPONSE);
  hello_resp_packet.version = htole32(SAHARA_CURRENT_VERSION);
  hello_resp_packet.version_compatible = htole32(SAHARA_COMPATIBLE_VERSION);

  hello_resp_packet.status = 0; /* default 0x00 for success */
  hello_resp_packet.mode = htole32(SAHARA_MODE_MEMORY_DEBUG);

  /* Copy the packet to Serial buffer */
  memmove(sSerialInfo.write, hello_resp_packetp, LEN_SAHARA_CMD_HELLO_RESPONSE);
  sSerialInfo.writeLen = LEN_SAHARA_CMD_HELLO_RESPONSE;

  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
  return LEN_SAHARA_CMD_MEMORY_DEBUG;
}

/***********
*
* Name:     sahara_send_cmd_switch_mode_pkt
* 
* Purpose:  This function is used to send a command switch mode packet to the device
*
* Parms:    None
*
* Return:   Expected length of the next packet received (hello packet)
*
* Abort:    none
*
* Notes:    none
*
************/
int sahara_send_cmd_switch_mode_pkt()
{
  cmd_switch_mode_pkt_t cmd_switch_packet;
  cmd_switch_mode_pkt_t *cmd_switch_packetp = &cmd_switch_packet;

  cmd_switch_packet.command = htole32(SAHARA_CMD_CMD_SWITCH);
  cmd_switch_packet.len = htole32(LEN_SAHARA_CMD_COMMAND_SWITCH_MODE);
  cmd_switch_packet.mode = htole32(SAHARA_MODE_MEMORY_DEBUG);

  memmove(sSerialInfo.write, cmd_switch_packetp, LEN_SAHARA_CMD_COMMAND_SWITCH_MODE);
  sSerialInfo.writeLen = LEN_SAHARA_CMD_COMMAND_SWITCH_MODE;
  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);

  return LEN_SAHARA_CMD_HELLO;
}

/***********
*
* Name:     sahara_send_cmd_reset_pkt
* 
* Purpose:  Send a reset command packet to have the target reset
*
* Parms:    None
*
* Return:   None
*
* Abort:    none
*
* Notes:    none
*
************/
void sahara_send_cmd_reset_pkt()
{
  cmd_reset_pkt_t cmd_reset_packet;
  cmd_reset_pkt_t *cmd_reset_packetp = &cmd_reset_packet;

  cmd_reset_packet.command = htole32(SAHARA_CMD_RESET);
  cmd_reset_packet.len = htole32(LEN_SAHARA_CMD_RESET);

  memmove(sSerialInfo.write, cmd_reset_packetp, LEN_SAHARA_CMD_RESET);
  sSerialInfo.writeLen = LEN_SAHARA_CMD_RESET;
  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
}

/***********
*
* Name:     sahara_verify_mem_region
* 
* Purpose:  Verify the memory table received from the raw data packet
*
* Parms:    None
*
* Return:   None
*
* Abort:    none
*
* Notes:    none
*
************/
void sahara_verify_mem_region()
{
  uint8 mem_region_num;
  uint8 i = 0;
  uint8  *bufp;
  uint32 mem_size = 0;

  /* calculate the number of memory regions */
  mem_region_num = sSerialInfo.readLen / (sizeof(sahara_memory_region_t) - sizeof(uint32));
  SaharaMemInfo.max_num_region = mem_region_num - 1;  /* array index start at 0 */
  SaharaMemInfo.current_region = 0;

  /* Parse the raw data */
  bufp = sSerialInfo.read;
  for (i=0; i<mem_region_num; i++)
  {
    memmove(&SaharaMemInfo.mem_region[i], bufp, sizeof(sahara_memory_region_t) - sizeof(uint32));
    SaharaMemInfo.mem_region[i].addr = htole32( SaharaMemInfo.mem_region[i].addr );
    SaharaMemInfo.mem_region[i].len = htole32( SaharaMemInfo.mem_region[i].len );

    /* initialize the read index for each region */
    SaharaMemInfo.mem_region[i].next_read = SaharaMemInfo.mem_region[i].addr;
    /* calculate the total size */
    mem_size += SaharaMemInfo.mem_region[i].len;
    bufp += (sizeof(sahara_memory_region_t) - sizeof(uint32));
  }

  SaharaMemInfo.total_mem_size = mem_size;
  SaharaMemInfo.total_mem_read = 0;

  /* make a copy in Sahara progress */
  memmove(&(SaharaProgress.data.sahara_MemDumpInfo),
          &(SaharaMemInfo), sizeof(sahara_memdump_info_t));
}

/***********
*
* Name:     sahara_process_mem_region_data
* 
* Purpose:  Process the raw data packet containing the region data
*
* Parms:    None
*
* Return:   None
*
* Abort:    none
*
* Notes:    none
*
************/
void sahara_process_mem_region_data()
{
  uint32 end_addr;
  uint32 i;

  sahara_memory_region_t *mem_regionp = &SaharaMemInfo.mem_region[SaharaMemInfo.current_region];
  static FILE *fout = NULL;
  char filename[MAX_PATH_NAME] = {0};

  /* calculate the end address for this memory region */
  end_addr = mem_regionp->addr + mem_regionp->len;

  /* The start address for this packet should have been mem_regionp->next_read
   * and the packet length should be sSerialInfo.readLen */

  /* Open file if this is first read */
  if (mem_regionp->addr == mem_regionp->next_read)
  {
    /* Check if output path is specified */
    if(memdump_path != NULL)
    {
      strcpy(filename, memdump_path);
      strcat(filename,"/");
      strncat(filename, (const char *)mem_regionp->filename, MAX_PATH_NAME - strlen(filename) - 1);
      fout = fopen((const char *)filename, "wb");
    }
    else
    {
      fout = fopen((const char *)mem_regionp->filename, "wb");
    }
    if (NULL == fout)
    {
      fprintf(stderr, "\nCould not open output file %s\n", mem_regionp->filename);
      return;
    }
  }

  /* write packet to file */
  if (fout != NULL)
  {
    for (i = 0; i<sSerialInfo.readLen; i++)
    {
      fputc(sSerialInfo.read[i], fout);
    }
    SaharaMemInfo.total_mem_read += sSerialInfo.readLen;
    /* calculate progress */
    SaharaProgress.progress = (uint8)((100 * (SaharaMemInfo.total_mem_read >> 8)) / (SaharaMemInfo.total_mem_size >> 8));
  }
  else
  {
    fprintf(stderr, "\nOutput file %s is not opened\n", mem_regionp->filename);
    mStopFlag = TRUE;
    return;
  }

  /* Close file if this is the last read */
  if (mem_regionp->next_read + sSerialInfo.readLen >= end_addr)
  {
    fclose(fout);
    fout = NULL;

    /* Set region info to read the next region */
    SaharaMemInfo.current_region += 1;
  }
  else
  {
    /* increment the read index */
    mem_regionp->next_read += sSerialInfo.readLen;
  }
}

/***********
*
* Name:     sahara_SendMemTableReadReq
* 
* Purpose:  Construct the memory read packet to read the memory table
*
* Parms:    None
*
* Return:   Expected length of the next packet received
*
* Abort:    none
*
* Notes:    none
*
************/
int sahara_SendMemTableReadReq(uint32 addr, uint32 len)
{
  memory_read_pkt_t memory_read_packet;
  memory_read_pkt_t *memory_read_packetp = &memory_read_packet;

  /* Initialize the packet */
  memory_read_packet.command = htole32(SAHARA_CMD_MEMORY_READ);
  memory_read_packet.len = htole32(LEN_SAHARA_CMD_MEMORY_READ);

  memory_read_packet.mem_addr = htole32(addr);
  memory_read_packet.mem_len = htole32(len);

  memmove(sSerialInfo.write, memory_read_packetp, LEN_SAHARA_CMD_MEMORY_READ);
  sSerialInfo.writeLen = LEN_SAHARA_CMD_MEMORY_READ;
  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);

  return htole32(memory_read_packet.mem_len);
}

/*************
*
* Name:     sahara_SendNextReadReq
* 
* Purpose:  Construct the memory read packet for reading memory region
*
* Parms:    resend_last_request - TRUE to idicate re-sending the last memory
*                                 read packet
*
* Return:   Length of memory requested.
*
* Abort:    none
*
* Notes:    none
*
**************/
int sahara_SendNextReadReq(bool resend_last_request)
{
  uint32 end_addr;
  static uint32 read_len;
  static memory_read_pkt_t memory_read_packet;
  memory_read_pkt_t *memory_read_packetp = &memory_read_packet;

  if (TRUE == resend_last_request)
  {
    SWI_PRINT_INFO("\nResending request for memory region %d. Address: %08X, length %08X\n",
                    SaharaMemInfo.current_region + 1,
                    (unsigned int)memory_read_packet.mem_addr,
                    (unsigned int)memory_read_packet.mem_len);
    Ramdump_WriteOnDMPort((char *)memory_read_packetp, memory_read_packetp->len);
    return read_len;
  }

  /* Initialize the packet */
  memory_read_packet.command = htole32(SAHARA_CMD_MEMORY_READ);
  memory_read_packet.len = htole32(LEN_SAHARA_CMD_MEMORY_READ);

  /* Start address */
  memory_read_packet.mem_addr = htole32(SaharaMemInfo.mem_region[SaharaMemInfo.current_region].next_read);
  /* Read size */
  end_addr = SaharaMemInfo.mem_region[SaharaMemInfo.current_region].addr +
             SaharaMemInfo.mem_region[SaharaMemInfo.current_region].len;
  if (end_addr - SaharaMemInfo.mem_region[SaharaMemInfo.current_region].next_read > SAHARA_MAX_MEM_READ_LEN)
  {
    read_len = SAHARA_MAX_MEM_READ_LEN;
  }
  else
  {
    read_len = end_addr - SaharaMemInfo.mem_region[SaharaMemInfo.current_region].next_read;
  }

  memory_read_packet.mem_len = htole32(read_len);

  SWI_PRINT_INFO("\nRequesting memory region %d. Address: %08X, length %08X\n",
                  SaharaMemInfo.current_region + 1,
                  (unsigned int)memory_read_packet.mem_addr,
                  (unsigned int)memory_read_packet.mem_len);

  memmove(sSerialInfo.write, memory_read_packetp, LEN_SAHARA_CMD_MEMORY_READ);
  sSerialInfo.writeLen = LEN_SAHARA_CMD_MEMORY_READ;
  Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);

  return read_len;
}

/***********
*
* Name:     sahara_StartRamDump
* 
* Purpose:  Start the memory dump session using Sahara protocol
*
* Parms:    portnum    - [IN] User specified port number.  Use -1 to force autodetection.
*           noreset    - [IN] Specify whether to reset device after memory dump
*
* Return:   eRamDumpStatus - Enum indicating success or failure cause
*
* Abort:    none
*
* Notes:    none
*
************/
enum eRamDumpStatus sahara_StartRamDump(
  int portnum,
  bool noreset)
{
  #define MAX_PKT_ERROR_COUNT  1000
  enum eRamDumpState MemDebugState = STATE_SAHARA_WAIT_HELLO;
  enum eRamDumpStatus status = eRAM_DUMP_STATUS_OK;
  int error_count = 0;
  int pkt_error_count = 0;
  bool waiting_raw_data = FALSE;
  uint32 commandID;
  uint32 pkt_len;
  uint32 mem_table_addr = 0;
  uint32 mem_table_len = 0;
  time_t startTime, currTime;
  time_t tLastRead;
  int last_read_len_request = 0;
  #ifndef RAMDUMP_WIN32
  int sleepVal = 0;
  #endif
  #ifdef SWI_DEBUG
  uint16 i;
  FILE *fout;
  #endif

  memory_debug_pkt_t *debug_packetp;
  hello_pkt_t *hello_packetp;

  /* Initialize the serial port */
  status = Ramdump_OpenDMPort(portnum);
  if (status != eRAM_DUMP_STATUS_OK)
  {
    return status;
  }

  /* Initialize the stopFlag */
  mStopFlag = FALSE;

  /* prompt the target to send hello. This is not mentioned in the state chart 
   * but the target seems to respond to cmd_switch_mode with a hello packet when
   * it is in wait_hello_response state.
   * Addendum: QCT code mentions that target will resend the hello packet if it
   * receives and invalid command while waiting for hello response
   */
  last_read_len_request = sahara_send_cmd_switch_mode_pkt();

  time(&startTime);
  time(&tLastRead);

  /* start of protocol based on state chart */
  while (1)
  {
    /* Check for break condition */
    if (TRUE == mStopFlag)
    {
      Ramdump_CloseDMPort();
      mStopFlag = FALSE;
      break;
    }

    #ifndef RAMDUMP_WIN32
    /* Wait for a few millisecond. No timer exist for reading and writing to
     * Linux Com port. Thus explicitly wait for few millisecond. The sleep time
     * depends on the maximum sahara packet length. Some values that work:
     *    len 0x0FF0, sleep 6 ms
     *    len 0x0800, sleep 4 ms
     *    len 0x0400, sleep 3 ms
     */
    sleepVal = TIMER_1_MILLISEC * 6;
    usleep(sleepVal);
    #endif

    /* Read data sent by target */
    sSerialInfo.readLen = Ramdump_ReadFromDMPort((char *)sSerialInfo.read, MAX_READ_BUFFER_LEN);
    if (sSerialInfo.readLen > 0)
    {
      time(&tLastRead);
      if (!waiting_raw_data)
      {
        /* Check packet validity. There is usually no error with command packet */
        pkt_len = get4bytesField(&sSerialInfo.read[4]);
        if ((sSerialInfo.readLen != pkt_len))
        {
          /* There is a mismatch, resend previous packet */
          SWI_PRINT_INFO("\nMismatch in expected command packet length at state: %d\n", MemDebugState);
          pkt_error_count++;
          if (pkt_error_count > MAX_PKT_ERROR_COUNT)
          {
            Ramdump_CloseDMPort();
            return eRAM_DUMP_STATUS_FAIL_CRC;
          }
          Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
          continue;
        }
        else
        {
          pkt_error_count = 0;
        }

        /* command ID is the first 4 bytes for all command packet */
        commandID = get4bytesField(sSerialInfo.read);
        switch(commandID)
        {
          case SAHARA_CMD_HELLO:
            /* received a Hello packet from the target */
            hello_packetp = (hello_pkt_t *)sSerialInfo.read;
            SaharaMemInfo.protocolVer = htole32(hello_packetp->version);
            SaharaMemInfo.protocolMin = htole32(hello_packetp->version_compatible);
            SaharaMemInfo.maxWriteSize = htole32(hello_packetp->cmd_pkt_len);
            MemDebugState = STATE_SAHARA_WAIT_COMMAND;
            break;

          case SAHARA_CMD_MEMORY_DEBUG:
            /* Received memory debug command from the target with memory table */
            debug_packetp = (memory_debug_pkt_t *)sSerialInfo.read;
            /* extract info */
            mem_table_addr = htole32(debug_packetp->table_addr);
            mem_table_len = htole32(debug_packetp->table_len);
            MemDebugState = STATE_SAHARA_WAIT_MEMORY_TABLE;
            break;

          case SAHARA_CMD_END_TRANSFER_IMAGE:
            /* Some unexpected error occurred. State chart indicates that host 
             * should send a reset command and then exit. Target's state chart
             * also indicates that it will only wait for reset command if there
             * is an error
             */
             SWI_PRINTF("\nReceived End image transfer packet\n");
             print_packet(sSerialInfo.read, sSerialInfo.readLen);
             sahara_send_cmd_reset_pkt();
             status = eRAM_DUMP_STATUS_FAIL_INV_PARAM;
            break;

          case SAHARA_CMD_RESET_RESP:
            SWI_PRINTF("\nReceived reset response\n");
            mStopFlag = TRUE;
            continue;

          default:
            error_count++;
            SWI_PRINTF("\nReceived invalid command: %08X\n", (unsigned int)commandID);
            if (error_count > 10)
            {
              Ramdump_CloseDMPort();
              return eRAM_DUMP_STATUS_FAIL_INV_PARAM;
            }
            break;
        }
      }
      else /* we are waiting for raw data like mem table */
      {
        /* check if the packet is unexpectedly an End_image_Tx packet.
         * It means there was an error */
        commandID = get4bytesField(sSerialInfo.read);
        pkt_len = get4bytesField(&sSerialInfo.read[4]);
        /* this should only be true if there is a real error, or a freaky coincidence ... */
        if ((SAHARA_CMD_END_TRANSFER_IMAGE == commandID) && (LEN_SAHARA_CMD_END_IMAGE_TRANSFER == pkt_len)
             && (LEN_SAHARA_CMD_END_IMAGE_TRANSFER == sSerialInfo.readLen))
        {
          /* Can only reset if there is an error */
          SWI_PRINTF("\nReceived unexpected End Image transfer while transferring data\n");
          print_packet(sSerialInfo.read, sSerialInfo.readLen);
          sahara_send_cmd_reset_pkt();
          status = eRAM_DUMP_STATUS_FAIL_INV_PARAM;
        }
        /* Check if the raw data length is as expected */
        if (last_read_len_request != sSerialInfo.readLen)
        {
          /* length mismatch */
          pkt_error_count++;
          if (pkt_error_count > MAX_PKT_ERROR_COUNT)
          {
            sahara_send_cmd_switch_mode_pkt();
            return eRAM_DUMP_STATUS_FAIL_CRC;
          }
          SWI_PRINT_INFO("\nPacket lenth [%X] does not match last requested [%X] in state %d. Resend last request\n",
                          sSerialInfo.readLen, last_read_len_request, MemDebugState);
          Ramdump_WriteOnDMPort((char *)sSerialInfo.write, sSerialInfo.writeLen);
          continue; /* Go back to beginning of the loop */
        }
        else
        {
          pkt_error_count = 0;
        }

        switch(MemDebugState)
        {
          case STATE_SAHARA_WAIT_MEMORY_TABLE:
            /* we are waiting for the raw data on the memory table */

            /* verify the table and go to the next state: WAIT_MEMORY_REGION */
            sahara_verify_mem_region();

            #ifdef SWI_DEBUG
            /* write the table to a file */
            fout = fopen("MemTable.BIN","wb");
            if (NULL != fout)
            {
              for (i=0;i<sSerialInfo.readLen;i++)
              {
                fputc(sSerialInfo.read[i],fout);
              }
              fclose(fout);
              fout = NULL;
            }
            #endif
            /* report the memory table data */
            DoProgressUpdate(&SaharaProgress);
            waiting_raw_data = FALSE;
            MemDebugState = STATE_SAHARA_WAIT_MEMORY_REGION;
            break;

          case STATE_SAHARA_WAIT_MEMORY_REGION:
            /* We are waiting for the memory region data */
            /* process the data */
            sahara_process_mem_region_data();
            waiting_raw_data = FALSE;
            /* Check if we are done with collecting data */
            if (SaharaMemInfo.current_region > SaharaMemInfo.max_num_region)
            {
              MemDebugState = STATE_SAHARA_CLEANUP;
            }
            break;

          default:
            error_count++;
            SWI_PRINTF("\nInvalid state when waiting raw data\n");
            if (error_count > 10)
            {
              Ramdump_CloseDMPort();
              return eRAM_DUMP_STATUS_FAIL_INV_PARAM;
            }
            break;
        }
      }
    }
    else
    {
      /* If we get here, it means the previously sent packet failed to generate
       * a response */
      if (STATE_SAHARA_WAIT_HELLO == MemDebugState)
      {
        /* if we are still waiting for a hello packet, try sending a command 
         * switch mode packet to prompt for a response
         */
        last_read_len_request = sahara_send_cmd_switch_mode_pkt();
      }
      time(&currTime);
      /* Time out after 30 seconds of no response */
      if ((currTime - tLastRead) > 30)
      {
        fprintf(stderr, "\nNo communication received for 30 seconds. Aborting\n");
        Ramdump_CloseDMPort();
        return eRAM_DUMP_STATUS_FAIL_NO_RESPONSE;
      }
    }

    SaharaProgress.state = MemDebugState;

    /* Respond to target */
    switch (MemDebugState)
    {
      case STATE_SAHARA_WAIT_HELLO:
        /* time out from here if waiting for more than 10 seconds */
        time(&currTime);
        if ((currTime - startTime) > 10)
        {
          fprintf(stderr, "\nNo hello received for 10 seconds. Aborting\n");
          Ramdump_CloseDMPort();
          return eRAM_DUMP_STATUS_FAIL_RETRIES;
        }
        SaharaProgress.progress = 0;
        DoProgressUpdate(&SaharaProgress);
        break;

      case STATE_SAHARA_WAIT_COMMAND:
        /* respond to the hello */
        SWI_PRINT_INFO("\nSend hello response. Wait for Debug info\n");
        last_read_len_request = sahara_send_hello_resp_pkt();
        SaharaProgress.progress = 0;
        DoProgressUpdate(&SaharaProgress);
        break;

      case STATE_SAHARA_WAIT_MEMORY_TABLE:
        /* send a read request to read the memory table */
        SWI_PRINT_INFO("\nRequesting memory table\n");
        last_read_len_request = sahara_SendMemTableReadReq(mem_table_addr, mem_table_len);
        waiting_raw_data = TRUE;
        break;

      case STATE_SAHARA_WAIT_MEMORY_REGION:
        /* Send a read request for the memory regions */
        if ((SaharaMemInfo.current_region <= SaharaMemInfo.max_num_region))
        {
          last_read_len_request = sahara_SendNextReadReq(FALSE);
          waiting_raw_data = TRUE;
        }
        DoProgressUpdate(&SaharaProgress);
        break;

      case STATE_SAHARA_CLEANUP:
        /* send reset command? or comannd switch mode */
        SaharaProgress.progress=100;
        DoProgressUpdate(&SaharaProgress);
        status = eRAM_DUMP_STATUS_OK;
        if (TRUE == noreset)
        {
          (void)sahara_send_cmd_switch_mode_pkt();
          mStopFlag = TRUE;
        }
        else
        {
          printf("\nResetting device ...\n");
          sahara_send_cmd_reset_pkt();
        }
        break;

      default:
        /* we shouldn't get here unless there is some really weird error */
        (void)sahara_send_cmd_switch_mode_pkt();
        Ramdump_CloseDMPort();
        SWI_PRINTF("\nCurrently in invalid state. Unknown error\n");
        return eRAM_DUMP_STATUS_FAIL_INV_STATE;
        break;
    }

  } /* While (1) */
  return status;
}

/***********
*
* Name:     protocol_detect
* 
* Purpose:  Ping the device to detect the protocol it is using
*
* Parms:    portnum  [IN]  -  User defined port number. Use -1 for auto detection
*           status   [OUT] -  success/failure status
*
* Return:   protocol type
*
* Abort:    none
*
* Note:     This function must check for Sahara protocol first. According to 
*           state machine, if the device uses Sahara, it will enter a reset state
*           if it receives an invalid packet while waiting for hello response
***********/
enum eProtocol protocol_detect(
  int portnum,
  enum eRamDumpStatus *status)
{
  uint32 cmdID;
  uint8 *cmdbuf;

  *status = Ramdump_OpenDMPort(portnum);
  if (eRAM_DUMP_STATUS_OK != *status)
  {
    SWI_PRINTF("Failed to open port\n");
    return PROTOCOL_MAX;
  }
  SWI_PRINT_INFO("Pinging device...\n");
  
  /* Send cmd switch pkt first to see if the device uses Sahara */
  sahara_send_cmd_switch_mode_pkt();
  usleep(TIMER_1_MILLISEC * 5);
  sSerialInfo.readLen = Ramdump_ReadFromDMPort((char *)sSerialInfo.read, MAX_READ_BUFFER_LEN);

  /* If device indeed use Sahara (got a reply) */
  if (sSerialInfo.readLen > 0)
  {
    /* Check if we got a hello */
    cmdID = get4bytesField(&sSerialInfo.read[0]);
    if (SAHARA_CMD_HELLO == cmdID)
    {
      SWI_PRINT_INFO("Protocol is Sahara\n");
      return PROTOCOL_SAHARA;
    }
  }
  /* else check if it is DMSS protocol by sending no-op packet */
  SWI_PRINT_INFO("\nNo response for the sahara hello\n");

  Ramdump_WriteOnDMPort((char *)DMSS_CMDNOOP_PKT, sizeof(DMSS_CMDNOOP_PKT));
  usleep(TIMER_1_MILLISEC * 5);
  sSerialInfo.readLen = Ramdump_ReadFromDMPort((char *)sSerialInfo.read, MAX_READ_BUFFER_LEN);
  /* Do we have a reply? */
  if (sSerialInfo.readLen > 0)
  {
    /* frtran - this part of the code is copied from DMSS_StartRamDump function */
      /* If the first byte is the Async HDLC ESC character, the command type is the next byte */
      cmdbuf = (sSerialInfo.read[0] == ASYNC_HDLC_FLAG) ? &sSerialInfo.read[1] : &sSerialInfo.read[0];

      /* We got a response? */
      if (*cmdbuf == DMSS_CMDACK)
      {
        SWI_PRINT_INFO("Protocol is DMSS\n");
        return PROTOCOL_DMSS;
      }
  }
  /* If we get here, it means there is no response from neither */
  SWI_PRINTF("No ping response received for both Sahara or DMSS protocol\n");
  return PROTOCOL_MAX;
}

/************
 * Name:     Ram_Dump_procedure
 *
 * Purpose:  RAM Dump Capturing Thread procedure
 *
 * Params:   ptr [IN] - RAM Dump port
 *
 * Return:   None
 *
 * Notes:    None
 ************/
static void Ram_Dump_procedure()
{
  enum eRamDumpStatus status = eRAM_DUMP_STATUS_STATUS_MAX;

  printf("-----------\nRAMDUMPTOOL %s\n-----------\n", VERSION_TAG);

  /* Create output dir for mem dump */
  struct stat st = {0};
  if (NULL != memdump_path)
  {
    if (-1 == stat((const char *)memdump_path, &st))
    {
      printf("Creating output directory: %s\n", memdump_path);
      if( -1 == mkdir(memdump_path, S_IRWXU | S_IRWXG | S_IRWXO))
      {
        fprintf(stderr, "error creating dir. errno = %d\n", errno);
        /* resort to using system call to create directory*/
        memset(shell_cmd, 0, SHELL_CMD_MAX_LEN);
        strcpy(shell_cmd,"mkdir -p ");
        strncat(shell_cmd, memdump_path, MAX_PATH_NAME);
        system(shell_cmd);
      }
    }
#ifdef DBG
    else
    {
      fprintf (stderr, "stat result: %d\n", errno);
    }
#endif
  }

  printf( "Initiating RAM Dump capture...\n");

  /* Check protocol */
  if (PROTOCOL_AUTO == memdump_protocol)
  {
    printf("Detecting device ...\n");
    memdump_protocol = protocol_detect(memdump_comport, &status);
    if (eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND == status)
    {
      fprintf(stderr, "Could not find DM port\n");
      return;
    }
  }

  switch (memdump_protocol)
  {
    case PROTOCOL_DMSS:
      /* Start Capturing the RAM Dump */
      if (FALSE == bForceSlow)
        status = dmss_StartRamDump(TRUE);

      /* First check if we should retry without High Speed protocol enabled */
      if (eRAM_DUMP_STATUS_FAIL_RETRIES == status || bForceSlow)
      {
        /* Retry without using the HS protocol */
        status = dmss_StartRamDump(FALSE);
      }
      break;

    case PROTOCOL_SAHARA:
      /* Start ram dump with sahara protocol */
      status = sahara_StartRamDump(memdump_comport, memdump_noreset);
      break;

    default:
      fprintf(stderr, "Unable to detect protocol\n");
      status = eRAM_DUMP_STATUS_FAIL_GENERIC;
      break;
  }

  /* check RAM Dump status */
  switch (status)
  {
    case eRAM_DUMP_STATUS_OK:
      /* We've completed the session */
      printf( "RAM Dump capture SUCCESS!!\n");
      break;

    case eRAM_DUMP_STATUS_FAIL_PORT_NOT_FOUND:
      /* Port not detected */
      fprintf(stderr, "Unable to find DM port\n");
      break;

    case eRAM_DUMP_STATUS_FAIL_NO_RESPONSE:
      /* Port detected and opened, but no DM response */
      fprintf(stderr, "Modem not responding, please try again\n");
      break;

    case eRAM_DUMP_STATUS_FAIL_PORT_BUSY_DM:
      /* Generic detected, but not available to be opened */
      fprintf(stderr, "Modem COM port(s) in use.  Shutdown all modem"
              "applications and try again.\n");
      break;

    case eRAM_DUMP_STATUS_FAIL_CRC:
      /* Too many check sum error */
      fprintf(stderr, "Ram dump aborted due to excessive data error\n");
      break;

    case eRAM_DUMP_STATUS_FAIL_INV_PARAM:
    case eRAM_DUMP_STATUS_FAIL_INV_STATE:
    case eRAM_DUMP_STATUS_FAIL_RETRIES:
      fprintf(stderr, "Protocol Error: %d\n", status);
      break;

    case eRAM_DUMP_STATUS_FAIL_GENERIC:
    default:
      fprintf(stderr, "Failed\n");
      break;
  }

  /* Close the serial port if remained opened in case an error is encountered 
   * and mark the RAM Dump completion flag as TRUE. */
  Ramdump_CloseDMPort();
  RAMDumpCaptured = TRUE;
}

/************
 * Name:     usage
 *
 * Purpose:  Prints the format for using RAM Dump tool from command line.
 *
 * Params:   pProgname [IN] - Name of the program to be printed
 *
 * Return:   None
 *
 * Notes:    None
 ************/
static void usage(
  const char *pProgname)
{
  printf("-----------\nRAMDUMPTOOL %s\n-----------\n", VERSION_TAG);
  printf("%s:  %s [-d <device>] [-c <comport>] [-o <output directory>] [-noreset] [-s]\n",
          __func__, pProgname);
  printf("%s", help_str);
}

/************
 * Name:     isOpt
 *
 * Purpose:  Check if a command line argument is a valid argument
 *
 * Params:   argv  [IN] - argument
 *
 * Return:   TRUE if valid, else FALSE
 *
 * Notes:    None
 ************/
bool isOpt(
  const char *argv)
{
  if ((0 == strcmp(argv, PORT_OPT))
      || (0 == strcmp(argv, PATH_OPT))
      || (0 == strcmp(argv, NORESET_OPT))
      || (0 == strcmp(argv, HELP_OPT))
      || (0 == strcmp(argv, DEVICE_OPT))
      || (0 == strcmp(argv, SLOW_OPT)))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/************
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Params:   argc  [IN] - number of arguments
 *           argv  [IN] - argument string
 *
 * Return:   EXIT_SUCCESS on completion, EXIT_FAILURE if there is an error in
 *           command line arguments
 *
 * Notes:    None
 ************/
int main(
  int argc,
  const char *argv[])
{
  int i = 0;
  int comport = -1;
  bool com_input = FALSE;
  char path[MAX_PATH_NAME] = "MemDumpOut";
  char *pathp = path;
  bool path_input = FALSE;
  bool protocol_input = FALSE;
  bool noreset = FALSE;
  enum eProtocol protocol = PROTOCOL_AUTO;

  /* check arguments */
  for (i = 1; i < argc; i++)
  {
    /* Validate option, which starts with "-" */
    if ((0 == strncmp(argv[i], "-", 1)) && !isOpt(argv[i]))
    {
      fprintf(stderr, "%s is not a valid option\n", argv[i]);
      exit(EXIT_FAILURE);
    }
    else if (0 == strcmp(argv[i], HELP_OPT))
    {
      usage(argv[0]);
      exit(EXIT_SUCCESS);
    }
    /* check comport */
    else if (!com_input && (0 == strcmp(argv[i], PORT_OPT)))
    {
      /* make sure there is a next argument and next argument is not an
       * option */
      if ((i + 1 < argc) && !isOpt(argv[i + 1]))
      {
        com_input = TRUE;
        comport = atoi(argv[i + 1]);
        i++;                    /* Skip the next argument */
      }
      else
      {
        fprintf(stderr, "%s requires an argument\n", PORT_OPT);
        exit(EXIT_FAILURE);
      }
    }
    /* check path */
    else if (!path_input && (0 == strcmp(argv[i], PATH_OPT)))
    {
      if ((i + 1 < argc) && !isOpt(argv[i + 1]))
      {
        path_input = TRUE;
        memset(path, 0, MAX_PATH_NAME);
        if (NULL != strncpy(path, argv[i + 1], MAX_PATH_NAME))
        {
          pathp = path;
          i++;                  /* Skip the next argument */
        }
        else
        {
          fprintf(stderr, "Unexpected error\n");
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        fprintf(stderr, "%s requires an argument\n", PATH_OPT);
        exit(EXIT_FAILURE);
      }
    }
    /* check reset option */
    else if (!noreset && (0 == strcmp(argv[i], NORESET_OPT)))
    {
      noreset = TRUE;
    }
    /* Check protocol option (based on device) */
    else if (!protocol_input && (0 == strcmp(argv[i], DEVICE_OPT)))
    {
      protocol_input = TRUE;
      /* Check if there is a valid argument after */
      if ((i + 1 < argc) && !isOpt(argv[i + 1]))
      {
        if (0 == strcmp(argv[i + 1], DEVICE_9X15))
        {
          protocol = PROTOCOL_DMSS;
        }
        else if (0 == strcmp(argv[i + 1], DEVICE_9X30))
        {
          protocol = PROTOCOL_SAHARA;
        }
        else
        {
          fprintf(stderr, "Unknown device\n");
          exit(EXIT_FAILURE);
        }
        i++;                    /* skip over the next argument */
      }
      else
      {
        fprintf(stderr, "%s requires an argument\n", DEVICE_OPT);
        exit(EXIT_FAILURE);
      }
    }
    /* Check if low speed option is specified. Only relevant for DMSS */
    else if (0 == strcmp(argv[i], SLOW_OPT))
    {
      printf( "Force slow speed protocol\n");
      bForceSlow = TRUE;
    }
  }
  /* Set options according to arguments */
  memdump_protocol = protocol;
  memdump_noreset = noreset;
  memdump_comport = comport;
  memdump_path = pathp;

  Ram_Dump_procedure();

  fprintf(stderr, "Exit Application!\n");
  exit(EXIT_SUCCESS);
}
