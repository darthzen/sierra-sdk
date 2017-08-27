/*************
 *
 * Filename:    callhandling.h
 *
 * Purpose:     Contains function for voice call handling
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "qmerrno.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <unistd.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <malloc.h>
#include <ctype.h>
#include <wchar.h>

/****************************************************************
*                       DEFINES
****************************************************************/
#define FALSE                   0
#define TRUE                    1
#define ENTER_KEY               0x0A
#define ENTER_KEY_PRESSED       0
#define OPTION_LEN              5
#define ESCAPE_KEY              0x1B
#define DEV_NODE_SZ             256
#define DEV_KEY_SZ              16
#define CALL_ERROR_CODE         0xffff
#define EOS                     '\0'
#define FLASH_PAYLOAD           1024
#define ASCII_0                 0x30
/* call type */
#define CALL_TYPE_VOICE         0x00

/* answer call */
#define AUTO_ANSWER_DISABLE     0x00
#define AUTO_ANSWER_ENABLE      0x01
#define CALL_STATE_ANSWER       0x02

/* call Direction */
#define MOBILE_ORIGINATED       0x01
#define MOBILE_TERMINATED       0x02

/* call waiting */
#define CALL_WAITING_REASON     0x0F

/* clir type */
#define CLIR_TYPE_SUPRESSION    0x01
#define CLIR_TYPE_INVOCATION    0x02

/* call control result type */
#define CC_RESULT_TYPE_VOICE    0x00
#define CC_RESULT_TYPE_SUPS     0x01
#define CC_RESULT_TYPE_USSD     0x02

/* preferred privacy */
#define VOICE_STANDARD_PRIVACY  0x00
#define VOICE_ENHANCED_PRIVACY  0x01

#define TECH_3GPP    (0)
#define TECH_3GPP2   (1)

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/
/**
 * \Name    device_info_t
 * \purpose It contains Device information parameters
 * \param   deviceNode
 *          - Device node
 *          - Length Range [1 to 256]
 * \param   deviceKey
 *          - Device key
 *          - Length Range [1 to 16]
 */
typedef struct device_info_param{
   CHAR deviceNode[DEV_NODE_SZ];
   CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/**
 *  \Name    callInfoVb
 *  \purpose This structure contains call information parameters
 *  \param   callId
 *           - Unique call identifier for the call in progress
 *
 *           number
 *           - Number of incoming and outgoing call in ASCII string,
 *             NULL terminated.
 *           - Length Range [1 to 81]
 *
 *  \param   callState
 *           - Call state.
 *             - 0x01 - Origination
 *             - 0x02 - Incoming
 *             - 0x03 - Conversation
 *             - 0x04 - Call is originating but waiting for call control
 *                      to complete
 *             - 0x05 - Alerting
 *             - 0x06 - Hold
 *             - 0x07 - Waiting
 *             - 0x08 - Disconnecting
 *             - 0x09 - End
 *             - 0x0A - call is in Setup state in 3GPP
 *             - 0xFF - Not Available
 *
 *  \param   callType
 *           - Call type.
 *             - 0x00 - Voice
 *             - 0x02 - Voice over IP
 *             - 0x06 - OTAPA
 *             - 0x07 - Standard OTASP
 *             - 0x08 - Nonstandard OTASP
 *             - 0x09 - Emergency
 *             - 0xFF - Not Available
 *
 *  \param   direction
 *           - Direction.
 *             - 0x01 - MO call
 *             - 0x02 - MT call
 *             - 0xFF - Not Available
 *
 *  \param   mode
 *           - Mode.
 *           - If the mode field is "0x01 - CDMA",the optional Service Option,
 *             Voice Privacy, and OTASP Status (only for OTASP calls) TLVs are
 *             included in the response.
 *             - 0x01 - CDMA
 *             - 0x02 - GSM
 *             - 0x03 - UMTS
 *             - 0x04 - LTE
 *             - 0x05 - TD-SCDMA
 *             - 0xFF - Not Available
 */
typedef struct
{
    BYTE callId;
    CHAR number[MAX_CALL_NO_LEN];
    CHAR callState[20];
    CHAR callType[20];
    CHAR direction[3];
    CHAR mode[10];
} callInfoVb;

/**
 *  \Name    eCALL_STATE
 *  \purpose It contains all the call state value
 *  \param   - 0x01 - eCALL_STATE_ORIGINATION - Origination
 *           - 0x02 - eCALL_STATE_INCOMING - Incoming
 *           - 0x03 - eCALL_STATE_CONNECTED - Conversation
 *           - 0x04 - CALL_STATE_CC_IN_PROGRESS -
 *              Call is originating but waiting for call control to complete
 *           - 0x05 - eCALL_STATE_RINGING - Alerting
 *           - 0x07 - eCALL_STATE_WAITING - Waiting
 *           - 0x08 - eCALL_STATE_DISCONNECTED - Disconnecting
 *           - 0x09 - eCALL_STATE_END - End
 *           - 0x0E - eCALL_IS_ON_HOLD - Hold
 *           - 0x0F - eCALL_IS_RETRIEVED - Retrieved
 */
enum eCALL_STATE
{
    eCALL_STATE_ORIGINATION   = 0x01,
    eCALL_STATE_INCOMING      = 0x02,
    eCALL_STATE_CONNECTED     = 0x03,
    eCALL_STATE_RINGING       = 0x05,
    eCALL_STATE_WAITING       = 0x07,
    eCALL_STATE_HOLD          = 0x06,
    eCALL_STATE_DISCONNECTED  = 0x08,
    eCALL_STATE_END           = 0X09,
    eCALL_IS_MPTY             = 0x10,
    eCALL_IS_ON_HOLD          = 0x0E,
    eCALL_IS_RETRIEVED        = 0x0F,
};

/**
 *  \Name    eMANAGE_CALLS_SUPS_TYPE
 *  \purpose It contains all the call supplementary type value
 *  \param   - 0x01 - eST_RELEASE_HELD_OR_WAITING
 *                    - Release is held or waiting
 *           - 0x02 - eST_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING
 *                    - Release is active and accepting held or waiting
 *           - 0x03 - eST_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
 *                    - Hold is active and accepting waiting or held
 *           - 0x04 - eST_HOLD_ALL_EXCEPT_SPECIFIED_CALL
 *                    - Hold all calls except a specified one
 *           - 0x05 - eST_MAKE_CONFERENCE_CALL
 *                    - Make a conference call
 *           - 0x08 - eST_END_ALL_CALLS
 *                    - End all calls
 *           - 0x09 - eST_RELEASE_SPECIFIED_CALL
 *                    - Release a specified call
 */
enum eMANAGE_CALLS_SUPS_TYPE
{
    eST_RELEASE_HELD_OR_WAITING               = 0x01,
    eST_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING = 0x02,
    eST_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD    = 0x03,
    eST_HOLD_ALL_EXCEPT_SPECIFIED_CALL        = 0x04,
    eST_MAKE_CONFERENCE_CALL                  = 0x05,
    eST_END_ALL_CALLS                         = 0x08,
    eST_RELEASE_SPECIFIED_CALL                = 0x09,
};

/**
 *  \Name    eBARRING_REASON
 *  \purpose It contains call barring service reason
 *  \param   - 0x07 - eBARR_ALLOUTGOING
 *                    - All outgoing calls are barred
 *           - 0x08 - eBARR_OUTGOINGINT
 *                    - Outgoing internal calls are barred
 *           - 0x09 - eBARR_OUTGOINGINTEXTOHOME
 *                    - Outgoing calls external to home are barred
 *           - 0x0A - eBARR_ALLINCOMING
 *                    - All incoming calls are barred
 *           - 0x0B - eBARR_INCOMINGROAMING
 *                    - Roaming incoming calls are barred
 *           - 0x0C - eBARR_ALLBARRING
 *                    - All calls are barred
 *           - 0x0D - eBARR_ALLOUTGOINGBARRING
 *                    - All outgoing calls are barred
 *           - 0x0E - eBARR_ALLINCOMINGBARRING
 *                    - All incoming calls are barred
 */
enum eBARRING_REASON
{
    eBARR_ALLOUTGOING          = 0x07,
    eBARR_OUTGOINGINT          = 0x08,
    eBARR_OUTGOINGINTEXTOHOME  = 0x09,
    eBARR_ALLINCOMING          = 0x0A,
    eBARR_INCOMINGROAMING      = 0x0B,
    eBARR_ALLBARRING           = 0x0C,
    eBARR_ALLOUTGOINGBARRING   = 0x0D,
    eBARR_ALLINCOMINGBARRING   = 0x0E
};

/**
 *  \Name    eCALL_MODE
 *  \purpose It contains call mode values
 *  \param   - 0x01 - eCALL_MODE_CDMA
 *                    - CDMA
 *           - 0x02 - eCALL_MODE_GSM
 *                    - GSM
 *           - 0x03 - eCALL_MODE_UMTS
 *                    - UMTS
 *           - 0x04 - eCALL_MODE_LTE
 *                    - LTE
 *           - 0x05 - eCALL_MODE_TD_SCDMA
 *                    - SCDMA
 */
enum eCALL_MODE
{
    eCALL_MODE_CDMA     = 0x01,
    eCALL_MODE_GSM      = 0x02,
    eCALL_MODE_UMTS     = 0x03,
    eCALL_MODE_LTE      = 0x04,
    eCALL_MODE_TD_SCDMA = 0x05
};

/**
 *  \Name    eUSER_OPTIONS
 *  \purpose It contains user options value for main menu
 *  \param   - 0x01 - eDIAL_CALL
 *                    - To dial call
 *           - 0x02 - eANSWER_CALL
 *                    - To answer a incoming call
 *           - 0x03 - eEND_CALL
 *                    - To end call
 *           - 0x04 - eRELEASE_HELD_OR_WAITING
 *                    - To release held or waiting call
 *           - 0x05 - eRELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING
 *                    - To end Active Call and Accept Held or Waiting Call
 *           - 0x06 - eHOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
 *                    - To hold Active Call and Accept Waiting or Held Call
 *           - 0x07 - eHOLD_ALL_EXCEPT_SPECIFIED_CALL
 *                    - To hold All Calls Except a Specified One
 *           - 0x08 - eMAKE_CONFERENCE_CALL
 *                    - To make a Conference Call
 *           - 0x09 - eEND_ALL_CALLS
 *                    - To end All Calls
 *           - 0x10 - eRELEASE_SPECIFIED_CALL
 *                    - To release a Specified Call
 *           - 0x11 - eSET_SUPS_SERVICE
 *                    - To set Supplementary Service
 *           - 0x12 - eSTART_DTMF
 *                    - To start DTMF On Call In Progress
 *           - 0x13 - eSTOP_DTMF
 *                    - To stop DTMF On Call In Progress
 *           - 0x14 - eGET_CLIR
 *                    - To get CLIR Service Status
 *           - 0x15 - eGET_CLIP
 *                    - To get CLIP Service Status
 *           - 0x16 - eSEND_FLASH
 *                    - To send Flash
 */
enum eUSER_OPTIONS
{
    eDIAL_CALL = 1,
    eANSWER_CALL,
    eEND_CALL,
    eRELEASE_HELD_OR_WAITING,
    eRELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING,
    eHOLD_ACTIVE_ACCEPT_WAITING_OR_HELD,
    eHOLD_ALL_EXCEPT_SPECIFIED_CALL,
    eMAKE_CONFERENCE_CALL,
    eEND_ALL_CALLS,
    eRELEASE_SPECIFIED_CALL,
    eSET_SUPS_SERVICE,
    eSTART_DTMF,
    eSTOP_DTMF,
    eGET_CLIR,
    eGET_CLIP,
    eSEND_FLASH,
    eRECEIVE_WAITING_CALL = 0x79,
    eREJECT_WAITING_CALL = 0x6E,
    eRECV_WAITING_CALL = 0x59,
    eREJ_WAITING_CALL = 0x4E,
};

/**
 *  \Name    eSUPS_SERVICE_VALUE
 *  \purpose It contains the value of options for supplementary services menu
 *  \param   - 1 - eCALL_SUPS_ACTIVATE
 *                 - To activate supplementary services
 *           - 2 - eCALL_SUPS_DEACTIVATE
 *                 - To deactivate supplementary services
 *           - 3 - eCALL_SUPS_REGISTER
 *                 - To register supplementary services
 *           - 4 - eCALL_SUPS_ERASE
 *                 - To erase supplementary services
 */
enum eSUPS_SERVICE_VALUE
{
    eCALL_SUPS_ACTIVATE = 1,
    eCALL_SUPS_DEACTIVATE,
    eCALL_SUPS_REGISTER,
    eCALL_SUPS_ERASE
};

/**
 *  \Name    eCALL_BARRING_REASON
 *  \purpose It contains the value of options for call barring services menu
 *  \param   - 1 - eCALL_BARR_ALLOUTGOING
 *                 - To bar all outgoing
 *           - 2 - eCALL_BARR_OUTGOINGINT
 *                 - To bar outgoing internal
 *           - 3 - eCALL_BARR_OUTGOINGINTEXTHOME
 *                 - To bar outgoing external to home
 *           - 4 - eCALL_BARR_INCOMINGROAMING
 *                 - To bar roaming incoming
 *           - 5 - eCALL_BARR_ALLBARRING
 *                 - To bar all calls
 *           - 6 - eCALL_BARR_ALLOUTGOINGBARR
 *                 - To bar all outgoing calls
 *           - 7 - eCALL_BARR_ALLINCOMINGBARR
 *                 - To bar all incoming calls
 * */
enum eCALL_BARRING_REASON
{
    eCALL_BARR_ALLOUTGOING = 1,
    eCALL_BARR_OUTGOINGINT,
    eCALL_BARR_OUTGOINGINTEXTHOME,
    eCALL_BARR_ALLINCOMING,
    eCALL_BARR_INCOMINGROAMING,
    eCALL_BARR_ALLBARRING,
    eCALL_BARR_ALLOUTGOINGBARR,
    eCALL_BARR_ALLINCOMINGBARR
};

/**
 *  \Name    eCALL_FORWARD_REASON
 *  \purpose It contains the value of call forward service reason
 *  \para    - 1 - eCALL_FWD_UNCONDITIONAL
 *                 - Unconditional
 *           - 2 - eCALL_FWD_MOBILEBUSY
 *                 - If mobile is busy
 *           - 3 - eCALL_FWD_NOREPLY
 *                 - If there is no reply
 *           - 4 - eCALL_FWD_UNREACHABLE
 *                 - If phone is unreachable
 */
enum eCALL_FORWARD_REASON
{
    eCALL_FWD_UNCONDITIONAL = 1,
    eCALL_FWD_MOBILEBUSY,
    eCALL_FWD_NOREPLY,
    eCALL_FWD_UNREACHABLE
};

/**
 *  \Name    eCALL_SUPS_SERVICE
 *  \purpose It contains type of supplementary services
 *  \param   - 1 - eCALL_FORWARD_SERVICES
 *                 - To configure call forward services
 *           - 2 - eCALL_BARRING_SEVICES
 *                 - To configure call barring services
 *           - 3 - eAUTO_ANSWER
 *                 - To configure auto answer
 *           - 4 - ePREFERRED_PRIVACY
 *                 - To configure preferred privacy
 */
enum eCALL_SUPS_SERVICE
{
    eCALL_FORWARD_SERVICES = 1,
    eCALL_BARRING_SEVICES,
    eCALL_WAITING_SEVICES,
    eAUTO_ANSWER,
    ePREFERRED_PRIVACY
};

/**
 *  \Name    eUSER_OPTION
 *  \purpose It contains numeric values to use as user choice
 *  \param   - 1 - eOPTION_1
 *                 - For option no. 1
 *           - 2 - eOPTION_2
 *                 - For option no. 2
  *          - 3 - eOPTION_3
 *                 - For option no. 3
 */
enum eUSER_OPTION
{
    eOPTION_1 = 1,
    eOPTION_2,
    eOPTION_3,
};

/**
 *  \Name    eFLASH_TYPE
 *  \purpose It contains the value of options for sending flash menu
 *  \param   - 1 - eSIMPLE_FLASH
 *                 - For simple flash option
 *           - 2 - eACT_ANSWER_HOLD
 *                 - For activate answer hold option
 *           - 3 - eDEACT_ANSWER_HOLD
 *                 - For deactivate answer hold option
 */
enum eFLASH_TYPE
{
    eSIMPLE_FLASH = 0,
    eACT_ANSWER_HOLD = 1,
    eDEACT_ANSWER_HOLD = 2
};

/**
 *  \Name    eCALL_TYPE_VALUE
 *  \purpose It contains call type value
 *  \param   - 0x00 - eVOICE
 *                    - Voice
 *           - 0x01 - eCALL_OVER_IP
 *                    - Call over IP phone
*/
enum eCALL_TYPE_VALUE
{
    eVOICE = 0x00,
    eCALL_OVER_IP = 0x02,
};

typedef void (*tFNcallStateInfo)(BOOL call_end);

/****************************************************************
*                      FUNCTIONS
****************************************************************/
/**
 *  \Name     StartSDK
 *
 *  \purpose  It starts the SDK by setting the SDK path, enumerates the device
 *            and connects to the SDK.
 *
 *  \param    none
 *
 *  \return   eQCWWAN_ERR_NONE on successfully starting SDK, else error code
 *
 *  \notes    none
 */
ULONG StartSDK();

/**
 *  \Name    GettingNotificationCallback
 *
 *  \purpose It gets notification for call back
 *
 *  \param   pNotificationTypeInfo
 *          -Contains the parameters passed for SLQSVoiceSetSUPSNotificationCallback by
 *           the device.
 *  \return  none
 *
 *  \notes   none
 */
void DisplayNotificationInfo(voiceSUPSNotification *pNotificationTypeInfo );

/**
 *  \Name    DisplayCallInfo
 *
 *  \purpose It Displays call information of all call in progress
 *
 *  \param   pCallBackInfo
 *           - This structure contains VoiceCall Information parameters.
 *  \return  none
 *
 *  \notes   none
 */
void DisplayCallInfo( voiceSetAllCallStatusCbkInfo *pCallBackInfo );

/**
 *  \Name    DisplayRecInfo
 *
 *  \purpose It Displays Record Event information
 *
 *  \param   pCallBackInfo
 *           - This structure contains voiceInfoRec Information parameters.
 *  \return  none
 *
 *  \notes   none
 */
void DisplayRecInfo( voiceInfoRec *pCallBackInfo );

/**
 *  \Name    SetCallStateReportCallback
 *
 *  \purpose set the call state when received an indication from modem
 *
 *  \param   pCallBack
 *           - This structure contains pCallBack Information parameters.
 *  \return  none
 *
 *  \notes   none
 */
void SetCallStateReportCallback(tFNcallStateInfo pCallback);

/**
 *  \Name    FlushStdinStream
 *
 *  \purpose Flush the stdin stream
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   fflush does not work for input stream.
 */
void FlushStdinStream(void);

/**
 *  \Name    SetCallBack
 *
 *  \purpose It subscribes the call back
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetCallBack(void);

/**
 *  \Name    ClearCallBack
 *
 *  \purpose It unsubscribes call back
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void ClearCallBack(void);

/**
 *  \Name    VoiceDialCall
 *
 *  \purpose Originates a voice call
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void VoiceDialCall(void);

/**
 *  \Name    VoiceAnswerCall
 *
 *  \purpose Answers an incoming voice call
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void VoiceAnswerCall(void);

/**
 *  \Name    VoiceEndCall
 *
 *  \purpose Ends a voice call
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void VoiceEndCall(void);

/**
 *  \Name    VoiceEndALLCall
 *
 *  \purpose It ends all the existing call
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void VoiceEndAllCall(void);

/**
 *  \Name    ManageCalls
 *
 *  \purpose Manages the calls by using the supplementary
 *           service applicable during the call.
 *
 *  \param   supstype
 *           - Supplementary service type during the call.
 *             - 0x01 - SUPS_TYPE_RELEASE_HELD_OR_WAITING
 *                      - Release is held or waiting
 *             - 0x02 - SUPS_TYPE_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING
 *                      - Release is active and accepting held or waiting
 *             - 0x03 - SUPS_TYPE_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
 *                      - Hold is active and accepting waiting or held
 *             - 0x04 - SUPS_TYPE_HOLD_ALL_EXCEPT_SPECIFIED_CALL
 *                      - Hold all calls except a specified one
 *             - 0x05 - SUPS_TYPE_MAKE_CONFERENCE_CALL
 *                      - Make a conference call
 *             - 0x06 - SUPS_TYPE_EXPLICIT_CALL_TRANSFER
 *                      - Explicit call transfer
 *             - 0x07 - SUPS_TYPE_CCBS_ACTIVATION
 *                      - Activate completion of calls to busy subscriber
 *             - 0x08 - SUPS_TYPE_END_ALL_CALLS
 *                      - End all calls
 *             - 0x09 - SUPS_TYPE_RELEASE_SPECIFIED_CALL
 *                      - Release a specified call
 *  \return  none
 *
 *  \notes   none
 */
void ManageCalls( BYTE supstype );

/**
 *  \Name    StartDTMFServices
 *
 *  \purpose It starts DTMF
 *
 *  \param   none

 *  \return  none
 *
 *  \notes   none
 */
void StartDTMFServices(void);

/**
 *  \Name    StopDTMFServices
 *
 *  \purpose It stops DTMF
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void StopDTMFServices(void);

/**
 *  \Name    CallForwardingServices
 *
 *  \purpose It provides call forwarding services
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void CallForwardingServices(void);

/**
 *  \Name    CallBarringServices
 *
 *  \purpose It provides call barring services
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void CallBarringServices(void);

/**
 *  \Name    CallWaitingServices
 *
 *  \purpose It provides call waiting services
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void CallWaitingServices(void);

/**
 *  \Name    AutoAnswerService
 *
 *  \purpose It set auto answer
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void AutoAnswerService(void);

/**
 *  \Name    SetVoiceSUPSService
 *
 *  \purpose It set supplementary services
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetVoiceSUPSService(void);

/**
 *  \Name    GetCLIRServiceStatus
 *
 *  \purpose It get CLIR services
 *           - 0 - Disable
 *           - 1 - Enable
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetCLIRServiceStatus(void);

/**
 *  \Name    GetCLIPServiceStatus
 *
 *  \purpose It gets CLIP services Status
 *           - 0 - Disable
 *           - 1 - Enable
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetCLIPServiceStatus(void);

/**
 *  \Name    SetPreferredPrivacy
 *
 *  \purpose It set preferred privacy and applicable for only 3GPP device
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetPreferredPrivacy(void);

/**
 *  \Name    SendFlash
 *
 *  \purpose It sends flash
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SendFlash(void);

/**
 *  \Name    QuitApplication
 *
 *  \purpose Closes the Application
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void QuitApplication(void);

/**
 *  \Name    SendFlashForWaitingCall
 *
 *  \purpose Send Flash QMI message
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SendFlashForWaitingCall(void);

void CallStateCBK(BOOL first_call);

int GetWirelessTechInfo(void);
