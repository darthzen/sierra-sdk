/*************
 *
 * Filename:    SLQSsampleCM.c
 *
 * Purpose:     Sample Connection Manager Test Application
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>

#define DEV_NODE_SZ 256
#define DEV_KEY_SZ  16
#define ARGS_MAX    256

/* enums */
enum {
  eSLQSStart,
  eQCWWAN2kEnumerateDevices,
  eQCWWANConnect,
  eQCWWANDisconnect,
  eQCWWANGetConnectedDeviceID,
  eGetSessionState,
  eStartv4DataSession,
  eStartv6DataSession,
  eStartv4v6DataSession,
  eStopDataSession,
  ePerformNetworkScan,
  eSLQSSetBandPreference,
  eSLQSSetSysSelectionPref,
  eSLQSKillSDKProcess,
  eExit
};

/* user types */
typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
}device_info_t;

typedef struct session_state_param{
  int iptype;
  BOOL connected;
  struct ssdatasession_params sd;
}session_state_t;

typedef struct networkscan_param{
  WORD  mcc;
  WORD  mnc;
  ULONG inuse;
  ULONG roaming;
  ULONG forbidden;
  ULONG preferred;
  CHAR  description[255];
}networkscan_t;

typedef struct networkreg_param{
  ULONG regType;
  WORD  mcc;
  WORD  mnc;
  ULONG rat;
}networkreg_t;

typedef struct sytemsel_param{
   sysSelectPrefParams SysSelectPrefParams;
}systemsel_t;

typedef ULONG (*apiproc)(void*,...);
struct apiProcTable_entry{
  char str[80];
  ULONG (*proc)(void*,...);
};

typedef struct profile_info{
  ULONG  profileType;
  ULONG *pPDPType;
  ULONG *pIPAddress;
  ULONG *pPrimaryDNS;
  ULONG *pSecondaryDNS;
  ULONG *pAuthentication;
  CHAR  *pName;
  CHAR  *pAPNName;
  CHAR  *pUsername;
  CHAR  *pPassword;
}profile_info_t;

typedef struct{
    enum eQCWWANError e;
    const char *es;
}slqserr_s;

/* Prototypes */
static ULONG wSLQSStart(void *);
static ULONG wQCWWANEnumerateDevices(device_info_t* pdevices);
static ULONG wQCWWANConnect(device_info_t* pdev);
static ULONG wQCWWANGetConnectedDevice(device_info_t* pdev);
static ULONG wGetSessionState(void *);
static ULONG wStartDataSession(session_state_t *ss);
static ULONG wStopDataSession(ULONG sessionId);
static ULONG wPerformNetworkScan(BYTE* pinstanceSize, networkscan_t* pinstances);
static ULONG wSLQSSetBandPreference(ULONGLONG bandpref);
static ULONG wSLQSSetSysSelectionPref(systemsel_t *pin);
static ULONG wQCWWANDisconnect(void *);
static ULONG wSLQSKillSDKProcess(void *);
static int executeApiProc(int);
static const char *slqserrstr(ULONG);

/* macros*/
#define rcprint(s, u) syslog(LOG_USER, "%s: rc = 0x%X, %s", s, u, slqserrstr(u))

/* path to sdk binary */
static char *sdkbinpath = NULL;

/* critical section vars */
static pthread_cond_t cond;
static pthread_mutex_t mutex;

/* state vars */
static BOOL devready = 0;
static BOOL sdkbound = 0;
static ULONG rc;

/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

/* wireless data service */
static ULONG GPPtech=1;
static ULONG GPPprofiles[] = {1,2,3};
static session_state_t ssv4[4];
static session_state_t ssv6[6];
static session_state_t ssv4v6[7];

/* network access service */
static BYTE instanceSize = 20;
static networkscan_t instances[20];
static networkreg_t nwreg;

static ULONGLONG    bandpref;
static systemsel_t  syssel;

/* wireless data service */
static profile_info_t dfltProf;

/* error strings */
static slqserr_s errstr[] =
{
    { eQCWWAN_ERR_INTERNAL,
      "eQCWWAN_ERR_INTERNAL" },

    { eQCWWAN_ERR_MEMORY,
      "eQCWWAN_ERR_MEMORY" },

    { eQCWWAN_ERR_INVALID_ARG,
      "eQCWWAN_ERR_INVALID_ARG" },

    { eQCWWAN_ERR_BUFFER_SZ,
      "eQCWWAN_ERR_BUFFER_SZ" },

    { eQCWWAN_ERR_NO_DEVICE,
      "eQCWWAN_ERR_NO_DEVICE" },

    { eQCWWAN_ERR_SWIDCS_IOCTL_ERR,
      "eQCWWAN_ERR_SWIDCS_IOCTL_ERR" },

    { eQCWWAN_ERR_SWIDCS_DEVNODE_NOT_FOUND,
      "eQCWWAN_ERR_SWIDCS_DEVNODE_NOT_FOUND" },

    { eQCWWAN_ERR_SWIDCS_IOCTL_ERR,
      "eQCWWAN_ERR_SWIDCS_IOCTL_ERR" },

    { eQCWWAN_ERR_SWIDCS_APP_DISCONNECTED,
      "eQCWWAN_ERR_SWIDCS_APP_DISCONNECTED" },

    { eQCWWAN_ERR_SWICM_QMI_SVC_NOT_SUPPORTED,
      "eQCWWAN_ERR_SWICM_QMI_SVC_NOT_SUPPORTED" },

    { eQCWWAN_ERR_QMI_MISSING_ARG,
      "eQCWWAN_ERR_QMI_MISSING_ARG" },

    { eQCWWAN_ERR_SWICM_SOCKET_IN_USE,
     "eQCWWAN_ERR_SWICM_SOCKET_IN_USE" },

    { 0, "" }
};

/* API table */
static struct apiProcTable_entry apiProcTable[] =
{
  { "SLQSStart",
    (apiproc)wSLQSStart},

  { "QCWWANEnumerateDevices",
    (apiproc)wQCWWANEnumerateDevices},

  { "QCWWANConnect",
    (apiproc)wQCWWANConnect},

  { "QCWWANDisconnect",
    (apiproc)wQCWWANDisconnect},

  { "QCWWANGetConnectedDevice",
    (apiproc)wQCWWANGetConnectedDevice},

  { "GetSessionState",
    (apiproc)wGetSessionState},

  { "Startv4DataSession",
    (apiproc)wStartDataSession},

  { "Startv6DataSession",
    (apiproc)wStartDataSession},

  { "Startv4v6DataSession",
    (apiproc)wStartDataSession},

  { "StopDataSession",
    (apiproc)wStopDataSession},

  { "PerformNetworkScan",
    (apiproc)wPerformNetworkScan},

  { "SLQSSetBandPreference",
    (apiproc)wSLQSSetBandPreference},

  { "SLQSSetSysSelectionPref",
    (apiproc)wSLQSSetSysSelectionPref},

#if 0
  { "SetDefaultProfile",
    (apiproc)wSetDefaultProfile},
#endif
  { "SLQSKillSDKProcess",
    (apiproc)wSLQSKillSDKProcess},

  { "Exit",
    NULL },

  { "",
    NULL }
};

/* Functions */
static const char *slqserrstr(ULONG er)
{
    int count = 0;

    while( errstr[count].e ){
        if( errstr[count].e == (enum eQCWWANError)er )
        {
            return errstr[count].es;
        }
        count++;
    }

    return "";
}

static void appstatechange()
{
    /* device is ready and application is already registered with SDK */
    if( devready && sdkbound )
    {
        syslog(LOG_USER, "%s: device ready, APP connected to SDK\n", __func__);
    }
    /* device is ready and application has not registered with SDK */
    else if( devready && !sdkbound )
    {
        syslog(LOG_USER, "%s: device ready, APP disconnected from SDK\n", __func__);
    }
    /* device is disconnected and application has registered with SDK */
    else if( sdkbound )
    {
        syslog(LOG_USER, "%s: device disconnected, APP connected to SDK\n", __func__);
    }
    /* device is disconnected and application has not registered with SDK */
    else
    {
        syslog(LOG_USER, "%s: device disconnected, APP disconnected from SDK\n", __func__);
    }
}

static void devstatecb(eDevState device_state)
{
    /* critical section - start */
    pthread_mutex_lock(&mutex);
    devready = device_state == DEVICE_STATE_READY ? 1 : 0 ;
    pthread_mutex_unlock(&mutex);
    /* critical section - end */
    rcprint("Device State Change Callback Invoked", (ULONG)device_state );
    appstatechange();
}

static void cleanup(const char *msg, ULONG ercode)
{
    syslog( LOG_USER, "%s: %s (0x%X)", __func__, msg, ercode );

    if( sdkbound )
        executeApiProc(eQCWWANDisconnect);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    free(sdkbinpath);

    if( eQCWWAN_ERR_NONE != ercode)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
}

/*------------------------------
  Network Access Service Set Band Preference

  ------------------------------*/
ULONG wSLQSSetBandPreference(ULONGLONG bandpref)
{
/* bandpref bitmask representation set below bands.
 * GSM DCS band, GSM Extended GSM band, GSM Primary GSM band
 * GSM 850 band, GSM PCS band, WCDMA EUROPE JAPAN and CHINA IMT 2100
 * WCDMA US PCS 1900, WCDMA US 850 band, WCDMA EUROPE and JAPAN 900 band*/
  bandpref = 0x2000004e80380;

  rc = SLQSSetBandPreference(bandpref );

  rcprint( __func__, rc );

  fprintf(  stderr, "bandPref: 0X%llx\n",
            bandpref );

  return rc;
}

/*------------------------------
  Network Access Service Set System Selection Preference

  ------------------------------*/
ULONG wSLQSSetSysSelectionPref(systemsel_t *pin)
{
  struct netSelectionPref NetSelPref = { 0, 0, 0 };
  WORD      ModePref    = 0x08; //UMTS

  memset( (char*) &pin->SysSelectPrefParams, 0, sizeof( sysSelectPrefParams) );

  pin->SysSelectPrefParams.pModePref           = &ModePref;
  pin->SysSelectPrefParams.pNetSelPref         = &NetSelPref;
  pin->SysSelectPrefParams.pNetSelPref->netReg = 0x00; /* automatic registration */
  pin->SysSelectPrefParams.pNetSelPref->mcc    = 0x00;
  pin->SysSelectPrefParams.pNetSelPref->mnc    = 0x00;

  rc = SLQSSetSysSelectionPref( &pin->SysSelectPrefParams  );

  rcprint( __func__, rc );

  fprintf(  stderr, "regtype: %d, mnc: %d, mnc: %d , Pref Mode %d\n",
            pin->SysSelectPrefParams.pNetSelPref->netReg,
            pin->SysSelectPrefParams.pNetSelPref->mcc,
            pin->SysSelectPrefParams.pNetSelPref->mnc,
            *pin->SysSelectPrefParams.pModePref );

  return rc;
}

ULONG wPerformNetworkScan(BYTE* pinstanceSize, networkscan_t* pinstances)
{
    rc = PerformNetworkScan(pinstanceSize, (BYTE *)pinstances);

    if( eQCWWAN_ERR_NONE == rc )
    {
        syslog(LOG_USER, "%s: pinstanceSize = %u\n", __func__, *pinstanceSize);
        for( int i =  *pinstanceSize ; i ; i-- )
        {
            fprintf( stderr,
                    "mcc: %d, mnc: %d, inuse: %u, roaming: %u forbidden:"\
                     "%u, preferred: %u, description: %s\n",
                     pinstances[i-1].mcc,
                     pinstances[i-1].mnc,
                     pinstances[i-1].inuse,
                     pinstances[i-1].roaming,
                     pinstances[i-1].forbidden,
                     pinstances[i-1].preferred,
                     pinstances[i-1].description );
        }
        nwreg.mcc = pinstances[0].mcc;
        nwreg.mnc = pinstances[0].mnc;
    }
    else
    {
        rcprint( __func__, rc );
    }

    return rc;
}
/*------------------------------
  Network Access Service END
  ------------------------------*/

/*-----------------------------
  Wireless Data Service START
  -----------------------------*/
static void DataSessionPrint(ULONG rc, session_state_t *pss)
{
    if( pss == NULL )
        return;

    fprintf(    stderr,
                "\taction: %s    Profile ID: %u   IP Family: %u\n" \
                "\tfailure reasons: gen: 0x%-8X v4: 0x%-8X v6: 0x%-8X\n" \
                "\treturn codes:    gen: %-8u   v4: %-8u   v6: %-8u\n" \
                "\tsession IDs:     cur: %-8u   v4: %-8u   v6: %-8u\n",
                (ULONG)pss->sd.action == 1 ? "START": "STOP",
                *pss->sd.pProfileId3GPP,
                (ULONG)pss->sd.ipfamily,
                pss->sd.failureReason,
                pss->sd.failureReasonv4,
                pss->sd.failureReasonv6,
                rc,
                pss->sd.rcv4,
                pss->sd.rcv6,
                pss->sd.sessionId,
                pss->sd.v4sessionId,
                pss->sd.v6sessionId );
}

static int findSessionId(session_state_t *pss, ULONG sid)
{
    int i;
    for( i = 0 ; i < 4 ; i++ )
    {
        if( pss[i].connected &&
            ( pss[i].sd.v4sessionId == sid || pss[i].sd.v6sessionId == sid) )
        {
            return i;
        }
    }
    return -1;
}

ULONG wStopDataSession(ULONG sessionId)
{
    int idx = -1;
    ULONG rc = eQCWWAN_ERR_INVALID_ARG;
    session_state_t *pss = NULL;

    /* v4 session id */
    if( -1 != (idx = findSessionId(ssv4, sessionId)) )
    {
        ssv4[idx].sd.action = 0;
        rc = SLQSStartStopDataSession(&ssv4[idx].sd);
        if( eQCWWAN_ERR_NONE == rc ) {
            ssv4[idx].connected = 0;
        }
        pss = &ssv4[idx];
    }

    /* v6 session id */
    else if( -1 != (idx = findSessionId(ssv6, sessionId)) )
    {
        ssv4[idx].sd.action = 0;
        rc = SLQSStartStopDataSession(&ssv6[idx].sd);
        if( eQCWWAN_ERR_NONE == rc ) {
            ssv6[idx].connected = 0;
        }
        pss = &ssv6[idx];
    }

    /* v4v6 session id */
    else if( -1 != (idx = findSessionId(ssv4v6, sessionId)) )
    {
        ssv4[idx].sd.action = 0;
        rc = SLQSStartStopDataSession(&ssv4v6[idx].sd);
        if( eQCWWAN_ERR_NONE == rc ) {
            ssv4v6[idx].connected = 0;
        }
        pss = &ssv4v6[idx];
    }

    DataSessionPrint(rc, pss);
    return rc;
}

static ULONG wStartDataSession(session_state_t *pss)
{
    int i;
    for( i = 0 ; i < 3 ; i++ )
    {
        if( !pss[i].connected )
        {
            pss[i].sd.action = 1;
            /* set the profile id to 1 for first data session, 2 for second one */
            *(pss[i].sd.pProfileId3GPP) = (ULONG)(i+1);
            rc = SLQSStartStopDataSession(&pss[i].sd );
            if( eQCWWAN_ERR_NONE == rc ) {
                pss[i].connected = 1;
            }
            DataSessionPrint(rc, &pss[i]);
            return rc;
        }
    }

    fprintf(stderr, "All v%s sessions are active\n", pss->iptype == 7 ? "4v6" :
                                                     pss->iptype == 4 ? "4"   : "6" );
    return rc;
}

static ULONG wGetSessionState(void *dummy)
{
    ULONG State;

    UNUSEDPARAM ( dummy );
    rc = GetSessionState( &State,0 );
    rcprint(__func__, rc );
    if( eQCWWAN_ERR_NONE == rc )
        fprintf( stderr,"session state = %u\n", State);

  return State;
}

/*-----------------------------
  Wireless Data Service  END
  -----------------------------*/

/*-----------------------------
  Device Connectivity START
  -----------------------------*/
static ULONG wQCWWANGetConnectedDevice(device_info_t* pdev)
{
  rc = QCWWAN2kGetConnectedDeviceID(
          DEV_NODE_SZ,
          pdev->deviceNode,
          DEV_KEY_SZ,
          pdev->deviceKey  );

  rcprint( __func__, rc );

  if( eQCWWAN_ERR_NONE == rc )
  {
      fprintf( stderr, "deviceNode: %s\ndeviceKey: %s\n",
               pdev->deviceNode,
               pdev->deviceKey  );

  }
  return rc;
}

static ULONG wSLQSStart(void *dummy)
{
    UNUSEDPARAM ( dummy );

    /* Set SDK executable path */
    if( eQCWWAN_ERR_NONE != (rc = SetSDKImagePath(sdkbinpath) ) )
    {
        cleanup( __func__, rc );
    }

    /* establish app<->sdk IPC */
    rc = SLQSStart(0, NULL);
    if( eQCWWAN_ERR_SWICM_SOCKET_IN_USE == rc )
    {
        rcprint("Another APP is using the SDK", rc);
        cleanup( __func__, rc );
    }
    else if( eQCWWAN_ERR_NONE != rc )
    {
        if( eQCWWAN_ERR_NONE != executeApiProc(eSLQSKillSDKProcess) )
        {
            rcprint( __func__, rc );
        }
        else
        {
            rcprint( __func__, rc );
            if( eQCWWAN_ERR_NONE != (rc = SLQSStart(0, NULL)) )
            {
                cleanup( __func__, rc );
            }
            else
                rcprint("SDK process restarted", rc );
        }
    }
    else
    {
        syslog(LOG_USER, "%s: APP<->SDK IPC init successful\n", __func__);
    }

    /* Register for device state change notification */
    rc = SetDeviceStateChangeCbk( &devstatecb );
    if( eQCWWAN_ERR_NONE != rc )
    {
        cleanup( "APP failed to register for Device State Change notification", rc );
    }
    else
    {
        syslog( LOG_USER,
                "%s: APP registered for Device State Change notification\n",
                __func__ );
    }

#ifdef AUTOCONNECT
    if( eQCWWAN_ERR_NONE != (rc  = executeApiProc(eQCWWANEnumerateDevices)) )
    {
        rcprint( __func__, rc );
    }
    else if( eQCWWAN_ERR_NONE != (rc  = executeApiProc(eQCWWANConnect)) )
    {
        rcprint( __func__, rc );
    }
    if( eQCWWAN_ERR_NONE != (rc  = executeApiProc(eStartDataSession)) )
    {
         rcprint( __func__, rc );
    }
#endif
    return rc;
}

/* Kill SDK process when all else fails. This can occur, for
   example, if the application was started with non root
   priveleges. Subsequent tries to start up any application
   will fail because the SDK requires root permission to access
   /dev/qcqmi device special files.
*/
static ULONG wSLQSKillSDKProcess(void *dummy)
{
    UNUSEDPARAM ( dummy );

    if( eQCWWAN_ERR_NONE != (rc = SLQSKillSDKProcess() ) )
    {
        perror(NULL);
        cleanup("Could not kill SDK process", -1);
    }

    rcprint( __func__, rc );
    sdkbound = 0;
    return 0;
}

/* Disconnect from Gobi API */
static ULONG wQCWWANDisconnect(void *dummy)
{
    UNUSEDPARAM ( dummy );

    rc = QCWWANDisconnect();
    if( eQCWWAN_ERR_NONE != rc )
    {
        rcprint("APP failed to disconnect from SDK", rc);
    }
    else
    {
        /* application no longer bound to SDK */
        sdkbound = 0;
        appstatechange();
    }

    return rc;
}

/* Bind to SDK */
static ULONG wQCWWANConnect(device_info_t* pdev)
{
    rc = QCWWANConnect( pdev->deviceNode, pdev->deviceKey );

    if( eQCWWAN_ERR_NONE == rc )
    {
        rcprint( __func__, rc );
        sdkbound = 1;
        appstatechange();
    }
    else
    {
        rcprint( __func__, rc );
    }

    return rc;
}

/* Enumerate QMI device */
static ULONG wQCWWANEnumerateDevices(device_info_t* pdevices)
{
    BYTE DevicesSize = 1;
    ULONG rc;

    memset( pdevices, 0, sizeof(devices) );
    rc = QCWWAN2kEnumerateDevices(  &DevicesSize,
                                    (BYTE *)(pdevices) );

    rcprint( __func__, rc);
    syslog( LOG_USER, "#devices: %d deviceNode: %s deviceKey: %s",
            DevicesSize,
            pdevices->deviceNode,
            pdevices->deviceKey  );


    if( eQCWWAN_ERR_NONE == rc )
    {
        /* critical section - start */
        pthread_mutex_lock(&mutex);
        devready = 1;
        pthread_mutex_unlock(&mutex);
        /* critical section - end */
        appstatechange();
    }

    return rc;
}

/*-----------------------------
  Device Connectivity END
  -----------------------------*/

static int executeApiProc(int i)
{
    int dummy;
    memset( &dfltProf, 0, sizeof(dfltProf) );

    switch(i){
    case eSLQSStart:
        rc = wSLQSStart((int *)&dummy);
        break;
    case eQCWWAN2kEnumerateDevices:
        rc = apiProcTable[i].proc(pdev);
        break;
    case eQCWWANConnect:
        rc = apiProcTable[i].proc(pdev);
        break;
    case eQCWWANDisconnect:
        rc = apiProcTable[i].proc(&dummy);
        break;
    case eQCWWANGetConnectedDeviceID:
        rc = apiProcTable[i].proc(pdev);
        break;
    case eGetSessionState:
        rc = apiProcTable[i].proc(&dummy);
        break;
    case eStartv4DataSession:
        rc = apiProcTable[i].proc(ssv4);
        break;
    case eStartv6DataSession:
        rc = apiProcTable[i].proc(ssv6);
        break;
    case eStartv4v6DataSession:
        rc = apiProcTable[i].proc(ssv4v6);
        break;
    case eStopDataSession:
        for ( i = 0; i < 3; i++)
        {
            if ( ssv4[i].sd.v4sessionId != 0)
            {
                rc = wStopDataSession(ssv4[i].sd.v4sessionId);
            }
            if ( ssv6[i].sd.v6sessionId != 0)
            {
                rc = wStopDataSession(ssv4[i].sd.v4sessionId);
            }
            if ( ssv4v6[i].sd.v4sessionId && ssv4v6[i].sd.v6sessionId )
            {
                rc = wStopDataSession(ssv4[i].sd.v4sessionId);
                rc = wStopDataSession(ssv4v6[i].sd.v6sessionId);
            }
        }
        break;
    case ePerformNetworkScan:
        rc = apiProcTable[i].proc(&instanceSize, instances);
        break;
    case eSLQSSetBandPreference:
        rc = apiProcTable[i].proc(&bandpref);
        break;
    case eSLQSSetSysSelectionPref:
        rc = apiProcTable[i].proc(&syssel);
        break;
#if 0
    case eSetDefaultProfile:
      rc = apiProcTable[i].proc(&dfltProf);
      break;
#endif
    case eSLQSKillSDKProcess:
        rc = apiProcTable[i].proc((void *)&dummy);
        break;
    default:
        syslog(LOG_USER, "unhandled request, i = %d\n", i);
        break;
    }

    return rc;
}

static void menudisplay()
{
    putchar('\n');
    for( int i =  eSLQSStart ; strlen(apiProcTable[i].str) ; i++)
        printf("%d: %s\n", i, apiProcTable[i].str);
}

static int prompt(char *buf, int size)
{
    printf( "Enter sequence of space delimited functions to"\
            " execute using the indices above:\n>");

    if( fgets(buf, size, stdin) == 0)
        return -1;

    return 0;
}

static void init()
{
    /* initialize synchronization variables */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    /* application data */
    int i;
    for( i = 0 ; i < 4 ; i++ )
    {
        ssv4[i].sd.pTechnology = &GPPtech;
        ssv4[i].sd.pProfileId3GPP = &GPPprofiles[i];
        ssv4[i].sd.ipfamily = 4;
        ssv4[i].sd.v4sessionId = 0;

        ssv6[i].sd.pTechnology = &GPPtech;
        ssv6[i].sd.pProfileId3GPP = &GPPprofiles[i];
        ssv6[i].sd.ipfamily = 6;
        ssv6[i].sd.v6sessionId = 0;

        ssv4v6[i].sd.pTechnology = &GPPtech;
        ssv4v6[i].sd.pProfileId3GPP = &GPPprofiles[i];
        ssv4v6[i].sd.ipfamily = 7;
        ssv4v6[i].sd.v4sessionId = 0;
        ssv4v6[i].sd.v6sessionId = 0;
    }
}

int main(int argc, char **argv)
{
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

    init();

    /* menu driver */
    while(1)
    {
        menudisplay();
        char lnbuf[1024];
        char *args[ARGS_MAX];
        if( prompt(lnbuf, ARGS_MAX) == -1 )
        {
            cleanup("exiting...", 0);
        }

        int i = 0;
        char delims[] = " \n";

        args[0] = strtok(lnbuf, delims);
        while( (args[++i] = strtok(NULL, delims)) != NULL);

        i = 0;
        while( args[i] != NULL )
        {
            int j = atoi(args[i]);
            if( j == eExit )
            {
                cleanup("Good bye!", eQCWWAN_ERR_NONE);
            }

            if( j < eSLQSStart )
            {
                syslog(LOG_USER, "%d: invalid menu option\n", j);
            }
            else
            {
                executeApiProc(j);
                args[i] = NULL; /* reset argument for next iteration */
            }
            i++; /* proceed to next argument */
        }
    }
}
