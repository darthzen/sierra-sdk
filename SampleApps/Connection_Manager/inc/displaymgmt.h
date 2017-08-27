/*************
 *
 * Filename:    displaymgmt.h
 *
 * Purpose:     Contains function for managing the capturing of logs for
 *              Connection manager application
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef __WINDOW_MGMT_H__
#define __WINDOW_MGMT_H__

#ifdef __cplusplus
extern "C" {
#endif

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

/****************************************************************
*                       #DEFINES
****************************************************************/
//#define DBG
#define TRUE    0
#define FALSE   1

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/

/* Logs's fields enum */
enum eUserWindowFields{
    eMANUFACTURE_ID,
    eMODEL_ID,
    eFIRMWARE_REVISION,
    eBOOT_VERSION,
    ePRI_VERSION,
    ePRL_VERSION,
    ePRL_PREFERENCE,
    eIMSI,
    eESN_NUMBER,
    eIMEI_NUMBER,
    eMEID_NUMBER,
    eHARDWARE_REVISION,
    eDEVICE_STATE,
    eHOME_NETWORK,
    eROAMING_STATUS,
    eDATA_CAPABILITIES,
    eSESSION_STATE,
    eDATA_BEARER,
    eDORMANCY_STATUS,
    eLU_REJECT,
    eRADIO_INTERFACE,
    eACTIVE_BAND_CLASS,
    eACTIVE_CHANNEL,
    eSIGNAL_STRENGTH,
    eECIO,
    eIO,
    eSINR,
    eRSRQ,
    ePDP_TYPE,
    eAUTHENTICATION,
    ePROFILE_NAME,
    eAPN_NAME,
    eUSER_NAME,
    eIP_ADDRESS,
    ePRIMARY_DNS,
    eSECONDARY_DNS,
    eCALL_STATUS,
    eTX_BYTES,
    eRX_BYTES,
    eCURRENT_TX_RATE,
    eCURRENT_RX_RATE,
    eMAX_TX_RATE,
    eMAX_RX_RATE,
    eMAX_DISPLAY_FIELDS,
};

/****************************************************************
*                       FUNCTIONS
****************************************************************/

extern void InitializeDisplay( void );
extern void QuitApplication( void );
extern void UpdateUserDisplay(
    int  windowFieldIdx,
    char *pData );
extern void SetWindowDisplayState( int state );
extern int GetWindowDisplayState( void );
extern void ResetDisplay( void );
extern void closeLogFile( void );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __WINDOW_MGMT_H__ */
