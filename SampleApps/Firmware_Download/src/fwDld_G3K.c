/*************
 *
 * Filename:    fwDld_G3K.c
 *
 * Purpose:     Contains routines needed to update Gobi3K devices
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "fwDld_G3K.h"
#include <syslog.h>
static volatile BOOL fwDwlComplete = FALSE;

/*
 * Name:     DevStateChgCbk
 *
 * Purpose:  Device State change callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void DevStateChgCbk_G3K(eDevState devstatus)
{
    /* If device is ready to communicate */
    if( devstatus ==  DEVICE_STATE_READY )
    {
        fprintf( stderr, "\nDevice Ready\n" );

        /* Unsubscribe from the callback */
        SetDeviceStateChangeCbk(NULL);
        fwDwlComplete = TRUE;
    }
}

/*
 * Name:     FwDwldCbk_G3K
 *
 * Purpose:  Firmware download completion callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDwldCbk_G3K(ULONG status)
{
    if ( eQCWWAN_ERR_NONE == status)
    {
        fprintf( stderr, "\nFirmware Download Completed" );
    }
    else if (eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED == status)
    {
        fprintf( stderr, "\nFirmware Not Downloaded" );
    }
    /* set firmware complete to true */
    fwDwlComplete = TRUE;

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);
}

/*
 * Name:     FwDloader_G3K
 *
 * Purpose:  Download firmware on a Gobi3K device
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDloader_G3K( CHAR *pImagePath )
{
    ULONG              resultCode = 0;
    ULONG              firmwareId = 0;
    struct qmifwinfo_s imgInformation;
    CHAR               completeImagePath[512];
    ULONG              len = 0;

    /* Concatenate image Path with '/' */
    memset( completeImagePath, 0, sizeof(completeImagePath) );
    strncpy( completeImagePath,
             pImagePath,
             sizeof(completeImagePath)-1);
    len = strlen(completeImagePath);

    if (len == 0)
    {
        fprintf( stderr, "Error: empty ImagePath\n" );
        quit_app(eFWDWL_ERR_PATH_NOT_SPECIFIED);
    }

    CHAR *pr = &completeImagePath[len - 1];
    if( *pr != '/' )
    {
        DIR *pDir = opendir(completeImagePath);
        if(pDir != NULL)
        {
            closedir(pDir);
            *(pr + 1) = '/';
        }
    }

    /* Get the image information about downloading image */
    resultCode = SLQSGetImageInfo( completeImagePath,
                                   &imgInformation );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "Error: Could not get details from the image at the path: %s\n", pImagePath );
        fprintf( stderr, "Failure Code : %u\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Could not get details from the image at the path: %s: %u", __func__, pImagePath, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Save the firmware id for the image being downloaded */
    firmwareId = imgInformation.dev.g.FirmwareID;

    /* Reaching here means, MBN image is found at specified path */
    /* Subscribe to Device State Change callback */
    resultCode = SetDeviceStateChangeCbk(DevStateChgCbk_G3K);
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Failed to register to device State Callback\n"\
                         "Failure Code: %d\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to register device State Callback: %u", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Subscribe to Firmware Download completion callback */
    resultCode = SetFwDldCompletionCbk(FwDwldCbk_G3K);
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Failed to register firmware Download Completion Callback\n"\
                         "Failure Code : %u\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to register firmware Download Completion Callback: %u", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Start downloading the firmware */
    resultCode = UpgradeFirmware2k( completeImagePath );
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Firmware Download Failed\n"\
                         "Failure Code : %u\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Firmware Download Failed: %u", __func__, resultCode);
        /* De register device state change and firmware download completion
         * callback.
         */
        SetFwDldCompletionCbk( NULL );
        SetDeviceStateChangeCbk( NULL );
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Keep displaying "." until fimrware downloads complete */
    fprintf( stderr, "Downloading Firmware. . .");
    fwDwlComplete = FALSE;
    while( !fwDwlComplete )
    {
        fprintf( stderr, " .");
        sleep(1);
    }

    memset( (void *)&imgInformation, 0, sizeof( imgInformation ) );

    /* Get the information about the currently running image on device */
    resultCode = SLQSGetFirmwareInfo( &imgInformation );

    /* Check if the firmware download is success */
    if( firmwareId == imgInformation.dev.g.FirmwareID )
    {
        fprintf( stderr, "INFO: Firmware Upgrade Success!!!\n" );
    }
    else
    {
#ifdef DBG
        fprintf( stderr, "Parsed FW Id from host : %x\n Current  FW Id : %x\n",
                         firmwareId, imgInformation.dev.g.FirmwareID );
#endif

        fprintf( stderr, "ERROR: Firmware Upgrade Failed!!!\n" );
        syslog (LOG_DEBUG, "%s: ERROR: Parsed FW Id from host : %x\n Current  FW Id : %x", __func__, firmwareId, imgInformation.dev.g.FirmwareID);
        quit_app(eFWDWL_ERR_FW_UPGRADE);
    }
}
