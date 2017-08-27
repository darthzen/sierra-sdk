/*************
 *
 * Filename:    mc7xxximgmgmt.c
 *
 * Purpose:     MC7xxx Image Management application
 *
 * Copyright: © 2011-2013 Sierra Wireless Inc., all rights reserved
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
#include <getopt.h>

/****************************************************************
*                       DEFINES
****************************************************************/
//#define MC7xxxDBG
#define ENTER_KEY          0x0A
#define OPTION_LEN         4
#define CORRUT_FW_IMG      39
#define SUCCESS            0
#define MAX_IMAGE_PATH     514
#define DEV_NODE_SZ        256
#define DEV_KEY_SZ         16
#define FALSE              0
#define TRUE               1
#define PID_LEN            10
#define MAX_WAIT_DEVICE_READY 6
#define IMG_FW_TYPE_SPK    4
/****************************************************************
*                       DATA STRUCTURE
****************************************************************/

/* Device information structure */
typedef struct device_info_param{
  CHAR deviceNode[DEV_NODE_SZ];
  CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/* User options enumeration */
enum eUserOptions{
    eDEV_IMAGE_INFO = 0x31,
    eDWL_BOOT_IMG,
    eDWL_FW_IMG,
    eDWL_NV_ITEM,
    eHOST_IMAGE_INFO,
    eHOST_NVU_INFO,
    eSWI_IMG_SWITCH,
    eGET_COMBO,
    eEXIT_APP = ENTER_KEY,
};

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/

/* path to sdk binary */
static char *sdkbinpath = NULL;

/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

/* firmware download */
static volatile BOOL fwdwlcomplete = FALSE;
static BOOL fwdwfail = FALSE;
static BYTE devicemode = DEVICE_STATE_DISCONNECTED;
static BOOL bootupdated = FALSE;
static BOOL sdkPathSet = FALSE;
static BOOL forceDld   = FALSE;
static BYTE g_modem_index = 0;
static BOOL g_crashStateIgnore = FALSE;

/* macros*/
#define rcprint(s, u) syslog(LOG_USER, "%s: rc = 0x%lX, %s", s, u)

/* Command line options to firmware download tool */
const char * const short_options = "s:hfd:i:";

/* Command line long options for firmware download tool */
const struct option long_options[] = {
    {"sdkpath", 1, NULL, 's'},      /* SDK path */
    {"help",    0, NULL, 'h'},      /* Provides terse help to users */
    {"force",   0, NULL, 'f'},      /* Force download */
    {"modem",   1, NULL, 'd'},      /* modem index */
    {"crashStateChecking",   0, NULL, 'i'},      /* crash state checking */
    {NULL,      0, NULL,  0 }       /* End of list */
};

// This is global as we would like to reuse it to get valid combibations
struct ImageList       imageList; 

// These are global as we would like to reuse it for SetFirmwarePreference
struct SWI_STRUCT_CarrierImage ValidCombinations[50]; // max 50 PRIs
ULONG ValidCombinationSize = 50;

/****************************************************************
*                       FUNCTIONS
****************************************************************/
/*
 * Name:     FlushStdinStream
 *
 * Purpose:  Flush the stdin stream
 *
 * Return:   None
 *
 * Notes:    fflush does not work for input stream.
 */
void FlushStdinStream( )
{
    int inputChar;

    /* keep on reading until an <New Line> or end of file is received */
    do
    {
        inputChar = getchar();

#ifdef MC7xxxDBG
    fprintf( stderr,  "inputChar: 0x%x\n", inputChar );
#endif
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != EOF ) );
}

struct cwefwinfo{
    CHAR amss[128];
    CHAR boot[128];
    CHAR pri[128];
};

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
    BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE != (rc = SetSDKImagePath(sdkbinpath)) )
    {
        return rc;
    }

    /* Establish APP<->SDK IPC */
    if( eQCWWAN_ERR_NONE != (rc = SLQSStart(modem_index, NULL)) )
    {
        if( eQCWWAN_ERR_NONE != SLQSKillSDKProcess() )
        {
            return rc;
        }
        else
        {
            if( eQCWWAN_ERR_NONE != (rc = SLQSStart(modem_index, NULL)) )
            {
                return rc;
            }
        }
    }

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

#ifdef MC7xxxDBG
    fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
             devicesSize,
             pdev->deviceNode,
             pdev->deviceKey );
#endif
        bootupdated = FALSE;
        /* Connect to the SDK */
        rc = QCWWANConnect( pdev->deviceNode,
                            pdev->deviceKey );
    }
    else if ( devicemode == DEVICE_STATE_BOOT )
    {
        bootupdated = TRUE;
    }

    return rc;
}

/*
 * Name:     GetImagePath
 *
 * Purpose:  Prompt the user for the path containing the CWE file
 *
 * Return:   None
 *
 * Notes:    Sets the entire buffer to zeros unless a valid non-zero length path is provided
 */
void GetImagePath(
    CHAR *pImagePath )
{
    CHAR  *pEndOfLine = NULL;
    WORD  len = 0;

    while(1)
    {
        /* clear the imagePath buffer */
        memset( pImagePath, 0, MAX_IMAGE_PATH);

        /* Print the sub menu */
        fprintf( stderr,
                 "\nPlease specify the path (upto 510 Characters) or press <Enter> to return to the main menu: " );

        fgets( pImagePath, MAX_IMAGE_PATH, stdin);

#ifdef MC7xxxDBG
    fprintf( stderr,  "Image Path: %s\n", pImagePath );
#endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == pImagePath[0] )
        {
            /* clear the imagePath buffer */
            memset( pImagePath, 0, MAX_IMAGE_PATH);
            return;
        }

        /* If '\n' character is not read, there are more characters in input
         * stream. Clear the input stream and prompt the user to enter the
         * valid path.
         */
        pEndOfLine = strchr( pImagePath, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
            continue;
        }

        /* fgets() is used to get user input, hence buffer overflow will never
         * occur. New line character is also scanned and put at the end of
         * buffer by fgets(). Remove the new line character from the end of the
         * buffer and NULL terminate it. Check if the path has a terminating
         * '/' character, else append a terminating '/' character.
         */
        len = strlen( pImagePath );
        if( pImagePath[len - 2] != '/' )
        {
            /* To check the path, we have to cut out '\n' in the end of the path */
            pImagePath[len - 1] = '\0';
            DIR *pDir = opendir(pImagePath);
            if(pDir != NULL)
            {
                closedir(pDir);
                pImagePath[len - 1]  = '/';
                pImagePath[len] = '\0';
            }
            else
            {
                if (strstr(pImagePath, ".cwe") ||
                    strstr(pImagePath, ".spk") ||
                    strstr(pImagePath, ".nvu"))
                {
                    /* This is the case which user provided the file name instead of path */
                    pImagePath[len - 1] = '\0';
                }
                else
                {
                    /* EM7455 IM switching stored on device case which a fake path can be provided */
                    pImagePath[len - 1]  = '/';
                    pImagePath[len] = '\0';
                }
                
            }
        }
        else
        {
            pImagePath[len - 1] = '\0';
        }

        break;
    }
    return;
}

/*
 * Name:     FirmwareDwldCbk
 *
 * Purpose:  Firmware download completion callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void FirmwareDwldCbk(ULONG status)
{
    switch (status)
    {
        case eQCWWAN_ERR_NONE:
        {
            fprintf( stderr, "\nFirmware download complete; waiting for device...\n" );
            /* set firmware complete to true */
            fwdwlcomplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED:
        {
            fprintf( stderr, "\nFirmware Not Downloaded\n" );
            fwdwfail = FALSE; // This is not an error as the modem did not need the FW to be downloaded
            /* set firmware complete to true */
            fwdwlcomplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FW_ENTER_DOWNLOAD_MODE:
        {
            fprintf( stderr, "\nEnter Download Mode\n" );
            return;
        }
        case eQCWWAN_ERR_SWIIM_FW_FLASH_COMPLETE:
        {
            fprintf( stderr, "\nFlash Complete, Waiting for Modem to Reboot\n" );
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

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);

}

/*
 * Name:     cweFwDisplay
 *
 * Purpose:  CWE image info. display
 *
 * Return:   None
 *
 * Notes:    none
 */
static void cweFwDisplay(struct cwefwinfo *pfw)
{
    /* QMI services are available only if the application is running */
    if ( devicemode == DEVICE_STATE_READY )
    {
        if( eQCWWAN_ERR_NONE ==
            GetFirmwareRevisions( (BYTE)sizeof(pfw->amss),
                                  pfw->amss,
                                  (BYTE)sizeof(pfw->boot),
                                  pfw->boot,
                                  (BYTE)sizeof(pfw->pri),
                                  pfw->pri ) )
        {
            fprintf( stderr,
                     "\nCWE Image Fields\n"\
                     "----------------\n"\
                     "BOOT Version: %s\nAMSS Version: %s\nPRI Version: %s\n",
                     pfw->boot, pfw->amss, pfw->pri );
        }
        else
        {
            fprintf( stderr, "AMSS: %s\nBOOT: %s\nPRI: %s\n", "", "", "");
        }
    }
    else
    {
        fprintf( stderr, "Device was started up in boot mode, cwe information "\
                "is not available\n");
    }
}

/*
 * Name:     spkgFwDisplay
 *
 * Purpose:  SPKG CWE image info. display
 *
 * Return:   None
 *
 * Notes:    none
 */
void spkgFwDisplay( struct qmifwinfo_s *pin )
{
    fprintf(    stderr,
                "\nCurrent Firmware info\n"\
                "---------------------\n"\
                "Model ID: %s\n"\
                "BOOT Version: %s\n"\
                "AMSS Version: %s\n"\
                "SKU ID: %s\n"\
                "Package ID: %s\n"\
                "Carrier: %s\n"\
                "PRI version: %s\n"\
                "Current Carrier Name: %s\n"\
                "Current Cairier Version: %s\n",
                pin->dev.s.modelid_str,  pin->dev.s.bootversion_str,
                pin->dev.s.appversion_str, pin->dev.s.sku_str,
                pin->dev.s.packageid_str , pin->dev.s.carrier_str,
                pin->dev.s.priversion_str, pin->dev.s.cur_carr_name,
                pin->dev.s.cur_carr_rev);
}

/*
 * Name:     ImageInfoDisplay
 *
 * Purpose:  image info. display
 *
 * Return:   None
 *
 * Notes:    none
 */
void ImageInfoDisplay( ULONG *itemsCount, struct SWI_STRUCT_CarrierImage *CarrierImages)
{
    ULONG index =0 ;
    fprintf(stderr,
                   "\nAll Carrier Images on Host and Device \n-------------------------------------\nIndex   CarrierId   FolderId   Storage Type   "\
                   "PriImageId     PriBuildId              FwImageId     FwBuildId"\
                   "\n");

    while(index < *itemsCount)
    {
         fprintf(stderr,
                       "%-8u"\
                       "%-12u"\
                       "%-11u"\
                       "%-15u"\
                       "%-15s"\
                       "%-24s"\
                       "%-14s"\
                       "%-14s\n",
                       index+1,
                       CarrierImages[index].m_nCarrierId,
                       CarrierImages[index].m_nFolderId,
                       CarrierImages[index].m_nStorage,
                       CarrierImages[index].m_PriImageId,
                       CarrierImages[index].m_PriBuildId,
                       CarrierImages[index].m_FwImageId,
                       CarrierImages[index].m_FwBuildId);
        index++;
    }
}

/*
 * Name:     GetDeviceImageInfo
 *
 * Purpose:  Get the information about the image running on the device.
 *
 * Return:   None
 *
 * Notes:    none
 */
void GetDeviceImageInfo()
{
    struct qmifwinfo_s  spkg;
    struct cwefwinfo    cwe;
    ULONG               rc;
    struct sGetDeviceSeriesResult result = {eGOBI_DEV_SERIES_UNKNOWN,eQCWWAN_ERR_ENUM_BEGIN};


    /* Get the information about the image loaded on the device */
    rc = SLQSGetFirmwareInfo( &spkg );
    if( eQCWWAN_ERR_NONE != rc )
    {
        memset( &spkg.dev.s, 0, sizeof(spkg.dev.s) );
    }

    eGetDeviceSeries(&result);
    if ( (result.eDevice == eGOBI_DEV_SERIES_9X30) ||
         (result.eDevice == eGOBI_DEV_SERIES_9X15) )
    {
        CurrentImgList currImgList;
        CurrImageInfo  currImgInfo[5];

        currImgList.pCurrImgInfo = currImgInfo;
        /* set the numberEntries to the array size of currImgInfo */
        currImgList.numEntries = 5;
        rc = SLQSSwiGetFirmwareCurr(&currImgList);
        if( eQCWWAN_ERR_NONE != rc )
        {
            fprintf(stderr,"SLQSSwiGetFirmwareCurr return failure!\n");
            return;
        }
        spkgFwDisplay( &spkg );
        /* Display build id of the active FW/PRI image */
        fprintf(stderr,"Build ID: %s\n", (CHAR*)currImgList.pCurrImgInfo[0].buildID);
    }
    else
    {
        spkgFwDisplay( &spkg );
    }
    memset(&cwe, 0, sizeof(cwe));
    cweFwDisplay( &cwe );
}

/*
 * Name:     FirmwareDownloader
 *
 * Purpose:  Download a CWE image to the device
 *
 * Return:   None
 *
 * Notes:    none
 */
void FirmwareDownloader(CHAR *pImagePath, BYTE modem_index)
{
    struct qmifwinfo_s  spkg;
    struct cwefwinfo    cwe;
    long                rclocal, rcfw, rc;
    BYTE                prevdevicemode;
    BYTE                ctr = 0;

    /* Reset the firmware download completion and device ready flags */
    fwdwlcomplete = FALSE;
    fwdwfail = FALSE;

    while(1)
    {
        fprintf( stderr, "\n-- Active Device Image Before Download --\n");
        rc = SLQSGetFirmwareInfo( &spkg );
        if( eQCWWAN_ERR_NONE != rc )
        {
            /* no AMSS SPKG support */
            memset( &spkg.dev.s, 0, sizeof(spkg.dev.s) );
        }
        memset(&cwe, 0, sizeof(cwe));
        cweFwDisplay( &cwe );
        spkgFwDisplay( &spkg );

        /* Get the information about the image located at specified path */
        rclocal = SLQSGetImageInfo( pImagePath, &spkg );

        /* Display the image information */
        if( eQCWWAN_ERR_NONE == rclocal )
        {
            fprintf( stderr,  "\n-- Preparing to Download SPKG CWE image --\n\n" );
            spkgFwDisplay( &spkg );
        }
        else
        {
            fprintf( stderr,  "\n-- Preparing to Download CWE image --\n" );
        }

        /* Valid firmware image is found, exit the loop */
        break;
    }

    rc = SetFwDldCompletionCbk( FirmwareDwldCbk );
    if( SUCCESS != rc )
    {
        fprintf( stderr, "REGISTRATION FAILED - Firmware Download Completion Callback\n"\
                         "Failure Code: %ld\n", rc );
        return;
    }

    /* set the crash state checking flag before calling UpgradeFirmware2k */
    SLQSSetCrashStateCheckIgnore(g_crashStateIgnore);

    /* Start downloading the firmware */
    rc = UpgradeFirmware2k( pImagePath );
    if( eQCWWAN_ERR_NONE != rc )
    {
        /* some firmware of MC7700 may reboot immediately after receiving
         * set firmware id command without sending out respond,
         * neglect this error to bypass issue*/
        if(eQCWWAN_ERR_NO_DEVICE !=rc )
        {
            fprintf( stderr, "Firmware Download Failed\n"\
                             "Failure Code: %ld\n", rc );
            return;
        }

        return;
    }

    fprintf( stderr, "\n\nDownloading Firmware");
    while( !fwdwlcomplete )
    {
        /* Display "." while firmware downloads */
        fprintf( stderr, ".");
        sleep(2);
    }

    if (fwdwfail)
    {
        fprintf( stderr, "Firmware Download Failed\n");
        return;
    }

    /* Give the firmware time to apply any additional CWE files that are
     * not firmware boot and/or application images
     */
    fprintf(stderr, "Applying Firmware/PRI updates - please wait 20 seconds...\n\n");
    sleep(20);

    prevdevicemode = devicemode;
    rc = SLQSGetDeviceMode ((BYTE *)&devicemode);

    /* If device was originally in boot mode, and is now in app mode,
     * re-connect the device to the SDK so that QMI services are properly set up.
     */
    if ( prevdevicemode == DEVICE_STATE_BOOT &&
         devicemode == DEVICE_STATE_READY)
    {
        /* Start the SDk */
        rc = StartSDK(modem_index);
        if( SUCCESS != rc )
        {
            free(sdkbinpath);

            /* Display the failure reason */
            fprintf( stderr, "Failed to start SDK: Exiting App\n"\
                             "Failure Code: %lu\n", rc );

            /* Failed to start SDK, exit the application */
            exit( EXIT_FAILURE );
         }
    }

    /* Get the information about the image loaded on the device */
    rcfw = SLQSGetFirmwareInfo( &spkg );

    /* Display the information of the newly loaded image on the device */
    fprintf( stderr, "\n-- Active Device Image After Download --\n");

    if( eQCWWAN_ERR_NONE != rcfw )
    {
        /* no AMSS SPKG support */
        memset( &spkg.dev.s, 0, sizeof(spkg.dev.s) );
    }

    memset(&cwe, 0, sizeof(cwe));
    /* display device information */
    while( ctr < MAX_WAIT_DEVICE_READY )
    {
         rc = SLQSGetDeviceMode ((BYTE *)&devicemode);
         if( devicemode != DEVICE_STATE_READY )
         {
             sleep(5);
             ctr++;
         }
         else
         {
             break;
         }
     }

    cweFwDisplay( &cwe );
    spkgFwDisplay( &spkg );

    fprintf( stderr, "\nExiting Firmware Downloader\n" );
}

/*
 * Name:     NVItemDownloader
 *
 * Purpose:  Download an NV item to the device
 *
 * Return:   None
 *
 * Notes:    none
 */
void NVItemDownloader(CHAR *pImagePath, BYTE modem_index)
{
    struct qmifwinfo_s  spkg;
    long                rclocal, rc;
    BYTE                prevdevicemode;

    /* Reset the firmware download completion and device ready flags */
    fwdwlcomplete = FALSE;
    fwdwfail = FALSE;

    while(1)
    {
        /* Get the information about the image located at specified path */
        rclocal = SLQSGetImageInfo( pImagePath, &spkg );

        /* Display the image information */
        if( eQCWWAN_ERR_NONE == rclocal )
        {
            fprintf( stderr,  "\n-- Downloading SPKG CWE NV Item --\n\n" );
            spkgFwDisplay( &spkg );
        }
        else
        {
            fprintf( stderr,  "\n-- Downloading CWE NV Item --\n" );
        }

        /* Valid firmware image is found, exit the loop */
        break;
    }

    rc = SetFwDldCompletionCbk( FirmwareDwldCbk );
    if( SUCCESS != rc )
    {
        fprintf( stderr, "REGISTRATION FAILED - Firmware Download Completion Callback\n"\
                         "Failure Code: %ld\n", rc );
        return;
    }

    /* set the crash state checking flag before calling UpgradeFirmware2k */
    SLQSSetCrashStateCheckIgnore(g_crashStateIgnore);

    /* Start downloading the firmware */
    rc = UpgradeFirmware2k( pImagePath );
    if( eQCWWAN_ERR_NONE != rc )
    {
        /* some firmware of MC7700 may reboot immediately after receiving
         * set firmware id command without sending out respond,
         * neglect this error to bypass issue*/
        if(eQCWWAN_ERR_NO_DEVICE !=rc )
        {
            fprintf( stderr, "NV Item Download Failed\n"\
                             "Failure Code: %ld\n", rc );
            return;
        }

        return;
    }

    fprintf( stderr, "\n\nDownloading NV Item");
    while( !fwdwlcomplete )
    {
        /* Display "." while NV Item downloads */
        fprintf( stderr, ".");
        sleep(2);
    }

    if (fwdwfail)
    {
        fprintf( stderr, "NV Item Download Failed\n");
        return;
    }

    /* Give the firmware time to apply any additional CWE files that are
     * not firmware boot and/or application images
     */
    fprintf(stderr, "Applying PRI updates - please wait 20 seconds...\n\n");
    sleep(20);

    prevdevicemode = devicemode;
    rc = SLQSGetDeviceMode ((BYTE *)&devicemode);

    /* If device was originally in boot mode, and is now in app mode,
     * re-connect the device to the SDK so that QMI services are properly set up.
     */
    if ( prevdevicemode == DEVICE_STATE_BOOT &&
         devicemode == DEVICE_STATE_READY)
    {
        /* Start the SDk */
        rc = StartSDK(modem_index);
        if( SUCCESS != rc )
        {
            free(sdkbinpath);

            /* Display the failure reason */
            fprintf( stderr, "Failed to start SDK: Exiting App\n"\
                             "Failure Code: %lu\n", rc );

            /* Failed to start SDK, exit the application */
            exit( EXIT_FAILURE );
         }
    }
    /* Display the information of the newly loaded image on the device */
    fprintf( stderr, "\n-- NV Item download complete --\n");
    fprintf( stderr, "\nExiting NV Item Downloader\n" );
}

/*
 * Name:     GetHostImageInfo
 *
 * Purpose:  Get the information about the image located on host at a specified
 *           path.
 *
 * Return:   None
 *
 * Notes:    none
 */
void GetHostImageInfo()
{
    struct  qmifwinfo_s spkg;
    CHAR    imagePath[MAX_IMAGE_PATH];
    long    rc;

    while(1)
    {
        memset(&spkg, 0, sizeof(spkg) );
        /* Receive the path of the image from the user */
        GetImagePath( imagePath );

        if( 0 == strnlen(imagePath, sizeof(imagePath)) )
        {
            /* empty string - return to main menu */
            return;
        }

        /* Get the information about the image located at specified path */
        rc = SLQSGetImageInfoMC77xx( imagePath, &spkg );

        if( eQCWWAN_ERR_NONE != rc )
        {
            fprintf( stderr,
                     "\nValid SPKG CWE file was not found. "\
                     "No fields are printed for non-SPKG CWE files.\n" );
        }

        /* Display the image information */
        spkgFwDisplay( &spkg );
    }
}

/*
 * Name:     GetNvuImageInfo
 *
 * Purpose:  Get the information about the nvu image located on host at a specified
 *           path.
 *
 * Return:   None
 *
 * Notes:    none
 */
void GetNvuImageInfo()
{
    struct  qmifwinfo_s spkg;
    CHAR    imagePath[MAX_IMAGE_PATH];
    long    rc;
    struct slqsfwinfo_s slqsImageInfo;
    BYTE   imgTypePRI = 3;

    while(1)
    {
        memset(&spkg, 0, sizeof(spkg) );
        memset(&slqsImageInfo, 0, sizeof(slqsImageInfo) );
        /* Receive the path of the image from the user */
        GetImagePath( imagePath );

        if( 0 == strnlen(imagePath, sizeof(imagePath)) )
        {
            /* empty string - return to main menu */
            return;
        }

        /* Get the information about the nvu image located at specified path */
        rc = SLQSGetImageInfo_9x15( imagePath, imgTypePRI, &slqsImageInfo );

        if( eQCWWAN_ERR_NONE != rc )
        {
            fprintf( stderr,
                     "\nValid NVU file was not found. "\
                     "No fields are printed for non-NVU files.\n" );
            memset(&spkg.dev.s, 0, sizeof(spkg.dev.s) );
        }
        spkg.dev.s = slqsImageInfo;
        /* Display the image information */
        spkgFwDisplay( &spkg );
    }
}

/*
 * Name:     SWIImageSwitching
 *
 * Purpose:  MDM9x30-based devices are capable of storing multiple
 *           carrier images on device.To switch image on device or from host.
 *
 *
 * Return:   None
 *
 * Notes:    none
 */
void SWIImageSwitching()
{
    ULONG  nRet = eQCWWAN_ERR_NONE;
    CHAR    imagePath[MAX_IMAGE_PATH];
    CHAR    newImagePath[MAX_IMAGE_PATH];
    CHAR    folderName[16];
    unsigned int  len;
    struct SWI_STRUCT_CarrierImage CarrierImages[20];
    ULONG  itemsCount = sizeof(CarrierImages)/sizeof(struct SWI_STRUCT_CarrierImage);
    ULONG  org_itemsCount;
    struct slqsfwinfo_s slqsImageInfo;
    struct qmifwinfo_s  spkg;

    while(1)
    {
        memset(&CarrierImages, 0, sizeof(CarrierImages));
        /* Reset the firmware download completion and device ready flags */
        fwdwlcomplete = FALSE;
        fwdwfail = FALSE;

        /* Receive the path of the image from the user */
        GetImagePath( imagePath );

        if( 0 == strnlen(imagePath, sizeof(imagePath)) )
        {
            /* empty string - return to main menu */
            return;
        }

        /* Get the information about the image loaded on the device */
        if ( DEVICE_STATE_READY == devicemode )
        {
            nRet = SLQSGetFirmwareInfo( &spkg );
        }

        if( eQCWWAN_ERR_NONE != nRet )
        {
            memset( &spkg.dev.s, 0, sizeof(spkg.dev.s) );
        }
        spkgFwDisplay(&spkg);

        /* Get the information about the all carrier images located at specified path on Host and device */
        org_itemsCount = itemsCount;
        nRet = SLQSSwiGetAllCarrierImages(&itemsCount, CarrierImages, imagePath );

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr,
                     "\nValid NVU file was not found. "\
                     "No fields are printed for non-NVU files.\n" );
            if (nRet == eQCWWAN_ERR_BUFFER_SZ)
            {
                fprintf( stderr,
                        "%d images slot provided, but needs %d\n", 
                        org_itemsCount, itemsCount);
            }
            return;
        }
        /* Display the image information */
        ImageInfoDisplay( &itemsCount, CarrierImages );

        fprintf( stderr, "\nPlease select from one of the above index or press <Enter> to return to main menu:\n");

        CHAR  selOption[OPTION_LEN];
        int iNumber = 0;
        while(1)
        {
            /* Receive the input from the user */

            fprintf(stderr, "Option: ");

            fgets( selOption, ( OPTION_LEN ), stdin );

            /* If '/n' character is not read, there are more characters in input
             * stream. Clear the input stream.
             */
            CHAR  *pEndOfLine = strchr( selOption, ENTER_KEY );
            if( NULL == pEndOfLine )
            {
                FlushStdinStream();
            }
            iNumber = atoi(selOption);
            if((ULONG)iNumber > itemsCount)
            {
                fprintf( stderr,"\nPlease Enter the correct option " );
                continue;
            }
            else if(iNumber < 1)
            {
                fprintf( stderr,"\nPlease Enter the correct option > 0" );
                continue;
            }
            else if(iNumber > 20)
            {
                fprintf( stderr,"\nPlease Enter the correct option < 20" );
                continue;
            }
            else
            {
                break;
            }
        }
        if (selOption[0] == ENTER_KEY)
            return;
        
        strcpy(newImagePath,imagePath);
        sprintf(folderName,"%d",CarrierImages[iNumber-1].m_nFolderId);
        strcat(newImagePath,folderName);
        strcat(newImagePath,"/");
        /*Warning to check if the image type is of SPK format*/
        if(eQCWWAN_ERR_NONE == SLQSGetImageInfo_9x15( newImagePath,
                                             (BYTE)IMG_FW_TYPE_SPK,
                                              &slqsImageInfo ))
        fprintf( stderr, "\nThis is SPK Image. Please note that 9x30 image switching method will not be applied for spk file, but it will be downloaded with default setting\n");

        fprintf( stderr, "\nDo you want to switch the firmware of your choice (Y/N):\n");
         /* Receive the input from the user */
         fprintf(stderr, "Option: ");

         CHAR  selOption1[OPTION_LEN];
         while(1)
         {
             fgets( selOption1, ( OPTION_LEN ), stdin );

             /* If '/n' character is not read, there are more characters in input
              * stream. Clear the input stream.
              */
             CHAR  *pEndOfLine1= strchr( selOption1, ENTER_KEY );
             if( NULL == pEndOfLine1 )
             {
                 FlushStdinStream();
             }
             len = strlen( selOption1 );

             if((selOption1[0] == 'Y' || selOption1[0] == 'y' || selOption1[0] == 'N' || selOption1[0] == 'n') && (len < 3 ))
             {
                 break;
             }
             else
             {
                 fprintf( stderr,"\nPlease Enter the correct option (y/n) " );
                 continue;
              }
         }
         switch( selOption1[0] )
         {
             case 'Y':
             case 'y':
                    if ( devicemode == DEVICE_STATE_DISCONNECTED )
                    {
                        fprintf( stderr, "This option is not supported while the "\
                                         "device is disconnected\n" );
                    }
                    else
                    {
                        nRet = SetFwDldCompletionCbk( FirmwareDwldCbk );
                        if( SUCCESS != nRet )
                        {
                            fprintf( stderr, "REGISTRATION FAILED - Firmware Download Completion Callback\n"\
                                             "Failure Code: %d\n", nRet );
                            return ;
                        }
                        /* Start downloading the firmware */
                        nRet = UpgradeFirmware2k( newImagePath );
                        if( eQCWWAN_ERR_NONE != nRet )
                        {
                            /* some firmware of MC7700 may reboot immediately after receiving
                             * set firmware id command without sending out respond,
                             * neglect this error to bypass issue*/
                            if(eQCWWAN_ERR_NO_DEVICE !=nRet )
                            {
                                fprintf( stderr, "Firmware Download Failed\n"\
                                                 "Failure Code: %d\n", nRet );
                                return ;
                            }

                            return ;
                         }
                         fprintf( stderr, "\n\nDownloading Firmware");
                         while( !fwdwlcomplete )
                         {
                             /* Display "." while firmware downloads */
                             fprintf( stderr, ".");
                             sleep(2);
                         }
                         if (fwdwfail)
                         {
                             fprintf( stderr, "Firmware Download Failed\n");
                             return;
                         }

                         /* Give the firmware time to apply any additional CWE files that are
                          * not firmware boot and/or application images
                          */
                        fprintf(stderr, "Applying Firmware/PRI updates - please wait 20 seconds...\n\n");
                        sleep(20);
                    }
                    break;
             case 'N':
             case 'n':
                  break;
             default:
                  break;
         }
    }

}

/*
 * Name:     IsValidImageFileExist
 *
 * Purpose:  check whether the provided path is valid or not
 *
 * Return:   True if two images are the same, False otherwise.
 *
 * Notes:    none
 */
BOOL IsValidImageFileExist(CHAR *pImgPath, BOOL ForceDld )
{
    struct  qmifwinfo_s spkg;
    long    rc;
    BOOL    validfile = FALSE;

    /* Receive the path of the image from the user */
    GetImagePath( pImgPath );

    if( 0 == strlen(pImgPath) )
    {
        /* empty string - return to main menu */
        return validfile;
    }

    /* Get the information about the image located at specified path */
    rc = SLQSGetImageInfoMC77xx( pImgPath, &spkg );

    if( eQCWWAN_ERR_NONE != rc )
    {
        fprintf( stderr,
                 "\nValid SPKG CWE file was not found. "\
                 "No fields are printed for non-SPKG CWE files.\n" );
        memset(&spkg.dev.s, 0, sizeof(spkg.dev.s) );
        if( ForceDld == TRUE )
            return TRUE;
        else
            return FALSE;
    }
    validfile = TRUE;

    return validfile;

}

/* the firmware doesn't support QMI object QMI_DMS_GET_CWE_SPKGS_INFO (0x5556),
   so skip the version comparison */
#if 0
/*
 * Name:     IsSameFwImage
 *
 * Purpose:  check whether the firmware image to be downloaded is same as the firmware image
 *                loaded on the device.
 *
 * Return:   True if two images are the same, False otherwise.
 *
 * Notes:    none
 */
BOOL IsSameFwImage(CHAR *pImagePath)
{
    struct  qmifwinfo_s spkg;
    struct  qmifwinfo_s devspkg;
    long    rc;
    BOOL    sameimage = FALSE;
    CHAR    str[15];

    /* Get the information about the image located at specified path */
    rc = SLQSGetImageInfoMC77xx( pImagePath, &spkg );

    if( eQCWWAN_ERR_NONE != rc )
    {
        fprintf( stderr,
                 "\nValid SPKG CWE file was not found. "\
                 "No fields are printed for non-SPKG CWE files.\n" );
        memset(&spkg.dev.s, 0, sizeof(spkg.dev.s) );
    }

    /* Get the information about the image loaded on the device */
    rc = SLQSGetFirmwareInfo( &devspkg );
    if( eQCWWAN_ERR_NONE != rc )
    {
        /* no AMSS SPKG support */
        memset( &devspkg.dev.s, 0, sizeof(devspkg.dev.s) );
    }

    /* extract the version number of the string */
    memcpy(str, &devspkg.dev.s.appversion_str[9], 11);
    str[11] = '\0';

    /* check if the firmware image to be donwloaded is same as the one loaded on the device */
    if (0 == strcmp(spkg.dev.s.appversion_str, str))
    {
        sameimage = TRUE;
    }

    return sameimage;

}
#endif

/*
 * Name:     PrintUsage
 *
 * Purpose:  prints the right usage options for using this application.
 *
 * Return:   none
 *
 * Notes:    none
 */
void PrintUsage()
{
    printf( "\r\n" );
    printf( "App usage: \r\n\r\n" );
    printf( "  <appName> -s <sdk_path> -d <modem_index> -f -h \n\n" );
    printf( "  -s  --sdkpath \n ");
    printf( "        Specifies the path to the slqssdk. This is a mandatory parameter.\n\n");
    printf( "  -d  --modem \n ");
    printf( "        Specifies the modem index. This is a optional parameter.\n\n");
    printf( "        Default to zero if not specified, that is first modem\n\n");
    printf( "  -f  --force \n" );
    printf( "        Force download the firmware image. This option is optional. \n" );
    printf( "        Using this parameter will skip the cwe header check on the image file.\n\n" );
    printf( "  -i  --ignore crash state checking or not.Default value is 0 means crash state checking is required\n" );
    printf( "          - 0: crash state checking required (default value)\n" );
    printf( "          - 1: ignore crash state checking\n" );
    printf( "  -h  --help  \n" );
    printf( "        This option prints the usage instructions.\n\n" );
}

/*
 * Name:     parseSwitches
 *
 * Purpose:  Parse the various command line switches .
 *
 * Return:   none
 *
 * Notes:    none
 */
void parseSwitches(
    int                  argc,
    char                 **argv )
{
    int next_option;

    /* Parse the command line before doing anything else */
    do
    {
        /* Read the next option until there are no more */
        next_option = getopt_long( argc, argv,
                                   short_options,
                                   long_options, NULL );

        switch( next_option )
        {
            case 's':
                /* Get the SDK path */
                sdkbinpath = optarg;
                sdkPathSet = TRUE;
                break;

            case 'h':
                /* Print usage information */
                PrintUsage();
                exit( EXIT_SUCCESS );
                break;

            case 'f':
                /* caller specifies a device type*/
                forceDld = TRUE;
                break;

            case 'd':
                g_modem_index = atoi(optarg);
                break;

            case 'i':
                g_crashStateIgnore = (BOOL) atoi(optarg);
                break;

            case '?':
                /* Caller has used an invalid option */
                printf("\nInvalid option\n" );

                /* Print the usage info and exit */
                PrintUsage();
                exit(EXIT_FAILURE);
                break;

            case -1:
                /* Done with options list */
                break;

            default:
                exit(EXIT_FAILURE);
                break;
        }
    }
    while( next_option != -1 );
}

void Get_Stored_Images(void)
{
    struct ImageList       *pimageList = &imageList;
    ULONG                 ulRet, imageListsize=sizeof(imageList);
    ULONG  idx1=0,idx2=0,idx3=0;
    fprintf(stderr, "ImageList size        : %zu\n", sizeof(struct ImageList));
    fprintf(stderr, "ImageList Size Before : %d\n",imageListsize);
    ulRet =  GetStoredImages( &imageListsize,(BYTE *)pimageList );	
    fprintf (stderr, "GetImagesPreference return : %u\n", ulRet);
    if (!ulRet)
    {	

        BYTE imageIdEntries   = 0;
        fprintf(stderr, "Bytes Copied     : %d\n",imageListsize);
        fprintf(stderr, "Numer of elements: %d\n", pimageList->listSize);
        for( idx1 = 0; idx1 < pimageList->listSize; idx1++ )
        {
            fprintf(stderr, "List Entry %d:\n", idx1 + 1 );
            fprintf(stderr, "  Image Type(0-FW/1-PRI): %d\n",
                    pimageList->imageIDEntries[idx1].imageType );
            fprintf(stderr, "  Max Images: %d\n",
                    pimageList->imageIDEntries[idx1].maxImages );
            fprintf(stderr, "  Executing Image Index: %x\n",
                    pimageList->imageIDEntries[idx1].executingImage );
            fprintf(stderr, "  No. of Elements in Image ID List: %x\n",
                    pimageList->imageIDEntries[idx1].imageIDSize );
            imageIdEntries =
                pimageList->imageIDEntries[idx1].imageIDSize ;

            fprintf(stderr, "\tImage ID List Elements:\n" );
            for ( idx2 = 0; idx2 < imageIdEntries; idx2++ )
            {
                struct ImageIdElement *pImageIdElement = NULL;
                pImageIdElement = ( struct ImageIdElement *)
                    &pimageList->imageIDEntries[idx1].
                    imageIDElement[idx2];
                fprintf(stderr, "\tImage ID List Element No.: %d\n",
                        idx2 + 1 );
                fprintf(stderr, "\tStorage Index: 0x%x\n",
                        pImageIdElement->storageIndex );
                fprintf(stderr, "\tfailure_count: 0x%x\n",
                        pImageIdElement->failureCount );
                fprintf(stderr, "\tImage ID: " );
                for ( idx3 = 0; idx3 < 16; idx3++ )
                {
                    fprintf(stderr, "%c",
                            pImageIdElement->imageID[idx3] );
                }
                fprintf(stderr, "\n");
                fprintf(stderr, "\tBuild ID Length: %d\n",
                        pImageIdElement->buildIDLength );
                if( pImageIdElement->buildIDLength)
                {
                    fprintf(stderr, "\tBuild ID:%s\n",
                            pImageIdElement->buildID );
                }
                fprintf(stderr, "\n");
            }
            fprintf(stderr, "\n");
        }		
    }
}

void Get_Valid_Combinations(void)
{
    struct ImageList       *pimageList = &imageList;
    ULONG idx = 0, ulRet = 0;
    memset (ValidCombinations,0,sizeof(ValidCombinations));
    ValidCombinationSize = 50;
    ulRet = SLQSGetValidFwPriCombinations (pimageList, &ValidCombinationSize, ValidCombinations);
    if (!ulRet)
    {
        fprintf (stderr, "Number of valid combibations: %d\n", ValidCombinationSize);
        fprintf (stderr, "Combination details\n");
        for( idx = 0; idx < ValidCombinationSize; idx++ )
        {
            fprintf (stderr, "Combination Index:: %d\n", idx);
            fprintf (stderr, "  Carrier ID  : %u\n", ValidCombinations[idx].m_nCarrierId);
            fprintf (stderr, "  Folder ID   : 0x%X\n", ValidCombinations[idx].m_nFolderId);
            fprintf (stderr, "  Storage ID  : %u\n", ValidCombinations[idx].m_nStorage);
            fprintf (stderr, "  PRI Build ID: %s\n", (char *)ValidCombinations[idx].m_PriBuildId);
            fprintf (stderr, "  FW Build ID : %s\n", (char *)ValidCombinations[idx].m_FwBuildId);		
            fprintf (stderr, "  PRI Image ID: %s\n", (char *)ValidCombinations[idx].m_PriImageId);
            fprintf (stderr, "  FW Image ID : %s\n", (char *)ValidCombinations[idx].m_FwImageId);		
        }		
    }
    else
        fprintf (stderr, "SLQSGetValidFwPriCombinations return: %d\n", ulRet);
    return;
}

/*
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Return:   EXIT_SUCCESS, EXIT_FAILURE on unexpected error
 *
 * Notes:    none
 */
int main( int argc, char *argv[])
{
    CHAR    imagePath[MAX_IMAGE_PATH];

    if ( SLQSGetPidof("modem-manager") > 0 ) //its running
    {
        fprintf( stderr,
            "firmware update don't work with modem-manager\n"
            "On debian, you can uninstall & kill the running process by:\n"
            "  sudo apt-get remove modemmanager\n"
            "  sudo killall modem-manager\n"
            );
        exit( EXIT_FAILURE );
    }

    /* Parse the command line switches  */
    parseSwitches( argc, argv );

    if( sdkPathSet == FALSE )
    {
        fprintf( stderr, "\nError: Mandatory SDK path not set \n" );
        /* Print usage information */
        PrintUsage();
        exit( EXIT_FAILURE );
    }

    /* Start the SDK */
    StartSDK(g_modem_index);
    fprintf( stderr, "\nRunning with device in %s mode\n",
            DEVICE_STATE_DISCONNECTED == devicemode
             ?  "disconnected"
             :  DEVICE_STATE_READY == devicemode
                ? "application"
                : "boot and hold" );

    while(1)
    {
        fprintf( stderr, "\nPlease select one of the following options or press <Enter> to exit:\n");
        /* Print the menu */
        switch(devicemode)
        {
            case DEVICE_STATE_READY:
                fprintf( stderr,
                         "1. Display the information for the executing device image\n" );
                /* fall through */
            case DEVICE_STATE_BOOT:
                fprintf( stderr,
                         "2. Download a boot loader image to the device\n" );
                fprintf( stderr,
                         "3. Download a firmware image to the device\n" );
                fprintf( stderr,
                         "4. Download an NV item to the device\n" );
                /* fall through */
            case DEVICE_STATE_DISCONNECTED:
                fprintf( stderr,
                         "5. Display the information for a particular spk/cwe image located on the host\n" );
                fprintf( stderr,
                         "6. Display the information for a particular nvu image located on the host\n" );
                fprintf( stderr,
                         "7. Image Switching on MC/EM74xx\n" );
                fprintf( stderr,
                         "8. Get valid fw/pri combination for MC/EM74xx\n" );
                break;

            default:
                break;
        };

        /* Receive the input from the user */
        fprintf(stderr, "Option: ");
        CHAR  selOption[OPTION_LEN];
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        CHAR  *pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

#ifdef MC7xxxDBG
    fprintf( stderr, "Selected Option: %s\n", selOption );
#endif

        /* If user has entered an invalid input, prompt again */
        if( 2 < strlen(selOption) )
        {
           continue;
        }

        /* Process user input */
        switch( selOption[0] ){
            case eDEV_IMAGE_INFO:
                if ( DEVICE_STATE_READY != devicemode )
                {
                    fprintf( stderr, "This option is only supported while the "\
                                     "device is in application mode.\n" );
                }
                else
                {
                    GetDeviceImageInfo();
                }
                break;

            case eDWL_BOOT_IMG:
                if ( devicemode == DEVICE_STATE_DISCONNECTED )
                {
                    fprintf( stderr, "This option is not supported while the "\
                                     "device is disconnected\n" );
                }
                else
                {
                    GetImagePath( imagePath );
                    if( 0 == strnlen(imagePath, sizeof(imagePath)) )
                    {
                        /* empty string - return to main menu */
                        break;
                    }
                    /* download firmware directly as some of the boot loader may not able to provide the
                     * version number as expected
                     */
                    FirmwareDownloader(imagePath, g_modem_index);
                    /* set bootupdated to TRUE so that a firmware image can be downloaded
                     * without checking the firmware version
                     */
                    bootupdated = TRUE;
                }
                break;

            case eDWL_FW_IMG:
                if ( devicemode == DEVICE_STATE_DISCONNECTED )
                {
                    fprintf( stderr, "This option is not supported while the "\
                                     "device is disconnected\n" );
                }
                else
                {
                    /* when boot loader was updated, it means that the firmware image is going to be downloaded
                     * is different with the one loaded on the device previously, so skip the version checking
                    */
                    if (bootupdated)
                    {
                         GetImagePath( imagePath );
                         if( 0 == strnlen(imagePath, sizeof(imagePath)) )
                         {
                             /* empty string - return to main menu */
                             break;
                         }
                         /* download firmware directly as some of the boot loader may not able to provide the
                          * version number as expected
                          */
                         FirmwareDownloader(imagePath, g_modem_index);
                    }
                    else
                    {
                        if (IsValidImageFileExist(imagePath, forceDld))
                        {
                            FirmwareDownloader(imagePath, g_modem_index);
                        }
                        else
                        {
                            fprintf( stderr, "Invalid path or file!\n");
                        }
                    }
                }
                break;

            case eDWL_NV_ITEM:
                GetImagePath( imagePath );
                if( 0 == strnlen(imagePath, sizeof(imagePath)) )
                {
                    /* empty string - return to main menu */
                    break;
                }
                /* download NV Item */
                NVItemDownloader(imagePath, g_modem_index);
                break;

            case eHOST_IMAGE_INFO:
                GetHostImageInfo();
                break;

            case eHOST_NVU_INFO:
                GetNvuImageInfo();
                break;

            case eSWI_IMG_SWITCH:
                 SWIImageSwitching();
                 break;

            case eGET_COMBO:
				Get_Stored_Images();
				Get_Valid_Combinations();
                break;

            case eEXIT_APP:
                fprintf( stderr, "Exiting Application!!!\n");
                QCWWANDisconnect();
                exit(EXIT_SUCCESS);
                break;

            default:
                break;
        }
    }
}
