/*************
 *
 * Filename:    fwDld_9x15.h
 *
 * Purpose:     Contains forward declarations, functions for fwDld_9x15.c
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef __FW_DLOAD_9x15_H__
#define __FW_DLOAD_9x15_H__

#include "fwDld_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
*                       #DEFINES
****************************************************************/
#define IMG_FW_TYPE_CWE 2
#define IMG_PRI_TYPE_NVU 3
#define IMG_FW_TYPE_SPK 4

/****************************************************************
*                       FUNCTIONS
****************************************************************/
void FwDloader_9x15( CHAR *pImagePath, BYTE slot );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __FW_DLOAD_9x15_H__ */
