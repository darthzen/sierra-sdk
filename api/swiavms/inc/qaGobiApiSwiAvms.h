/**
 * \ingroup swiavms
 *
 * \file    qaGobiApiSwiAvms.h
 *
 * \brief   SWI Open Mobile Alliance Device Management Service API function
 *          prototypes
 * SWI AVMS QMI Service revision 1.14
 */

/*
 * Copyright: © 2017 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_SWIAVMS_H__
#define __GOBI_API_SWIAVMS_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_GET_SETTINGS_AVMS_APN_STRING_LENGTH 49
#define MAX_GET_SETTINGS_AVMS_UNAME_STRING_LENGTH 29
#define MAX_GET_SETTINGS_AVMS_PWD_STRING_LENGTH 29

/**
 *  Starts an AVMS session.
 *
 *  \param  sessionType[IN]
 *          - Session type
 *              - 0x01 - FOTA, to check availability of FW Update. 
 *                       This field is mandatory in OMA-DM case but not necessary in LWM2M one. 
 *                       To keep compatibility, this shall be used in LWM2M without any specific treatment.
 *
 *  \param  pSessionResponse[OUT]
 *          - Session Response
 *              - 0x01 -Available. 
 *              - 0x02 -Not Available.
 *              - 0x03 -Check Timed Out
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
ULONG SLQSAVMSStartSession(
    BYTE sessionType,
    ULONG *pSessionResponse );

/**
 *  Stop an ongoing AVMS session.
 *
 *  \param  session[IN]
 *          - Session
 *              - 0x01 - FOTA, to check availability of FW Update.
 *              - 0xFF - Cancel any active OMADM session.
 *
 *  \param   pSessionRep[OUT]
 *          - Session
 *              - 0x01 - FOTA session suspended.
 *              - 0xFF - Suspend ongoing FOTA session or stop any other active  AVMS session.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
ULONG SLQSAVMSStopSession(
    BYTE sessiontype,
    BYTE *pSessionRep);



#define QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_VERSION_LENGTH 128
#define QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_NAME_LENGTH 128
#define QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_DESC_LENGTH 1024

/**
 * @param bBinaryType : Type
 *                      - 1 - Firmware.
 *                      - 2 - User App.
 *                      - 3 - Legato Framework.
 * @param bStat : State 
 *                      - 0x01 - No binary update available.
 *                      - 0x02 - Query binary Download.
 *                      - 0x03 - Binary Downloading.
 *                      - 0x04 - Binary downloaded.
 *                      - 0x05 -Query Binary Update.
 *                      - 0x06 - Binary updating.
 *                      - 0x07 - Binary updated.
 * @param bUserInputRequest:  Bit mask of available user inputs.
 *                      - 0x00 - No user input required. Informational indication.
 *                      - 0x01 - Accept.
 *                      - 0x02 - Reject.
 * @param wUserInputTimeout: Timeout for user input in minutes. A value of 0 means no time-out.
 * @param ulPkgDownloadSize: The size (in bytes) of the update package
 * @param ulPkgDownloadComplete:  The number of bytes being downloaded.
 *                                For downloading state, this value shall be > 0 and 
 *                                incremented toward the pkg_dload_size. 
 *                                For other states, the value shall be 0 as it is meaningless.
 * @param wUpdateCompeteStatus: Result code.This field should be looked at only 
                                when the AVMS session is complete.
 * @param bSerity: Serity.
 *                      - 0x01 - Mandatory.
 *                      - 0x02 - Optional.
 * @param wVersionLength: Length of FW Version string in bytes.
 * @param szVersion: FW Version string in ASCII.
 * @param wNameLength: Length Package Name string in bytes.
 * @param szName: Package Name in UCS2.
 * @param wDescriptionLength: Length of description in bytes.
 * @param szDescription: Description of Update Package in USC2.
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportBinaryUpdateSessionInfoResp
{
    BYTE bBinaryType;
    BYTE bState;
    BYTE bUserInputRequest;
    WORD wUserInputTimeout;
    ULONG ulPkgDownloadSize;
    ULONG ulPkgDownloadComplete;
    WORD wUpdateCompeteStatus;
    BYTE bSerity;
    WORD wVersionLength;
    CHAR szVersion[QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_VERSION_LENGTH];
    WORD wNameLength;
    CHAR szName[QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_NAME_LENGTH];
    WORD wDescriptionLength;
    CHAR szDescription[QMISWIAVMSSESSIONGETINFO_BINARYUPDATESESSIONINFO_DESC_LENGTH];
    BYTE TlvPresent;    
}QmiSwiAvmsEventReportBinaryUpdateSessionInfoResp;

#define QMISWIAVMSSESSIONGETINFO_CONFIG_ALERT_MSG_LENGTH 200

/**
 * @param bState: State.                 
 *                  - 0x01 - AVMS Read Request.
 *                  - 0x02 - AVMS Change Request.
 *                  - 0x03 - AVMS Config Complete.
 * @param bUserInputRequest: Bit mask of available user inputs.
 *                  - 0x00 - No user input required. Informational indication.
 *                  - 0x01 - Accept.
 *                  - 0x02 - Reject.
 * @param wUserInputTimeout: Timeout for user input in seconds. A value of 0 means no time-out
 * @param wAlertMsgLength: Length of Alert message string in bytes.
 * @param szAlertMsg: Alert message in UCS2.
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportConfigResp
{
    BYTE bState;
    BYTE bUserInputRequest;
    WORD wUserInputTimeout;
    WORD wAlertMsgLength;
    CHAR szAlertMsg[QMISWIAVMSSESSIONGETINFO_CONFIG_ALERT_MSG_LENGTH];
    BYTE TlvPresent;
}QmiSwiAvmsEventReportConfigResp;

/**
 * @param bNotification: Notification.
 *                  - 0x14 - Module starts sending data to server.
 *                  - 0x15 - Authentication with the server.
 *                  - 0x16 - session with the server is ended.
 * @param wSessionStatus: This field will set to the session status for notifications 
 *                        that occur at the end of a session, zero for all other notifications.
 *                          - 0x0000: Successful: Session succeeded
 *                          - 0x0001: Break: Session succeeded
 *                          - 0x0002: Large Object Handled: Session succeeded
 *                          - 0x0003: No status: Session succeeded
 *                          - 0x0004: No more commands: Session succeeded
 *                          - 0x0005: User cancel: Session cancelled
 *                          - 0x0100-0x01FF: General errors
 *                          - 0x0200-0x02FF: Syncml errors
 *                          - 0x0300-0x03FF: Authentication errors
 *                          - 0x0400-0x04FF: Protocol errors
 *                          - 0x0500-0x05FF: Tree errors (DM Only)
 *                          - 0x0600-0x06FF: Not applicable
 *                          - 0x0700-0x07FF: Trigger errors
 *                          - 0x0800-0x08FF: FUMO errors
 *                          - 0x0900-0x09FF: Communication errors
 *                          - 0x0A00-0x0AFF: Parsing errors
 *                          - 0x0B00-0x0CFF: Not applicable
 *                          - 0x7F00-0x7F12:Insignia errors
 *                          - 0x7F13: Illegal text: Text received contains illegal characters
 *                          - 0x7F14: Download failure: Failed to download FOTA image
 *                          - 0x7F15: Empty session: Session ran successfully, but no information wasa updated
 *                          - 0x7F16: Factory reset successful: Factory reset succeeded
 *                          - 0x7F17: Factory reset fail: Factory reset failed
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportNotificationResp
{
    BYTE bNotification;
    WORD wSessionStatus;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportNotificationResp;

/**
 * @param bUserInputRequest: Bit mask of available user inputs.
 *                              - 0x00 - No user input required. Informational indication.
 *                              - 0x01 - Accept.
 *                              - 0x02 - Reject.
 * @param bUserInputRequest: Timeout for user input in minutes. A value of 0 means no time-out.
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportConnectionRequestResp
{
    BYTE bUserInputRequest;
    WORD wUserInputTimeout;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportConnectionRequestResp;

/**
 * @param wWamsChangeMask: Mask of WAMS parameters changed. 
 *                          - By default set to 0xFF for all changes.
 *                          - 0x01 - device_login
 *                          - 0x02 - device_MD5_key
 *                          - 0x04 - server_login
 *                          - 0x08 - server_MD5_key
 *                          - 0x10 - server_URL
 *                          - 0x20 - Nonce
 *                          - 0x40 - Application key
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportWAMSParamChangeResp
{
    WORD wWamsChangeMask;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportWAMSParamChangeResp;

/**
 * @param bPackageID: Package ID of the application binary that this 
 *                    AVMS_EVENT_ID notification is for.
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportPackageIDResp
{
    BYTE bPackageID;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportPackageIDResp;

/**
 * @param bRegStatus: LWM2M Registration status.
 *                      - 0: Need Bootstrap.
 *                      - 1: Bootstrap made.
 *                      - 2: Register made.
 *                       -3: Update made.
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportRegStatusResp
{
    BYTE bRegStatus;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportRegStatusResp;

/**
 * @param bType: Notification type.
 *                      - 0: Data session closed.
 *                      - 1: Data session activated.
 *                      - 2: Register made.
 *                       -3: Data session error.
 * @param wErrorCode: LWM2M Session error code.
 *                      - 0x0000: none
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportDataSessionStatusResp
{
    BYTE bType;
    WORD wErrorCode;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportDataSessionStatusResp;

/**
 * @param bType: Session Type.
 *                      - 0: Bootstrap session.
 *                      - 1. DM session
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportSessionTypeResp
{
    BYTE bType;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportSessionTypeResp;

/**
 * @param wHTTPStatus: See RFC 7231.
 *                      - 100  - Continue
 *                      - 101  - Switching Protocols
 *                      - 200  - OK
 *                      - 201  - Created
 *                      - 202  - Accepted
 *                      - 203  - Non-Authoritative Information
 *                      - 204  - No Content
 *                      - 205  - Reset Content
 *                      - 206  - Partial Content
 *                      - 300  - Multiple Choices
 *                      - 301  - Moved Permanently
 *                      - 302  - Found
 *                      - 303  - See Other
 *                      - 304  - Not Modified
 *                      - 305  - Use Proxy
 *                      - 307  - Temporary Redirect
 *                      - 400  - Bad Request
 *                      - 401  - Unauthorized
 *                      - 402  - Payment Required
 *                      - 403  - Forbidden
 *                      - 404  - Not Found
 *                      - 405  - Method Not Allowed
 *                      - 406  - Not Acceptable
 *                      - 407  - Proxy Authentication Required
 *                      - 408  - Request Timeout
 *                      - 409  - Conflict
 *                      - 410  - Gone
 *                      - 411  - Length Required
 *                      - 412  - Precondition Failed
 *                      - 413  - Payload Too Large
 *                      - 414  - URI Too Long
 *                      - 415  - Unsupported Media Type
 *                      - 416  - Range Not Satisfiable
 *                      - 417  - Expectation Failed
 *                      - 426  - Upgrade Required
 *                      - 500  - Internal Server Error
 *                      - 501  - Not Implemented
 *                      - 502  - Bad Gateway
 *                      - 503  - Service Unavailable
 *                      - 504  - Gateway Timeout
 *                      - 505  - HTTP Version Not Supported
 * @param TlvPresent: Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 */
typedef struct _QmiSwiAvmsEventReportHTTPStatusResp
{
    WORD wHTTPStatus;
    BYTE TlvPresent;
}QmiSwiAvmsEventReportHTTPStatusResp;

/**
 *  Returns information related to the current AVMS session.
 *
 *  \param  pBinaryUpdteSessionInfo[OUT]
 *          - See \ref QmiSwiAvmsEventReportBinaryUpdateSessionInfoResp for more information.
 *
 *  \param  pAVMSConfig[OUT]
 *          - See \ref QmiSwiAvmsEventReportConfigResp for more information.
 *
 *  \param  pNotif[OUT]
 *          - See \ref QmiSwiAvmsEventReportNotificationResp for more information.
 *
 *  \param  pPackageID[OUT]
 *          - See \ref QmiSwiAvmsEventReportPackageIDResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
ULONG SLQSAVMSGetSessionInfo(
    QmiSwiAvmsEventReportBinaryUpdateSessionInfoResp *pBinaryUpdteSessionInfo,
    QmiSwiAvmsEventReportConfigResp *pAVMSConfig,
    QmiSwiAvmsEventReportNotificationResp *pNotif,
    QmiSwiAvmsEventReportPackageIDResp *pPackageID
);

/**
 *  Sends the specified AVMS selection for the current network initiated
 *  session.
 *
 *  \param  selection[IN]
 *          - AVMS User Selection.
 *              - 0x01 - Accept.
 *              - 0x02 - Reject.
 *              - 0x03 - Defer.
 *
 *  \param  pDeferTime[IN]
 *          - Defer time in minutes. A value of 0 will cause the prompt to be resent immediately.
 *
 *  \param  pClientPerfOperationFlag[IN]
 *          - Client operation flag after accept.
 *              - 0 - if modem performs the operation (download or update).
 *              - 1 - if client performs the operation (download or update).
 *
 *  \param  pPackageID[IN]
 *          - Package ID (relevant to Application binary package)
 *              - Package ID of the application binary that the client 
 *                is referring to for its selection. If this field is 
 *                absent, the modem will assume the package of the 
 *                most recent AVMS_EVENT_ID notification.          
 *
 *  \param  pRejectReason[IN]
 *          - Reject reason.
 *
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
ULONG SLQSAVMSSelection(
    BYTE selection ,
    ULONG *pDeferTime,
    BYTE *pClientPerfOperationFlag,
    BYTE *pPackageID,
    WORD *pRejectReason
);

 /*
  * This structure contains the SLQSAVMSSetSettings APN Info to connect to AVMS server.
  *
  * \param  bAPNLength - APN Length(Max 49).
  *
  * \param  szAPN - APN.
  *
  * \param  bUnameLength - User Name Length (Max 29).
  *
  * \param  szUname - User Name.
  *
  * \param  bPWDLength - Password Length (Max 29).
  *
  * \param  szPWD - Password.
  *
  */
typedef struct _SLQSAVMSSettingsAPNInfo
{
    BYTE    bAPNLength;
    CHAR    *pAPN;
    BYTE    bUnameLength;
    CHAR    *pUname;
    BYTE    bPWDLength;
    CHAR    *pPWD;
}SLQSAVMSSettingsAPNInfo;
#define MAX_AVMS_RETRY_TIMER_NUMBER    8
/*
 * This structure contains the SLQSAVMSSettings Polling timer to connect to AVMS server.
 *
 * \param  Timers[0] - Timer 1.
 *                      - 0-20160 (min)
 *                      - 0:retry disabled
 * \param  Timers[1] - Timer 2.
 *                      - 1-20160 (min)
 * \param  Timers[2] - Timer 3.
 *                      - 1-20160 (min)
 * \param  Timers[3] - Timer 4.
 *                      - 1-20160 (min)
 * \param  Timers[4] - Timer 5.
 *                      - 1-20160 (min)
 * \param  Timers[5] - Timer 6.
 *                      - 1-20160 (min)
 * \param  Timers[6] - Timer 7.
 *                      - 1-20160 (min)
 * \param  Timers[7] - Timer 8.
 *                      - 1-20160 (min)
 *
 */
typedef struct _SLQSAVMSSettingsConnectionRetryTimers
{
    WORD Timers[MAX_AVMS_RETRY_TIMER_NUMBER];
}SLQSAVMSSettingsConnectionRetryTimers;

/*
 * This structure contains the SLQSAVMSSettings Min/Max Period of an Observation.
 *
 * \param  min - Min period in seconds.
 *
 * \param  max - Max period in seconds.
 *
 */
typedef struct _SLQSAVMSSettingsPeriodsInfo
{
    ULONG min;
    ULONG max;
}SLQSAVMSSettingsPeriodsInfo;

/*
 * This structure contains the SLQSAVMSSettings.
 *
 * \param  pOMADMEnabled - OMA-DM Enabled.
 *                         - 0x00000010: Client Initiated FUMO (CIFUMO).
 *                         - 0x00000020: Network Initiated  FUMO (NIFUMO).
 * \param  pAutoConnect - Automatic connection to AVMS server on reception of SMS wakeup.
 *                         - 0x00 - FALSE.
 *                         - 0x01 - TRUE.
 * \param  pFwAutodownload - FOTA Automatic Download.
 *                         - 0x00 - FALSE.
 *                         - 0x01 - TRUE.
 * \param  pFwAutoUpdate - FOTA Automatic Update.
 *                         - 0x00 - FALSE.
 *                         - 0x01 - TRUE.
 * \param  pFwAutoSDM - OMA Automatic UI Alert Response.
 *                         - 0x00 - Disabled.
 *                         - 0x01 - Enabled Accept.
 *                         - 0x02 - Enabled Reject.
 * \param  pPollingTimer - Polling timer to connect to AVMS server.
 *                         - 0-525600 (min)
 *                         - 0:disabled 
 * \param  pConnectionRetryTimers - Connection Retry timers.
 *                         - See \ref SLQSAVMSSettingsConnectionRetryTimers for more information.
 * \param  pAPNInfo - APN Configuration.
 *                         - See \ref SLQSAVMSSettingsAPNInfo for more information.
 * \param  pNotificationStore - Notification Storing When Disabled or Offline.
 *                         - 0 - Disabled.
 *                         - 1 - Enabeld.
 * \param  pPeroidsInfo - Min/Max Period of an Observation.
 *                         - See \ref SLQSAVMSSettingsPeriodsInfo for more information.
 */
typedef struct _SLQSAVMSSettings
{
    ULONG             *pOMADMEnabled;
    BYTE              *pAutoConnect;
    BYTE              *pAutoReboot;
    BYTE              *pFwAutodownload;
    BYTE              *pFwAutoUpdate;
    BYTE              *pFwAutoSDM;
    ULONG             *pPollingTimer;
    SLQSAVMSSettingsConnectionRetryTimers *pConnectionRetryTimers;
    SLQSAVMSSettingsAPNInfo *pAPNInfo;
    BYTE              *pNotificationStore;
    SLQSAVMSSettingsPeriodsInfo             *pPeroidsInfo;
} SLQSAVMSSettings;
    ULONG SLQSAVMSGetSettings(
       SLQSAVMSSettings *settings);

/*
 * This structure contains the SLQSAVMSSetSettings Min/Max Period of an Observation.
 *
 * \param  ulMin - Min period in seconds.
 *
 * \param  ulMax - Max period in seconds.
 *
 */
typedef struct _QmiSwiAvmsSetSettingsPeriodInfo
{
    ULONG ulMin;
    ULONG ulMax;
}QmiSwiAvmsSetSettingsPeriodInfo;

#define MAX_SWI_AVMS_SET_SETTING_APN_LENGTH 49
#define MAX_SWI_AVMS_SET_SETTING_UNAME_LENGTH 29
#define MAX_SWI_AVMS_SET_SETTING_PWD_LENGTH 29
/*
 * This structure contains the SLQSAVMSSetSettings APN Info to connect to AVMS server.
 *
 * \param  bAPNLength - APN Length(Max 49).
 *
 * \param  szAPN - APN.
 *
 * \param  bUnameLength - User Name Length (Max 29).
 *
 * \param  szUname - User Name.
 *
 * \param  bPWDLength - Password Length (Max 29).
 *
 * \param  szPWD - Password.
 *
 */

typedef struct _QmiSwiAvmsSetSettingsAPNInfo
{
    BYTE bAPNLength;
    CHAR szAPN[MAX_SWI_AVMS_SET_SETTING_APN_LENGTH];
    BYTE bUnameLength;
    CHAR szUname[MAX_SWI_AVMS_SET_SETTING_UNAME_LENGTH];
    BYTE bPWDLength;
    CHAR szPWD[MAX_SWI_AVMS_SET_SETTING_PWD_LENGTH];
}QmiSwiAvmsSetSettingsAPNInfo;


#define MAX_SWI_AVMS_SET_SETTING_CONNECTION_RETRY_TIMMERS 8
/*
 * This structure contains the SLQSAVMSSetSettings Polling timer to connect to AVMS server.
 *
 * \param  Timers[0] - Timer 1.
 *                      - 0-20160 (min)
 *                      - 0:retry disabled
 * \param  Timers[1] - Timer 2.
 *                      - 1-20160 (min)
 * \param  Timers[2] - Timer 3.
 *                      - 1-20160 (min)
 * \param  Timers[3] - Timer 4.
 *                      - 1-20160 (min)
 * \param  Timers[4] - Timer 5.
 *                      - 1-20160 (min)
 * \param  Timers[5] - Timer 6.
 *                      - 1-20160 (min)
 * \param  Timers[6] - Timer 7.
 *                      - 1-20160 (min)
 * \param  Timers[7] - Timer 8.
 *                      - 1-20160 (min)
 *
 */

typedef struct _QmiSwiAvmsSetSettingsConnectionRetryTimers
{
    WORD  Timers[MAX_SWI_AVMS_SET_SETTING_CONNECTION_RETRY_TIMMERS];
    
}QmiSwiAvmsSetSettingsConnectionRetryTimers;

/**
 *  Sets the AVMS settings requested.
 *
 *  \param  bAutoConnect[IN]
 *          - Auto Connect
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 *  \param  bAutoReboot[IN]
 *          - Automatic device reboot when the request is 
 *            received from AirVantage server
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 *  \param  bFwAutodownload[IN]
 *          - Firmware Auto Download
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 *  \param  bFwAutoUpdate[IN]
 *          - Firmware Auto Update
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 * \param FwAutoSDM[Optional] - OMA Automatic UI Alert Response
 *                      - 0x00 - DISABLED
 *                      - 0x01 - ENABLED ACCEPT
 *                      - 0x02 - ENABLED REJECT
 *
 * \param  PollingTimer[Optional]
 *            - 0-525600 (min)
 *            - 0:disabled
 *
 * \param  ConnectionRetryTimers[Optional]
 *            - See \ref QmiSwiAvmsSetSettingsConnectionRetryTimers for more information
 *
 * \param  APNInfo[Optional]
 *            - See \ref QmiSwiAvmsSetSettingsAPNInfo for more information
 *
 * \param  NotifStore[Optional]
 *            - 0 : Disabled.
 *            - 1 : Enabeld.
 *
 *  \param  PeroidInfo[Optional]
 *            - See \ref QmiSwiAvmsSetSettingsPeriodInfo for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
typedef struct _SLQSAVMSSetSettingsReqPara
{
    BYTE *pAutoConnect;
    BYTE *pAutoReboot;
    BYTE *pFwAutodownload;
    BYTE *pFwAutoUpdate;
    BYTE  *pFwAutoSDM;
    ULONG *pPollingTimer;
    QmiSwiAvmsSetSettingsConnectionRetryTimers  *pConnectionRetryTimers;
    QmiSwiAvmsSetSettingsAPNInfo  *pAPNInfo;
    BYTE *pNotifStore;
    QmiSwiAvmsSetSettingsPeriodInfo *pPeroidInfo;
}SLQSAVMSSetSettingsReq;

;
/**
 *  Sets AVMS settings requested.
 *
 *  \param  Request[IN]
 *          - See \ref SLQSAVMSSetSettingsReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */

ULONG SLQSAVMSSetSettings(
    SLQSAVMSSetSettingsReq Request);

;
/**
 *  Sets AVMS settings requested without Auto Reboot.
 *
 *  \param  Request[IN]
 *          - See \ref SLQSAVMSSetSettingsReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */

ULONG SLQSAVMSSetSettingsIgnoreAutoReboot(
    SLQSAVMSSetSettingsReq Request);


/**
 *  Sets AVMS Event Report.
 *
 *  \param  NULL
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */

ULONG SLQSAVMSSetEventReport();

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SWIAVMS_H__ */
