/*************
 *
 * Filename:    fwDld_9x07.c
 *
 * Purpose:     Contains routines needed to update MDM 9x07 based devices
 *
 * Copyright:   2017 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "fwDld_9x07.h"

static volatile BOOL fwdwlcomplete = FALSE;
static BOOL fwdwfail = FALSE;
extern BOOL downloadmode;

/*
 * Name:     FwDwldCbk_9x07
 *
 * Purpose:  Firmware download completion callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDwldCbk_9x07(ULONG status)
{
    switch (status)
    {
        case eQCWWAN_ERR_NONE:
        {
            /* set firmware complete to true */
            fwdwlcomplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED:
        {
            fprintf( stdout, "Firmware Not Downloaded\n" );
            fwdwfail = FALSE; // This is not an error as the modem did not need the FW to be downloaded
            /* set firmware complete to true */
            fwdwlcomplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FW_ENTER_DOWNLOAD_MODE:
        {
            if (!downloadmode)
            {
                fprintf(stdout, "DONE\n");
                downloadmode = TRUE;
            }
            return;
        }
        case eQCWWAN_ERR_SWIIM_FW_FLASH_COMPLETE:
        {
            fprintf(stdout, "DONE\n");
            fprintf(stdout, "Rebooting the module\n" );
            return;
        }
        default:
        {
            fprintf( stderr, "ERROR: Firmware Download failed. Callback status %u", status);
            fwdwfail = TRUE;
            /* set firmware complete to true */
            fwdwlcomplete = TRUE;
            break;
        }
    }
}

/*
 * Name:     FwDloader_9x07
 *
 * Purpose:  Download an image to a MDM 9x07 based device
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDloader_9x07( CHAR *pImagePath )
{
    ULONG rc;
    CHAR completeImagePath[512];
    ULONG len = 0;

    /* Reset the firmware download completion and device ready flags */
    fwdwlcomplete = FALSE;
    fwdwfail = FALSE;

    /* disable buffering on stdout */
    setbuf(stdout, NULL);

    fprintf(stdout, "Communicating with the target\n");
    rc = SetFwDldCompletionCbk( FwDwldCbk_9x07 );
    if( eQCWWAN_ERR_NONE != rc )
    {
        fprintf( stderr, "ERROR: Failed to register firmware Download Completion Callback\n"\
                         "Failure Code: %d\n", rc );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to register firmware Download Completion Callback: %u", __func__, rc);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Concatenate image Path with file name */
    memset( completeImagePath, 0, sizeof(completeImagePath) );
    strncpy( completeImagePath,
             pImagePath,
             sizeof(completeImagePath)-1 );
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

    /* Start downloading the firmware */
    rc = SLQSUpgradeFirmware9x07(completeImagePath);
    if( eQCWWAN_ERR_NONE != rc )
    {
        fprintf( stderr, "Firmware Download Failed\n"\
                         "Failure Code: %d\n", rc );
        syslog (LOG_DEBUG, "%s: ERROR: Firmware Download Failed: %u", __func__, rc);
        quit_app(eFWDWL_ERR_SDK);
        return;
    }
    fprintf(stdout, "DONE\n");
    if (!downloadmode)
    {
        fprintf(stdout, "Switching to firmware download mode\n");
    }
    while ( !downloadmode)
    {
        /* Display "." while firmware downloads */
        fprintf(stdout, ".");
        sleep(2);
    }
    fprintf(stdout, "Downloading the firmware\n");
    while( !fwdwlcomplete )
    {
        /* Display "." while firmware downloads */
        fprintf(stdout, ".");
        sleep(2);
    }
    fprintf(stdout, "DONE\n");
    if (fwdwfail)
    {
        fprintf( stderr, "ERROR: Firmware Download Failed\n");
        quit_app(eFWDWL_ERR_SDK);
    }

    fprintf( stdout, "Firmware Download SUCCESS\n");
}
