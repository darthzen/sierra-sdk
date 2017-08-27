/*************
 *
 * Filename:    fwDld_main.h
 *
 * Purpose:     Contains forward declarations and structures used by
 *              main file of firmware download application
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef __FW_DLOAD_MAIN_H__
#define __FW_DLOAD_MAIN_H__

#include "fwDld_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
*                       #DEFINES
****************************************************************/
#define rcprint(s, u) syslog(LOG_USER, "%s: rc = 0x%lX, %s", s, u)
#define MAX_PATH_LEN 255
#define MAX_DEV_LEN  5

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/

/* Device Type enum */
enum DeviceTypes
{
    eDEV_MDM9x00,
    eDEV_MDM9x15,
    eDEV_MDM9x30,
    eDEV_GOBI3K,
    eDEV_MDM9x07,
    eDEV_INVALID = -1,
};

/* Structure to store user provided parameters */
struct fwDld_options
{
    ULONG devType;
    char  path[MAX_PATH_LEN];
};

/****************************************************************
*                       FUNCTIONS
****************************************************************/

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __FW_DLOAD_MAIN_H__ */
