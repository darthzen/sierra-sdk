/*************
 *
 * Filename:    pdsservices.h
 *
 * Purpose:     Contains function for voice call handling
 *
 * Copyright:   © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef GPS_SERVICES_H_
#define GPS_SERVICES_H_

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
#define ENTER_KEY               0x0A
#define ENTER_KEY_PRESSED       0
#define OPTION_LEN              5
#define DEV_NODE_SZ             256
#define DEV_KEY_SZ              16
#define DISABLE                 0x00
#define ENABLE                  0x01
#define HEXZERO                 0x00

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
 *  \Name    starttrackingsession
 *  \purpose This structure contains all parameters which are used
 *           to strat tracking session
 *  \param   sessionControl
 *           - Control method:
 *             - 0x0 - Manual
 *
 *  \param  sessionType
 *          - Type:
 *            - 0x0 - New
 *
 *  \param  sessionOperation
 *          - Operating mode:
 *            - 0x00 - Standalone
 *            - 0x01 - MS-based
 *
 *  \param  sessionServerOption
 *          - Location server option:
 *              - 0x0 - Default
 *
 *  \param  fixTimeout
 *          - Maximum time to work on each fix (in seconds, max 255)
 *
 *  \param  fixInterval
 *          - interval between position fix requests (in seconds)
 *
 *  \param  fixCount
 *          - Count of position fix requests for this session
 *            (must be at least 1)
 *
 *  \param  fixAccuracy
 *          - Time between fixes (in ms)
 */
typedef struct
{
    BYTE              sessionControl;
    BYTE              sessionType;
    BYTE              sessionOperation;
    BYTE              sessionServerOption;
    BYTE              fixTimeout;
    ULONG             fixInterval;
    ULONG             fixCount;
    ULONG             fixAccuracy;
} starttrackingsession;

/**
 *  \Name    eUSER_OPTIONS
 *  \purpose It contains user options value for main menu
 *  \param   - 0x01 - eSET_GPS_SERVICE_STATE
 *                    - To set GPS service state
 *           - 0x02 - eGET_GPS_SERVICE_STATE
 *                    - To get GPS service state
 *           - 0x03 - eSTART_TRACKING_SESSION
 *                    - To start tracking session
 *           - 0x04 - eEND_TRACKING_SESSION
 *                    - To end tracking session
 */
enum MAIN_MENU
{
    eSET_GPS_SERVICE_STATE = 1,
    eGET_GPS_SERVICE_STATE,
    eSTART_TRACKING_SESSION,
    eEND_TRACKING_SESSION,
    eSRV_AUTO_TRACKING_SESSION,
    ePORT_AUTO_TRACKING_SESSION
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
 *  \Name    eGPS_TRACKING_STATE
 *  \purpose It contains GPS tracking state value
 *  \param   - 0 - eUNKNOWN
 *                 - Unknown
 *           - 1 - eINACTIVE
 *                 - Active
  *          - 2 - eACTIVE
 *                 - Inactive
 */
enum eGPS_TRACKING_STATE
{
    eUNKNOWN = 0,
    eINACTIVE,
    eACTIVE
};

/****************************************************************
*                       FUNCTIONS
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
ULONG StartSDK( void );

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
void SetCallBack( void );

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
void ClearCallBack( void );

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
void QuitApplication( void );

/*
 * \Name     PDSStateCallBack
 *
 * \purpose  It starts dial call functionality
 *
 * \param    enabledStatus
 *           - GPS enabled status
 *             - 0 - Disable
 *               - 1 - Enable
 *
 * \param    trackingStatus
 *           - GPS tracking status
 *             - 0 - Unknown
 *             - 1 - Inactive
 *             - 2 - Active
 *  \return    none
 *
 *  \notes     none
 */
void PDSStateCallBack( ULONG enabledStatus, ULONG trackingStatus);

/*
 *  \Name      NMEACallBack
 *
 *  \purpose   It starts dial call functionality
 *
 *  \param     pNMEAData
 *             - NULL-terminated string containing the position data
 *               in NMEA sentence format
 *
 *  \return    none
 *
 *  \notes     none
 */
void NMEACallBack( const char *pNMEAData );

/*
 *  \Name    GetGPSServiceState
 *
 *  \purpose It gets GPS service state
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetGPSServiceState( void );

/*
 *  \Name    PrintMainMenu
 *
 *  \purpose It prints main menu
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void PrintMainMenu( void );

/*
 *  \Name    SetGPSServiceState
 *
 *  \purpose It set GPS service state
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetGPSServiceState( void );

/*
 *  \Name    GetAutoTrackingState
 *
 *  \purpose It gets auto tracking state
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetAutoTrackingState( void );

/*
 *  \Name    SrvAutoTrackingsession
 *
 *  \purpose It set service auto tracking session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SrvAutoTrackingsession( void );

/*
 *  \Name    PortAutoTrackingsession
 *
 *  \purpose It set port auto tracking session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void PortAutoTrackingsession( void );

/*
 *  \Name    StartTrackingSession
 *
 *  \purpose It starts tracking session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void StartTrackingSession( void );

/*
 *  \Name    EndTrackingSession
 *
 *  \purpose It end tracking session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void EndTrackingSession( void );

#endif /* GPS_SERVICES_H_ */
