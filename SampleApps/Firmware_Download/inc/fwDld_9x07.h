/*************
 *
 * Filename:    fwDld_9x07.h
 *
 * Purpose:     Contains forward declarations, functions for fwDld_9x07.c
 *
 * Copyright:   2017 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef __FW_DLOAD_9x07_H__
#define __FW_DLOAD_9x07_H__

#include "fwDld_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
*                       #DEFINES
****************************************************************/
#define TRUE 1
#define FALSE 0

/****************************************************************
*                       FUNCTIONS
****************************************************************/
void FwDloader_9x07( CHAR *pImagePath );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __FW_DLOAD_9x07_H__ */
