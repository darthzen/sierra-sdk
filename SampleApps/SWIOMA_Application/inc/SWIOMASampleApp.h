/*************
 *
 * Filename:    SWIOMASampleApp.h
 *
 * Purpose:     Contains function for SWIOMA
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

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
#include "stdbool.h"

/****************************************************************
*                       DEFINES
****************************************************************/
#define FALSE                   0
#define ENTER_KEY               0x0A
#define ENTER_KEY_PRESSED       0
#define OPTION_LEN              5
#define ESCAPE_KEY              0x1B
#define DEV_NODE_SZ             256
#define DEV_KEY_SZ              16
#define CALL_ERROR_CODE         0xffff
#define EOS                     '\0'

#define SESSION_TYPE_FOTA   0x00
#define SESSION_TYPE_CONFIG 0x01

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/
/* Device information structure */
typedef struct device_info_param{
   CHAR deviceNode[DEV_NODE_SZ];
   CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/* user options */
enum eUSER_OPTIONS
{
    eSTART_SESSION            = 1,
    eCANCEL_SESSION           = 2,
    eGET_SESSION_INFO         = 3,
    eGET_OMADM_SETTING        = 4,
    eSET_OMADM_SETTING        = 5,
    eSEL_DOWNLOAD_FIRMWARE    = 6,
    eSEL_UPDATE_FIRMWARE      = 7,

};

/* call state information for FOTA */
enum eSTATE_FOTA
{
    eNO_FIRMWARE_AVAILABLE    = 0x01,
    eQUERY_FIRMWARE_DOWNLOAD  = 0x02,
    eFIRMWARE_DOWNLOADING     = 0x03,
    eFIRMWARE_DOWNLOADED      = 0x04,
    eQUERY_FIRMWARE_UPDATE    = 0x05,
    eFIRMWARE_UPDATING        = 0x06,
    eFIRMWARE_UPDATED         = 0x07,
    eSEL_READ_REQUEST         = 0x08,
    eSEL_CHANGE_REQUEST       = 0x09
};

/* call state information for CONFIG */
enum eSTATE_CONFIG
{
    eREAD_REQUEST    = 0x01,
    eCHANGE_REQUEST  = 0x02,
    eCONFIG_COMPLETE = 0x03,
};

/* call state information for CONFIG */
enum eSWIOMA_SETTING
{
    eENABLE =1,
    eDISABLE,
};

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
 *  \Name    ClearCallBack
 *
 *  \purpose It unsubscribes callback
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void ClearCallBack(void);

/**
 *  \Name    SetCallBack
 *
 *  \purpose It subscribes the callback
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetCallBack(void);

/**
 *  \Name    StartSession
 *
 *  \purpose Start the Session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
bool StartSession(void);

/**
 *  \Name    CancelSession
 *
 *  \purpose Cancel the Session
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void CancelSession(void);

/**
 *  \Name    SendSelection
 *
 *  \purpose Send the Selection
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SendSelection(void);

/**
 *  \Name    GetSessionInfo
 *
 *  \purpose Get Session Information
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetSessionInfo(void);

/**
 *  \Name    GetSwiOmaDMSetting
 *
 *  \purpose Get SWIOMA Setting
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void GetSwiOMADMSetting(void);

/**
 *  \Name    SetSwiOmaDMSetting
 *
 *  \purpose Set SWIOMA Setting
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void SetSwiOMADMSetting(void);

/**
 *  \Name    printmenu
 *
 *  \purpose Display main menu
 *
 *  \param   none
 *
 *  \return  none
 *
 *  \notes   none
 */
void printmenu( void );

/**
 *  \Name    cbkSetSLQSOMADMAlertCallback
 *
 *  \Purpose SetSLQSOMADMAlertCallback API callback function
 *
 *  \param   ULONG sessionType,
 *           BYTE* psessionTypeFields
 *
 *  \return  none
 *
 *  \notes   none
 */
void cbkSetSLQSOMADMAlertCallback( ULONG sessionType,BYTE* psessionTypeFields );

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
