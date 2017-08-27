/**************
 *
 * Filename:    connectionmgr.c
 *
 * Purpose:     Connection Manager application
 *
 * Copyright: © 2011-2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include "displaymgmt.h"
#include "qaGobiApiWds.h"
#include "qaGobiApiQos.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "qmerrno.h"

/****************************************************************
*                       DEFINES
****************************************************************/
#define SUCCESS                  0
#define FAIL                     1
#define ENTER_KEY                0x0A
#define ENTER_KEY_PRESSED        0
#define OPTION_LEN               5
#define IP_ADDRESS_LEN           15
#define IPADDREESS_OCTET_MASK    0x000000FF
#define PDP_IPV4                 0
#define IPv4_FAMILY_PREFERENCE   0x04
#define IPv6_FAMILY_PREFERENCE   0x06
#define IPv4v6_FAMILY_PREFERENCE 0x07
#define MAX_FIELD_SIZE           128
#define POWER_OFF_MODE           3
#define POWER_RESET_MODE         4

/* APN, User Name and Profile name size should be 3 greater than the actual
 * characters to be read. This will help to ensure that user should not enter
 * more than maximum allowed characters.
 */
#define MAX_APN_SIZE            104
#define MAX_PROFILE_NAME_SIZE   17
#define MAX_USER_NAME_SIZE      129
#define DEV_NODE_SZ             256
#define DEV_KEY_SZ              16
#define RESP_BUFFER_LEN         128
#define MAX_PROFILES            16
#define PROFILE_TYPE_UMTS       0
#define MIN_PROFILES            1
#define START_DATA_SEESION      1
#define STOP_DATA_SEESION       0
#define TECHNOLOGY_3GPP         1
#define TECHNOLOGY_3GPP2        2
#define TEN_SECONDS_TIMER       10
#define ERR_GENERAL             1
#define ERR_ENUM_END            34
/* Note: only two instances are supported */
#define MIN_INST_ID             0
#define MAX_INST_ID             1

#define nMaxStrLen              0xFF

#define CDMA_PROFILE_OFFSET     (100)

/* macros*/
#define rcprint(s, u) syslog( LOG_USER, "%s: rc = 0x%X", s, u )

/****************************************************************
*                       DATA STRUCTURE
****************************************************************/

/* Device information structure */
typedef struct device_info_param{
  CHAR deviceNode[DEV_NODE_SZ];
  CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/* Profile Information structure */
struct profileInformation{
    ULONG profileType;
    ULONG PDPType;
    ULONG IPAddress;
    ULONG primaryDNS;
    ULONG secondaryDNS;
    ULONG Authentication;
    CHAR  profileName[MAX_PROFILE_NAME_SIZE];
    CHAR  APNName[MAX_APN_SIZE];
    CHAR  userName[MAX_USER_NAME_SIZE];
    CHAR  password[MAX_FIELD_SIZE];
};

/* Profile indexes for profile existing on device */
struct profileIndexesInfo{
    BYTE profileIndex[MAX_PROFILES];
    BYTE totalProfilesOnDevice;
};

/* session state enumeration */
enum eSessionStates{
    eSTATE_DISCONNECTED = 1,
    eSTATE_CONNECTED,
    eSTATE_SUSPENDED,
    eSTATE_AUTHENTICATING,
};

/* Data bearer Enumeration */
enum eDataBearer{
    eBEARER_CDMA_RTT = 1,
    eBEARER_CDMA_REV_0,
    eBEARER_GPRS,
    eBEARER_WCDMA,
    eBEARER_CDMA_REV_A,
    eBEARER_EDGE,
    eBEARER_HSDPA,
    eBEARER_WCDMA_DL,
    eBEARER_HSDPA_DL,
    eBEARER_LTE,
    eBEARER_CDMA_EHRPD,
    eBEARER_HSDPA_PLUS_WCDMA,
    eBEARER_HSDAP_PLUS_HSUPA,
    eBEARER_DCHSDAP_PLUS_WCDMA,
    eBEARER_DCHSDAP_PLUS_HSUPA,
    eBEARER_HSDPA_PLUS_64QAM,
    eBEARER_HSDPA_PLUS_HSUPA,
    eBEARER_TDSCDMA,
    eBEARER_TDSCDMA_HSDPA
};

/* Dormancy status enumeration */
enum eDormancyStatus{
    eTRAFFIC_CHANNEL_DORMANT = 1,
    eTRAFFIC_CHANNEL_ACTIVE,
};

/* Radio Interface enumeration */
enum eRadioInterface{
    eNO_SERVICE,
    eCDMA_1xRTT,
    eCDMA_1xEVDO,
    eAMPS,
    eGSM,
    eUMTS,
    eWLAN,
    eGPS,
    eLTE,
};

/* Roaming Indicator enumeration */
enum eRoamingIndicator{
    eROAMING,
    eHOME,
    eROAMING_PARTNER,
};

/* User options enumeration */
enum eUserOptions{
    eSTART_UMTS_DATA_SESSION = 1,
    eSTART_LTE_DATA_SESSION,
    eSTART_CDMA_DATA_SESSION,
    eSTART_CDMA_RUIM_DATASESSION,
    eSTOP_DATA_SESSION,
    eDISPLAY_ALL_PROFILES,
    eDISPLAY_SINGLE_PROFILE,
    eCREATE_PROFILE,
    eMODIFY_PROFILE_SETTINGS,
    eDELETE_PROFILE,
    eRESET_DEVICE,
    eSET_POWER_MODE,
    eQOS_TESTING
};

/* SDK Process Verification */
enum eCheckSDK{
    eVERIFY_SDK_PROCESS = 0x31
};

/****************************************************************
*                    GLOBAL DATA
****************************************************************/
/* path to sdk binary */
static char *sdkbinpath = NULL;

/* device connectivity */
static struct profileIndexesInfo indexInfo;
static device_info_t             devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t             *pdev = &devices[0];
static BOOL                      devready = 0;
struct ssdatasession_params sumts[MAX_INST_ID+1];
struct ssdatasession_params slte[MAX_INST_ID+1];
static BYTE sessionNum = 0;
static BYTE sessionNumLte = 0;


/****************************************************************
*              FORWARD FUNCTION DECLARATION
****************************************************************/
static void DisplayDeviceinfo( void );
static void DisplayProfileInfo( void );
static void DisplayAllProfiles( void );
static void IPUlongToDot( ULONG IPAddress, char* pIPAddress );
static void UnSubscribeCallbacks();
int GetInstanceId();
typedef void( *sighandler_t )( int );

/****************************************************************
*                       FUNCTIONS
****************************************************************/

/****************************************************************
*                    COMMON FUNCTIONS
****************************************************************/
/*
 * Name:     FlushStdinStream
 *
 * Purpose:  Flush the stdin stream
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    fflush does not work for input stream.
 */
void FlushStdinStream( )
{
    int inputChar;

    /* keep on reading until a <New Line> or end of file is received */
    do
    {
        inputChar = getchar();

        #ifdef DBG
        fprintf( stderr,  "inputChar: 0x%x\n", inputChar );
        #endif
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != EOF ) );
}

/*
 * Name:     QuitApplication
 *
 * Purpose:  Closes the Application
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void QuitApplication()
{
    free(sdkbinpath);
    fprintf( stderr, "Exiting Application!!!\n" );
    UnSubscribeCallbacks();
    closeLogFile();
    QCWWANDisconnect();
    exit( EXIT_SUCCESS );
}

/*************************************************************************
 *                         Call Back Functions
 ************************************************************************/
/*
 * Name:     DevStateChgCbk
 *
 * Purpose:  Device State change callback
 *
 * Params:   devstatus - the current state of the device
 *
 * Return:   None
 *
 * Notes:    None
 */
void DevStateChgCbk( eDevState devstatus )
{
    BYTE count;
    /* If device is ready to communicate */
    if( devstatus ==  DEVICE_STATE_READY )
    {
        DisplayDeviceinfo();
        UpdateUserDisplay( eDEVICE_STATE, "DEVICE CONNECTED" );
        devready = 1;
    }
    else if( devstatus ==  DEVICE_STATE_DISCONNECTED )
    {
        /* Device is disconnected, Reset all the display fields */
        ResetDisplay();
        for( count = 0; count <= MAX_INST_ID; count++ )
        {
            slte[count].v4sessionId = 0;
            sumts[count].v6sessionId = 0;
        }

        /* Reset Global varibales related to session if device resets */
        sessionNum = 0;
        sessionNumLte = 0;
        UpdateUserDisplay( eDEVICE_STATE, "DEVICE DISCONNECTED" );
    }
    else
    {
        ResetDisplay();
        UpdateUserDisplay( eDEVICE_STATE, "UNKNOWN" );
    }
}

void NetChgCbk( ULONG q_depth, BYTE isThrottle, BYTE instance )
{
    syslog(LOG_DEBUG, "q_depth = %d  isThrottle = %u instance = %u\n", q_depth, isThrottle, instance);
}

/*
 * Name:     LURejectCallback
 *
 * Purpose:  LU Reject Callback function
 *
 * Params:   serviceDomain - Service domain
 *                           - 1 - Circuit Switched
 *           rejectCause   - Reject cause
 *                           - See 3GPP TS 24.008, Section 4.4.4.7
 *
 * Return:   None
 *
 * Notes:    None
 */
void LURejectCallback(
   ULONG serviceDomain,
   ULONG rejectCause )
{
    CHAR LURRejectBuf[10];

    UNUSEDPARAM ( serviceDomain );

    /* Update the LU reject reason field of the user window */
    snprintf( LURRejectBuf,sizeof(LURRejectBuf)-1, "%u", rejectCause );
    UpdateUserDisplay( eLU_REJECT, LURRejectBuf );
}

/*
 * Name:     SignalStrengthCallback
 *
 * Purpose:  Signal Strength Callback function
 *
 * Params:   signalStrength - Received signal strength (in dBm)
 *           radioInterface - Radio interface technology of the signal being
 *                            measured
 *
 * Return:   None
 *
 * Notes:    None
 */
void SignalStrengthCallback(
   INT8  signalStrength,
   ULONG radioInterface )
{
    CHAR signalStrengthBuf[10];

    UNUSEDPARAM ( radioInterface );

    /* Update the Signal Strength field of the user window */
    sprintf( signalStrengthBuf, "%d", signalStrength );
    UpdateUserDisplay( eSIGNAL_STRENGTH, signalStrengthBuf );
}

/*
 * Name:     DisplayRadioInterface
 *
 * Purpose:  Display the radio interface field in the log file
 *
 * Params:   radioInterface - Radio interface technology of the signal being
 *                            measured
 * Return:   None
 *
 * Notes:    None
 */
void DisplayRadioInterface( ULONG radioInterface )
{
    /* Update the Radio Interface in logs */
    if( eNO_SERVICE == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "NO SERVICE" );
    }
    else if( eCDMA_1xRTT == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "CDMA 1xRTT" );
    }
    else if( eCDMA_1xEVDO == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "CDMA 1xEV-DO" );
    }
    else if( eAMPS == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "AMPS" );
    }
    else if( eGSM == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "GSM" );
    }
    else if( eUMTS == radioInterface )
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "UMTS" );
    }
    else if( eWLAN == radioInterface)
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "WLAN" );
    }
    else if( eGPS == radioInterface)
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "GPS" );
    }
    else if( eLTE == radioInterface)
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "LTE" );
    }
    else
    {
        UpdateUserDisplay( eRADIO_INTERFACE, "UNKNOWN" );
    }
}

/*
 * Name:     RFInfoCallback
 *
 * Purpose:  RF Information Callback function
 *
 * Params:   radioInterface  - Radio interface technology of the signal being
 *                             measured
 *           activeBandClass - Active band class
 *           activeChannel   - Active channel
 *                             - 0 - Channel is not relevant to the reported
 *                                   technology
 *
 * Return:   None
 *
 * Notes:    None
 */
void RFInfoCallback(
   ULONG radioInterface,
   ULONG activeBandClass,
   ULONG activeChannel )
{
    CHAR activeBand[5];

    UNUSEDPARAM ( activeChannel );

    /* Update the Radio Interface field in logs */
    DisplayRadioInterface( radioInterface );

    /* Update the Active Band field in logs */
    snprintf( activeBand,sizeof(activeBand)-1, "%u", activeBandClass );
    UpdateUserDisplay( eACTIVE_BAND_CLASS, activeBand );

    /* Update the Active Channel field in logs */
    UpdateUserDisplay( eACTIVE_CHANNEL, "0" );
}

/*
 * Name:     WDSCallback
 *
 * Purpose:  WDS functionality such as TX/RX byte count, Data Bearer, Dormancy Status Callback function
 *
 * Return:   None
 *
 * Notes:    None
 */
void WDSCallback( slqsWdsEventInfo *pWdsEventInfo )
{
    CHAR  bytesCount[MAX_FIELD_SIZE];

    /* Update the TX bytes */
    sprintf( bytesCount, "%llu",
            (pWdsEventInfo->pTotalBytesTX!=NULL)? *(pWdsEventInfo->pTotalBytesTX) : 0 );
    UpdateUserDisplay( eTX_BYTES, bytesCount );

    /* Reset the buffer */
    memset( bytesCount, 0, MAX_FIELD_SIZE );

    /* Update the RX bytes */
    sprintf( bytesCount, "%llu",
            (pWdsEventInfo->pTotalBytesRX!=NULL)? *(pWdsEventInfo->pTotalBytesRX) : 0 );
    UpdateUserDisplay( eRX_BYTES, bytesCount );

    /* Update the Data Bearer field in logs */
    if ( NULL != pWdsEventInfo->pDataBearer )
    {
        switch (*(pWdsEventInfo->pDataBearer))
        {
            case eBEARER_CDMA_RTT:
               UpdateUserDisplay( eDATA_BEARER, "CDMA 1xRTT" );
                  break;

            case eBEARER_CDMA_REV_0:
               UpdateUserDisplay( eDATA_BEARER, "CDMA 1xEV-DO Rev 0" );
                  break;

            case eBEARER_GPRS:
               UpdateUserDisplay( eDATA_BEARER, "GPRS" );
                  break;

            case eBEARER_WCDMA:
               UpdateUserDisplay( eDATA_BEARER, "WCDMA" );
                  break;

            case eBEARER_CDMA_REV_A:
               UpdateUserDisplay( eDATA_BEARER, "CDMA 1xEV-DO Rev A" );
                  break;

            case eBEARER_EDGE:
               UpdateUserDisplay( eDATA_BEARER, "EDGE" );
                  break;

            case eBEARER_HSDPA:
               UpdateUserDisplay( eDATA_BEARER, "HSDPA DL, WCDMA UL" );
                  break;

            case eBEARER_WCDMA_DL:
               UpdateUserDisplay( eDATA_BEARER, "WCDMA DL, HSUPA UL" );
                  break;

            case eBEARER_LTE:
               UpdateUserDisplay( eDATA_BEARER, "LTE" );
                  break;

            case eBEARER_CDMA_EHRPD:
               UpdateUserDisplay( eDATA_BEARER, "CDMA EHRPD" );
                  break;

            case eBEARER_HSDPA_PLUS_WCDMA:
               UpdateUserDisplay( eDATA_BEARER, "HSDPA+ and WCDMA" );
                  break;

            case eBEARER_HSDAP_PLUS_HSUPA:
               UpdateUserDisplay( eDATA_BEARER, "HSDPA+ and HSUPA" );
                  break;

            case eBEARER_DCHSDAP_PLUS_WCDMA:
               UpdateUserDisplay( eDATA_BEARER, "DC_HSDPA+ and WCDMA" );
                  break;

            case eBEARER_DCHSDAP_PLUS_HSUPA:
               UpdateUserDisplay( eDATA_BEARER, "DC_HSDAP+ and HSUPA" );
                  break;

            case eBEARER_HSDPA_PLUS_64QAM:
               UpdateUserDisplay( eDATA_BEARER, "HSDPA+ and 64QAM" );
                  break;

            case eBEARER_HSDPA_PLUS_HSUPA:
               UpdateUserDisplay( eDATA_BEARER, "HSDPA+, 64QAM and HSUPA" );
                  break;

            case eBEARER_TDSCDMA:
               UpdateUserDisplay( eDATA_BEARER, "TDSCDMA" );
                  break;

            case eBEARER_TDSCDMA_HSDPA:
               UpdateUserDisplay( eDATA_BEARER, "TDSCDMA and HSDPA" );
                  break;

            default:
                  UpdateUserDisplay( eDATA_BEARER, "UNKNOWN" );
                  break;
        }

    }

    /* Update the Dormancy Status field in logs */
    if ( NULL != pWdsEventInfo->pDormancyStatus )
    {
        if( eTRAFFIC_CHANNEL_DORMANT ==  *(pWdsEventInfo->pDormancyStatus) )
        {
            UpdateUserDisplay( eDORMANCY_STATUS, "CHANNEL DORMANT" );
        }
        else if( eTRAFFIC_CHANNEL_ACTIVE ==  *(pWdsEventInfo->pDormancyStatus) )
        {
            UpdateUserDisplay( eDORMANCY_STATUS, "CHANNEL ACTIVE" );
        }
        else
        {
            UpdateUserDisplay( eDORMANCY_STATUS, "UNKOWN" );
        }
    }

}

static ULONG QosRequest()
{
    ULONG   resultCode;

    swiQosIds qosIds;
    ULONG    ids[2];
    qosIds.pIds = ids;
    qosIds.sz = sizeof(ids);

    swiQosReq qosReq;

    swiQosFlow txFlow;
    tokenBucket bucket;
    dataRate rate;

    ULONG   latency = 150;
    ULONG   jitter = 50;
    BYTE    lteQci = 2;
    memset(&qosReq, 0, sizeof(qosReq));
    memset(&txFlow, 0, sizeof(txFlow));
    qosReq.pTxFlow = &txFlow;

    txFlow.index = 100;
    txFlow.pLatency = &latency;
    txFlow.pJitter = &jitter;
    txFlow.pLteQci = &lteQci;

    bucket.peakRate = 100;
    bucket.tokenRate = 200;
    bucket.bucketSz = 300;
    txFlow.pTokenBucket = &bucket;

    rate.dataRateMax = 100;
    rate.guaranteedRate = 200;
    txFlow.pDataRate = &rate;
    resultCode = SLQSQosReq(0, &qosReq, &qosIds );
    if( eQCWWAN_ERR_NONE == resultCode )
    {
        int i;
        for(i=0;i<qosIds.sz;i++)
        {
            fprintf(stderr, "Qos id %u\n", qosIds.pIds[i]);
        }
        return qosIds.pIds[0];
    }
    return ~0;
}

static void QosRelease(ULONG qos_id)
{
    swiQosIds qosIds;
    ULONG ids[1];
    ids[0] = qos_id;
    qosIds.sz = sizeof(ids)/sizeof(ULONG);
    qosIds.pIds = ids;
    SLQSQosRel(0, &qosIds);
}

static void QosGetGranted(ULONG qos_id)
{
    ULONG   resultCode;
    ULONG   latency, jitter;
    BYTE    lteQci;
    dataRate rate;
    swiQosGranted   granted;
    swiQosFlow      txFlow;
    memset(&granted, 0, sizeof(granted));
    memset(&txFlow, 0, sizeof(txFlow));
    txFlow.pLatency = &latency;
    txFlow.pJitter = &jitter;
    txFlow.pLteQci = &lteQci;
    txFlow.pDataRate = &rate;
    granted.pTxFlow = &txFlow;

    resultCode = SLQSQosGetGranted(0, qos_id, &granted );

    if( eQCWWAN_ERR_NONE == resultCode )
    {
        fprintf(stderr, "Qos Get Granted Tx flow index : %d\n", granted.pTxFlow->index);
        fprintf(stderr, "Qos Get Granted Tx flow latency : %u\n", *granted.pTxFlow->pLatency);
        fprintf(stderr, "Qos Get Granted Tx flow jitter : %u\n", *granted.pTxFlow->pJitter);
        fprintf(stderr, "Qos Get Granted Tx flow LTE QCI : %d\n", *granted.pTxFlow->pLteQci);
        fprintf(stderr, "Qos Get Granted Tx flow Max rate : %u\n",
                granted.pTxFlow->pDataRate->dataRateMax);
        fprintf(stderr, "Qos Get Granted Tx flow guaranteed rate : %u\n",
                granted.pTxFlow->pDataRate->guaranteedRate);
    }
}

/*
 * Name:     SessionStateCallback
 *
 * Purpose:  Session State Callback function
 *
 * Params:   state            - Current Link Status
 *                              - 1 Disconnected
 *                              - 2 Connected
 *                              - 3 Suspended (Unsupported)
 *                              - 4 Authenticating
 *           sessionEndReason - Call End Reason
 *
 * Return:   None
 *
 * Notes:    None
 */
void SessionStateCallback(
    slqsSessionStateInfo *pSessionInfo )
{
    UNUSEDPARAM ( pSessionInfo->sessionEndReason );

    /* Update the session state field in logs */
    if( eSTATE_DISCONNECTED == pSessionInfo->state)
    {
        UpdateUserDisplay( eSESSION_STATE, "DISCONNECTED" );
        UpdateUserDisplay( eCALL_STATUS, "DISCONNECTED" );
    }
    else if( eSTATE_CONNECTED == pSessionInfo->state)
    {
        UpdateUserDisplay( eSESSION_STATE, "CONNECTED" );
    }
    else if( eSTATE_SUSPENDED == pSessionInfo->state)
    {
        UpdateUserDisplay( eSESSION_STATE, "SUSPENDED" );
    }
    else if( eSTATE_AUTHENTICATING == pSessionInfo->state)
    {
        UpdateUserDisplay( eSESSION_STATE, "AUTHENTICATING" );
    }
    else
    {
        UpdateUserDisplay( eSESSION_STATE, "UNKNOWN" );
    }
}

/*
 * Name:     RoamingIndicatorCallbck
 *
 * Purpose:  Roaming Indicator Callback function
 *
 * Params:   roaming - Roaming Indication
 *                     - 0  - Roaming
 *                     - 1  - Home
 *                     - 2  - Roaming partner
 *                     - >2 - Operator defined values
 *
 * Return:   None
 *
 * Notes:    None
 */
void RoamingIndicatorCallbck( ULONG roaming )
{
    /* Update the roaming status */
    if( eROAMING == roaming )
    {
        UpdateUserDisplay( eROAMING_STATUS, "ON ROAMING" );
    }
    else if( eHOME == roaming )
    {
        UpdateUserDisplay( eROAMING_STATUS, "IN HOME" );
    }
    else if( eROAMING_PARTNER == roaming )
    {
        UpdateUserDisplay( eROAMING_STATUS, "ROAMING PARTNER" );
    }
    else
    {
        UpdateUserDisplay( eROAMING_STATUS, "UNKNOWN" );
    }
}

/*
 * Name:     RSSICallbck
 *
 * Purpose:  Received Signal Strength Information Callback function
 *
 * Params:   sRssiInfo - structure of type SLQSSignalStrengthsInformation
 *                       containing received signal strength information.
 *
 * Return:   None
 *
 * Notes:    None
 */
void RSSICallback(
    struct SLQSSignalStrengthsInformation sRssiInfo )
{
    CHAR respBuf[MAX_FIELD_SIZE];

    /* Reset the buffer and copy the data to be displayed */
    if( 0 != sRssiInfo.rxSignalStrengthInfo.rxSignalStrength )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",sRssiInfo.rxSignalStrengthInfo.rxSignalStrength );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
    }

    /* Update the value of ECIO */
    if( 0 != sRssiInfo.ecioInfo.ecio )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", sRssiInfo.ecioInfo.ecio );
        UpdateUserDisplay( eECIO, respBuf );
    }

    /* Reset the buffer and copy the data to be displayed */
    if( 0 != sRssiInfo.io )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%u", sRssiInfo.io );
        UpdateUserDisplay( eIO, respBuf );
    }

    /* Update the value of SINR */
    if( 0 != sRssiInfo.sinr )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", sRssiInfo.sinr );
        UpdateUserDisplay( eSINR, respBuf );
    }

    /* Update the value of RSRQ */
    if( 0 != sRssiInfo.rsrqInfo.rsrq )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", sRssiInfo.rsrqInfo.rsrq );
        UpdateUserDisplay( eRSRQ, respBuf );
    }
}
/*
 * Name:     SigInfoCallBack
 *
 * Purpose:  Received Signal Strength Information Callback function
 *
 * Params:   pInfo - structure of type Signal Info containing received
 *                   signal strength information.
 *
 * Return:   None
 *
 * Notes:    None
 */

void SigInfoCallBack(
        nasSigInfo *pInfo )
{
    CHAR respBuf[MAX_FIELD_SIZE];
    if ( pInfo->pCDMASigInfo )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pCDMASigInfo->rssi );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pCDMASigInfo->ecio );
        UpdateUserDisplay( eECIO, respBuf );
   }
    if ( pInfo->pHDRSigInfo )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pHDRSigInfo->rssi );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pHDRSigInfo->ecio );
        UpdateUserDisplay( eECIO, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", pInfo->pHDRSigInfo->sinr );
        UpdateUserDisplay( eSINR, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", pInfo->pHDRSigInfo->io );
        UpdateUserDisplay( eIO, respBuf );
    }
    if ( pInfo->pGSMSigInfo )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",*pInfo->pGSMSigInfo );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
    }
    if ( pInfo->pWCDMASigInfo )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pWCDMASigInfo->rssi );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pWCDMASigInfo->ecio );
        UpdateUserDisplay( eECIO, respBuf );
    }
    if ( pInfo->pLTESigInfo )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%d",pInfo->pLTESigInfo->rssi );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%d", pInfo->pLTESigInfo->rsrq);
        UpdateUserDisplay( eRSRQ, respBuf );
    }
    if ( pInfo->pTDSCDMASigInfoExt )
    {
        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%f",pInfo->pTDSCDMASigInfoExt->rssi );
        UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf,"%f",pInfo->pTDSCDMASigInfoExt->ecio );
        UpdateUserDisplay( eECIO, respBuf );

        memset( respBuf, 0, MAX_FIELD_SIZE );
        sprintf( respBuf, "%f", pInfo->pTDSCDMASigInfoExt->sinr );
        UpdateUserDisplay( eSINR, respBuf );
    }
}
/*
 * Name:     UnSubscribeCallbacks
 *
 * Purpose:  De register all the callbacks registered at the beginning.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
static void UnSubscribeCallbacks()
{
    ULONG                            resultCode = 0;
    BYTE                             instance;

    resultCode = SLQSSetSessionStateCallback( NULL );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for SessionStateCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    for ( instance = 0; instance <= MAX_INST_ID; instance++ )
    {
        resultCode = SLQSSetWdsEventCallback( NULL, 0, instance, IPv4v6_FAMILY_PREFERENCE );
        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Failed to Unsubscribe for DataBearerCallback\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            #endif
        }

        resultCode = SLQSSetWdsEventCallback( NULL, 10, instance, IPv4v6_FAMILY_PREFERENCE );
        if( SUCCESS != resultCode )
        {
             #ifdef DBG
             fprintf( stderr, "Failed to Unsubscribe for DormancyStatusCallback\n" );
             fprintf( stderr, "Failure Code : %u\n", resultCode );
             #endif
        }

        resultCode = SLQSSetWdsEventCallback( NULL, 0, instance, IPv4v6_FAMILY_PREFERENCE );
        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Failed to Unsubscribe for ByteTotalsCallback\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            #endif
        }
    }

    resultCode = SetRFInfoCallback( NULL );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for RFInfoCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetLURejectCallback( NULL );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for LURejectCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetDeviceStateChangeCbk( NULL );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for LURejectCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetRoamingIndicatorCallback( NULL );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for RoamingIndicatorCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SLQSNasSigInfo2CallBack(NULL,NULL);
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to Unsubscribe for SLQSNasSigInfo2CallBack\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }
}

/*
 * Name:     SubscribeCallbacks
 *
 * Purpose:  Subscribed to all the required callbacks from the device.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void SubscribeCallbacks()
{
    ULONG                            resultCode = 0;

    WORD              cdmaRssiThreshList[2] = { -1020,-400 };/* -102dB, -40dB */
    BYTE              cdmaRssiThreshListlen = 0x02;
    CDMARSSIThresh    cdmaRssiThresh;

    WORD              cdmaEcioThreshList[2] = { -400, -310 }; /* -20dB, -15.5dB */
    BYTE              cdmaEcioThreshListLen = 0x02;
    CDMAECIOThresh    cdmaEcioThresh;

    WORD              hdrRssiThreshList[2] = { -500, -150 }; /* -50dB, -15dB */
    BYTE              hdrRssiThresListLen = 2;
    HDRRSSIThresh     hdrRssiThresh;

    WORD              hdrEcioThreshList[2] = { -400, -310 }; /* -20dB, -15.5dB */
    BYTE              hdrEcioThresListLen = 2;
    HDRECIOThresh     hdrEcioThresh;

    WORD              hdrSinrThreshList[2] = { 0x01, 0x03 }; /* -6dB, -3dB */
    BYTE              hdrSinrThresListLen = 2;
    HDRSINRThreshold  hdrSinrThresh;

    WORD              hdrIoThreshList[2] = { -1110, -730 }; /* -110dB, -73dB */
    BYTE              hdrIoThresListLen = 2;
    HDRIOThresh       hdrIoThresh;

    WORD              gsmRssiThreshList[2] = { -950, -800 }; /* -95dB, -80dB */
    BYTE              gsmRssiThresListLen = 2;
    GSMRSSIThresh     gsmRssiThresh;

    WORD              wcdmaRssiThreshList[2] = { -1000, -200 }; /* -100dB, -20dB */
    BYTE              wcdmaRssiThresListLen = 2;
    WCDMARSSIThresh   wcdmaRssiThresh;

    WORD              wcdmaEcioThreshList[2] = { -400, -310 }; /* -20dB, -15.5dB */
    BYTE              wcdmaEcioThresListLen = 2;
    WCDMAECIOThresh   wcdmaEcioThresh;

    WORD              lteRssiThreshList[2] = { -1000, -400 }; /* -100dB, -40dB */
    BYTE              lteRssiThresListLen = 2;
    LTERSSIThresh     lteRssiThresh;

    WORD              lteSnrThreshList[2] = { -198, -230 }; /* -19.8dB, 23dB */
    BYTE              lteSnrThresListLen = 2;
    LTESNRThreshold   lteSnrThresh;

    WORD              lteRsrqThreshList[2] = { -110, -60 }; /* -11dB, -6dB */
    BYTE              lteRsrqThresListLen = 2;
    LTERSRQThresh     lteRsrqThresh;

    WORD              lteRsrpThreshList[2] = { -1250, -640 }; /* -125dB, -64dB */
    BYTE              lteRsrpThresListLen = 2;
    LTERSRPThresh     lteRsrpThresh;

    WORD              tdscdmaRscpThreshList[2] = { -950, -800 }; /* -95dB, -80dB */
    BYTE              tdscdmaRscpThresListLen = 2;
    TDSCDMARSCPThresh tdscdmaRscpThresh;

    float             tdscdmaRssiThreshList[2] = { -950, -800 }; /* -95dB, -80dB */
    BYTE              tdscdmaRssiThresListLen = 2;
    TDSCDMARSSIThresh tdscdmaRssiThresh;

    float             tdscdmaEcioThreshList[2] = { -400, -310 }; /* -20dB, -15.5dB */
    BYTE              tdscdmaEcioThresListLen = 2;
    TDSCDMAECIOThresh tdscdmaEcioThresh;

    float             tdscdmaSinrThreshList[2] = { 0x01, 0x03 };
    BYTE              tdscdmaSinrThresListLen = 2;
    TDSCDMASINRThresh tdscdmaSinrThresh;

    BYTE                rptRate = 2;
    BYTE                avgPeriod = 2;
    LTESigRptConfig     lteSigRptCfg;

    setSignalStrengthInfo      req;

    /* Initialize the structure */
    memset( (char*)&req, 0, sizeof(req) );

    /* Assign request parameters */
    cdmaRssiThresh.CDMARSSIThreshListLen = cdmaRssiThreshListlen;
    cdmaRssiThresh.pCDMARSSIThreshList   = cdmaRssiThreshList;
    req.pCDMARSSIThresh                  = &cdmaRssiThresh;

    cdmaEcioThresh.CDMAECIOThreshListLen = cdmaEcioThreshListLen;
    cdmaEcioThresh.pCDMAECIOThreshList   = cdmaEcioThreshList;
    req.pCDMAECIOThresh                  = &cdmaEcioThresh;

    hdrRssiThresh.HDRRSSIThreshListLen   = hdrRssiThresListLen;
    hdrRssiThresh.pHDRRSSIThreshList     = hdrRssiThreshList;
    req.pHDRRSSIThresh                   = &hdrRssiThresh;

    hdrEcioThresh.HDRECIOThreshListLen   = hdrEcioThresListLen;
    hdrEcioThresh.pHDRECIOThreshList     = hdrEcioThreshList;
    req.pHDRECIOThresh                   = &hdrEcioThresh;

    hdrSinrThresh.HDRSINRThreshListLen   = hdrSinrThresListLen;
    hdrSinrThresh.pHDRSINRThreshList     = hdrSinrThreshList;
    req.pHDRSINRThresh                   = &hdrSinrThresh;

    hdrIoThresh.HDRIOThreshListLen       = hdrIoThresListLen;
    hdrIoThresh.pHDRIOThreshList         = hdrIoThreshList;
    req.pHDRIOThresh                     = &hdrIoThresh;

    gsmRssiThresh.GSMRSSIThreshListLen   = gsmRssiThresListLen;
    gsmRssiThresh.pGSMRSSIThreshList     = gsmRssiThreshList;
    req.pGSMRSSIThresh                   = &gsmRssiThresh;

    wcdmaRssiThresh.WCDMARSSIThreshListLen   = wcdmaRssiThresListLen;
    wcdmaRssiThresh.pWCDMARSSIThreshList     = wcdmaRssiThreshList;
    req.pWCDMARSSIThresh                     = &wcdmaRssiThresh;

    wcdmaEcioThresh.WCDMAECIOThreshListLen   = wcdmaEcioThresListLen;
    wcdmaEcioThresh.pWCDMAECIOThreshList     = wcdmaEcioThreshList;
    req.pWCDMAECIOThresh                     = &wcdmaEcioThresh;

    lteRssiThresh.LTERSSIThreshListLen   = lteRssiThresListLen;
    lteRssiThresh.pLTERSSIThreshList     = lteRssiThreshList;
    req.pLTERSSIThresh                   = &lteRssiThresh;

    lteSnrThresh.LTESNRThreshListLen   = lteSnrThresListLen;
    lteSnrThresh.pLTESNRThreshList     = lteSnrThreshList;
    req.pLTESNRThresh                  = &lteSnrThresh;

    lteRsrqThresh.LTERSRQThreshListLen   = lteRsrqThresListLen;
    lteRsrqThresh.pLTERSRQThreshList     = lteRsrqThreshList;
    req.pLTERSRQThresh                  = &lteRsrqThresh;

    lteRsrpThresh.LTERSRPThreshListLen = lteRsrpThresListLen;
    lteRsrpThresh.pLTERSRPThreshList   = lteRsrpThreshList;
    req.pLTERSRPThresh                 = &lteRsrpThresh;

    tdscdmaRscpThresh.TDSCDMARSCPThreshListLen = tdscdmaRscpThresListLen;
    tdscdmaRscpThresh.pTDSCDMARSCPThreshList   = tdscdmaRscpThreshList;
    req.pTDSCDMARSCPThresh                     = &tdscdmaRscpThresh;

    tdscdmaRssiThresh.TDSCDMARSSIThreshListLen = tdscdmaRssiThresListLen;
    tdscdmaRssiThresh.pTDSCDMARSSIThreshList   = (ULONG*)tdscdmaRssiThreshList;
    req.pTDSCDMARSSIThresh                     = &tdscdmaRssiThresh;

    tdscdmaEcioThresh.TDSCDMAECIOThreshListLen = tdscdmaEcioThresListLen;
    tdscdmaEcioThresh.pTDSCDMAECIOThreshList   = (ULONG*)tdscdmaEcioThreshList;
    req.pTDSCDMAECIOThresh                     = &tdscdmaEcioThresh;

    tdscdmaSinrThresh.TDSCDMASINRThreshListLen = tdscdmaSinrThresListLen;
    tdscdmaSinrThresh.pTDSCDMASINRThreshList   = (ULONG*)tdscdmaSinrThreshList;
    req.pTDSCDMASINRThresh                     = &tdscdmaSinrThresh;

    lteSigRptCfg.rptRate   = rptRate;
    lteSigRptCfg.avgPeriod = avgPeriod;
    req.pLTESigRptConfig   = &lteSigRptCfg;

    BYTE                             instance;

    resultCode = SLQSSetSessionStateCallback( &SessionStateCallback );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for SessionStateCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    for( instance = 0; instance <= MAX_INST_ID; instance++ )
    {
        /* enable wds callback for both IPV4 and IPV6 client by default in this sample application,  */
          /* on customer application, it is up to the applcation to resgister which ip family wds callback */
          /* depend on the network capability */
        resultCode = SLQSSetWdsEventCallback( WDSCallback, 5, instance, IPv4v6_FAMILY_PREFERENCE );
        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Failed to subscribe for WDSCallback\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            #endif
        }
    }

    resultCode = SetRFInfoCallback( RFInfoCallback );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for RFInfoCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetLURejectCallback( LURejectCallback );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for LURejectCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetDeviceStateChangeCbk( DevStateChgCbk );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for SetDeviceStateChangeCbk\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SetNetChangeCbk(0, NetChgCbk, 2, 0, 1 );
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "Failed to subscribe for SetNetChangeCbk\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
    }

    resultCode = SetRoamingIndicatorCallback( RoamingIndicatorCallbck );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for RoamingIndicatorCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }

    resultCode = SLQSNasSigInfo2CallBack(SigInfoCallBack , &req);

    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe for SigInfoCallBack\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
    }
}

/*
 * Name:     IsConnectedDeviceGOBI
 *
 * Purpose:  Checks whether the connected device is a GOBI device or not.
 *
 * Params:   None
 *
 * Return:   TRUE   - If the connected device is a GOBI device.
 *           FALSE  - If the connected device is not a GOBI device
 *
 * Notes:    None
 */
BYTE IsConnectedDeviceGOBI()
{
    struct sGetDeviceSeriesResult result;
    eGetDeviceSeries(&result);

    /* If the device is a GOBI device */
    if ( result.eDevice == eGobi_DEV_SERIES_MC83 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 * Name:     ValidateIPAddressOctet
 *
 * Purpose:  Validates the received  octet of the IP Address.
 *
 * Params:   pIPAddressOctet - Pointer to the buffer containing IP Address
 *                             octet to be validated.
 *           len             - length of the passed buffer
 *
 * Return:   SUCCESS   - In case valid digits are there in the received octet of
 *                       the IP Address
 *           FAIL      - If invalid octet is received
 *
 * Notes:    None
 */
BYTE ValidateIPAddressOctet( CHAR* pIPAddressOctet, int len )
{
    if( len == 3)
    {
        /* If length of the octet is 3, first digit should be between 0 and 2 */
        if( ( '0' > pIPAddressOctet[0] ) ||
            ( '2' < pIPAddressOctet[0] ) )
        {
            return FAIL;
        }

        /* If first digit is 2 then second digit should not be greater than 5 */
        if( '2' == pIPAddressOctet[0] )
        {
            if( '5' < pIPAddressOctet[1] )
            {
                return FAIL;
            }

            /* If second digit is also 5 then third digit should not be greater
             * than 5.
             */
            if( '5' == pIPAddressOctet[1] )
            {
                if( '5' < pIPAddressOctet[2] )
                {
                    return FAIL;
                }
            }
        }

        if( ( '0' > pIPAddressOctet[1] ) ||
            ( '9' < pIPAddressOctet[1] ) )
        {
            return FAIL;
        }

        if( ( '0' > pIPAddressOctet[2] ) ||
            ( '9' < pIPAddressOctet[2] ) )
        {
            return FAIL;
        }
    }

    if( 2 == len )
    {
        if( ( '0' > pIPAddressOctet[0] ) ||
            ( '9' < pIPAddressOctet[0] ) )
        {
            return FAIL;
        }

        if( ( '0' > pIPAddressOctet[1] ) ||
            ( '9' < pIPAddressOctet[1] ) )
        {
            return FAIL;
        }
    }

    if( 1 == len )
    {
        if( ( '0' > pIPAddressOctet[0] ) ||
            ( '9' < pIPAddressOctet[0] ) )
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

/*
 * Name:     IPUlongToDot
 *
 * Purpose:  Convert the IP address passed by the user in the form of ULONG
 *           value to a DOT format and copy it into the user buffer.
 *
 * Params:   IPAddress  - IP Address to be converted in dot notation.
 *           pIPAddress - Buffer to store IP Address converted to dot notation.
 *
 * Return:   None
 *
 * Notes:    None
 */
static void IPUlongToDot( ULONG IPAddress, char* pIPAddress )
{
    CHAR  tempBuf[5];
    BYTE  idx = 0;
    BYTE  shift = 0;
    ULONG tempIPAddress = 0;
    int   IPAddr = 0;

    for( idx = 4; idx > 0; idx-- )
    {
        shift = ( idx - 1 ) * 8;
        tempIPAddress = IPAddress >> shift;
        IPAddr = tempIPAddress & IPADDREESS_OCTET_MASK;
        sprintf( tempBuf, "%d", IPAddr );
        strcat( pIPAddress, tempBuf );
        tempIPAddress = 0;
        if( 1 >= idx )
        {
            continue;
        }
        strcat( pIPAddress, "." );
    }
}

/*
 * Name:     IPDotToUlong
 *
 * Purpose:  Convert the IP address passed by the user in dot notation to
 *           a ULONG value.
 *
 * Params:   pIPAddress - IP Address to be converted to ULONG value.
 *           pAddress   - ULONG pointer to store converted IP Address.
 *
 * Return:   SUCCESS   - In case valid IP Address is provided by the user
 *           FAIL      - In case invalid IP Address is provided by the user
 *
 * Notes:    None
 */
BYTE IPDotToUlong( char* pIPAddress, ULONG* pAddress )
{
    CHAR* pCharacterOccurence = NULL;
    int   IPAddressOctet = 0;
    BYTE  len = 0, noOfShift = 8;
    ULONG IPAddress = 0;

    /* Being here means correct no. of dots are there in the IP  address. Jump
     * to first occurrence of the dot.
     */
    pCharacterOccurence = strtok( pIPAddress,"." );
    while( NULL != pCharacterOccurence )
    {
        len = strlen( pCharacterOccurence );
        if( ( 0 == len ) || ( 3 < len ) )
        {
            #ifdef DBG
            fprintf( stderr, "Incorrect octet length : %d\n",len );
            #endif
            *pAddress = 0;
            return FAIL;
        }

        /* Check if the valid digits have been entered in IP Address */
        if( FAIL == ValidateIPAddressOctet( pCharacterOccurence, len ) )
        {
            #ifdef DBG
            fprintf( stderr, "Incorrect characters in octet : %s\n",
                              pCharacterOccurence );
            #endif
            *pAddress = 0;
            return FAIL;
        }

        IPAddressOctet = atoi( pCharacterOccurence );
        if( 255 < IPAddressOctet )
        {
            #ifdef DBG
            fprintf( stderr, "Incorrect octet value : %d\n",IPAddressOctet );
            #endif
            *pAddress = 0;
            return FAIL;
        }
        len = 0;

        /* Store the octet */
        IPAddress = ( IPAddress << noOfShift ) | IPAddressOctet;

        #ifdef DBG
        fprintf( stderr, "IP Address Octet Value: %s, Integer: %d\n",
                          pCharacterOccurence, IPAddressOctet );
        fprintf( stderr, "IP Address : %lx\n",IPAddress );
        #endif
        IPAddressOctet = 0;

        /* look for the next dot */
        pCharacterOccurence = strtok( NULL, "." );
    }

    *pAddress = IPAddress;
    #ifdef DBG
    fprintf( stderr, "Final IP Address : %lx\n",*pAddress );
    #endif
    return SUCCESS;
}

/*
 * Name:     GetIPFromUser
 *
 * Purpose:  Prompt the user to enter the IP address and copy it in the passed
 *           buffer.
 *
 * Return:   SUCCESS   - In case valid IP Address is entered by the user
 *           ENTER_KEY - If enter key is pressed by the user
 *
 * Params:   pAddressString - Name of the address to be retrieved.
 *           pIPAddress     - Buffer to receive the address from user.
 *           pAddress       - Pointer to store received IP address after
 *                            conversion from dot notation to ULONG value.
 *
 * Notes:    None
 */
BYTE GetIPFromUser( char *pAddressString, char *pIPAddress, ULONG *pAddress )
{
    int  len = 0;
    BYTE count = 0, returnCode = 0, IPAddressWrong = FALSE;
    CHAR *pCharacterOccurence = NULL, *pEndOfLine = NULL;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease enter %s Address in xxx.xxx.xxx.xxx format,"\
                         " or press <Enter> to leave the field as blank:\n%s"\
                         "Address : ", pAddressString, pAddressString );

        /* Clear the buffer */
        memset( pIPAddress, 0, MAX_FIELD_SIZE );

        /* Receive the input from the user */
        fgets( pIPAddress, MAX_FIELD_SIZE, stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( pIPAddress, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        #ifdef DBG
        fprintf( stderr, "Entered Address : %s\n", pIPAddress );
        #endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == pIPAddress[0] )
        {
            *pAddress = 0;
            return ENTER_KEY;
        }

        len = strlen( pIPAddress );
        pIPAddress[len - 1] = '\0';
        len -= 1;

        /* Validate the address entered by the user */
        /* Check the IP Address Length */
        if( IP_ADDRESS_LEN < len )
        {
            #ifdef DBG
            fprintf( stderr, "Incorrect Address Length : %d\n",len );
            #endif
            continue;
        }

        /* Check if there is nothing followed by a Dot in the IP address or
         * there are two adjacent dots.
         */
        pCharacterOccurence = strchr( pIPAddress,'.' );
        while ( NULL != pCharacterOccurence )
        {
            if( ( '.'  == pCharacterOccurence[1] ) ||
                ( '\0' == pCharacterOccurence[1] ) )
            {
                #ifdef DBG
                fprintf( stderr, "Two Adjacent dots or NULL after a dot:"\
                                  "Wrong IP Address\n" );
                #endif
                IPAddressWrong = TRUE;
                break;
            }
            count++;
            pCharacterOccurence = strchr( ( pCharacterOccurence + 1 ),'.' );
        }

        /* If there are more than three dots in the IP address */
        if( ( count != 3 ) || ( IPAddressWrong == TRUE ) )
        {
            #ifdef DBG
            fprintf( stderr, "Incorrect No. of dots in address : %d\n",count );
            #endif
            IPAddressWrong = FALSE;
            count = 0;
            continue;
        }

        count = 0;

        /* Convert the IP address from DOT notation to ULONG */
        returnCode = IPDotToUlong( pIPAddress, pAddress );

        /* If IP Address is not correct */
        if( SUCCESS != returnCode )
        {
            continue;
        }

        /* Valid IP Address has been retrieved */
        return SUCCESS;
    }
}

/*
 * Name:     GetStringFromUser
 *
 * Purpose:  Prompt the user to enter a string and store it in the received
 *           buffer.
 *
 * Params:   pFieldName - Name of the string to be retrieved.
 *           pBuffer    - Buffer to receive the string from user.
 *           bufLen     - Length of the buffer to receive the string from user.
 *
 * Return:   SUCCESS   - If the user enters a valid value
 *           ENTER_KEY - If the user presses the Enter key
 *
 * Notes:    None
 */
BYTE GetStringFromUser( CHAR* pFieldName, CHAR* pBuffer, int bufLen )
{
    CHAR *pEndOfLine = NULL;
    int  len = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease enter %s (up to %d Characters),"\
                         " or press <Enter> to leave the field as blank:\n",
                         pFieldName, ( bufLen - 3 ) );

        /* Clear the buffer */
        memset( pBuffer, 0, bufLen );

        /* Receive the input from the user */
        fgets( pBuffer, bufLen, stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( pBuffer, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        #ifdef DBG
        fprintf( stderr, "Entered Value : %s\n", pBuffer );
        #endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == pBuffer[0] )
        {
            return ENTER_KEY;
        }

        /* Remove the enter key read at the end of the buffer */
        len = strlen( pBuffer );
        pBuffer[len - 1] = '\0';
        len -= 1;

        /* If the user has entered string of more than 17 characters */
        if( ( bufLen - 3) < len )
        {
            continue;
        }
        return SUCCESS;
    }
}

/*
 * Name:     GetPDPType
 *
 * Purpose:  Prompt the user to enter the PDP Type.
 *
 * Params:   None
 *
 * Return:   PDP Type value provided by the user(between 0 - 3),
 *           or hex value of enter key if pressed by the user.
 *
 * Notes:    None
 */
int GetPDPType()
{
    CHAR PDPType[OPTION_LEN];
    BYTE PDPTypeValue = 0;

    while(1)
    {
        CHAR *pEndOfLine = NULL;
        int len = 0;
        /* Print the menu */
        fprintf( stderr, "\nPlease enter PDP Type value( 0 - IPv4, 1 - PPP, "\
                         "2 - IPV6, 3 - IPV4V6 ),or press <Enter> to exit:\n"\
                         "Option : ");

        /* Receive the input from the user */
        fgets( PDPType, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( PDPType, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        #ifdef DBG
        fprintf( stderr, "PDP Type Value : %s\n", PDPType );
        #endif

        len = strlen( PDPType );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == PDPType[0] )
            return ENTER_KEY;

        /* If user has entered an invalid input, prompt again */
        if( 2 < len )
        {
            continue;
        }

        /* Convert the authentication value provided by user into integer */
        PDPType[ len - 1 ] = '\0';
        PDPTypeValue = atoi( PDPType );

        /* Validate the Authentication value */
        if( 3 < PDPTypeValue )
        {
            continue;
        }
        return PDPTypeValue;
    }
}

/*
 * Name:     GetAuthenticationValue
 *
 * Purpose:  Prompt the user to enter the Authentication value.
 *
 * Params:   None
 *
 * Return:   Authentication value provided by the user(between 0 - 3),
 *           or hex value of enter key if pressed by the user.
 *
 * Notes:    None
 */
int GetAuthenticationValue()
{
    CHAR authenticationValue[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE authValue = 0;
    int  len = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease enter Authentication value( 0 - None, 1 -"\
                         "PAP, 2 - CHAP, 3 - PAP/CHAP ),or press <Enter> to "\
                         "leave the field as blank:\nOption : ");

        /* Receive the input from the user */
        fgets( authenticationValue, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( authenticationValue, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        #ifdef DBG
        fprintf( stderr, "Authentication Value : %s\n", authenticationValue );
        #endif

        len = strlen( authenticationValue );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == authenticationValue[0] )
            return ENTER_KEY;

        /* If user has entered an invalid input, prompt again */
        if( 2 < len )
        {
            continue;
        }

        /* Convert the authentication value provided by user into integer */
        authenticationValue[ len - 1 ] = '\0';
        authValue = atoi( authenticationValue );

        /* Validate the Authentication value */
        if( 3 < authValue )
        {
            continue;
        }
        return authValue;
    }
}

/*
 * Name:     UpdateProfileInfo
 *
 * Purpose:  Updates the Profile Information of the connected device on the user
 *           output window.
 *
 * Params:   display      - Flag to check whether to display values or not.
 *                          - TRUE  - Display the values received
 *                          - FALSE - Display UNKNOWN for profile parameters
 *           pProfileInfo - Pointer to the structure containing value to be
 *                          displayed for profile parameters.
 *
 * Return:   None
 *
 * Notes:    None
 */
void UpdateProfileInfo( BYTE display, struct profileInformation *pProfileInfo )
{
    CHAR respBuf[MAX_FIELD_SIZE];

    if( display == FALSE)
    {
        UpdateUserDisplay( ePDP_TYPE, "UNKNOWN" );
        UpdateUserDisplay( eAUTHENTICATION, "UNKNOWN" );
        UpdateUserDisplay( ePROFILE_NAME, "UNKNOWN" );
        UpdateUserDisplay( eAPN_NAME, "UNKNOWN" );
        UpdateUserDisplay( eUSER_NAME, "UNKNOWN" );
        return;
    }

    /* Reset the buffer and copy the data to be displayed */
    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%u", pProfileInfo->PDPType );
    UpdateUserDisplay( ePDP_TYPE, respBuf );

    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%u", pProfileInfo->Authentication );
    UpdateUserDisplay( eAUTHENTICATION, respBuf );

    UpdateUserDisplay( ePROFILE_NAME, pProfileInfo->profileName );
    UpdateUserDisplay( eAPN_NAME, pProfileInfo->APNName );
    UpdateUserDisplay( eUSER_NAME, pProfileInfo->userName );
}

/******************************************************************************
* Option 8 : Delete a profile stored on the device
******************************************************************************/
/*
 * Name:     GetUserProfileId
 *
 * Purpose:  Prompt the user to enter the profile id whose information needs to
 *           be retrieved.
 *
 * Params:   None
 *
 * Return:   Profile index selected by the user(between 1 - 16),
 *           or hex value of enter key if pressed by the user else FALSE.
 *
 * Notes:    None
 */
int GetUserProfileId()
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE profileIdx = 0;
    int len = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease provide a profile id(1-16), or press <Enter> to exit: ");

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

        #ifdef DBG
        fprintf( stderr, "Selected Option : %s\n", selOption );
        #endif

        len = strlen( selOption );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return ENTER_KEY_PRESSED;

        /* If user has entered an invalid input, prompt again */
        if( 3 < len )
        {
            continue;
        }
        else if( 3 == len )
        {
            /* If the user has provided a 2 digit profile id, validate the
             * second digit as atoi() will ignore it.
             */
            if( ( selOption[ len - 2 ] < '0' ) ||
                ( selOption[ len - 2 ] > '6' ) )
            {
                continue;
            }
        }

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        profileIdx = atoi( selOption );

        /* If an invalid profile id is provided by the user */
        if( ( MIN_PROFILES > profileIdx ) ||
            ( MAX_PROFILES < profileIdx ) )
        {
            continue;
        }
        else
        {
             break;
        }
    }
    return profileIdx;
}

/*
 * Name:     GetInstanceId
 *
 * Purpose:  Prompt the user to enter the instanceId whose information needs to
 *           be retrieved.
 *
 * Params:   None
 *
 * Return:   instance ID selected by the user(between 1 - 16),
 *           or hex value of enter key if pressed by the user else FALSE.
 *
 * Notes:    None
 */
int GetInstanceId()
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE profileIdx = 0;
    int len = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nEnter a Instance id(0-2) to Start Application, or press <Enter> to exit: ");

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
        #ifdef DBG
        fprintf( stderr, "Selected Option : %s\n", selOption );
        #endif

        len = strlen( selOption );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return ENTER_KEY_PRESSED;

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        profileIdx = atoi( selOption );
        if ( profileIdx <= 2 )
        {
            break;
        }
        else
        {
           fprintf(stderr,"\nInstance ID is not valid\n");
           exit(0);
        }

    }
    return profileIdx;
}

/*
 * Name:     DeleteProfileFromDevice
 *
 * Purpose:  Delete the profile from the device for the profile id provided by
 *           the user.
 *
 * Params:   None
 *
 * Return:   None.
 *
 * Notes:    None
 */
void DeleteProfileFromDevice()
{
    ULONG                          resultCode = 0;
    ULONG                          profileType = PROFILE_TYPE_UMTS;
    BYTE                           profileId;
    ULONG                          PDPType;
    ULONG                          IPAddress;
    ULONG                          primaryDNS;
    ULONG                          secondaryDNS;
    ULONG                          authentication;
    CHAR                           profileName[MAX_PROFILE_NAME_SIZE];
    CHAR                           APNName[MAX_APN_SIZE];
    CHAR                           Username[MAX_USER_NAME_SIZE];
    struct SLQSDeleteProfileParams profileToDelete;
    WORD                           extendedErrorCode = 0;

    while(1)
    {
        /* Display all the profiles stored on the device */
        DisplayAllProfiles();

        /* If no profile exist on the device, return */
        if( 0 == indexInfo.totalProfilesOnDevice )
        {
            fprintf( stderr, "No Profile exist on the device for deletion "\
                             "or check device connectivity\n\n");
            return;
        }

        while(1)
        {
            /* Receive the user input */
            profileId = GetUserProfileId();

            /* If only <ENTER> is pressed by the user, return to main menu */
            if( ENTER_KEY_PRESSED == profileId )
                return;

            resultCode = SLQSGetProfile( profileType,
                                         profileId,
                                         &PDPType,
                                         &IPAddress,
                                         &primaryDNS,
                                         &secondaryDNS,
                                         &authentication,
                                         MAX_PROFILE_NAME_SIZE,
                                         profileName,
                                         MAX_APN_SIZE,
                                         APNName,
                                         MAX_USER_NAME_SIZE,
                                         Username,
                                         &extendedErrorCode );

            /* If the profile does not exist on the device or we failed to
             * retrieve the information about the profile.
             */
            if( SUCCESS != resultCode )
            {
                if( ( resultCode >= ERR_GENERAL ) &&
                    ( resultCode < ERR_ENUM_END ) )
                {
                    fprintf( stderr, "Profile retrieving Failed\n"\
                                    "Failure cause - %u\nError Code - %d\n\n",
                                    resultCode, extendedErrorCode );
                }
                else
                {
                    #ifdef DBG
                    fprintf( stderr, "Profile retrieving Failed for "\
                                     "Profile Id : %d\nFailure cause - %u\n"\
                                     "Error Code - %d\n\n",
                                    profileId, resultCode, extendedErrorCode );
                    #endif
                }
                extendedErrorCode = 0;
                continue;
            }
            break; /* Being here means, valid profile id is retrieved */
        }

        /* Delete the profile from the device */
        profileToDelete.profileType  = 0;
        profileToDelete.profileIndex = profileId;
        resultCode = SLQSDeleteProfile( &profileToDelete, &extendedErrorCode );

        /* If we fail to delete the profile */
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Profile Deletion Failed\nFailure cause - %u\n"\
                             "Error Code - %d\n\n",
                             resultCode, extendedErrorCode );
            continue;
        }

        fprintf( stderr, "Profile for index %d deleted successfully\n",
                          profileId );
    }
}

/******************************************************************************
* Option 7 : Modify the settings of an existing profile stored on the device
******************************************************************************/
/*
 * Name:     ModifyProfileSettings
 *
 * Purpose:  Modify the Profile settings of the profile id selected by the user
 *           with the values entered by the user.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void ModifyProfileSettings()
{
    struct profileInformation profileInfo;
    ULONG                     resultCode;
    BYTE                      profileId = 0, returnCode = 0, idx = 0;
    BYTE                      profileIdMatch = FALSE;
    CHAR                      IPAddress[MAX_FIELD_SIZE];
    ULONG                     *pAuthentication =NULL,*pPrimaryDNSAddress = NULL;
    ULONG                     *pSecondaryDNSAddress = NULL,*pIPAddress = NULL;
    CHAR                      *pAPNName = NULL, *pUserName = NULL;
    CHAR                      *pProfileName = NULL, *pPassword = NULL;

    profileInfo.profileType = PROFILE_TYPE_UMTS;
    profileInfo.PDPType = PDP_IPV4;

    while( 1 )
    {
        /* Reset the pointers */
        pAuthentication = NULL; pPrimaryDNSAddress = NULL;
        pSecondaryDNSAddress = NULL; pIPAddress = NULL;
        pAPNName = NULL; pUserName = NULL;
        pProfileName = NULL; pPassword = NULL;

        /* Display all the profiles stored on the device */
        DisplayAllProfiles();

        if( 0 == indexInfo.totalProfilesOnDevice )
        {
            fprintf( stderr, "No Profile exist on device for modification"\
                             "or check device connectivity\n\n");
            return;
        }

       while(1)
       {
            /* Prompt the user to enter the profile id whose values need to be
             * modified.
             */
            profileId = GetUserProfileId();

            /* If only <ENTER> is pressed by the user, return to main menu */
            if( ENTER_KEY_PRESSED == profileId )
            {
                return;
            }

            /* If the user has enter an invalid profile id */
            for( idx = 0; idx < indexInfo.totalProfilesOnDevice; idx++ )
            {
                if( profileId == indexInfo.profileIndex[idx] )
                {
                     profileIdMatch = TRUE;
                     break;
                }
            }

            if( profileIdMatch == TRUE )
            {
                profileIdMatch = FALSE;
                break;
            }
        }

        /* Prompt the user to enter the profile parameter values */
        /* Get the IP Address */
        returnCode = GetIPFromUser( "IP", IPAddress, &profileInfo.IPAddress );
        if( ENTER_KEY != returnCode )
        {
            pIPAddress = &profileInfo.IPAddress;
        }

        /* Get the Primary DNS Address */
        returnCode = GetIPFromUser( "PrimaryDNS Address", IPAddress,
                                             &profileInfo.primaryDNS );
        if( ENTER_KEY != returnCode )
        {
            pPrimaryDNSAddress = &profileInfo.primaryDNS;
        }
        returnCode = 0;

        /* Get the Secondary DNS Address */
        returnCode = GetIPFromUser( "SecondaryDNS Address", IPAddress,
                                               &profileInfo.secondaryDNS );
        if( ENTER_KEY != returnCode )
        {
            pSecondaryDNSAddress = &profileInfo.secondaryDNS;
        }
        returnCode = 0;

        /* Get Authentication From the user */
        profileInfo.Authentication = GetAuthenticationValue();
        if( ENTER_KEY != profileInfo.Authentication )
        {
            pAuthentication = &profileInfo.Authentication;
        }
        returnCode = 0;

        /* Get Profile Name from the user, Max size is 14 characters */
        returnCode = GetStringFromUser( "Profile Name", profileInfo.profileName,
                                                        MAX_PROFILE_NAME_SIZE );
        if( ENTER_KEY != returnCode )
        {
            pProfileName = profileInfo.profileName;
        }
        returnCode = 0;

        /* Get APN Name from the user */
        returnCode = GetStringFromUser( "APN Name", profileInfo.APNName,
                                                    MAX_APN_SIZE );
        if( ENTER_KEY != returnCode )
        {
            pAPNName = profileInfo.APNName;
        }
        returnCode = 0;

        /* Get User Name from the user */
        returnCode = GetStringFromUser( "User Name", profileInfo.userName,
                                                     MAX_USER_NAME_SIZE );
        if( ENTER_KEY != returnCode )
        {
            pUserName = profileInfo.userName;
        }
        returnCode = 0;

        /* Get Password from the user */
        returnCode = GetStringFromUser( "Password", profileInfo.password,
                                                    MAX_FIELD_SIZE );
        if( ENTER_KEY != returnCode )
        {
            pPassword = profileInfo.password;
        }

        /* Set the profile with the required fields */
        resultCode = SLQSSetProfile( profileInfo.profileType,
                                     profileId,
                                     &profileInfo.PDPType,
                                     pIPAddress,
                                     pPrimaryDNSAddress,
                                     pSecondaryDNSAddress,
                                     pAuthentication,
                                     pProfileName,
                                     pAPNName,
                                     pUserName,
                                     pPassword );
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Profile Modification Failed\n"\
                             "Failure cause - %u\n", resultCode );
            continue;
        }
        fprintf( stderr, "Profile Settings updated successfully for Profile ID:"
                          " %d\n",profileId );
    }
}

/******************************************************************************
* Option 6 : Create a profile on the device
******************************************************************************/
/*
 * Name:     CreateProfile
 *
 * Purpose:  Create the Profile with the values provided by the user.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void CreateProfile()
{
    struct profileInformation  profileInfo;
    struct CreateProfileIn     profileToCreate;
    struct CreateProfileOut    profileCreatedresult;
    ULONG                      resultCode;
    BYTE                       profileType = PROFILE_TYPE_UMTS, returnCode = 0;
    BYTE                       PDPType = 0, profileindex = 0, authValue = 0;
    USHORT                     extendedErrorCode;
    CHAR                       IPAddress[MAX_FIELD_SIZE];
    BYTE                       profileTypeOut;

    while( 1 )
    {
        /* Reset the input params */
        memset( (void *)&profileToCreate, 0, sizeof( profileToCreate ) );
        memset( (void *)&profileCreatedresult,
                0,
                sizeof( profileCreatedresult ) );
        profileToCreate.pProfileType = &profileType;
        profileindex = 0;
        extendedErrorCode = 0;
        profileCreatedresult.pProfileType  = &profileTypeOut;
        profileCreatedresult.pProfileIndex = &profileindex;
        profileCreatedresult.pExtErrorCode = &extendedErrorCode;

        /* Display all the profiles stored on the device */
        DisplayAllProfiles();

        /* Get PDP Type */
        PDPType = GetPDPType();
        if( ENTER_KEY == PDPType )
        {
            return;
        }
        profileToCreate.curProfile.SlqsProfile3GPP.pPDPtype = &PDPType;

        /* Get the IP Address */
        returnCode = GetIPFromUser( "IP", IPAddress, &profileInfo.IPAddress );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pIPv4AddrPref
                                            = &profileInfo.IPAddress;
        }

        /* Get the Primary DNS Address */
        returnCode = GetIPFromUser( "PrimaryDNS Address", IPAddress,
                                             &profileInfo.primaryDNS );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pPriDNSIPv4AddPref
                                                = &profileInfo.primaryDNS;
        }
        returnCode = 0;

        /* Get the Secondary DNS Address */
        returnCode = GetIPFromUser( "SecondaryDNS Address", IPAddress,
                                               &profileInfo.secondaryDNS );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pSecDNSIPv4AddPref =
                            &profileInfo.secondaryDNS;
        }
        returnCode = 0;

        /* Get Authentication From the user */
        authValue = GetAuthenticationValue();
        if( ENTER_KEY != authValue )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pAuthenticationPref =
                            &authValue;
        }
        returnCode = 0;

        /* Get Profile Name from the user, Max size is 14 characters */
        returnCode = GetStringFromUser( "Profile Name", profileInfo.profileName,
                                                        MAX_PROFILE_NAME_SIZE );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pProfilename =
                                              profileInfo.profileName;
        }
        returnCode = 0;

        /* Get APN Name from the user */
        returnCode = GetStringFromUser( "APN Name", profileInfo.APNName,
                                                    MAX_APN_SIZE );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pAPNName =
                            profileInfo.APNName;
        }
        returnCode = 0;

        /* Get User Name from the user */
        returnCode = GetStringFromUser( "User Name", profileInfo.userName,
                                                     MAX_USER_NAME_SIZE );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pUsername =
                            profileInfo.userName;
        }
        returnCode = 0;

        /* Get Password from the user */
        returnCode = GetStringFromUser( "Password", profileInfo.password,
                                                    MAX_FIELD_SIZE );
        if( ENTER_KEY != returnCode )
        {
            profileToCreate.curProfile.SlqsProfile3GPP.pPassword =
                            profileInfo.password;
        }

        /* Set the profile with the required fields */
        resultCode = SLQSCreateProfile( &profileToCreate,
                                        &profileCreatedresult );
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Profile Creation Failed\n"\
                     "Failure cause  - %u\nFailure Reason - %d\n",
                     resultCode, *(profileCreatedresult.pExtErrorCode) );
            continue;
        }
        fprintf( stderr, "Profile created successfully for Profile ID:"
                          " %d\n", *(profileCreatedresult.pProfileIndex) );
    }
}

/******************************************************************************
* Option 5 : Display the settings for a particular profile stored on the device
******************************************************************************/
/*
 * Name:     DisplayProfileInfo
 *
 * Purpose:  Display the profile information for the profile index provided by
 *           the user.
 *
 * Params:   None
 *
 * Return:   None.
 *
 * Notes:    None
 */
static void DisplayProfileInfo()
{
    ULONG resultCode = 0;
    ULONG profileType = PROFILE_TYPE_UMTS;
    BYTE  profileId;
    ULONG PDPType;
    ULONG IPAddress = (ULONG) -1;
    CHAR  bufIPAddress[MAX_FIELD_SIZE];
    ULONG primaryDNS;
    CHAR  bufPrimaryDNS[MAX_FIELD_SIZE];
    ULONG secondaryDNS;
    CHAR  bufSecondaryDNS[MAX_FIELD_SIZE];
    ULONG authentication;
    CHAR  profileName[MAX_PROFILE_NAME_SIZE];
    CHAR  APNName[MAX_APN_SIZE];
    CHAR  Username[MAX_USER_NAME_SIZE];
    WORD  extendedErrorCode = 0;
    int w3 = -3, w5 = -5, w8 = -8, w20 = -20;

    while(1)
    {
        /* Receive the user input */
        profileId = GetUserProfileId();

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY_PRESSED == profileId )
        {
            return;
        }
        /* Initialize the buffers */
        primaryDNS   = 0;
        secondaryDNS = 0;

        resultCode = SLQSGetProfile( profileType,
                                     profileId,
                                     &PDPType,
                                     &IPAddress,
                                     &primaryDNS,
                                     &secondaryDNS,
                                     &authentication,
                                     MAX_PROFILE_NAME_SIZE,
                                     profileName,
                                     MAX_APN_SIZE,
                                     APNName,
                                     MAX_USER_NAME_SIZE,
                                     Username,
                                     &extendedErrorCode );

        /* If the profile does not exist on the device or we failed to retrieve
         * the information about the profile.
         */
        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Profile retrieving Failed\n"\
                             "Failure cause - %u\nError Code - %d\n\n",
                             resultCode, extendedErrorCode );
            #endif
            extendedErrorCode = 0;
            continue;
        }

        /* Reset the buffers */
        memset( bufIPAddress, 0, MAX_FIELD_SIZE );
        memset( bufPrimaryDNS, 0, MAX_FIELD_SIZE );
        memset( bufSecondaryDNS, 0, MAX_FIELD_SIZE );

        /* Convert ULONG to Dot notation for display */
        IPUlongToDot( IPAddress, bufIPAddress );
        IPUlongToDot( primaryDNS, bufPrimaryDNS );
        IPUlongToDot( secondaryDNS, bufSecondaryDNS );

        /* Display the header */
        fprintf( stderr, "%5$*1$s%6$*2$s%7$*4$s%8$*4$s%9$*4$s%10$*3$s%11$*4$s"
                         "%12$*4$s%13$*4$s\n",
                         w3, w8, w5, w20, "ID", "PDPType", "IPAddress",
                         "PrimaryDNS", "SecondaryDNS", "Auth", "ProfileName",
                         "APNName", "UserName" );

        /* Display the retrieved profile information */
        fprintf( stderr, "%5$*1$d%6$*2$u%7$*4$s%8$*4$s%9$*4$s%10$*3$u%11$*4$s"
                         "%12$*4$s%13$*4$s\n",
                         w3, w8, w5, w20, profileId, PDPType, bufIPAddress,
                         bufPrimaryDNS, bufSecondaryDNS, authentication,
                         profileName, APNName, Username );
    }
}

/*************************************************************************
* Option 4 : Display all the profiles stored on the device
*************************************************************************/
/*
 * Name:     DisplayAllProfiles
 *
 * Purpose:  Display all the profiles stored on the device.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
static void DisplayAllProfiles()
{
    ULONG resultCode = 0;
    ULONG profileType = PROFILE_TYPE_UMTS;
    BYTE  profileId;
    ULONG PDPType;
    CHAR  bufIPAddress[MAX_FIELD_SIZE];
    ULONG IPAddress;
    CHAR  bufPrimaryDNS[MAX_FIELD_SIZE];
    ULONG primaryDNS;
    CHAR  bufSecondaryDNS[MAX_FIELD_SIZE];
    ULONG secondaryDNS;
    ULONG authentication;
    CHAR  profileName[MAX_PROFILE_NAME_SIZE];
    CHAR  APNName[MAX_APN_SIZE];
    CHAR  Username[MAX_USER_NAME_SIZE];
    WORD  extendedErrorCode = 0;
    int w3 = -3, w5 = -5, w8 = -8, w20 = -20;
    indexInfo.totalProfilesOnDevice = 0;

    /* Display the header */
    fprintf( stderr, "\n%5$*1$s%6$*2$s%7$*4$s%8$*4$s%9$*4$s%10$*3$s%11$*4$s"
                     "%12$*4$s%13$*4$s\n",
                     w3, w8, w5, w20, "ID", "PDPType", "IPAddress",
                     "PrimaryDNS", "SecondaryDNS", "Auth", "ProfileName",
                      "APNName", "UserName" );

    /* Retrieve the information for all the profiles loaded on the device */
    for( profileId = MIN_PROFILES; profileId <= MAX_PROFILES; profileId++ )
    {
        /* Initialize the buffers */
        memset( profileName, 0, MAX_PROFILE_NAME_SIZE );
        memset( APNName, 0, MAX_APN_SIZE );
        memset( Username, 0, MAX_USER_NAME_SIZE );
        IPAddress      = 0;
        primaryDNS     = 0;
        secondaryDNS   = 0;
        authentication = 0;

        resultCode = SLQSGetProfile( profileType,
                                     profileId,
                                     &PDPType,
                                     &IPAddress,
                                     &primaryDNS,
                                     &secondaryDNS,
                                     &authentication,
                                     MAX_PROFILE_NAME_SIZE,
                                     profileName,
                                     MAX_APN_SIZE,
                                     APNName,
                                     MAX_USER_NAME_SIZE,
                                     Username,
                                     &extendedErrorCode );

        /* If the profile does not exist on the device or we failed to retrieve
         * the information about the profile.
         */
        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Profile retrieving Failed for Profile Id : "\
                             "%d\nFailure cause - %u\nError Code - %d\n\n",
                             profileId, resultCode, extendedErrorCode );
            #endif
            extendedErrorCode = 0;
            continue;
        }

        /* Store the profile indexes for successfully retrieved profiles */
        indexInfo.profileIndex[indexInfo.totalProfilesOnDevice] = profileId;
        indexInfo.totalProfilesOnDevice++;

        /* Reset the buffers */
        memset( bufIPAddress, 0, MAX_FIELD_SIZE );
        memset( bufPrimaryDNS, 0, MAX_FIELD_SIZE );
        memset( bufSecondaryDNS, 0, MAX_FIELD_SIZE );

        /* Convert ULONG to Dot notation for display */
        IPUlongToDot( IPAddress, bufIPAddress );
        IPUlongToDot( primaryDNS, bufPrimaryDNS );
        IPUlongToDot( secondaryDNS, bufSecondaryDNS );

        /* Display the retrieved profile information */
        fprintf( stderr, "%5$*1$d%6$*2$u%7$*4$s%8$*4$s%9$*4$s%10$*3$u%11$*4$s"
                         "%12$*4$s%13$*4$s\n",
                         w3, w8, w5, w20, profileId, PDPType, bufIPAddress,
                         bufPrimaryDNS, bufSecondaryDNS, authentication,
                         profileName, APNName, Username );
    }
}

/*
 * Name:     DisplayAllProfile
 *
 * Purpose:  Call DisplayAllProfiles to display all the profiles stored on the
 *           device.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void DisplayAllProfile()
{
    DisplayAllProfiles();
    if( 0 == indexInfo.totalProfilesOnDevice )
    {
        fprintf( stderr, "No Profile exist on the device"\
                         "or check device connectivity\n\n" );
    }
}

/*************************************************************************
* Option 3 : Stop the currently active Data Session
*************************************************************************/
/*
 * Name:     StopCurrentDataSession
 *
 * Purpose:  Stop the ongoing data session
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void StopCurrentDataSession()
{
    ULONG resultCode = 0;
    BYTE  callActiv  = 0;
    int i;
    BOOL isLTE, isUMTS;
    struct ssdatasession_params *pSession;

    /* sumts & slte is defined for index 0 & 1 only */
    for( i = 0; i <= MAX_INST_ID; i++ )
    {
        isLTE = 0;
        isUMTS = 0;
        /* Call status
         * 3 - Both IPv4 and IPv6 calls active
         * 2 - Only IPv6 call active
         * 1 - Only IPv4 call active
         * 0 - No call active */

        /* check UMTS */
        callActiv =  sumts[i].v4sessionId && sumts[i].v6sessionId ?
                     3:( sumts[i].v6sessionId? 2:( sumts[i].v4sessionId? 1 : 0) );
        if( 0 != callActiv )
        {
            isUMTS = 1;
        }

        /* check LTE */
        callActiv =  slte[i].v4sessionId && slte[i].v6sessionId ?
                     3:( slte[i].v6sessionId? 2:( slte[i].v4sessionId? 1 : 0) );

        /* If callActiv is 0, no call is in progress */
        if( 0 != callActiv )
        {
            isLTE = 1;
        }

        if ( isUMTS && isLTE)
        {
            fprintf( stderr, "both LTE & UMTS data session are started\n" );
            continue;
        }

        if ( isUMTS )
        {
            pSession = &sumts[i];
        }
        else if ( isLTE )
        {
            pSession = &slte[i];
        }
        else
        {
            /* no UMTS or LTE connection for this QMI intsance */
            continue;
        }

        /* Data session values are internally managed by SLQS - session ID
         * value passed here has no impact. Error codes returned provide the
         * result of IPv4v6 session stop */
        pSession->action = STOP_DATA_SEESION;
        resultCode = SLQSStartStopDataSession( pSession );

        switch ( callActiv )
        {
            case 3:
                /* Both sessions were active */
                if( eQCWWAN_ERR_NONE == resultCode )
                {
                    /* If success is returned it indicates both sessions were
                     * successfully stopped */
                     fprintf( stderr, "IPv4v6 sessions stopped successfully\n" );
                     pSession->v4sessionId = 0;
                     pSession->v6sessionId = 0;
                }
                else if( eQCWWAN_ERR_SWICM_V4DWN_V6UP == resultCode )
                {
                    fprintf( stderr,
                             "Failed to stop the IPv4v6 - IPv6 data call\n" );
                }
                else if( eQCWWAN_ERR_SWICM_V4UP_V6DWN == resultCode )
                {
                    fprintf( stderr,
                             "Failed to stop the IPv4v6 - IPv4 data call\n" );
                }
                else
                {
                    fprintf( stderr,
                             "Failed to stop both IPv4v6 calls\n" );
                }
                break;

            case 2:
                /* IPv6 session was active */
                if( eQCWWAN_ERR_NONE == resultCode )
                {
                    /* If success is returned it indicates IPv6 session was
                     * successfully stopped */
                     fprintf( stderr, "IPv6 session stopped successfully\n" );
                     pSession->v6sessionId = 0;
                }
                else
                {
                    fprintf( stderr,
                             "Failed to stop IPv6 session\n" );
                }
                break;
            case 1:
               /* IPv4 session was active */
               if( eQCWWAN_ERR_NONE == resultCode )
               {
                   /* If success is returned it indicates IPv4 session was
                    * successfully stopped */
                   fprintf( stderr, "IPv4 session stopped successfully\n" );
                   pSession->v4sessionId = 0;
               }
               else
               {
                   fprintf( stderr,
                            "Failed to stop IPv4 session\n" );
               }
               break;
            default:
               break;
        }
        /* Print the failure code - */
        fprintf( stderr, "Failure Code : %u\n", resultCode );

        /* NOTE: when testing with CMW500 on MC7750 & MC7710
         * stopping two data session without delay cause
         * UL Out of Sync on Equipment end
         */
        sleep(2);
    }
    sessionNum = 0;
    sessionNumLte = 0;
}

/*************************************************************************
 * Option 2 : Start LTE Data Session
 ************************************************************************/
/*
 * Name:     UpdateProfileInformation
 *
 * Purpose:  Update the Profile Information on the user output window with the
 *           profile id selected by user to start data session.
 *
 * Params:   profileId - Profile Id whose information needs to be updated.
 *
 * Return:   None
 *
 * Notes:    None
 */
void UpdateProfileInformation( ULONG profileId )
{
    struct profileInformation profileInfo;
    WORD                      extendedErrorCode = 0;
    ULONG                     resultCode = 0;

    profileInfo.profileType = PROFILE_TYPE_UMTS;

    /* Get the Profile information for the profile id passed by the user */
    resultCode = SLQSGetProfile( profileInfo.profileType,
                                 profileId,
                                 &( profileInfo.PDPType ),
                                 &( profileInfo.IPAddress ),
                                 &( profileInfo.primaryDNS ),
                                 &( profileInfo.secondaryDNS ),
                                 &( profileInfo.Authentication ),
                                 MAX_PROFILE_NAME_SIZE - 1,
                                 profileInfo.profileName,
                                 MAX_APN_SIZE - 1,
                                 profileInfo.APNName,
                                 MAX_USER_NAME_SIZE - 1,
                                 profileInfo.userName,
                                 &extendedErrorCode );

    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Profile retrieving Failed for Profile Id : "\
                         "%u\nFailure cause - %lu\nError Code - %d\n\n",
                         profileId, resultCode, extendedErrorCode );
        #endif
        UpdateProfileInfo( FALSE, &profileInfo );
        extendedErrorCode = 0;
        return;
    }

    /* Update the profile information on user window */
    UpdateProfileInfo( TRUE, &profileInfo );
}

/*
 * Name:     GetIPFamilyPreference
 *
 * Purpose:  Prompt the user to enter the IP Family preference
 *
 * Params:   None
 *
 * Return:   IPFamilyPreference
 *
 * Notes:    None
 */
int GetIPFamilyPreference()
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE IPFamilyPreference = IPv4_FAMILY_PREFERENCE; /* default to IPV4 */
    int len = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr,"\n1. IPV4 (default)\n" \
                        "2. IPV6\n" \
                        "3. IPV4V6\n" \
                        "Please select IP family preference for the call, or press <Enter> to exit: ");

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

        #ifdef DBG
        fprintf( stderr, "Selected Option : %s\n", selOption );
        #endif

        len = strlen( selOption );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return ENTER_KEY_PRESSED;

        /* If user has entered an invalid input, prompt again */
        if( 2 > len )
        {
            continue;
        }
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        int selection = atoi( selOption );

        switch (selection)
        {
            case 1:
                IPFamilyPreference = IPv4_FAMILY_PREFERENCE;
                break;
            case 2:
                IPFamilyPreference = IPv6_FAMILY_PREFERENCE;
                break;
            case 3:
                IPFamilyPreference = IPv4v6_FAMILY_PREFERENCE;
                break;
            default:
                continue; /* reject everything else */
                break;
        }

        if ( (IPFamilyPreference == IPv4_FAMILY_PREFERENCE) ||
             (IPv6_FAMILY_PREFERENCE == IPFamilyPreference) ||
             (IPv4v6_FAMILY_PREFERENCE == IPFamilyPreference) )
        {
            break;
        }
    }
    return IPFamilyPreference;
}

static ULONG getRegState()
{
    ULONG nRet;
    SrvStatusInfo      cdmassi;
    SrvStatusInfo      hdrssi;
    GSMSrvStatusInfo   gsmssi;
    GSMSrvStatusInfo   wcdmassi;
    GSMSrvStatusInfo   ltessi;
    nasGetSysInfoResp resp;
    resp.pCDMASrvStatusInfo  = &cdmassi;
    resp.pHDRSrvStatusInfo   = &hdrssi;
    resp.pGSMSrvStatusInfo   = &gsmssi;
    resp.pWCDMASrvStatusInfo = &wcdmassi;
    resp.pLTESrvStatusInfo   = &ltessi;
    resp.pCDMASysInfo        = NULL;
    resp.pHDRSysInfo         = NULL;
    resp.pGSMSysInfo         = NULL;
    resp.pWCDMASysInfo       = NULL;
    resp.pLTESysInfo         = NULL;
    resp.pAddCDMASysInfo     = NULL;
    resp.pAddHDRSysInfo      = NULL;
    resp.pAddGSMSysInfo      = NULL;
    resp.pAddWCDMASysInfo    = NULL;
    resp.pAddLTESysInfo      = NULL;
    resp.pGSMCallBarringSysInfo = NULL;
    resp.pWCDMACallBarringSysInfo  = NULL;
    resp.pLTEVoiceSupportSysInfo   = NULL;
    resp.pGSMCipherDomainSysInfo   = NULL;
    resp.pWCDMACipherDomainSysInfo = NULL;
    ULONG RegistrationState = 0xFF;

    nRet = SLQSNasGetSysInfo( &resp );

    if( eQCWWAN_ERR_NONE == nRet )
    {
        if ( resp.pCDMASrvStatusInfo->srvStatus == 2 )
        {
            RegistrationState = (ULONG)resp.pCDMASrvStatusInfo->srvStatus;
        }
        else if ( resp.pHDRSrvStatusInfo->srvStatus == 2 )
        {
            RegistrationState = (ULONG) resp.pHDRSrvStatusInfo->srvStatus;
        }
        else if ( resp.pGSMSrvStatusInfo->srvStatus == 2 )
        {
            RegistrationState = (ULONG)resp.pGSMSrvStatusInfo->srvStatus;
        }
        else if ( resp.pWCDMASrvStatusInfo->srvStatus == 2 )
        {
            RegistrationState = (ULONG)resp.pWCDMASrvStatusInfo->srvStatus;
        }
        else if ( resp.pLTESrvStatusInfo->srvStatus ==2 )
        {
            RegistrationState =(ULONG)resp.pLTESrvStatusInfo->srvStatus;
        }
        return RegistrationState;
    }
    else
    {
        return ~0;
    }
}

/*
 * Name:     StartLteCdmaDataSession
 *
 * Purpose:  Starts a LTE or CDMA Data Session
 *
 * Params:   isCdma - TRUE for CDMA connection
 *
 * Return:   None
 *
 * Notes:    None
 */
void StartLteCdmaDataSession(BOOL isCdma)
{
    ULONG                       technology = (isCdma) ? TECHNOLOGY_3GPP2: TECHNOLOGY_3GPP;
    ULONG                       profileId3gpp;
    ULONG                       resultCode = 0;
    BYTE                        IPFamilyPreference = 0, profileIdMatch = FALSE;
    BYTE                        idx = 0;
    ULONG                       regState;
    CHAR                        responseBuffer[RESP_BUFFER_LEN];

    /* If connected device is GOBI, return after displaying an error message
     * as LTE data call is not supported on GOBI devices.
     */
    if( IsConnectedDeviceGOBI() == TRUE )
    {
        fprintf( stderr, "LTE/CDMA Data call not supported on this device!!!\n" );
        return;
    }

    /* check registration state */
    regState = getRegState();
    if ( 2 != regState )
    {
        fprintf( stderr, "Modem not registered to network, reg state %d\n", regState);
    }

    /* Get the Model ID */
    resultCode = GetModelID( RESP_BUFFER_LEN, responseBuffer );
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "Failed to get Model ID\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        return;
    }

    if ( (0 == strcmp("SL9090", responseBuffer) ||\
          0 == strcmp("MC9090", responseBuffer)) && (1 == isCdma))
    {
        /* Fill the information for required data session, for SL9090, it only supports mono PDN,
           hence, do not consider mutiple PDN in this case */
        slte[0].action = START_DATA_SEESION;
        slte[0].instanceId = 0;
        slte[0].pTechnology = &technology;
        slte[0].ipfamily = IPv4_FAMILY_PREFERENCE;

        resultCode = SLQSStartStopDataSession(&slte[0]);
        if (SUCCESS != resultCode )
        {
            fprintf( stderr, "\nFailed to start CDMA Data Session\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode);
            fprintf( stderr, "WDS Call End Reason : %u\n", slte[0].failureReason );
            fprintf( stderr,"Verbose Failure Reason Type: %u\n", slte[0].verbFailReasonType );
            return;
        }
    }
    else
    {
        /* handle data connection for NON-SL9090 modules */
        /* Query user for IP Family Preference */
        IPFamilyPreference = GetIPFamilyPreference();

        /* Display all the profiles stored on the device */
        DisplayAllProfiles();

        if( 0 == indexInfo.totalProfilesOnDevice )
        {
            fprintf( stderr, "No Profile exist on the device for Data session\n"
                     "or check device connectivity\n\n" );
            return;
        }

        while(1)
        {
            /* Get the profile id using which the data session needs to be started */
            profileId3gpp  = GetUserProfileId();

            /* If only <ENTER> is pressed by the user, return to main menu */
            if( ENTER_KEY_PRESSED == profileId3gpp )
                return;

            /* If the user has enter an invalid profile id */
            for( idx = 0; idx < indexInfo.totalProfilesOnDevice; idx++ )
            {
                if( profileId3gpp == indexInfo.profileIndex[idx] )
                {
                    profileIdMatch = TRUE;
                    break;
                }
            }

            if( profileIdMatch == TRUE )
            {
                break;
            }
        }

        /* Fill the information for required data session */
        slte[sessionNumLte].action = START_DATA_SEESION;
        slte[sessionNumLte].instanceId = sessionNumLte;/* InstanceId  will be same as sessionNumLte */
        slte[sessionNumLte].pTechnology = &technology;
        if (isCdma)
        {
            slte[sessionNumLte].pProfileId3GPP = NULL;
            profileId3gpp += CDMA_PROFILE_OFFSET;
            slte[sessionNumLte].pProfileId3GPP2 = &profileId3gpp;
        }
        else
        {
            slte[sessionNumLte].pProfileId3GPP = &profileId3gpp;
            slte[sessionNumLte].pProfileId3GPP2 = NULL;
        }
        slte[sessionNumLte].ipfamily = IPFamilyPreference;
        resultCode = SLQSStartStopDataSession(&slte[sessionNumLte]);

        /* Update the Profile information on user output window with the profile
         * selected by the user.
         */
        UpdateProfileInformation( profileId3gpp );

        /* Several reasons are possible to have a non-zero result code
         *         Result Code                    Reason
         *
         * eQCWWAN_ERR_SWICM_V4UP_V6DWN  - IPv4v6 family preference was set. IPv4
         *                                 call succeeded, IPv6 call failed.
         * eQCWWAN_ERR_SWICM_V4DWN_V6UP  - IPv4v6 family preference was set. IPv4
         *                                 call failed, IPv6 call succeeded.
         * eQCWWAN_ERR_SWICM_V4DWN_V6DWN - IPv4v6 family preference was set. IPv4
         *                                 call succeeded, IPv6 call failed.
         * eQCWWAN_ERR_SWICM_V4UP_V6UP   - IPv4v6 family preference was set. Both
         *                                 IPv4 and IPv6 call succeeded.
         * eQCWWAN_ERR_xxx               - IPv4/IPv6 family preference was set and
         *                                 data call failed.
         */

        if( SUCCESS != resultCode )
        {
            if( IPv4v6_FAMILY_PREFERENCE == IPFamilyPreference )
            {
                fprintf( stderr,
                         "\nStart IPv4v6 LTE/CDMA data session( Instance: %x ) status:\n",
                         sessionNumLte );
                switch( resultCode)
                {
                    case eQCWWAN_ERR_SWICM_V4UP_V6DWN:
                        fprintf( stderr, "1. IPv4 session - connected\n" );
                        fprintf( stderr, "2. IPv6 session - disconnected\n" );
                        UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                        break;
                    case eQCWWAN_ERR_SWICM_V4DWN_V6UP:
                        fprintf( stderr, "1. IPv4 session - disconnected\n" );
                        fprintf( stderr, "2. IPv6 session - connected\n" );
                        UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                        break;
                    case eQCWWAN_ERR_SWICM_V4DWN_V6DWN:
                        fprintf( stderr, "1. IPv4 session - disconnected\n" );
                        fprintf( stderr, "2. IPv6 session - disconnected\n" );
                        break;
                    case eQCWWAN_ERR_SWICM_V4UP_V6UP:
                        fprintf( stderr, "1. IPv4 session - connected: %u \n", slte[sessionNumLte].v4sessionId);
                        fprintf( stderr, "2. IPv6 session - connected: %u \n", slte[sessionNumLte].v6sessionId);
                        UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                        break;
                    default:
                        break;
                }
            }
            else
            {
                fprintf( stderr, "\nFailed to start LTE/CDMA Data Session" );
                fprintf( stderr, "Failure Code : %u(0x%04x)\n", resultCode
                        , resultCode );
                fprintf( stderr, "WDS Call End Reason : %u(0x%04x)\n",
                        slte[sessionNumLte].failureReason,
                        slte[sessionNumLte].failureReason );
                fprintf( stderr,
                         "\nVerbose Failure Reason Type: %u(0x%04x)\n",
                         slte[sessionNumLte].verbFailReasonType,
                         slte[sessionNumLte].verbFailReasonType );
                fprintf( stderr,
                         "\nVerbose Failure Reason : %u(0x%04x)\n",
                         slte[sessionNumLte].verbFailReason,
                         slte[sessionNumLte].verbFailReason );

                /* Clear any session ID's which may have been assigned by the SLQS API. This
                 * is required when data session for same instance is already active. */
                if( IPv4_FAMILY_PREFERENCE == IPFamilyPreference  )
                {
                    slte[sessionNumLte].v4sessionId = 0;
                }
                if( IPv6_FAMILY_PREFERENCE == IPFamilyPreference )
                {
                    slte[sessionNumLte].v6sessionId = 0;
                }
                return;
            }
        }

        sessionNumLte++;
        if ( sessionNumLte > MAX_INST_ID+1 )
        {
            sessionNumLte = 0;
        }
    }

    if (isCdma)
    {
        fprintf( stderr, "CDMA Data Session started successfully\n" );
    }
    else
    {
        /* Technology is LTE. Display success if IPFamily is not IPv4v6. */
        if( IPv4v6_FAMILY_PREFERENCE != IPFamilyPreference )
        {
            fprintf( stderr, "LTE Data Session started successfully\n" );
        }
    }

    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
}

/*************************************************************************
 * Option 1 : Start UMTS Data Session
 ************************************************************************/
/*
 * Name:     StartUMTSDataSession
 *
 * Purpose:  Starts a UMTS Data Session
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void StartUMTSDataSession()
{
    ULONG                       technology = TECHNOLOGY_3GPP;
    ULONG                       profileId3gpp;
    ULONG                       resultCode = 0;
    BYTE                        profileIdMatch = FALSE;
    BYTE                        idx = 0;
    BYTE                        ipfamily;
    ULONG                       regState;

    /* Display all the profiles stored on the device */
    DisplayAllProfiles();

    if( 0 == indexInfo.totalProfilesOnDevice )
    {
        fprintf( stderr, "No Profile exist on the device for Data session\n"
                         "or check device connectivity\n\n");
        return;
    }

    /* check registration state */
    regState = getRegState();
    if ( 2 != regState )
    {
        fprintf( stderr, "Modem not registered to network, reg state %d\n", regState);
    }

    while(1)
    {
        /* Get the profile id using which the data session needs to be started */
        profileId3gpp  = GetUserProfileId();

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY_PRESSED == profileId3gpp )
            return;

        /* If the user has enter an invalid profile id */
        for( idx = 0; idx < indexInfo.totalProfilesOnDevice; idx++ )
        {
            if( profileId3gpp == indexInfo.profileIndex[idx] )
            {
                profileIdMatch = TRUE;
                break;
            }
        }

        if( profileIdMatch == TRUE )
        {
            break;
        }
    }

    /* Get the profile id using which the data session needs to be started */
    ipfamily = GetIPFamilyPreference();

    sumts[sessionNum].action = START_DATA_SEESION;
    sumts[sessionNum].instanceId = sessionNum;/* InstanceId  will be same as  sessionNum*/
    sumts[sessionNum].pTechnology = &technology;
    sumts[sessionNum].pProfileId3GPP = &profileId3gpp;
    sumts[sessionNum].pProfileId3GPP2 = NULL;
    sumts[sessionNum].ipfamily = ipfamily;

    /* Update the Profile information on user output window with the profile
     * selected by the user.
     */
    UpdateProfileInformation( profileId3gpp );

    /* Starts the data session */
    resultCode = SLQSStartStopDataSession(&sumts[sessionNum]);

    /* If call fails */
    /* Several reasons are possible to have a non-zero result code
     *         Result Code                    Reason
     *
     * eQCWWAN_ERR_SWICM_V4UP_V6DWN  - IPv4v6 family preference was set. IPv4
     *                                 call succeeded, IPv6 call failed.
     * eQCWWAN_ERR_SWICM_V4DWN_V6UP  - IPv4v6 family preference was set. IPv4
     *                                 call failed, IPv6 call succeeded.
     * eQCWWAN_ERR_SWICM_V4DWN_V6DWN - IPv4v6 family preference was set. IPv4
     *                                 call succeeded, IPv6 call failed.
     * eQCWWAN_ERR_SWICM_V4UP_V6UP   - IPv4v6 family preference was set. Both
     *                                 IPv4 and IPv6 call succeeded.
     * eQCWWAN_ERR_xxx               - IPv4/IPv6 family preference was set and
     *                                 data call failed.
     */

    if( SUCCESS != resultCode )
    {
        if( IPv4v6_FAMILY_PREFERENCE == ipfamily )
        {
            fprintf( stderr,
                     "\nStart IPv4v6 UMTS data session( Instance: %x ) status:\n",
                     sessionNum );
            switch( resultCode)
            {
                case eQCWWAN_ERR_SWICM_V4UP_V6DWN:
                    fprintf( stderr, "1. IPv4 session - connected\n" );
                    fprintf( stderr, "2. IPv6 session - disconnected\n" );
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                case eQCWWAN_ERR_SWICM_V4DWN_V6UP:
                    fprintf( stderr, "1. IPv4 session - disconnected\n" );
                    fprintf( stderr, "2. IPv6 session - connected\n" );
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                case eQCWWAN_ERR_SWICM_V4DWN_V6DWN:
                    fprintf( stderr, "1. IPv4 session - disconnected\n" );
                    fprintf( stderr, "2. IPv6 session - disconnected\n" );
                    break;
                case eQCWWAN_ERR_SWICM_V4UP_V6UP:
                    fprintf( stderr, "1. IPv4 session - connected: %u \n", slte[sessionNumLte].v4sessionId);
                    fprintf( stderr, "2. IPv6 session - disconnected: %u \n", slte[sessionNumLte].v6sessionId);
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                default:
                    break;
            }
        }
        else
        {
            fprintf( stderr, "Failed to start UMTS Data Session\n" );
            fprintf( stderr, "Failure Code : %u(0x%04x)\n",
                     resultCode,
                     resultCode );
            fprintf( stderr, "WDS Call End Reason : %u(0x%04x)\n",
                     sumts[sessionNum].failureReason,
                     sumts[sessionNum].failureReason);
            fprintf( stderr,
                     "\nVerbose Failure Reason Type: %u(0x%04x)\n",
                     sumts[sessionNum].verbFailReasonType,
                     sumts[sessionNum].verbFailReasonType );
            fprintf( stderr,
                     "\nVerbose Failure Reason : %u(0x%04x)\n",
                     sumts[sessionNum].verbFailReason,
                     sumts[sessionNum].verbFailReason );

            /* Clear any session ID's which may have been assigned by the SLQS API. This
             * is required when data session for same instance is already active. */
            if( IPv4_FAMILY_PREFERENCE == ipfamily )
            {
                sumts[sessionNum].v4sessionId = 0;
            }
            if( IPv6_FAMILY_PREFERENCE == ipfamily )
            {
                sumts[sessionNum].v6sessionId = 0;
            }
            return;
        }
    }
    sessionNum++;
    if ( sessionNum > MAX_INST_ID+1 )
    {
        sessionNum = 0;
    }
    /* Technology is LTE. Display success if IPFamily is not IPv4v6. */
    if( IPv4v6_FAMILY_PREFERENCE != ipfamily )
    {
        fprintf( stderr, "UMTS Data Session started successfully\n" );
    }
    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
}

/*
 * Name:     StartCdmaDataSessionRUIM
 *
 * Purpose:  Starts a CDMA Data Session for RUIM Firmware
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void StartCdmaDataSessionRUIM()
{
    ULONG regState;
    BYTE  IPFamilyPreference;

    /* If connected device is GOBI, return after displaying an error message
     * as this data call is not supported on GOBI devices.
     */
    if( IsConnectedDeviceGOBI() == TRUE )
    {
        fprintf( stderr, "This CDMA Data call is not supported on this device!!!\n" );
        return;
    }

    /* check registration state */
    regState = getRegState();
    if ( 2 != regState )
    {
        fprintf( stderr, "Modem not registered to network, reg state %d\n", regState);
    }

    slte[sessionNumLte].action          = START_DATA_SEESION;
    slte[sessionNumLte].instanceId      = sessionNumLte;
    slte[sessionNumLte].pTechnology     = NULL;

    /* Get IP Family Preference from the user*/
    IPFamilyPreference = GetIPFamilyPreference();
    if( ENTER_KEY_PRESSED != IPFamilyPreference )
    {
        slte[sessionNumLte].ipfamily = IPFamilyPreference;
    }
    else
    {
        fprintf( stderr, "Choosing Default IPfamily preference = IPV4 \n");
        slte[sessionNumLte].ipfamily = IPv4_FAMILY_PREFERENCE;
    }

    ULONG resultCode = SLQSStartStopDataSession(&slte[sessionNumLte]);

    /* Several reasons are possible to have a non-zero result code
     *         Result Code                    Reason
     *
     * eQCWWAN_ERR_SWICM_V4UP_V6DWN  - IPv4v6 family preference was set. IPv4
     *                                 call succeeded, IPv6 call failed.
     * eQCWWAN_ERR_SWICM_V4DWN_V6UP  - IPv4v6 family preference was set. IPv4
     *                                 call failed, IPv6 call succeeded.
     * eQCWWAN_ERR_SWICM_V4DWN_V6DWN - IPv4v6 family preference was set. IPv4
     *                                 call succeeded, IPv6 call failed.
     * eQCWWAN_ERR_SWICM_V4UP_V6UP   - IPv4v6 family preference was set. Both
     *                                 IPv4 and IPv6 call succeeded.
     * eQCWWAN_ERR_xxx               - IPv4/IPv6 family preference was set and
     *                                 data call failed.
     */
    if( SUCCESS != resultCode )
    {
        if( IPv4v6_FAMILY_PREFERENCE == IPFamilyPreference )
        {
            fprintf( stderr, "\nStart IPv4v6 CDMA data session status:\n" );
            switch( resultCode)
            {
                case eQCWWAN_ERR_SWICM_V4UP_V6DWN:
                    fprintf( stderr, "1. IPv4 session start succeeded\n" );
                    fprintf( stderr, "2. IPv6 session start failed\n" );
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                case eQCWWAN_ERR_SWICM_V4DWN_V6UP:
                    fprintf( stderr, "1. IPv4 session start failed\n" );
                    fprintf( stderr, "2. IPv6 session start succeeded\n" );
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                case eQCWWAN_ERR_SWICM_V4DWN_V6DWN:
                    fprintf( stderr, "1. IPv4 session start failed\n" );
                    fprintf( stderr, "2. IPv6 session start failed\n" );
                    break;
                case eQCWWAN_ERR_SWICM_V4UP_V6UP:
                    fprintf( stderr, "1. IPv4 session start succeeded -  %u \n", slte[sessionNumLte].v4sessionId);
                    fprintf( stderr, "2. IPv6 session start succeeded -  %u \n", slte[sessionNumLte].v6sessionId);
                    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
                    break;
                default:
                    break;
            }
        }
        else
        {
            fprintf( stderr, "\nFailed to start CDMA Data Session" );
            fprintf( stderr, "Failure Code : %u(0x%04x)\n", resultCode
                    , resultCode );
            fprintf( stderr, "WDS Call End Reason : %u(0x%04x)\n",
                    slte[sessionNumLte].failureReason,
                    slte[sessionNumLte].failureReason );
            fprintf( stderr,
                     "\nVerbose Failure Reason Type: %u(0x%04x)\n",
                     slte[sessionNumLte].verbFailReasonType,
                     slte[sessionNumLte].verbFailReasonType );
            fprintf( stderr,
                     "\nVerbose Failure Reason : %u(0x%04x)\n",
                     slte[sessionNumLte].verbFailReason,
                     slte[sessionNumLte].verbFailReason );
            return;
        }
    }

    sessionNumLte++;
    if ( sessionNumLte > MAX_INST_ID+1 )
    {
        sessionNumLte = 0;
    }
    fprintf( stderr, "CDMA Data Session started successfully\n" );
    UpdateUserDisplay( eCALL_STATUS, "CONNECTED" );
}

/*************************************************************************
 *             Display Window Fields Initialization functions.
 ************************************************************************/
/*
 * Name:     UpdateRSSInfoDisplay
 *
 * Purpose:  Reset the RSS fields on user output window if display flag is FALSE
 *           else set the fields with the passed values..
 *
 * Params:   display  - Flag to check whether to display values or not.
 *                      - TRUE  - Display the values received
 *                      - FALSE - Display UNKNOWN for RSSI parameters
 *           pRssInfo - Pointer to the structure containing value to be
 *                      displayed for RSSI parameters.
 *
 * Return:   None
 *
 * Notes:    None
 */
void UpdateRSSInfoDisplay(
    BYTE                          display,
    struct slqsSignalStrengthInfo *pRssInfo )
{
    CHAR respBuf[MAX_FIELD_SIZE];
    BYTE idx = 0;

    /* Reset the RSS values on user window */
    if( display == FALSE)
    {
        UpdateUserDisplay( eECIO, "UNKOWN" );
        UpdateUserDisplay( eIO, "UNKNOWN" );
        UpdateUserDisplay( eSINR, "UNKNOWN" );
        UpdateUserDisplay( eRSRQ, "UNKNOWN" );
        return;
    }

    /* Reset the buffer and copy the data to be displayed */
    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%d", pRssInfo->Io );
    UpdateUserDisplay( eIO, respBuf );

    /* Update the value of SINR */
    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%d", pRssInfo->sinr );
    UpdateUserDisplay( eSINR, respBuf );

    /* Update the value of RSRQ */
    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%d", pRssInfo->rsrqInfo.rsrq );
    UpdateUserDisplay( eRSRQ, respBuf );

    /* If ECIO information is not present */
    if( 0 == pRssInfo->ecioListLen )
    {
        return;
    }

    /* Get the ECIO value for current radioif */
    for( idx = 0; idx < pRssInfo->ecioListLen; idx++ )
    {
        if( pRssInfo->ecioList[idx].radioIf ==
            pRssInfo->rxSignalStrengthList[0].radioIf )
        {
            break;
        }
    }

    /* Update the value of ECIO */
    memset( respBuf, 0, MAX_FIELD_SIZE );
    sprintf( respBuf, "%d", pRssInfo->ecioList[idx].ecio );
    UpdateUserDisplay( eECIO, respBuf );
}
/*************************************************************************
 *             Display Window Fields Initialization functions.
 ************************************************************************/
/*
 * Name:     UpdateSigInfoDisplay
 *
 * Purpose:  Reset the Sig Info fields on user output window if display flag is FALSE
 *           else set the fields with the passed values..
 *
 * Params:   display  - Flag to check whether to display values or not.
 *                      - TRUE  - Display the values received
 *                      - FALSE - Display UNKNOWN for RSSI parameters
 *           pRssInfo - Pointer to the structure containing value to be
 *                      displayed for RSSI parameters.
 *
 * Return:   None
 *
 * Notes:    None
 */
void UpdateSigInfoDisplay(
    BYTE                    display,
    nasGetSigInfoResp       *pSigInfo )
{
    CHAR respBuf[MAX_FIELD_SIZE];

    /* Reset the RSS values on user window */
    if( display == FALSE )
    {
        UpdateUserDisplay( eECIO, "UNKOWN" );
        UpdateUserDisplay( eIO, "UNKNOWN" );
        UpdateUserDisplay( eSINR, "UNKNOWN" );
        UpdateUserDisplay( eRSRQ, "UNKNOWN" );
        return;
    }
    if ( NULL != pSigInfo->pCDMASSInfo )
    {
        if(-1 != pSigInfo->pCDMASSInfo->ecio)
        {
            /* Update the value of ECIO */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pCDMASSInfo->ecio );
            UpdateUserDisplay( eECIO, respBuf );
        }
        if(-1 != pSigInfo->pCDMASSInfo->rssi)
        {
            /* Update the value of RSSI */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pCDMASSInfo->rssi );
            UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
        }
    }
    if ( NULL != pSigInfo->pHDRSSInfo )
    {
        if(-1 != pSigInfo->pHDRSSInfo->ecio)
        {
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pHDRSSInfo->ecio );
            UpdateUserDisplay( eECIO, respBuf );
        }
        if(0xFF != pSigInfo->pHDRSSInfo->sinr)
        {
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pHDRSSInfo->sinr );
            UpdateUserDisplay( eSINR, respBuf );
        }

        if(-1 != pSigInfo->pHDRSSInfo->io)
        {
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pHDRSSInfo->io );
            UpdateUserDisplay( eIO, respBuf );
        }
    }
    if ( NULL != pSigInfo->pWCDMASSInfo )
    {
        if(-1 != pSigInfo->pWCDMASSInfo->ecio)
        {
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pWCDMASSInfo->ecio );
            UpdateUserDisplay( eECIO, respBuf );
        }
        if(-1 != pSigInfo->pWCDMASSInfo->rssi)
        {
            /* Update the value of RSSI */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pWCDMASSInfo->rssi );
            UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
        }
    }
    if ( NULL != pSigInfo->pLTESSInfo )
    {
        if(-1 != pSigInfo->pLTESSInfo->rsrq)
        {
            /* Update the value of RSRQ */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pLTESSInfo->rsrq );
            UpdateUserDisplay( eRSRQ, respBuf );
        }
        if(-1 != pSigInfo->pLTESSInfo->rssi)
        {
            /* Update the value of RSSI */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", pSigInfo->pLTESSInfo->rssi );
            UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
        }
    }
    if ( NULL != pSigInfo->pGSMSSInfo )
     {
        if(-1 != *pSigInfo->pGSMSSInfo)
        {
            /* Update the value of RSSI */
            memset( respBuf, 0, MAX_FIELD_SIZE );
            sprintf( respBuf, "%d", *pSigInfo->pGSMSSInfo );
            UpdateUserDisplay( eSIGNAL_STRENGTH, respBuf );
        }
     }
}
/*
 * Name:     DisplayRSSInfo
 *
 * Purpose:  Display receive Signal strength information
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void DisplayRSSInfo()
{
    CDMASSInfo  cdmaSSInfo;
    HDRSSInfo   hdrSSInfo;
    INT8        gsmSSInfo;
    CDMASSInfo  wcdmaSSInfo;
    LTESSInfo   lteSSInfo;
    nasGetSigInfoResp resp;

    ULONG                         resultCode = 0;
    memset(&resp,0,sizeof(nasGetSigInfoResp));
    resp.pCDMASSInfo  = &cdmaSSInfo;
    resp.pHDRSSInfo   = &hdrSSInfo;
    resp.pGSMSSInfo   = &gsmSSInfo;
    resp.pWCDMASSInfo = &wcdmaSSInfo;
    resp.pLTESSInfo   = &lteSSInfo;


    /* Get the information about the received Signal Strength */
    resultCode = SLQSNasGetSigInfo( &resp );

    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get RSS Information\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateSigInfoDisplay( FALSE, NULL );
        return;
    }

    /* Display the RSS Information */
    UpdateSigInfoDisplay( TRUE, &resp );
}

/*
 * Name:     DisplaySerialNo
 *
 * Purpose:  Display various Serial numbers of the connected device
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void DisplaySerialNo()
{
    ULONG resultCode;
    CHAR  ESNBuf[MAX_FIELD_SIZE];
    CHAR  IMEIBuf[MAX_FIELD_SIZE];
    CHAR  MEIDBuf[MAX_FIELD_SIZE];

    /* Get the Serial no.s */
    resultCode = GetSerialNumbers( MAX_FIELD_SIZE,
                                   ESNBuf,
                                   MAX_FIELD_SIZE,
                                   IMEIBuf,
                                   MAX_FIELD_SIZE,
                                   MEIDBuf );

    /* If call fails */
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Serial No.s\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif

        UpdateUserDisplay( eESN_NUMBER, "UNKNOWN" );
        UpdateUserDisplay( eIMEI_NUMBER, "UNKNOWN" );
        UpdateUserDisplay( eMEID_NUMBER, "UNKNOWN" );
        return;
    }

    /* If ESN number is not available */
    if( 0 == strlen( ESNBuf ) )
    {
        UpdateUserDisplay( eESN_NUMBER, "UNAVAILABLE" );
    }
    else
    {
        UpdateUserDisplay( eESN_NUMBER, ESNBuf );
    }

    /* If IMEI number is not available */
    if( 0 == strlen( IMEIBuf ) )
    {
        UpdateUserDisplay( eIMEI_NUMBER, "UNAVAILABLE" );
    }
    else
    {
        UpdateUserDisplay( eIMEI_NUMBER, IMEIBuf );
    }

    /* If MEID number is not available */
    if( 0 == strlen( MEIDBuf ) )
    {
        UpdateUserDisplay( eMEID_NUMBER, "UNAVAILABLE" );
    }
    else
    {
        UpdateUserDisplay( eMEID_NUMBER, MEIDBuf );
    }
}

/*
 * Name:     DisplayProfileInformation
 *
 * Purpose:  Display the Profile Information of the connected device
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void DisplayProfileInformation()
{
    ULONG resultCode = 0;
    struct profileInformation profileInfo;

    profileInfo.profileType = PROFILE_TYPE_UMTS;

    /* Get the Default Profile */
    resultCode = GetDefaultProfile(  profileInfo.profileType,
                                     &( profileInfo.PDPType ),
                                     &( profileInfo.IPAddress ),
                                     &( profileInfo.primaryDNS ),
                                     &( profileInfo.secondaryDNS ),
                                     &( profileInfo.Authentication ),
                                     MAX_PROFILE_NAME_SIZE - 1,
                                     profileInfo.profileName,
                                     MAX_APN_SIZE - 1,
                                     profileInfo.APNName,
                                     MAX_USER_NAME_SIZE - 1,
                                     profileInfo.userName );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Default Profile\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateProfileInfo( FALSE, &profileInfo );
        return;
    }

    /* Update the profile information on user window */
    UpdateProfileInfo( TRUE, &profileInfo );
}

/*
 * Name:     DisplayDeviceinfo
 *
 * Purpose:  Display the information about the connected device
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
static void DisplayDeviceinfo()
{
    ULONG resultCode = 0, sessionStateV4 = 0, sessionStateV6 = 0;
    CHAR  responseBuffer[RESP_BUFFER_LEN];
    CHAR  bootBuffer[RESP_BUFFER_LEN];
    CHAR  priBuffer[RESP_BUFFER_LEN];
    WORD  PRLVersion = 0;
    BYTE  PRLPreference = 0xFF;
    dmsCurrentPRLInfo CurrentPRLInfo;
    WORD  MCC, MNC, SID, NID;
    CHAR  *pNAString = "UNAVAILABLE";

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the Device Manufacture ID */
    resultCode = GetManufacturer( RESP_BUFFER_LEN, responseBuffer );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Manufacture ID\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eMANUFACTURE_ID, pNAString );
    }
    else
    {
        /* Update the Manufacture ID field of the user window */
        UpdateUserDisplay( eMANUFACTURE_ID, responseBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the Model ID */
    resultCode = GetModelID( RESP_BUFFER_LEN, responseBuffer );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Model ID\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eMODEL_ID, pNAString );
    }
    else
    {
        /* Update the Model ID field of the user window */
        UpdateUserDisplay( eMODEL_ID, responseBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the Firmware Revision */
    resultCode = GetFirmwareRevisions( RESP_BUFFER_LEN, responseBuffer,
                                       RESP_BUFFER_LEN, bootBuffer,
                                       RESP_BUFFER_LEN, priBuffer);
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Firmware Revision\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eFIRMWARE_REVISION, pNAString );
        UpdateUserDisplay( eBOOT_VERSION, pNAString );
        UpdateUserDisplay( ePRI_VERSION, pNAString );
    }
    else
    {
        /* Update the Firmware Revision field of the user window */
        UpdateUserDisplay( eFIRMWARE_REVISION, responseBuffer );
        UpdateUserDisplay( eBOOT_VERSION, bootBuffer );
        UpdateUserDisplay( ePRI_VERSION, priBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the PRL Version */
    CurrentPRLInfo.pPRLVersion = &PRLVersion;
    CurrentPRLInfo.pPRLPreference = &PRLPreference;
    resultCode = SLQSGetCurrentPRLInfo( &CurrentPRLInfo );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get PRL Version\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( ePRL_VERSION, pNAString );
        UpdateUserDisplay( ePRL_PREFERENCE, pNAString );
    }
    else
    {
        /* Copy the PRL Version into a buffer */
        sprintf ( responseBuffer, "%d", *(CurrentPRLInfo.pPRLVersion) );

        /* Update the PRL Version field of the user window */
        UpdateUserDisplay( ePRL_VERSION, responseBuffer );

        /* Copy the PRL Preference into a buffer */
        sprintf ( responseBuffer, "%d", *(CurrentPRLInfo.pPRLPreference) );

        /* Update the PRL Preference field of the user window */
        UpdateUserDisplay( ePRL_PREFERENCE, responseBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the IMSI */
    resultCode = GetIMSI( RESP_BUFFER_LEN, responseBuffer );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get IMSI\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eIMSI, pNAString );
    }
    else
    {
        /* Update the IMSI field of the user window */
        UpdateUserDisplay( eIMSI, responseBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the Hardware Revision */
    resultCode = GetHardwareRevision( RESP_BUFFER_LEN, responseBuffer );
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Hardware Revision\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eHARDWARE_REVISION, pNAString );
    }
    else
    {
        /* Update the Hardware Revision field of the user window */
        UpdateUserDisplay( eHARDWARE_REVISION, responseBuffer );
    }

    /* Reset the buffer */
    memset( responseBuffer, 0, RESP_BUFFER_LEN );

    /* Get the Home Network */
    resultCode = GetHomeNetwork( &MCC,
                                 &MNC,
                                 RESP_BUFFER_LEN,
                                 responseBuffer,
                                 &SID,
                                 &NID);
    if( SUCCESS != resultCode )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to get Home Network\n" );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        #endif
        UpdateUserDisplay( eHOME_NETWORK, pNAString );
    }
    else
    {
        /* Update the Home Network field in logs */
        UpdateUserDisplay( eHOME_NETWORK, responseBuffer );
    }

    /* Update the Profile Information fields in logs */
    DisplayProfileInformation();

    /* Update the Serial number fields in logs */
    DisplaySerialNo();

    /* Update the call status field as DISCONNECTED */
    UpdateUserDisplay( eCALL_STATUS, "DISCONNECTED" );
    int instance =0;
    for ( instance = 0; instance <=MAX_INST_ID; instance++ )
    {
        /* Get the Session State */
        resultCode = SLQSGetSessionState( &sessionStateV4,
                                        &sessionStateV6,
                                        instance );

        if( SUCCESS != resultCode )
        {
            #ifdef DBG
            fprintf( stderr, "Failed to get Session State\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            #endif
            fprintf( stderr, "Instance ID :%d\n", instance);
            fprintf( stderr, "Failed to get Session State\n" );
        }
        else
        {
            fprintf( stderr, "Instance ID :%d\n", instance);
            fprintf( stderr, "Session StateV4:%d\n", sessionStateV4 );
            fprintf( stderr, "Session StateV6:%d\n", sessionStateV6 );
        }
    }

    /* Display the receive Signal strength information */
    DisplayRSSInfo();

    /* Initialize the IP and DNS Address fields */
    UpdateUserDisplay( eIP_ADDRESS, "0.0.0.0" );
    UpdateUserDisplay( ePRIMARY_DNS, "0.0.0.0" );
    UpdateUserDisplay( eSECONDARY_DNS, "0.0.0.0" );
}

/*************************************************************************
 * Application starting and SDK initialization functions.
 ************************************************************************/
/*
 * Name:     StartSDK
 *
 * Purpose:  It starts the SDK by setting the SDK path, enumerates the device
 *           and connects to the SDK.
 *
 * Params:   None
 *
 * Return:   SUCCESS on successfully starting SDK, else error code
 *
 * Notes:    None
 */
ULONG StartSDK(BYTE modem_index)
{
    ULONG resultCode  = 0;
    BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( SUCCESS != (resultCode = SetSDKImagePath(sdkbinpath)) )
    {
        rcprint( __func__, resultCode );
        return resultCode;
    }

    /* Establish APP<->SDK IPC */
    if( SUCCESS != (resultCode = SLQSStart(modem_index, NULL)) )
    {
        /* first attempt failed, kill SDK process */
        if( SUCCESS != SLQSKillSDKProcess() )
        {
            return resultCode;
        }
        else
        {
            /* start new SDK process */
            if( SUCCESS != (resultCode = SLQSStart(modem_index, NULL)) )
            {
                return resultCode;
            }
        }
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

/*************************************************************************
  Signal handler: To take care of <defunct> process
*************************************************************************/
/*
 * Name:     appSignalInstall
 *
 * Purpose:  install signal handler
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void appSignalInstall( unsigned int signo,  void (*functionp)(int, siginfo_t *, void *) )
{
    struct sigaction sa;

    if (functionp == NULL)
        return;
    sa.sa_sigaction = functionp;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_NODEFER | SA_SIGINFO;

    if (sigaction(signo, &sa, NULL) < 0)
    {
        fprintf(stderr,"Signal handling falied");
    }
}

/*
 * Name:     appSIGCHLDhandler
 *
 * Purpose:  appSIGCHLDhandler
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void appSIGCHLDhandler (int signo, siginfo_t *siginfop, void *contextp)
{
    int status;

    UNUSEDPARAM ( signo );
    UNUSEDPARAM ( siginfop );
    UNUSEDPARAM ( contextp );

    /* Allow the SDK process to terminate */
    wait(&status);
}

/*
 * Name:     SignalInit
 *
 * Purpose:  initialize signal handler
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void SignalInit()
{
   appSignalInstall (SIGCHLD, appSIGCHLDhandler);
}

/*
 * Name:     PowerCycleDev
 *
 * Purpose:  Power cycle the device
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void PowerCycleDev()
{
    ULONG resultCode = 0;
    devready = 0;

    resultCode = SetDeviceStateChangeCbk( DevStateChgCbk );
    if( SUCCESS != resultCode  )
    {
        fprintf( stderr, "REGISTRATION FAILED - Device State Change Callback\n"\
                         "Failure Code: %d\n", resultCode );
        return;
    }

    /* Reset Device, the only transition for RESET is OFFLINE -> RESET, so set the device to OFFLINE first, then reset */
     resultCode = SetPower( POWER_OFF_MODE );
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "Failed to set device offline:"
                         "Failure Code : %u\n", resultCode );
        return;
    }
 
    /* Reset Device */
    resultCode = SetPower( POWER_RESET_MODE );


    /* Reset Device */
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "Failed to Reset device:"
                         "Failure Code : %u\n", resultCode );
        return;
    }

    /* Reset the session ID */
    fprintf( stderr, "Device power cycled:Wait for device to be Ready \n");

    while( !devready )
    {
        fprintf( stderr, ".");
        sleep(2);
    }

}

/*************************************************************************
* Option 10 : Set Power Mode
*************************************************************************/
/*
 * Name:     SetPowerMode
 *
 * Purpose:  Sets the power mode
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void SetPowerMode()
{
    ULONG resultCode = 0;
    CHAR  selOption[OPTION_LEN];
    CHAR  *pEndOfLine = NULL;
    ULONG power_mode = 0;

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease select one of the following power modes or press <Enter> to return:\n"\
                         "0. Online (default)\n"\
                         "1. Low power (airplane) mode\n"\
                         "2. Factory test mode\n"\
                         "3. Offline\n"\
                         "4. Reset\n"\
                         "5. Power off\n"\
                         "6. Persistent low power (airplane) mode\n"\
                         "Option : ");

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

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return ;

        /* If user has entered an invalid input, prompt again */
        if( 2 < strlen(selOption) )
        {
            continue;
        }

        if( selOption[0] > '6' )
        {
            continue;
        }
        power_mode = selOption[0] - 0x30 ;

        /* Call SetPower API */
        resultCode = SetPower(power_mode);

        /* Display the result of Set Power Mode */
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "\nFailed to Set Device Power Mode\n" );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            return;
        }
        fprintf( stderr, "\nPower Mode: %d is Set \n",power_mode );
    }
}

static void QosGetNetworkSupport()
{
    ULONG   resultCode;
    BYTE status;
    resultCode = SLQSQosGetNetworkStatus(0, &status);
    if( eQCWWAN_ERR_NONE == resultCode )
    {
        fprintf(stderr, "Qos Network support status: %d\n", status);
    }
}

static void QosGetFlowStatus(ULONG qos_id)
{
    ULONG   resultCode;
    BYTE status;
    resultCode = SLQSQosGetFlowStatus(0, qos_id, &status);
    if( eQCWWAN_ERR_NONE == resultCode )
    {
        fprintf(stderr, "Qos Flow status[0x%x]: %d\n", qos_id, status);
    }
}

void QosTesting()
{
    ULONG qos_id;
    qos_id = QosRequest();
    QosGetNetworkSupport();
    QosGetFlowStatus(qos_id);
    QosGetGranted(qos_id);
    QosRelease(qos_id);
}

/*
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Params:   None
 *
 * Return:   EXIT_SUCCESS, EXIT_FAILURE on unexpected error
 *
 * Notes:    None
 */
int main( int argc, const char *argv[])
{
    int value;
    BYTE modem_index;
    ULONG resultCode = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    ULONG userOption;

    if( argc < 3 )
    {
        fprintf( stderr, "usage: %s <path to sdk binary> <modem_index>\n", argv[0] );
        fprintf( stderr, "where modem_index start from 0\n");
        exit( EXIT_SUCCESS );
    }

    if( NULL == (sdkbinpath = malloc(strlen(argv[1]) + 1)) )
    {
        perror(__func__);
        exit( EXIT_FAILURE );
    }

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1);

    value = atoi(argv[2]);
    if ( 
            (value < 0)
            || (value > 8)
       )
    {
        fprintf( stderr, "cannot convert second param into modem_index\n");
        exit( EXIT_FAILURE );
    }
    modem_index = value;

    /* Start the SDK */
    resultCode = StartSDK(modem_index);

    if( SUCCESS != resultCode )
    {
        free(sdkbinpath);

        /* Display the failure reason */
        fprintf( stderr,  "Failed to start SDK : Exiting App\n"\
                          "Failure Code : %u\n", resultCode );

        /* Failed to start SDK, exit the application */
        exit( EXIT_FAILURE );
    }

    /* Initialize the output log file */
    InitializeDisplay();

    /* Being here means, device is connected, update the required field */
    UpdateUserDisplay( eDEVICE_STATE, "DEVICE CONNECTED" );

    /* Display the information about the connected device */
    DisplayDeviceinfo();

    /* Subscribe to all the required callbacks */
    SubscribeCallbacks();

    /* Initialize signal handler */
    SignalInit();

    while(1)
    {
        /* Print the menu */
        fprintf( stderr, "\nPlease select one of the following options or press <Enter> to exit:\n"\
                         "1.  Start UMTS Data Session\n"\
                         "2.  Start LTE Data Session\n"\
                         "3.  Start CDMA Data Session\n"\
                         "4.  Start RUIM data session\n"
                         "5.  Stop the currently active Data Session\n"\
                         "6.  Display all the profiles stored on the device\n"\
                         "7.  Display the settings for a particular profile stored on the device\n"\
                         "8.  Create a Profile on the device\n"\
                         "9.  Modify the settings of an existing profile stored on the device\n"\
                         "10. Delete a profile stored on the device\n"\
                         "11. Power cycle the device\n"\
                         "12. Set Device Power Mode\n"
                         "13. Qos testing\n"
                         "Option : ");

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

        #ifdef DBG
        fprintf( stderr, "Selected Option : %s\n", selOption );
        #endif

        len = strlen( selOption );

        /* If only <ENTER> is pressed by the user quit Application */
        if( ENTER_KEY == selOption[0] )
        {
            QuitApplication();
            return ENTER_KEY_PRESSED;
        }

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userOption = atoi( selOption );

        /* Process user input */
        switch( userOption ){
            case eSTART_UMTS_DATA_SESSION:
                StartUMTSDataSession();
                break;

            case eSTART_LTE_DATA_SESSION:
                StartLteCdmaDataSession(0);
                break;

            case eSTART_CDMA_DATA_SESSION:
                StartLteCdmaDataSession(1);
                break;

            case eSTART_CDMA_RUIM_DATASESSION:
                StartCdmaDataSessionRUIM();
                break;

            case eSTOP_DATA_SESSION:
                StopCurrentDataSession();
                break;

            case eDISPLAY_ALL_PROFILES:
                DisplayAllProfile();
                break;

            case eDISPLAY_SINGLE_PROFILE:
                DisplayProfileInfo();
                break;

            case eCREATE_PROFILE:
                CreateProfile();
                break;

            case eMODIFY_PROFILE_SETTINGS:
                ModifyProfileSettings();
                break;

            case eDELETE_PROFILE:
                DeleteProfileFromDevice();
                break;

            case eRESET_DEVICE:
                PowerCycleDev();
                break;

            case eSET_POWER_MODE:
                SetPowerMode();
                break;

            case eQOS_TESTING:
                QosTesting();
                break;

            default:
                break;
        }
    }
}
