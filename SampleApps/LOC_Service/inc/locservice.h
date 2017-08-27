/*************
 *
 * Filename:    locservice.h
 *
 * Purpose:     Contains function for LOC Service
 *
 * Copyright:   © 2016 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef LOC_SERVICE_H_
#define LOC_SERVICE_H_

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

#define IFPRINTF(fp,s,p)   if( NULL != p ){ fprintf(fp,s,*p); }

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
 *  \Name    eUSER_OPTIONS
 *  \purpose It contains user options value for main menu
 *  \param   - 0x01 - eQMI_LOC_START
 *                    - To start loc service
 */
enum MAIN_MENU
{
    eQMI_LOC_SET_OP_MODE=1,
    eQMI_LOC_START ,
    eQMI_LOC_STOP,
};
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

void LocEventPositionCB( QmiCbkLocPositionReportInd *pMsg);

#endif /* LOC_SERVICE_H_ */
