/*
 *
 * Filename:    SMSSampleApp.c
 *
 * Purpose:     SMS Application
 *
 * Copyright: © 2011-2013 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include "SMSSampleApp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <unistd.h>

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/

/* path to sdk binary */
static char *sdkbinpath = NULL;

/* Device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];
#define MAX_COPY_BUF_SIZE 20
static  CHAR *copy_buf[MAX_COPY_BUF_SIZE];
static BOOL buf_allocated = FALSE;
static BYTE concatenated_sms_counter = 0;

/* SMS Message List */
messageLists *pMessageListsNV,*pMessageListsUIM;

const CHAR *memoryNV  = "NV";
const CHAR *memoryUIM = "UIM";

/* Stores value of parameters in case of callback is invoked */
BYTE  messageMode       = INVALID_TECH;
ULONG messageCount      = 0;
ULONG gMsgCountTag      = 0;
ULONG gMessageTag       = 0 ;
indexMapMessage indexmaps[256];

/******************************************************************************
                       Forward Declarations
******************************************************************************/
/* Read SMS with the provided Index */
void WGetSMS4Index();
void DisplayLongSMS(struct wcdmaLongMsgDecodingParams *WcdmaLongMsgDecodingParams);

/****************************************************************************
*                       Functions
*****************************************************************************/
/*
 * Name:     Octet2bin
 *
 * Purpose:  Converts an octet(ASCII) to a 8-Bit value
 *
 * Return:   BYTE
 *
 * Notes:    None
 */
BYTE Octet2bin(CHAR* octet)
{
    BYTE result = 0;

    if((octet[0]<48)||(octet[1]<48))
        return 0;
    if ( octet[0]>57 )
        result += octet[0]-55;
    else
        result += octet[0]-48;
    result = result<<4;
    if ( octet[1]>57 )
        result += octet[1]-55;
    else
        result += octet[1]-48;
    return result;
}

/*
 * Name:     GetMsgProtocol
 *
 * Purpose:  Fetch the message protocol used in device
 *
 * Return:   None
 *
 * Notes:    None
 */
void GetMsgProtocol()
{
    ULONG nRet;
    smsMsgprotocolResp MsgProtocol;
    BYTE  instanceSize = nMaxStrLen;
    struct RFBandInfoElements RFBandInfo[nMaxStrLen];

    nRet = SLQSSmsGetMessageProtocol(&MsgProtocol);
    if ( eQCWWAN_ERR_NONE == nRet )
    {
        if ( CDMA == MsgProtocol.msgProtocol )
        {
            messageMode = CDMA;
        }
        else if ( UMTS == MsgProtocol.msgProtocol )
        {
            messageMode = UMTS;
        }
        else
        {
            fprintf(stderr, "MessageProtocol not supported \n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        nRet = GetRFInfo(&instanceSize, RFBandInfo);
        if (!nRet)
        {
            if ( instanceSize > 0 && instanceSize < nMaxStrLen)
            {
                switch( RFBandInfo[0].radioInterface)
                {
                    case CDMA_1xRTT:
                    case CDMA_1xEVDO:
                        messageMode = CDMA;
                        break;

                    case GSM_RI:
                    case UMTS_RI:
                        messageMode = UMTS;
                    break;

                    case LTE_RI:
                    {
                        LTESysInfo         ltesi;
                        nasGetSysInfoResp  resp;
                        ULONG              nRet;

                        IMSARegistrationStatus imsResp;
                        BYTE  lImsRegStatus = 0xFF;
                        WORD  lImsRegErrCode = 0xFFFF;
                        ULONG lNewImsRegStatus = 0xFFFFFFFF;

                        /* set default message mode to UMTS, it will be overrided if special case hit */
                        messageMode = UMTS;
                        /* set all TLV of nasGetSysInfoResp to NULL */
                        memset(&resp, 0, sizeof(resp));
                        resp.pLTESysInfo = &ltesi;

                        memset(&imsResp, 0, sizeof(imsResp));
                        imsResp.pImsRegStatus    = &lImsRegStatus;
                        imsResp.pImsRegErrCode   = &lImsRegErrCode;
                        imsResp.pNewImsRegStatus = &lNewImsRegStatus;

                        nRet = SLQSNasGetSysInfo( &resp );
                        if ( nRet != eQCWWAN_ERR_NONE )
                        {
                            fprintf(stderr, "Cannot get NAS Sys Info!\n");
                        }
                        else
                        {
                            /* check if it is Verizon LTE network or not */
                            if ( (memcmp(&resp.pLTESysInfo->MCC[0], "311", 3) == 0) &&
                                 (memcmp(&resp.pLTESysInfo->MNC[0], "480", 3) == 0))
                            {
                                nRet = SLQSGetIMSARegStatus( &imsResp);
                                /* if IMS is not registered, message mode should be set to CDMA for fallback */
                                if ((nRet == eQCWWAN_ERR_NONE) && (*imsResp.pImsRegStatus == 0))
                                {
                                    messageMode = CDMA;
                                }
                                else if ((nRet == eQCWWAN_ERR_NONE) && (*imsResp.pImsRegStatus == 1))
                                {
                                    GetIMSSMSConfigParams resp;
                                    BYTE smsFormat;
        
                                    memset(&resp, 0, sizeof(resp));
                                    resp.pSMSFormat = &smsFormat;
        
                                    /* if IMS is registered, checked the SMS format it reported */
                                    nRet = SLQSGetIMSSMSConfig(&resp);
                                    /* SMS Format: 0 -3GPP2, 1 - 3GPP */
                                    if (nRet == eQCWWAN_ERR_NONE)
                                    {
                                        if ( *resp.pSMSFormat == 0)
                                        {
                                            messageMode = CDMA;
                                        }
                                        else
                                        {
                                            messageMode = UMTS;
                                        }
                                    }
                                }
                            }
                        }

                        break;
                    }

                    default:
                        free(sdkbinpath);
                        fprintf(stderr, "RF Technology is not supported\n");
                        exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            free(sdkbinpath);
            fprintf(stderr, "RF Technology could not be determined,please verify SIM\
                             functionality (at+cpin) and device settings\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Name:     ReadNewSMS
 *
 * Purpose:  Displays received SMS
 *
 * Return:   None
 *
 * Notes:
 */
void ReadNewSMS(ULONG storageType, ULONG msgIndex)
{
    ULONG messageSize = nMaxStrLen + 2;
    ULONG messageTag = 0,messageFormat = 0;
    BYTE  message[1000];
    CHAR  ascii[MAXTEXT]= {0};
    ULONG nRet;
    BYTE  SenderAddrLength = 16;
    BYTE  TextMsgLength    = 160;
    BYTE  SMSCAddrLength   = 16;
    BYTE  AddressType      = 0;
    CHAR  SenderAddr[16]   = {'\0'};
    CHAR  SMSCAddr[MAX_SMSC_LENGTH]     = {'\0'};
    BYTE  Refnum[5] = {'\0'};
    BYTE  Totalnum[5] = {'\0'};
    BYTE  Seqnum[5] = {'\0'};
    BOOL  IsUDHPresent;
    BYTE  i;

    /* Execute GetSMS API with storage index */
    nRet = SLQSGetSMS( storageType,
                       msgIndex,
                       &messageTag,
                       &messageFormat,
                       (ULONG *)&messageSize,
                       &message[0],
                       &messageMode);
    if ( !nRet )
    {
         fprintf( stderr, "\nSMS read is successful\n" );
    }
    else
    {
        fprintf(stderr,"\n UIM memory does not contain any messages\n");
    }

    /* Modify the tag of the Message that has been read */
    if( TAG_MSG_MT_UNREAD == messageTag )
    {
        SLQSModifySMSStatus( storageType,
                             msgIndex,
                             TAG_MSG_MT_READ,
                             &messageMode );
    }

    if ( UMTS == messageMode )
    {
        /* Structure to decode PDU message to ASCII */
        struct wcdmaLongMsgDecodingParams WcdmaLongMsgDecodingParams;

        WcdmaLongMsgDecodingParams.pMessage          = message;
        WcdmaLongMsgDecodingParams.pSenderAddrLength = &SenderAddrLength;
        WcdmaLongMsgDecodingParams.pTextMsgLength    = &TextMsgLength;
        WcdmaLongMsgDecodingParams.pScAddrLength     = &SMSCAddrLength;

        WcdmaLongMsgDecodingParams.pSenderAddr = SenderAddr;
        WcdmaLongMsgDecodingParams.pTextMsg    = ascii;
        WcdmaLongMsgDecodingParams.pScAddr     = SMSCAddr;
        WcdmaLongMsgDecodingParams.pReferenceNum = Refnum;
        WcdmaLongMsgDecodingParams.pTotalNum   = Totalnum;
        WcdmaLongMsgDecodingParams.pPartNum    = Seqnum;
        WcdmaLongMsgDecodingParams.pIsUDHPresent = &IsUDHPresent;

        nRet = SLQSWCDMADecodeLongTextMsg( &WcdmaLongMsgDecodingParams );

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr,"Failed to Read SMS\n");
            fprintf( stderr,"Failure Code: %u\n", nRet );
        }

        switch( nRet )
        {
            case eQCWWAN_ERR_SWISMS_MSG_CORRUPTED:
                fprintf( stderr,"Message is not available or is corrupted\n" );
                break;

            case eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED:
                fprintf( stderr," SMSC/Mobile number is not correct\n" );
                break;

            case eQCWWAN_ERR_BUFFER_SZ:
                fprintf( stderr," Internal Error - Can not read SMS \n" );
                break;

            case eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG:
                fprintf( stderr," Message exceeds 160 characters\n" );
                break;

            case eQCWWAN_ERR_NONE:
                if ( *WcdmaLongMsgDecodingParams.pIsUDHPresent)
                {
                    concatenated_sms_counter++;
                    if (!buf_allocated && (WcdmaLongMsgDecodingParams.pTotalNum!= NULL))
                    {
                        for ( i = 0; i < *WcdmaLongMsgDecodingParams.pTotalNum; i++)
                        {
                            copy_buf[i] = malloc( (*WcdmaLongMsgDecodingParams.pTextMsgLength)*sizeof(CHAR) );
                            memset(copy_buf[i], 0, (*WcdmaLongMsgDecodingParams.pTextMsgLength)*sizeof(CHAR));
                            buf_allocated = TRUE;
                        }
                    }
                    strcpy(copy_buf[*WcdmaLongMsgDecodingParams.pPartNum-1], WcdmaLongMsgDecodingParams.pTextMsg);
                    if (concatenated_sms_counter == (*WcdmaLongMsgDecodingParams.pTotalNum))
                    {
                        DisplayLongSMS(&WcdmaLongMsgDecodingParams);
                    }
                }

                else
                {
                    AddressType = Octet2bin(WcdmaLongMsgDecodingParams.pScAddr);
                    /* It is an international number add '+' in pScAddr */
                    if (AddressType == 0x91)
                        fprintf( stderr, "\nService Center Number = +%s\n",
                                 WcdmaLongMsgDecodingParams.pScAddr+2);
                    else
                        fprintf( stderr, "\nService Center Number = %s\n",
                                 WcdmaLongMsgDecodingParams.pScAddr+2 );

                    AddressType = Octet2bin(WcdmaLongMsgDecodingParams.pSenderAddr);

                    /* It is an international number add '+' in pSendAddr */
                    if (AddressType == 0x91)
                        fprintf( stderr, "\nSender's Number = +%s\n",
                                 WcdmaLongMsgDecodingParams.pSenderAddr+2);
                    else
                        fprintf( stderr, "\nSender's Number = %s\n",
                                 WcdmaLongMsgDecodingParams.pSenderAddr+2 );

                    fprintf( stderr, "Date = %s\n",
                             WcdmaLongMsgDecodingParams.Date );
                    fprintf( stderr, "Time = %s\n",
                             WcdmaLongMsgDecodingParams.Time );
                    fprintf( stderr, "Message: %s\n",
                             WcdmaLongMsgDecodingParams.pTextMsg );
                }
                break;

            default:
                break;
        }
       fprintf(stderr,"\n");
    }
    if ( CDMA == messageMode )
    {
        struct cdmaMsgDecodingParams CdmaMsgDecodingParams;

        memset(&CdmaMsgDecodingParams, 0, sizeof(CdmaMsgDecodingParams));
        CdmaMsgDecodingParams.pMessage = message;
        CdmaMsgDecodingParams.messageLength = messageSize;
        CdmaMsgDecodingParams.pSenderAddr = SenderAddr;
        CdmaMsgDecodingParams.pSenderAddrLength = &SenderAddrLength;
        CdmaMsgDecodingParams.pTextMsg = (WORD*)ascii;
        CdmaMsgDecodingParams.pTextMsgLength = &TextMsgLength;

        nRet = SLQSCDMADecodeMTTextMsg(&CdmaMsgDecodingParams);

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr,"Failed to Read SMS\n");
            fprintf( stderr,"Failure Code: %u\n", nRet );
        }
        switch( nRet )
        {
            case eQCWWAN_ERR_SWISMS_MSG_CORRUPTED:
                fprintf( stderr,"Message is not available or is corrupted\n" );
                break;

            case eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED:
                fprintf( stderr," SMSC/Mobile number is not correct\n" );
                break;

            case eQCWWAN_ERR_BUFFER_SZ:
                fprintf( stderr," Internal Error - Can not read SMS \n" );
                break;

            case eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG:
                fprintf( stderr," Message exceeds 160 characters\n" );
                break;

            case eQCWWAN_ERR_NONE:
                /* Print SMS details */
                fprintf(stderr,"Sender Number: %s\n",CdmaMsgDecodingParams.pSenderAddr);
                fprintf(stderr,"SMS Text: %s\n",(CHAR*)CdmaMsgDecodingParams.pTextMsg);
                fprintf(stderr,"Time Stamp: %d:%d:%d:%d:%d:%d\n",
                                CdmaMsgDecodingParams.mcTimeStamp[0],
                                CdmaMsgDecodingParams.mcTimeStamp[1],
                                CdmaMsgDecodingParams.mcTimeStamp[2],
                                CdmaMsgDecodingParams.mcTimeStamp[3],
                                CdmaMsgDecodingParams.mcTimeStamp[4],
                                CdmaMsgDecodingParams.mcTimeStamp[5]);
                break;
             default:
                break;
        }
    }
}

/*
 * Name:     FlushStdinStream
 *
 * Purpose:  Flush the stdin stream
 *
 * Return:   None
 *
 * Notes:    fflush does not work for input stream.
 */
void FlushStdinStream( )
{
    ULONG inputChar;

    /* keep on reading until an <New Line> or end of file is received */
    do
    {
        inputChar = getchar();

#ifdef DBG
    fprintf( stderr,  "inputChar: %x\n", inputChar );
#endif
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != (ULONG)EOF )       &&
            ( inputChar != ESCAPE_KEY) );
}

/*
 * Name:     StartSDK
 *
 * Purpose:  It starts the SDK by setting the SDK path, enumerates the device
 *           and connects to the SDK.
 *
 * Return:   eQCWWAN_ERR_NONE on successfully starting SDK, else error code
 *
 * Notes:    none
 */
ULONG StartSDK()
{
    ULONG resultCode = 0;
    BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE !=
       (resultCode = SetSDKImagePath(sdkbinpath)) )
    {
        fprintf(stderr, "Failed to set the SDK Image path (%x)\n", resultCode);
        return resultCode;
    }

    if( eQCWWAN_ERR_SWICM_SOCKET_IN_USE == (resultCode = SLQSStart(0, NULL)) )
    {
        fprintf(stderr, "Another APP is currently using the SDK (%x)\n", resultCode);
        return resultCode;
    }
    else if( eQCWWAN_ERR_NONE != resultCode )
    {
        return resultCode;
    }

    /* Enumerate the device */
    while (QCWWAN2kEnumerateDevices(&devicesSize, (BYTE *)pdev) != 0)
    {
        printf ("\nUnable to find device..\n");
        sleep(1);
    }

#ifdef DBG
    fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
            devicesSize,
            pdev->deviceNode,
            pdev->deviceKey );
#endif

    /* Connect to the SDK */
    resultCode = QCWWANConnect( pdev->deviceNode,
                                pdev->deviceKey );
    return resultCode;
}

/*
 * Name:     ListAllMessages
 *
 * Purpose:  Allocated memory and stores SMS List
 *
 * Return:   None
 *
 * Notes:    none
 */
void ListAllMessages()
{
    messageLists *pAllMessages;
    ULONG        tempSizeNV  = 0;
    ULONG        tempSizeUIM = 0;
    USHORT       count;
    USHORT       msgCount;
    ULONG        RequestedTag;
    ULONG        messageListSizeNV  = nMaxStrLen;
    ULONG        messageListSizeUIM = nMaxStrLen;
    ULONG        nRet;
    memset(indexmaps, 0, 256*sizeof(indexMapMessage));

    /* Allocate memory for NV storage type */
    pAllMessages = (messageLists *)malloc(4*nMaxStrLen*sizeof(messageLists));

    /* Check if memory allocation is successful or not */
    if( !pAllMessages )
    {
        fprintf(stderr,"Memory allocation failed:");
        return;
    }

    for( count = 0; count < 4 ; count++)
    {
        RequestedTag       = count;
        messageListSizeNV  = nMaxStrLen;
        messageListSizeUIM = nMaxStrLen;

        nRet = SLQSGetSMSList(STORAGEINDEX_NV,
                              &RequestedTag,
                              &messageListSizeNV,
                              (BYTE*)(pAllMessages+ tempSizeNV),
                              &messageMode );
        if( nRet )
        {
             messageListSizeNV = 0;
        }
        tempSizeNV = tempSizeNV + messageListSizeNV;
    }

    if(tempSizeNV)
    {
        RequestedTag       = count;
        messageListSizeNV  = nMaxStrLen;
        messageListSizeUIM = nMaxStrLen;
        for(msgCount = 0; msgCount < tempSizeNV; msgCount++)
        {
            indexmaps[msgCount].memoryType    = "NV";
            indexmaps[msgCount].originalIndex =
                                pAllMessages[msgCount].messageIndex;
            indexmaps[msgCount].mappedIndex   = msgCount;
            indexmaps[msgCount].messageTag    =
                                pAllMessages[msgCount].messageTag;
        }
    }

    /* Set memory buffer to zero to gather list for UIM */
    memset(pAllMessages,0,tempSizeNV);

    /* Get messages from UIM */
    for( count = 0; count < 4 ; count++)
    {
        RequestedTag       = count;
        messageListSizeNV  = nMaxStrLen;
        messageListSizeUIM = nMaxStrLen;

        nRet = SLQSGetSMSList(STORAGEINDEX_UIM,
                              &RequestedTag,
                              &messageListSizeUIM,
                              (BYTE*)(pAllMessages + tempSizeUIM),
                              &messageMode );
        if( nRet )
        {
            messageListSizeUIM = 0;
        }
        tempSizeUIM = tempSizeUIM + messageListSizeUIM;
    }

    if(tempSizeUIM)
    {
        for(msgCount = 0; msgCount < tempSizeUIM; msgCount++)
        {
             indexmaps[msgCount+tempSizeNV].memoryType = "UIM";
             indexmaps[msgCount+tempSizeNV].originalIndex =
                                        pAllMessages[msgCount].messageIndex;
             indexmaps[msgCount+tempSizeNV].mappedIndex =
                                            msgCount+tempSizeNV;
             indexmaps[msgCount+tempSizeNV].messageTag =
                                        pAllMessages[msgCount].messageTag;
        }
    }
    /* Free the allocated memory for message list */
    free( pAllMessages );

    messageCount = tempSizeNV + tempSizeUIM;
}

/*
 * Name:     Displaymsgs
 *
 * Purpose:  Display the message list depending on message tag
 *
 * Params:   pMessageTag[IN] - Tag of message to be displayed
 *
 * Return:   None
 *
 * Notes:    none
 */

ULONG Displaymsgs( ULONG *pMessageTag )
{
    ULONG msgCount;
    ULONG msgCountTag = 0;
    fprintf(stderr,"\nStorage type\t  messageIndex\t   Message tag\n");
    for(msgCount = 0; msgCount < messageCount; msgCount++)
    {
        if ( indexmaps[msgCount].messageTag == *pMessageTag )
        {
            fprintf(stderr,
                           "%6s",
                           indexmaps[msgCount].memoryType);
            fprintf(stderr,
                           "%14d",
                           indexmaps[msgCount].mappedIndex);
            fprintf(stderr,
                           "%18d\n",
                           indexmaps[msgCount].messageTag);
            msgCountTag++;
        }

        if( 4 == *pMessageTag )
        {
            fprintf(stderr,
                           "%6s",
                           indexmaps[msgCount].memoryType);
            fprintf(stderr,
                           "%14d",
                           indexmaps[msgCount].mappedIndex);
            fprintf(stderr,
                          "%18d\n",
                          indexmaps[msgCount].messageTag);
        }
    }
    if ( 0 == *pMessageTag ||
         1 == *pMessageTag ||
         2 == *pMessageTag ||
         3 == *pMessageTag )
    {
        /* Print Total size if list */
        fprintf(stderr,
                "Total SMS messages in the corresponding list = %u\n",
                msgCountTag);
    }

    gMsgCountTag = msgCountTag;
    if( 4 == *pMessageTag )
    {
        /* Print Total size if list */
        fprintf(stderr,
                "Total SMS messages in the corresponding list = %u\n",
                messageCount);
    }
    return messageCount;
}

void DisplayLongSMS(struct wcdmaLongMsgDecodingParams *pWcdmaLongMsgDecodingParams)
{
    CHAR tmp[2000] = {0};
    BYTE i;
    BYTE  AddressType = 0;

    for ( i = 0; i < *pWcdmaLongMsgDecodingParams->pTotalNum; i++)
    {
        strcat(tmp, copy_buf[i]);
    }

    AddressType = Octet2bin(pWcdmaLongMsgDecodingParams->pScAddr);
    /* It is an international number add '+' in pScAddr */
    if (AddressType == 0x91)
        fprintf( stderr, "\nService Center Number = +%s\n",
                 pWcdmaLongMsgDecodingParams->pScAddr+2);
    else
        fprintf( stderr, "\nService Center Number = %s\n",
                 pWcdmaLongMsgDecodingParams->pScAddr+2 );

    AddressType = Octet2bin(pWcdmaLongMsgDecodingParams->pSenderAddr);

    /* It is an international number add '+' in pSendAddr */
    if (AddressType == 0x91)
        fprintf( stderr, "\nSender's Number = +%s\n",
                 pWcdmaLongMsgDecodingParams->pSenderAddr+2);
    else
        fprintf( stderr, "\nSender's Number = %s\n",
                 pWcdmaLongMsgDecodingParams->pSenderAddr+2 );

    fprintf( stderr, "Date = %s\n",
             pWcdmaLongMsgDecodingParams->Date );
    fprintf( stderr, "Time = %s\n",
             pWcdmaLongMsgDecodingParams->Time );

    fprintf( stderr,"Message:%s\n", tmp );

    concatenated_sms_counter = 0;
    buf_allocated = FALSE;
    for ( i = 0; i < *pWcdmaLongMsgDecodingParams->pTotalNum; i++)
    {
        if (copy_buf[i] != NULL)
            free(copy_buf[i]);
    }
}

/*
 * Name:     ShowList
 *
 * Purpose:  Display the message list depending on memory type
 *
 * Params:   pMemoryType[IN] - Type of memory from which user want to see
 *                             the message
 *
 * Return:   None
 *
 * Notes:    None
 */
ULONG ShowList( CHAR *pMemoryType )
{
    ULONG msgCount;
    ULONG msgCountTag = 0;
    fprintf(stderr,"\nStorage type\t  messageIndex\t   Message tag\n");

    /* Update list with recently received message */
    ListAllMessages();
    for(msgCount = 0; msgCount < messageCount; msgCount++)
    {
        if ( *indexmaps[msgCount].memoryType == *pMemoryType )
        {
            fprintf(stderr,
                           "%6s",
                           indexmaps[msgCount].memoryType);
            fprintf(stderr,
                           "%14d",
                           indexmaps[msgCount].mappedIndex);
            fprintf(stderr,
                           "%18d\n",
                           indexmaps[msgCount].messageTag);
            msgCountTag++;
        }
    }
    return msgCountTag;
}

/*
 * Name:     NewSMSCallback
 *
 * Purpose:  CallBack function for NewSMSCallback
 *
 * Return:   None
 *
 * Notes:    none
 */
void NewSMSCallback(
    ULONG storageType,
    ULONG messageIndex )
{
    ReadNewSMS(storageType, messageIndex);
}

/*
 * Name:     ActivateSMSCallback
 *
 * Purpose:  To Activate NewSMSCallback
 *
 * Return:   None
 *
 * Notes:    none
 */
void ActivateSMSCallback()
{
    ULONG nRet;
    nRet = SetNewSMSCallback(&NewSMSCallback);
    if ( nRet != 0 )
    {
        fprintf( stderr,
                 "Registration for incoming SMS message"
                 " notifications failed: %u\n", nRet );
    }
    else
    {
        fprintf( stderr,
                 "Registration for incoming SMS message"
                 " notifications succeeded\n");
    }
}


void getEncodingscheme( CHAR *encodingscheme )
{
    CHAR  *pEndOfLine = NULL;
    BOOL  bFlag = FALSE;
    while(1)
    {
        fprintf( stderr,
                 "\nEnter the encoding scheme for message(0 for default 7Bit, 4 for 8 Bit, 8 for UCS2 Encoding )"\
                 " followed by <Enter> key:\n" );

        fgets( encodingscheme, OPTION_LEN, stdin);
        switch( encodingscheme[0] )
        {
            case '0':
            case '4':
            case '8':
                bFlag = TRUE;
                break;
            default:
                fprintf(stderr," Encoding Scheme entered is invalid!\n");
                break;
        }
        if( bFlag == TRUE )
        {
            break;
        }
    }

    pEndOfLine = strchr( encodingscheme, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
        FlushStdinStream();
    }

    if( encodingscheme == pEndOfLine )
        return;
}

BYTE getIsSmsOnIms()
{
    CHAR  *pEndOfLine = NULL;
    CHAR   szUserInput[OPTION_LEN];
    BOOL  bFlag = FALSE;
    BYTE iRet = 0xff;
    while(1)
    {
        fprintf( stderr,
                 "\nEnter the SMS is Over IMS (0 for default Not Specify , 1 for SMS Not on IMS, 2 for SMS on IMS )"\
                 " followed by <Enter> key:\n" );

        fgets( szUserInput, OPTION_LEN, stdin);
        switch( szUserInput[0] )
        {
            case '1':
                bFlag = TRUE;
                iRet = 0;
                break;
            case '2':
                bFlag = TRUE;
                iRet = 1;
                break;
            case '0':
                iRet = 0xff;
                bFlag = TRUE;
                break;
            default:
                break;
        }
        if( bFlag == TRUE )
        {
            break;
        }
    }

    pEndOfLine = strchr( szUserInput, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
        FlushStdinStream();
    }
    return iRet;
}

/*
 * Name:     WsendSMS
 *
 * Purpose:  Handle user-selected send SMS option
 *
 * Return:   None
 *
 * Notes:    none
 */
void WsendSMS()
{
    CHAR  *pEndOfLine = NULL;
    ULONG nRet;
    ULONG messageFailureCode;
    CHAR  messageFormat[4];
    CHAR  mobileNo[22] = {'\0'};
    CHAR  smsMessage[1000] = {'\0'};
    ULONG idx;
    ULONG flag = 0;
    CHAR  encodingscheme[OPTION_LEN] = {'\0'};
    BYTE   *pIsSsmOnIms = NULL;
    BYTE   iSmsOnIms = 0xff;

    while(1)
    {
        /* Take mobile as input from user */
        fprintf( stderr,
                 "Enter mobile number with <Country Code Digits>"
                 " followed by ENTER KEY:\n" );

        fgets( mobileNo, 22, stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( mobileNo, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }
        else
        {
            *pEndOfLine = 0;
        }
        if( mobileNo == pEndOfLine )
            return;

        for(idx = 0; idx <= 2; idx++)
        {
            if( !isdigit(mobileNo[idx]) && (mobileNo[idx] != '+'))
            {
                fprintf(stderr,"Please enter valid digits!\n\n");
                flag = 1;
                break;
            }
        }
        if ( 1 == flag )
        {
            flag = 0;
            continue;
        }
        if( strlen(mobileNo) > 20 )
        {
            fprintf(stderr,"Please enter a valid mobile number\n\n");
            mobileNo[20] = '\0';
            continue;
        }
        break;
    }

    fprintf(stderr,
                  "Enter the message to send"\
                  " followed by the <Enter> key\n" );

    fgets( smsMessage, 1000, stdin);
    /* If '/n' character is not read, there are more characters in input
     * stream. Clear the input stream.
     */
    pEndOfLine = strchr( smsMessage, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
        FlushStdinStream();
    }
    else
    {
        *pEndOfLine = 0;
    }

    if( smsMessage == pEndOfLine )
        return;

    /* get encoding method to determine the length of every concatenated message */
    getEncodingscheme(encodingscheme);

    while(1)
    {
        fprintf( stderr,
                 "Enter the message format(0 for CDMA, 6 for GSM/WCDMA)"\
                 " followed by <Enter> key:\n" );

        fgets( messageFormat, OPTION_LEN, stdin);
        if( '6' != messageFormat[0] && '0' != messageFormat[0] )
        {
            fprintf(stderr,"Message format entered is invalid!\n");
            continue;
        }
        break;
    }
    pEndOfLine = strchr( messageFormat, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
        FlushStdinStream();
    }
    iSmsOnIms = getIsSmsOnIms();
    if(iSmsOnIms!=0xff)
    {
        pIsSsmOnIms = &iSmsOnIms;
    }
    if( messageFormat == pEndOfLine )
        return;

    nRet = SLQSSendLongSMS( messageFormat[0] - 0x30,
                    strlen(smsMessage),
                    smsMessage,
                    encodingscheme[0] - 0x30,
                    &messageFailureCode,
                    mobileNo,
                    pIsSsmOnIms );
    if ( !nRet )
    {
        fprintf(stderr, "Send SMS Successful\n");
    }
    else
    {
        fprintf(stderr, "Message Failure reason : %u\n", messageFailureCode );
    }
 }

/*
 * Name:     WReadSMS
 *
 * Purpose:  Wrapper function for API GetSMS
 *
 * Return:   None
 *
 * Notes:    none
 */

void WReadSMS(void)
{
    ULONG  RequestedTag;
    CHAR   *pEndOfLine;
    CHAR   selOption[OPTION_LEN];

    while(1)
    {
        /* Update the list with recently received Messages */
        ListAllMessages();
        fprintf( stderr,
                 "\nSelect the type of message you want to read"\
                 " press <enter> to go back to main menu:\n"\
                 "1. Read messages\n"\
                 "2. Unread messages\n"\
                 "3. Mobile originated messages that have been sent to"
                 " the recipient\n"\
                 "4. Mobile originated messages that have yet to be sent"
                 " to the recipient\n"\
                 "Option: " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /*
         * If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        /* If user has entered an invalid input, prompt again */
        if( 2 < strlen(selOption) )
        {
            continue;
        }

        if( selOption[0] > '4' || selOption[0] == ENTER_KEY )
        {
            fprintf( stderr, "Back to main Menu!\n");
            return;
        }

        RequestedTag = selOption[0] - eREAD_MESSAGES;
        gMessageTag = RequestedTag;
        if( 0 == Displaymsgs( &RequestedTag) )
            continue;

        WGetSMS4Index();

    }
}

BYTE GetStorageTypeFromIndex(ULONG index)
{
   BYTE storageType = 0;

   if (strcmp(indexmaps[index].memoryType, "UIM") == 0)
   {
       storageType = 0;
   }
   else if (strcmp(indexmaps[index].memoryType, "NV") == 0)
   {
       storageType = 1;
   }
   return storageType;
}

void DecodeSMS(BYTE messageMode, BYTE messageSize, BYTE *pMessage)
{
    CHAR  ascii[MAXTEXT]= {'\0'};
    BYTE  SenderAddrLength = 16;
    BYTE  TextMsgLength = 160;
    CHAR  SenderAddr[16] = {'\0'};
    BYTE  SMSCAddrLength   = 16;
    CHAR  SMSCAddr[MAX_SMSC_LENGTH]     = {'\0'};
    BYTE AddressType = 0;
    ULONG nRet;

    if ( UMTS == messageMode )
    {
        /* Structure to decode PDU message to ASCII */
        struct wcdmaMsgDecodingParams WcdmaMsgDecodingParams;

        WcdmaMsgDecodingParams.pMessage          = pMessage;
        WcdmaMsgDecodingParams.pSenderAddrLength = &SenderAddrLength;
        WcdmaMsgDecodingParams.pTextMsgLength    = &TextMsgLength;
        WcdmaMsgDecodingParams.pScAddrLength     = &SMSCAddrLength;

        WcdmaMsgDecodingParams.pSenderAddr = SenderAddr;
        WcdmaMsgDecodingParams.pTextMsg    = ascii;
        WcdmaMsgDecodingParams.pScAddr     = SMSCAddr;

        nRet = SLQSWCDMADecodeMTTextMsg( &WcdmaMsgDecodingParams );

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr,"Failed to Read SMS \n");
            fprintf( stderr,"Failure Code: %u\n", nRet );
        }

        switch( nRet )
        {
            case eQCWWAN_ERR_SWISMS_MSG_CORRUPTED:
                fprintf( stderr,"Message is not available or is corrupted\n" );
                break;

            case eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED:
                fprintf( stderr," SMSC/Mobile number is not correct\n" );
                break;

            case eQCWWAN_ERR_BUFFER_SZ:
                fprintf( stderr," Internal Error - Can not read SMS \n" );
                break;

            case eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG:
                fprintf( stderr," Message exceeds 160 characters\n" );
                break;

            case eQCWWAN_ERR_NONE:
                AddressType = Octet2bin(WcdmaMsgDecodingParams.pScAddr);
                /* It is an international number add '+' in pScAddr */
                if (AddressType == 0x91)
                    fprintf( stderr, "\nService Center Number = +%s\n",
                             WcdmaMsgDecodingParams.pScAddr+2);
                else
                    fprintf( stderr, "\nService Center Number = %s\n",
                             WcdmaMsgDecodingParams.pScAddr+2 );

                AddressType = Octet2bin(WcdmaMsgDecodingParams.pSenderAddr);

                /* It is an international number add '+' in pSendAddr */
                if (AddressType == 0x91)
                    fprintf( stderr, "\nSender's Number = +%s\n",
                             WcdmaMsgDecodingParams.pSenderAddr+2);
                else
                    fprintf( stderr, "\nSender's Number = %s\n",
                             WcdmaMsgDecodingParams.pSenderAddr+2 );

                fprintf( stderr, "Date = %s\n",
                         WcdmaMsgDecodingParams.Date );
                fprintf( stderr, "Time = %s\n",
                         WcdmaMsgDecodingParams.Time );
                fprintf( stderr, "Message: %s\n",
                         WcdmaMsgDecodingParams.pTextMsg );
                fprintf( stderr, "Message Length: %d\n",
                         *WcdmaMsgDecodingParams.pTextMsgLength );
                //printMsg();
                break;

            default:
                break;
        }
       fprintf(stderr,"\n");
    }

    else if ( CDMA == messageMode )
    {
        struct cdmaMsgDecodingParams CdmaMsgDecodingParams;
        
        memset(&CdmaMsgDecodingParams, 0, sizeof(CdmaMsgDecodingParams));
        CdmaMsgDecodingParams.pMessage = pMessage;
        CdmaMsgDecodingParams.messageLength = messageSize;
        CdmaMsgDecodingParams.pSenderAddr = SenderAddr;
        CdmaMsgDecodingParams.pSenderAddrLength = &SenderAddrLength;
        CdmaMsgDecodingParams.pTextMsg = (WORD*)ascii;
        CdmaMsgDecodingParams.pTextMsgLength = &TextMsgLength;

        nRet = SLQSCDMADecodeMTTextMsg(&CdmaMsgDecodingParams);

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr,"Failed to Read SMS \n");
            fprintf( stderr,"Failure Code: %u\n", nRet );
        }
        switch( nRet )
        {
            case eQCWWAN_ERR_SWISMS_MSG_CORRUPTED:
                fprintf( stderr,"Message is not available or is corrupted\n" );
                break;

            case eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED:
                fprintf( stderr," SMSC/Mobile number is not correct\n" );
                break;

            case eQCWWAN_ERR_BUFFER_SZ:
                fprintf( stderr," Internal Error - Can not read SMS \n" );
                break;

            case eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG:
                fprintf( stderr," Message exceeds 160 characters\n" );
                break;

            case eQCWWAN_ERR_NONE:
                /* Print SMS details */
                fprintf(stderr,"Sender Number: %s\n",CdmaMsgDecodingParams.pSenderAddr);
                fprintf(stderr,"SMS Text: %s\n",(CHAR*)CdmaMsgDecodingParams.pTextMsg);
                fprintf(stderr,"Time Stamp: %d:%d:%d:%d:%d:%d\n",
                                CdmaMsgDecodingParams.mcTimeStamp[0],
                                CdmaMsgDecodingParams.mcTimeStamp[1],
                                CdmaMsgDecodingParams.mcTimeStamp[2],
                                CdmaMsgDecodingParams.mcTimeStamp[3],
                                CdmaMsgDecodingParams.mcTimeStamp[4],
                                CdmaMsgDecodingParams.mcTimeStamp[5]);
                break;
             default:
                break;
        }
    }
}

/*
 * Name:     WGetSMS4Index
 *
 * Purpose:  Read SMS with the provided Index
 *
 * Return:   None
 *
 * Notes:    none
 */
void WGetSMS4Index()
{
    ULONG idx;
    ULONG nRet;
    ULONG flag = 0;
    ULONG messageTag;
    ULONG messageFormat;
    ULONG messageIndex;
    CHAR  Option[OPTION_LEN];
    ULONG messageSize = nMaxStrLen + 2;
    BYTE  message[nMaxStrLen + 2]= {'\0'};
    ULONG strorageType;

    while(1)
    {
        /* reset message buffer and size for every single read */
        memset(message, 0, messageSize);
        messageSize = nMaxStrLen + 2;

        if ( 0 == gMsgCountTag )
        {
            break;
        }
        fprintf(stderr,"\nEnter the 'Message Index' of the SMS"
                       " message to read or press <Enter> to go back:\n");

        /* Receive the input from the user */
        fgets( Option, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        if( Option[0] == ENTER_KEY)
        {
            return;
        }
        if( isdigit(Option[0]))
        {
            messageIndex = atoi(Option);
            for( idx = 0; idx < messageCount ; idx++ )
            {
                if (indexmaps[idx].mappedIndex == messageIndex)
                {
                    messageIndex = indexmaps[idx].originalIndex;
                    flag = 1;
                    break;
                }
            }
            if ( !flag )
            {
                fprintf(stderr,"This Index not present in list!\n");
                continue;
            }
            flag = 0;
        }
        else
        {
            fprintf(stderr,"Index entered is not correct!\n");
            continue;
        }

        strorageType = GetStorageTypeFromIndex(idx);

        nRet = SLQSGetSMS( strorageType,
                           messageIndex,
                           &messageTag,
                           &messageFormat,
                           (ULONG *)&messageSize,
                           &message[0],
                           &messageMode );
        if ( eQCWWAN_ERR_NONE ==  nRet )
        {
            fprintf( stderr,
                     "Read from %s memory successful\nMessage Tag: %u\n"\
                     "Message Format: %u\n",
                      strorageType == 0 ? "UIM" : "NV",\
                      gMessageTag, messageFormat );

            /* Decode the SMS and display it */ 
            DecodeSMS(messageMode, messageSize, message);

            /* Modify the tag of the Message that has been read */
            if( TAG_MSG_MT_UNREAD == gMessageTag )
            {
                SLQSModifySMSStatus( strorageType,
                                     messageIndex,
                                     TAG_MSG_MT_READ,
                                     &messageMode );
            }

            /* Update the list of messages as tags have been modified*/
            ListAllMessages();

            /* Display Message list */
            Displaymsgs(&gMessageTag);
        }
        else
        {
            fprintf( stderr, "SLQSGetSMS failed! Error code = %u\n", nRet);
        }

    }
}

/*
 * Name:     ExecuteDeleteSMS
 *
 * Purpose:  Deletes SMS with provided parameter
 *
 * Params:   select[IN] - Indicates below memory type
 *                        1 - UIM memory
 *                        2 - NV memory
 * Return:   None
 *
 * Notes:    none
 */

void ExecuteDeleteSMS( CHAR select )
{
    ULONG nRet;
    ULONG messageIndex;
    ULONG StorageType = -1;
    CHAR  Option[4];
    ULONG flag = 0;
    ULONG idx = 0;

    /* Execute only when option 2 is selected */
    while(1)
    {
        if ( select <= '2' )
        {
            if ( '1' == select )
            {
                 if( 0 == ShowList("UIM") )
                 {
                     fprintf(stderr," UIM Memory does not contain any messages!\n");
                     return;
                 }
                 else
                 {
                     /* Initialize storage type with UIM memory */
                     StorageType = STORAGEINDEX_UIM;
                 }
            }
            if ( '2' == select )
            {
                 if( 0 == ShowList("NV") )
                 {
                     fprintf(stderr," NV Memory does not contain any messages!\n");
                     return;
                 }
                 else
                 {
                     /* Initialize storage type with NV memory */
                     StorageType = STORAGEINDEX_NV;
                 }

            }
            fprintf(stderr,"Enter Index of message to delete"
                           " or press <Enter> to exit:\n");

            /* Receive the input from the user */
            fgets( Option, ( OPTION_LEN ), stdin );

            /* If '/n' character is not read, there are more characters in input
             * stream. Clear the input stream.
             */
            if ( Option[0] == ENTER_KEY )
            {
                return;
            }
            if( isdigit(Option[0]))
            {
                messageIndex = atoi(Option);
                for( idx = 0; idx < messageCount ; idx++ )
                {
                    if ( indexmaps[idx].mappedIndex == messageIndex )
                    {
                        messageIndex = indexmaps[idx].originalIndex;
                        flag = 1;
                        break;
                    }
                }
                if ( !flag )
                {
                    fprintf(stderr,"This Index not present in list!\n");
                    continue;
                }
                flag = 0;
            }
            else
            {
                fprintf(stderr,"Index entered is not correct!\n");
            }

            nRet = SLQSDeleteSMS( StorageType,
                                  &messageIndex,
                                  NULL,
                                  &messageMode );
            if (!nRet)
            {
                fprintf(stderr,"Message Deleted\n");
            }
            else
            {
                fprintf(stderr,"Message Deletion unsuccessful\n");
            }
        }
        else
        {
            break;
        }

    }

    if ( '3' == select )
    {
        StorageType = 0;
        nRet = SLQSDeleteSMS( StorageType,
                              NULL,
                              NULL,
                             &messageMode );
           if (!nRet)
       {
           fprintf(stderr,"All Messages in UIM Memory Deleted\n");
       }
       else
       {
           fprintf(stderr,"SMS Deletion unsuccessful\n");
       }
   }
   if ( '4' == select )
   {
       StorageType = 1;
       nRet = SLQSDeleteSMS( StorageType,
                             NULL,
                             NULL,
                            &messageMode );
       if (!nRet)
       {
           fprintf(stderr,"All Messages in NV Memory Deleted\n");
       }
       else
       {
           fprintf(stderr,"SMS Deletion unsuccessful\n");
       }
  }

}

/*
 * Name:     WDeleteMessage
 *
 * Purpose:  Displays Menu for delete message
 *
 * Return:   None
 *
 * Notes:    none
 */

void WDeleteMessage()
{
    CHAR  *pEndOfLine;
    CHAR  selOption[OPTION_LEN];

    while(1)
    {
        /* Update List with recently received message */
        ListAllMessages();
        fprintf( stderr,
                 "\nSelect the type of message you want to delete"\
                 " press <enter> to go back to main menu:\n"\
                 "1. Delete a specific SMS message from UIM memory\n"\
                 "2. Delete a specific SMS message from NV memory\n"\
                 "3. Delete all the messages stored in UIM memory\n"\
                 "4. Delete all the messages stored in NV memory\n"\
                 "Option: " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /*
         * If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        /* If user has entered an invalid input, prompt again */
        if( 2 < strlen(selOption) )
        {
            continue;
        }

        if( selOption[0] > '4' || selOption[0] == ENTER_KEY )
        {
            fprintf( stderr, "Back to main Menu!\n");
            return;
        }
        ExecuteDeleteSMS(selOption[0]);
    }
}

/*
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Return:   EXIT_SUCCESS, EXIT_FAILURE on unexpected error
 *
 * Notes:    none
 */
int main(int argc, const char *argv[])
{
    ULONG resultCode = 0;
    CHAR  selOption[OPTION_LEN];
    CHAR  *pEndOfLine = NULL;

   if( argc < 2 )
    {
        fprintf( stderr, "usage: %s <path to sdk binary>\n", argv[0] );
        exit( EXIT_SUCCESS );
    }

    if( NULL == (sdkbinpath = malloc(strlen(argv[1]) + 1)) )
    {
        perror(__func__);
        exit( EXIT_FAILURE );
    }

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1);

    /* Start the SDK */
    resultCode = StartSDK();
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        free(sdkbinpath);

        /* Display the failure reason */
        fprintf( stderr,
                 "Failed to start SDK: Exiting App\n"\
                 "Failure Code: %u\n", resultCode );

        /* Failed to start SDK, exit the application */
        exit( EXIT_FAILURE );
    }

    GetMsgProtocol();

    while(1)
    {
         /* Checks for Notificationflag to read next message */
         fprintf( stderr,
                  "\nPlease select one of the following options or"
                  " press <Enter> to exit:\n"\
                  "1. Register CallBack\n"\
                  "2. Send SMS\n"\
                  "3. Read SMS\n"\
                  "4. Delete SMS\n"\
                  "Option: " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }
        /* If user has entered an invalid input, prompt again */
        if( 2 < strlen(selOption) )
        {
            continue;
        }

        /* Execute the option provided by user */
        switch( selOption[0] )
        {
            case eREGISTER_CALLBACK:
                ActivateSMSCallback();
                break;

            case eSEND_SMS:
                WsendSMS();
                break;

            case eREADSMS:
                WReadSMS();
                break;

            case eDELETESMS:
                WDeleteMessage();
                break;

            case eEXIT_APP:
                free(sdkbinpath);
                QCWWANDisconnect();
                exit(EXIT_SUCCESS);
                break;

            default:
               break;
        }
    }
}
