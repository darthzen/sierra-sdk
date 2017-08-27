/*************
 *
 * Filename:    fwDld_9x15.c
 *
 * Purpose:     Contains routines needed to update MDM 9x15 based devices
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "fwDld_9x15.h"
#include <syslog.h>
#include <dirent.h>

#define FW_DOWNLOAD_TIMEOUT_SEC 180
extern int g_kill_sdk;

static volatile BOOL fwDwlDone = FALSE;
static BOOL fwdwfail = FALSE;
extern BOOL downloadmode;
extern BOOL g_crashStateIgnore;

char carriers[][20] =
{
    {"VODAFONE"},
    {"VERIZON"},
    {"ATT"},
    {"SPRINT"},
    {"TMOBILE"},
    {"GENERIC"},
    {"TELEFONICA"},
    {"ITALIA"},
    {"ORANGE"},
    {"ORANGE-EU"},
    {"TELSTRA"},
    {"BELL"},
    {"TELUS"},
    {"ROGERS"},
    {"DOCOMO"},
    {"SWISSCOM"},
    {"AERIS"},
    {"PTCRB"},
};

/*
 * Name:     DevStateChgCbk
 *
 * Purpose:  Device State change callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void DevStateChgCbk_9x15(eDevState devstatus)
{
    /* If device is ready to communicate */
    if( devstatus ==  DEVICE_STATE_READY )
    {
        /* Unsubscribe from the callback */
        SetDeviceStateChangeCbk(NULL);
        fwDwlDone = TRUE;
    }
}

BOOL IsGIMDownloadNeeded(CHAR *filePath)
{
    ULONG resultCode = 0;
    BYTE len;
    struct slqsfwinfo_s spkInfo;
    BOOL matched = FALSE;

    len = strlen(filePath);
    CHAR *pr = &filePath[len - 1];
    if( *pr != '/' )
    {
        DIR *pDir = opendir(filePath);
        if(pDir != NULL)
        {
            closedir(pDir);
            *(pr + 1) = '/';
        }
    }

    resultCode = SLQSGetImageInfo_9x15( filePath,
                         IMG_FW_TYPE_SPK,
                         &spkInfo);
    if ( eQCWWAN_ERR_NONE == resultCode)
    {
        BYTE i;
        for ( i = 0; i < sizeof(carriers)/sizeof(carriers[0]); i++)
        {
            if ( strcmp(spkInfo.carrier_str, carriers[i]) == 0)
            {
                matched = TRUE;
                break;
            }
        }
    }

    return matched;
}

/*
 * Name:     FwDwldCbk_9x15
 *
 * Purpose:  Firmware download completion callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDwldCbk_9x15(ULONG status)
{
    switch (status)
    {
        case eQCWWAN_ERR_NONE:
        {
            /* set firmware complete to true */
            fwDwlDone = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED:
        {
            fprintf( stdout, "\nFirmware Not Downloaded" );
            fwdwfail = FALSE; // This is not an error as the modem did not need the FW to be downloaded
            /* set firmware complete to true */
            fwDwlDone = TRUE;

            if (!downloadmode)
            {
                fprintf(stdout, "\nbreak wait for download mode loop\n");
                downloadmode = TRUE;
            }
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
            fwDwlDone = TRUE;
            break;
        }
    }

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);
}

BOOL IsValidPath(CHAR *pImgPath)
{
    struct dirent *pDirent;
    DIR *pDir;
    BOOL retval =0;
    
    pDir = opendir (pImgPath);
    if (pDir == NULL)
        return 0;
    else
    {
        while ((pDirent = readdir(pDir)) != NULL)
        {
            if (pDirent->d_name[0]!='.')
            {        
                if ((strstr(pDirent->d_name, ".cwe")) ||
                    (strstr(pDirent->d_name, ".nvu")) ||
                    (strstr(pDirent->d_name, ".spk")))
                {
                    retval = 1;  
                }
            }
        }
    }
    closedir (pDir);
    return retval;
}


/*
 * Name:     FwDloader_9x15_NoGIM
 *
 * Purpose:  Download firmware on a 9x15 based device using non-GIM mode
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDloader_9x15_NoGIM( CHAR *pImagePath )
{
    ULONG rc = 0;
    CHAR  completeImagePath[512];
    ULONG len = 0;

    /* Reset the firmware download completion and device ready flags */
    fwDwlDone = FALSE;
    fwdwfail = FALSE;

    /* disable buffering on stdout */
    setbuf(stdout, NULL);

    fprintf(stdout, "Communicating with the target\n");

    rc = SetFwDldCompletionCbk( FwDwldCbk_9x15 );
    if( eQCWWAN_ERR_NONE != rc )
    {
        fprintf( stderr, "ERROR: Failed to register firmware Download Completion Callback\n"\
                         "Failure Code: %d\n", rc );
        quit_app(eFWDWL_ERR_SET_CBK);
    }

    /* Concatenate image Path with '/' */
    memset( completeImagePath, 0, sizeof(completeImagePath) );
    
    len = strlen(pImagePath);
    if (len > (sizeof(completeImagePath) - 2) )
    {
        fprintf( stderr, "Error: ImagePath too long to process\n" );
        fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_PATH_TOO_LONG );
        quit_app(eFWDWL_ERR_PATH_TOO_LONG);
    }

    if (len == 0)
    {
        fprintf( stderr, "Error: empty ImagePath\n" );
        fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_PATH_NOT_SPECIFIED );
        quit_app(eFWDWL_ERR_PATH_NOT_SPECIFIED);
    }

    strncpy( completeImagePath,
             pImagePath,
             len );
    CHAR *pr = &completeImagePath[len - 1];
    if( *pr != '/' )
    {
        DIR *pDir = opendir(completeImagePath);
        if(pDir != NULL)
        {
            closedir(pDir);
            *(pr + 1) = '/';
            if (!IsValidPath(completeImagePath))
            {
                fprintf( stderr,
                         "ERROR: Failed to retrieve both PRI or firmware image information"\
                         " - Please check path and try again\n");
                fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_INVALID_PATH );
                quit_app(eFWDWL_ERR_INVALID_PATH);
            }
        }
    }

    /* set the crash state checking flag before calling UpgradeFirmware2k */
    SLQSSetCrashStateCheckIgnore(g_crashStateIgnore);

    /* Start downloading the firmware */
    rc = UpgradeFirmware2k( completeImagePath );
    if( eQCWWAN_ERR_NONE != rc )
    {
        /* some firmware of MC7700 may reboot immediately after receiving
         * set firmware id command without sending out respond,
         * neglect this error to bypass issue*/
        if(eQCWWAN_ERR_NO_DEVICE !=rc )
        {
            fprintf( stderr, "Firmware Download Failed\n"\
                             "Failure Code: %d\n", rc );
            syslog (LOG_DEBUG, "%s: Failure Code: %d", __func__, rc);
            quit_app(eFWDWL_ERR_SDK);
        }
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
    while( !fwDwlDone )
    {
        /* Display "." while firmware downloads */
        fprintf( stderr, ".");
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

/*
 * Name:     FwDloader_9x15_GIM
 *
 * Purpose:  Download firmware on a 9x15 based device using GIM mode
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDloader_9x15_GIM( CHAR *pImagePath, BYTE slot_id )
{
    ULONG              resultCode = 0;
    struct qmifwinfo_s FwImgInfo, NVImgInfo, SpkImgInfo;
    CurrentImgList     CurrImgList;
    CurrImageInfo      currImgInfo[5];
    BYTE               numEntries  = 5;
    CHAR               priVer[16];
    CHAR               pkgVer[SLQSFWINFO_PACKAGEID_SZ];
    CHAR               carrier[SLQSFWINFO_PACKAGEID_SZ];
    CHAR               AppVer[SLQSFWINFO_PACKAGEID_SZ];
    CHAR               completeImagePath[512];
    ULONG              len = 0;
    BOOL               skipPRIChk = FALSE;
    BOOL               skipMDMChk = FALSE;
    BOOL               spkFlag = FALSE;
    FirmwareUpdatStat  FirmwareUpdatStatus;
    BYTE               ImgType     = 0;
    ULONG              RefData     = 0;
    BYTE               RefStrLen   = 0xFF;
    BYTE               RefStr[255] = "\0";
    BYTE               LogStrLen   = 0xFF;
    BYTE               LogStr[255] = "\0";
    int                count =0;
    int NVImgInfoAvail = 0;

    fwDwlDone = FALSE;
    memset(&FirmwareUpdatStatus,0,sizeof(FirmwareUpdatStat));
    FirmwareUpdatStatus.pImgType      = &ImgType;
    FirmwareUpdatStatus.pRefData      = &RefData;
    FirmwareUpdatStatus.pRefStringLen = &RefStrLen;
    FirmwareUpdatStatus.pRefString    = RefStr;
    FirmwareUpdatStatus.pLogStringLen = &LogStrLen;
    FirmwareUpdatStatus.pLogString    = LogStr;

    
    /* Initialize variables */
    memset( priVer, 0, sizeof(priVer) );
    memset( pkgVer, 0, sizeof(pkgVer) );
    memset( carrier, 0, sizeof(carrier) );
    memset( AppVer, 0, sizeof(AppVer) );

    CurrImgList.pCurrImgInfo = currImgInfo;
    CurrImgList.numEntries   = numEntries;

    memset( completeImagePath, 0, sizeof(completeImagePath) );
    len = strlen(pImagePath);
    if (len > (sizeof(completeImagePath) - 2) )
    {
        fprintf( stderr, "Error: ImagePath too long to process\n" );
        fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_PATH_TOO_LONG );
        quit_app(eFWDWL_ERR_PATH_TOO_LONG);
    }
    strncpy( completeImagePath,
             pImagePath,
             strlen(pImagePath) );
    len = strlen(completeImagePath);

    if (len == 0)
    {
        fprintf( stderr, "Error: empty ImagePath\n" );
        fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_PATH_NOT_SPECIFIED );
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

    /* disable buffering on stdout */
    setbuf(stdout, NULL);
    fprintf(stdout, "Communicating with the target\n");

    /* Get Firmware image details */
    resultCode = SLQSGetImageInfo_9x15( completeImagePath,
                                        IMG_FW_TYPE_CWE,
                                        &FwImgInfo.dev.s );

    if (eQCWWAN_ERR_NONE != resultCode)
    {
        resultCode = SLQSGetImageInfo_9x15( completeImagePath,
                                     IMG_FW_TYPE_SPK,
                                     &FwImgInfo.dev.s );
    }

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr,
                 "INFO: no cwe file presents : (0x%x)\n", resultCode);
        syslog (LOG_DEBUG, "%s: no cwe file presents : %d", __func__, resultCode);
        skipMDMChk = TRUE;
    }
    else
    {
        /* Retrieve appversion( firmware version ) from fw image */
        strncpy( AppVer,
                 FwImgInfo.dev.s.appversion_str,
                 strlen(FwImgInfo.dev.s.appversion_str) );
    }

    /* Get PRI image details */
    resultCode = SLQSGetImageInfo_9x15( completeImagePath,
                                        IMG_PRI_TYPE_NVU,
                                        &NVImgInfo.dev.s );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr,
                 "INFO: no pri file presents : (0x%x)\n", resultCode);
        syslog (LOG_DEBUG, "%s: no pri file presents : %d", __func__, resultCode);
        skipPRIChk = TRUE;
    }
    else
    {
        NVImgInfoAvail = 1;
        /* Save the firmware details for the image being downloaded */
        /* Retrieve PRI version, pkg version, carrier from PRI image */
        strncpy( priVer,
                 NVImgInfo.dev.s.priversion_str,
                 strlen(NVImgInfo.dev.s.priversion_str) );

        strncpy( pkgVer,
                 NVImgInfo.dev.s.packageid_str,
                 strlen(NVImgInfo.dev.s.packageid_str) );

        strncpy( carrier,
                 NVImgInfo.dev.s.carrier_str,
                 strlen(NVImgInfo.dev.s.carrier_str) );
    }

    resultCode = SLQSGetImageInfo_9x15( completeImagePath,
                                 IMG_FW_TYPE_SPK,
                                 &SpkImgInfo.dev.s );

    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        spkFlag = TRUE;
        /* construct the image id and build id from spk file */
        memset(&FwImgInfo.dev.s, 0, sizeof(FwImgInfo.dev.s));
        memset(&NVImgInfo.dev.s, 0, sizeof(NVImgInfo.dev.s));
        strncpy( FwImgInfo.dev.s.appversion_str, SpkImgInfo.dev.s.appversion_str,
                 strlen(SpkImgInfo.dev.s.appversion_str) );
        strncpy( FwImgInfo.dev.s.packageid_str, "?", 1  );
        strncpy( FwImgInfo.dev.s.priversion_str, "?", 1 );
        strncpy( FwImgInfo.dev.s.carrier_str, "?", 1 );
        strncpy( NVImgInfo.dev.s.appversion_str, SpkImgInfo.dev.s.appversion_str,
                 strlen(SpkImgInfo.dev.s.appversion_str));
        strncpy( NVImgInfo.dev.s.packageid_str, SpkImgInfo.dev.s.packageid_str,
                 strlen(SpkImgInfo.dev.s.packageid_str));
        strncpy( NVImgInfo.dev.s.priversion_str, SpkImgInfo.dev.s.priversion_str,
                 strlen(SpkImgInfo.dev.s.priversion_str));
        strncpy( NVImgInfo.dev.s.carrier_str, SpkImgInfo.dev.s.carrier_str,
                 strlen(SpkImgInfo.dev.s.carrier_str));

        /* Save the firmware details for the image being downloaded */
        /* Retrieve PRI version, pkg version, carrier from PRI image */
        strncpy( AppVer, NVImgInfo.dev.s.appversion_str,
                 strlen(NVImgInfo.dev.s.appversion_str) );
        strncpy( priVer, NVImgInfo.dev.s.priversion_str,
                 strlen(NVImgInfo.dev.s.priversion_str) );

        strncpy( pkgVer, NVImgInfo.dev.s.packageid_str,
                 strlen(NVImgInfo.dev.s.packageid_str) );

        strncpy( carrier, NVImgInfo.dev.s.carrier_str,
                 strlen(NVImgInfo.dev.s.carrier_str) );
    }


    /* If both images were not found, it indicates an issue */
    if( skipMDMChk == TRUE && skipPRIChk == TRUE &&  spkFlag == FALSE)
    {
        fprintf( stderr,
                 "ERROR: Failed to retrieve both PRI and firmware image information"\
                 " - Please check path and try again\n");
        fprintf( stderr, "Failure Code:%d\n", eFWDWL_ERR_INVALID_PATH );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to retrieve both PRI and firmware image information", __func__);
        quit_app(eFWDWL_ERR_SDK);
    }

    if ( NVImgInfoAvail && strncmp( NVImgInfo.dev.s.sku_str, "9999999", 7) )
    {
        fprintf( stdout, "INFO: OEM PRI\n");
        skipPRIChk = TRUE;
    }

    /* Reaching here means, image has been found */
    /* Subscribe to Device State Change callback */
    resultCode = SetDeviceStateChangeCbk(DevStateChgCbk_9x15);
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Failed to register to device State Callback\n"\
                         "Failure Code: %d\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to register to device State Callback: %d", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Subscribe to Firmware Download completion callback */
    resultCode = SetFwDldCompletionCbk(FwDwldCbk_9x15);
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Failed to register firmware Download Completion Callback\n"\
                         "Failure Code : %u\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to register firmware Download Completion Callback: %d", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* set the crash state checking flag before calling UpgradeFirmware2k */
    SLQSSetCrashStateCheckIgnore(g_crashStateIgnore);

    /* Start downloading the firmware */
    if ( slot_id == 0 )
    {
        resultCode = UpgradeFirmware2k( completeImagePath );
    }
    else
    {
        resultCode = DownloadToSlot(completeImagePath, FwImgInfo.dev.s,
                                       NVImgInfo.dev.s, slot_id, 1);
    }

    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "ERROR: Firmware Download Failed\n"\
                         "Failure Code : %u\n", resultCode );

        /* Deregister device state change and firmware download completion
         * callback.
         */
        SetFwDldCompletionCbk( NULL );
        SetDeviceStateChangeCbk( NULL );
        syslog (LOG_DEBUG, "%s: ERROR: Firmware Download Failed: %d", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
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

    while( !fwDwlDone )
    {
        fprintf( stderr, ".");
        sleep(2);
    }
    fprintf(stdout, "DONE\n");

    /* Intialize current running image structure */
    memset( (void *)&CurrImgList, 0, sizeof( CurrImgList ) );
    memset( (void *)&currImgInfo, 0, sizeof( currImgInfo ) );

    CurrImgList.numEntries = numEntries;
    CurrImgList.pCurrImgInfo = currImgInfo;
    count = 0;
    do
    {
        sleep(1);
        resultCode = SLQSSwiGetFwUpdateStatus( &FirmwareUpdatStatus );
        if(resultCode==eQCWWAN_ERR_NONE)
        {
            break;
        }
        if(count==0)
        {
            fprintf(stdout,"\nWaiting Device Ready!\n");
        }
        else if(count>=(FW_DOWNLOAD_TIMEOUT_SEC-1))
        {
            fprintf(stdout,"\nWaiting Device Timeout!\n");
        }
        fprintf(stdout,". ");
    }while(count++<FW_DOWNLOAD_TIMEOUT_SEC);

    if(FirmwareUpdatStatus.pLogString)
    {
        if(strlen((char*)FirmwareUpdatStatus.pLogString)>0)
        {
            syslog (LOG_DEBUG, "Get firmware update status\n"\
              "LogString: %s\n", FirmwareUpdatStatus.pLogString);
            fprintf(stdout,"\nGet firmware update status:%s!\n",FirmwareUpdatStatus.pLogString);
        }
    }

    resultCode = SLQSSwiGetFirmwareCurr( &CurrImgList );
    if( eQCWWAN_ERR_NONE != resultCode)
    {
        fprintf( stderr, "ERROR: Failed to get firmware details after download\n"\
                         "Failure Code: %d\n", resultCode );
        syslog (LOG_DEBUG, "%s: ERROR: Failed to get firmware details after download: %d", __func__, resultCode);
        quit_app(eFWDWL_ERR_SDK);
    }

    /* Check if the firmware download is success */
    /* Compare PRI, package, carrier and app version strings */
    if( skipPRIChk == FALSE )
    {
        if( 0 != strncmp( CurrImgList.priver, priVer, sizeof(priVer) ) ||
            0 != strncmp( CurrImgList.pkgver, pkgVer, sizeof(pkgVer) ) ||
            0 != strncmp( CurrImgList.carrier, carrier, sizeof(carrier) ) )
        {
            fprintf( stdout, "CurrImgList.priver, priVer: %s/%s\n", CurrImgList.priver, priVer);
            fprintf( stdout, "CurrImgList.pkgVer, pkgVer: %s/%s\n", CurrImgList.pkgver, pkgVer);
            fprintf( stdout, "CurrImgList.carrier, carrier: %s/%s\n", CurrImgList.carrier, carrier);
            fprintf( stdout, "ERROR: PRI Upgrade Failed!!!\n" );
            quit_app(eFWDWL_ERR_FW_UPGRADE);
        }
        fprintf( stdout, "PRI Download SUCCESS\n" );
    }
    if( skipMDMChk == FALSE || spkFlag == TRUE )
    {
        if(  0 != strncmp( CurrImgList.fwvers, AppVer, sizeof(AppVer) ) )
        {
            fprintf( stderr, "ERROR: Firmware Upgrade Failed!!!\n" );
            quit_app(eFWDWL_ERR_FW_UPGRADE);
        }
        fprintf( stdout, "Firmware Download SUCCESS\n" );
    }

    quit_app(eFWDWL_SUCCESS);
}

/*
 * Name:     FwDloader_9x15
 *
 * Purpose:  Download firmware on a 9x15 based device
 *
 * Return:   None
 *
 * Notes:    none
 */
void FwDloader_9x15( CHAR *pImagePath, BYTE slot_id )
{
    ULONG              resultCode = 0;
    CurrentImgList     CurrImgList;
    CurrImageInfo      currImgInfo[5];
    BYTE               numEntries  = 5;
    CHAR               ModelId[128] = {0};
    BYTE               stringsize = sizeof(ModelId);

    memset( (void *)&CurrImgList, 0, sizeof( CurrImgList ) );
    memset( (void *)&currImgInfo, 0, sizeof( currImgInfo ) );
    CurrImgList.pCurrImgInfo = currImgInfo;
    CurrImgList.numEntries   = numEntries;

    GetModelID( stringsize, ModelId );

    if (strstr (ModelId, "AR"))
    {
        /* for all AR series, treat ot as No GIM since the AR firmware does not
           use Goib Image Switching */
        FwDloader_9x15_NoGIM( pImagePath );
    }
    else
    {
        /* There are 2 possible scenario's determined by calling SLQSSwiGetFwCurr
         * 1) Device does not support Gobi IM - In this case use same procedure as
         *    FwDloader_9x00. In this case, we need only one file.
         * 2) Device supports GIM but data returned is blank. Use normal procedure. */
        resultCode = SLQSSwiGetFirmwareCurr( &CurrImgList );
        if( eQCWWAN_ERR_NONE != resultCode)
        {
            /* Assume that device is in non GIM Mode. Device issues can be detected
             * on subsequent SLQS API calls */
            FwDloader_9x15_NoGIM( pImagePath );
        }
        else
        {
            /* check if any special handling needed for spk file download */
            if ( IsGIMDownloadNeeded(pImagePath))
            {
                /* Device supports GIM. We do not need the firmware details as of now. */
                FwDloader_9x15_GIM( pImagePath, slot_id );
            }
            else
            {
                /* not a well formated spk file, go through the mc77xx code path to perform fw download */
                FwDloader_9x15_NoGIM( pImagePath );
            }
        }
    }
}
