/*************
 *
 * Filename:    fwDld_common.h
 *
 * Purpose:     Contains routines, variables common to all devices
 *              for firmware download
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef __FW_DLOAD_COMMON_H__
#define __FW_DLOAD_COMMON_H__

#include "SWIWWANCMAPI.h"
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
#include <sys/stat.h>
#include <termios.h>
#include <getopt.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************
*                       FUNCTIONS
****************************************************************/
#define TRUE 1
#define FALSE 0

#ifdef __cplusplus
} /* extern "C" { */
#endif

// Application returned error codes
// Starts from 100
enum fwdwl_error_codes
{
    eFWDWL_SUCCESS = 0,             //       Success
    eFWDWL_ERR_GENERAL = 100,       // 100 - Generic FW download error
    eFWDWL_ERR_SDK,                 // 101 - SDK specific error. Please check syslog for SDK error codes
    eFWDWL_ERR_SET_CBK,             // 102 - Error in setting SDK callbacks
    eFWDWL_ERR_PATH_TOO_LONG,       // 103 - Path is too long
    eFWDWL_ERR_PATH_NOT_SPECIFIED,  // 104 - Mandatory SDK/FW path not set
    eFWDWL_ERR_FW_UPGRADE,          // 105 - Post FW download check indicates that the FW upgrade has failed. 
                                    //       For example, PRI not updated as expected, FW version not updated etc
    eFWDWL_ERR_INVALID_DEV,         // 106 - Invalid device
    eFWDWL_ERR_SDK_NOT_STARTED,     // 107 - Unable to start the SDK process. Check syslog for SDK error codes
    eFWDWL_ERR_INVALID_PATH,        // 108 - Invalid Path or No valid firmware or nvu file exist in the path
    eFWDWL_ERR_END
};

int quit_app(int);

#endif /* __FW_DLOAD_COMMON_H__ */
