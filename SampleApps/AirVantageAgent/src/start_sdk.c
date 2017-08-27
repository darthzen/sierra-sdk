/*************
 *
 * Filename:    start_sdk.c
 *
 * Purpose:     start sdk with readyagent enable
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#define _GNU_SOURCE

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/****************************************************************
*                       DEFINES
****************************************************************/
#define DEV_NODE_SZ        256
#define DEV_KEY_SZ         16
#define FALSE              0
#define TRUE               1
/****************************************************************
*                       DATA STRUCTURE
****************************************************************/

/* Device information structure */
typedef struct device_info_param{
  CHAR deviceNode[DEV_NODE_SZ];
  CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/* path to sdk binary */
static char *sdkbinpath = NULL;
#if 0
/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

static BYTE devicemode = DEVICE_STATE_DISCONNECTED;
#endif

/*
 * Name:     StartSDK
 *
 * Purpose:  It starts the SDK by setting the SDK path, enumerates the device
 *           and connects to the SDK.
 *
 * Return:   SUCCESS on successfully starting SDK, else error code
 *
 * Notes:    none
 */
ULONG StartSDK(BYTE modem_index)
{
    ULONG rc = 0;
    //BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE != (rc = SetSDKImagePath(sdkbinpath)) )
    {
        return rc;
    }

    /* Establish APP<->SDK IPC */
    if( eQCWWAN_ERR_NONE != (rc = SLQSStart(modem_index,NULL)) )
    {
        return rc;
    }

    return rc;
#if 0
    rc = SLQSGetDeviceMode ((BYTE *)&devicemode);

    /* Can enumerate and connect only if device is in Application mode */
    if ( devicemode == DEVICE_STATE_READY )
    {
        /* Enumerate the device */
        while( QCWWAN2kEnumerateDevices( &devicesSize, (BYTE *)(pdev) ) != 0 )
        {
            printf ("\nUnable to find device..\n");
            sleep(1);
        }
        fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
             devicesSize,
             pdev->deviceNode,
             pdev->deviceKey );
             
        /* Connect to the SDK */
        rc = QCWWANConnect( pdev->deviceNode,
                            pdev->deviceKey );
    }
    return rc;
#endif
}

int main( int argc, const char *argv[])
{
    if( argc < 1 )
    {
        fprintf( stderr, "usage: %s <path to sdk binary> <path to Readyagent>\n", argv[0] );
        exit( EXIT_SUCCESS );
    }

    if( NULL == (sdkbinpath = malloc(strlen(argv[1]) + 1)) )
    {
        perror(__func__);
        exit( EXIT_FAILURE );
    }

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1);

    /* Start the SDk */
    StartSDK(0);
    
    return 1;
}
