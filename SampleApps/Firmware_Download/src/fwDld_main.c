/*************
 *
 * Filename:    fwDld_main.c
 *
 * Purpose:     Command line based firmware download application
 *
 * Copyright: © 2013 Sierra Wireless Inc., all rights reserved
 *
 **************/
#define _GNU_SOURCE

#include "fwDld_main.h"
#include "fwDld_9x00.h"
#include "fwDld_G3K.h"
#include "fwDld_9x15.h"
#include "fwDld_9x07.h"
#include <syslog.h>
/****************************************************************
*                       DEFINES
****************************************************************/
#define DEV_NODE_SZ        256
#define DEV_KEY_SZ         16
#define SN_LEN             64

/****************************************************************
*                       DATA STRUCTURE
****************************************************************/
//#define FWDLDDBG
/* Device information structure */
typedef struct device_info_param{
  CHAR deviceNode[DEV_NODE_SZ];
  CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/

/* path to sdk binary */
static char sdkbinpath[512] = {'\0'};

/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];
static BOOL bootupdated = FALSE;
static BYTE slot = 0;
BOOL downloadmode = FALSE;
BOOL g_crashStateIgnore = FALSE;


/* Firmware download variables */
static char* imgPath   = NULL;
static char* devType   = NULL;
static BYTE  g_modem_index = 0;
int g_kill_sdk = 0;
static char  g_sn[SN_LEN+1] = {0};
static char* imgFilename   = NULL;


static BYTE devmode = DEVICE_STATE_DISCONNECTED;

/* Command line options to firmware download tool */
const char * const short_options = "s:hd:f:i:p:m:u:l:k";

/* Command line long options for firmware download tool */
const struct option long_options[] = {
    {"sdkpath",  1, NULL, 's'},      /* SDK path */
    {"help",     0, NULL, 'h'},      /* Provides terse help to users */
    {"device",   1, NULL, 'd'},      /* Running device */
    {"path",     1, NULL, 'p'},      /* Specify the pathname */
    {"modem",    1, NULL, 'm'},      /* modem index */
    {"sn",       1, NULL, 'u'},      /* serial number or usb path */
    {"kill"   ,  0, NULL, 'k'},
    {"filename", 1, NULL, 'f'},      /* filename*/
    {"crashStateChecking",   0, NULL, 'i'},      /* crash state checking */
    {"slotid",  1, NULL, 'l'},      /* slot id of modem */
    {NULL,       0, NULL,  0 }       /* End of list */
};

static struct fwDld_options userOptData;

/* macros*/


/****************************************************************
*                       FUNCTIONS
****************************************************************/

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
ULONG StartSDK(BYTE modem_index, char *sn)
{
    ULONG rc = 0;
    BYTE  devicesSize = 1;
    char * szSN = NULL;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE != (rc = SetSDKImagePath(sdkbinpath)) )
    {
        return rc;
    }

    /* Handle FSN number */
    if( (sn != NULL) && (sn[0] != '\0') )
    {
        szSN = sn;
    }

    /* Establish APP<->SDK IPC */
    if( eQCWWAN_ERR_NONE != (rc = SLQSStart(modem_index,szSN)) )
    {
        if( eQCWWAN_ERR_NONE != SLQSKillSDKProcess() )
        {
            return rc;
        }
        else
        {
            if( eQCWWAN_ERR_NONE != (rc = SLQSStart(modem_index,szSN)) )
            {
                return rc;
            }
        }
    }

    rc = SLQSGetDeviceMode ((BYTE *)&devmode);

    if ( devmode == DEVICE_STATE_BOOT)
    {
        downloadmode = TRUE;
    }

    /* Can enumerate and connect only if device is in Application mode */
    if ( devmode == DEVICE_STATE_READY )
    {
        /* Enumerate the device */
        while( QCWWAN2kEnumerateDevices( &devicesSize, (BYTE *)(pdev) ) != 0 )
        {
            printf ("\nUnable to find device..\n");
            sleep(1);
        }

#ifdef FWDLDDBG
    fprintf( stdout,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
             devicesSize,
             pdev->deviceNode,
             pdev->deviceKey );
#endif
        bootupdated = FALSE;
        /* Connect to the SDK */
        rc = QCWWANConnect( pdev->deviceNode,
                            pdev->deviceKey );
    }
    else if ( devmode == DEVICE_STATE_BOOT )
    {
        bootupdated = TRUE;
    }
    else if ( devmode == DEVICE_STATE_DISCONNECTED )
    {
        rc = eQCWWAN_ERR_NO_DEVICE;
    }

    return rc;
}

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
    printf( "  <appName> -s <sdk_path> -d [9x00/9x15/g3k] -p [pathname] -m [modem_index] -h \n\n" );
    printf( "  -s  --sdkpath \n ");
    printf( "        Specifies the path to the slqssdk. This is a mandatory parameter.\n\n");
    printf( "  -m  --modem \n ");
    printf( "        Specifies the modem index. This is a optional parameter.\n\n");
    printf( "        Default to zero if not specified, that is first modem\n\n");
    printf( "  -u  --serial number or usb path \n ");
    printf( "        Specifies the usb path. This is a optional parameter.\n\n");
    printf( "  -d  --device [9x00/9x15/9x30/g3k] \n" );
    printf( "        Specifies the device type. Must be lower cases. This option is optional.\n" );
    printf( "        If this option is omitted, 9x15 is the default setting.\n" );
    printf( "          - 9x00: This is a 9x00 based device.\n" );
    printf( "          - 9x15: This is a 9x15 based device.\n" );
    printf( "          - 9x30: This is a 9x30 based device.\n" );
    printf( "          - g3k:  This is a Gobi device.\n" );
    printf( "          - 9x07: This is a 9x07 based device.\n\n" );
    printf( "  -p  --path [folder to firmware images]\n" );
    printf( "        This specifies the folder location of the firmware images. This option is mandatory. \n" );
    printf( "        Usage of this parameter depends on type of device.\n" );
    printf( "          - 9x00: Specify the path containing the image.\n" );
    printf( "          - 9x15: Specify the path containing a combined image or separate images.\n" );
    printf( "          - 9x30: Specify the path containing a combined image or separate images.\n" );
    printf( "          - g3k: Should be in format <file path>/x , x should be a number,\n" );
    printf( "                 and this folder should contain both AMSS and UQCN.\n" );
    printf( "          - 9x07: Specify the path containing the image.\n" );
    printf( "  -k  --kill kill SDK process on exits\n" );
    printf( "  -f  --File path\n" );
    printf( "  -i  --ignore crash state checking or not.Default value is 0 means crash state checking is required\n" );
    printf( "          - 0: crash state checking required (default value)\n" );
    printf( "          - 1: ignore crash state checking\n" );
    printf( "  -l  --slot index of modem, this option is only available for EM74xx/MC74xx\n" );
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
    char                 **argv,
    struct fwDld_options *userData)
{
    char cwd[512] = {0};
    char tmppath[512] = {0};
    char *pch;

    UNUSEDPARAM( userData );

    int next_option;

    /* construct the default path of slqssdk in case it is not specified */
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
       /* check if current directory is the location of fwdldhost */
       if ( strstr(argv[0], "./fwdld") == argv[0])
       {
           /* if yes, just concatenate the /slqssdk to cwd directly */
           strcat(cwd, "/slqssdk");
       }
       else
       {
           /* GDB case, check the path of argv[0], when running in gdb mode,
            * argv[0] will provide the full path */
           if (argv[0][0] == '/')
           {
               strncpy(cwd, argv[0],sizeof(cwd)-1);
           }
           else
           {
               /* Non GDB case */
               strncpy(tmppath, argv[0],sizeof(tmppath)-1);
               /* skip the first character '.' of argv[0] while making the string copy */
               strcat(cwd, tmppath+1);
           }
           /* locate the last '/', the string after '/' is the name of the
            * fwdld executable which should be skipped in this case */
           pch = strrchr(cwd, '/');
           if ( pch != NULL)
           {
               /* null terminated to filter "/fwdld"*/
               cwd[pch-cwd] = '\0';
               /* concatenate the /slqssdk to cwd */
               strcat(cwd,"/slqssdk");
           }
       }
       strcpy(sdkbinpath, cwd);
    }
    else
    {
       fprintf(stderr, "getcwd() error\n");
    }

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
                strcpy(sdkbinpath, optarg);
                break;

            case 'h':
                /* Print usage information */
                PrintUsage();
                quit_app(eFWDWL_SUCCESS);
                break;

            case 'd':
                /* caller specifies a device type*/
                devType = optarg;
                break;

            case 'p':
                /* caller specifies a path to the SDK executable */
                imgPath = optarg;
                break;

            case '?':
                /* Caller has used an invalid option */
                printf("\nInvalid option\n" );

                /* Print the usage info and exit */
                PrintUsage();
                quit_app(eFWDWL_SUCCESS);
                break;

            case 'm':
                /* caller specifies a modem index*/
                g_modem_index = atoi(optarg);
                break;
            case 'u':
                /* caller specifies a specific device */
                strncpy(g_sn, optarg, SN_LEN);
                g_sn[SN_LEN] = '\0';
                break;

            case 'k':
                /* caller specifies a modem index*/
                g_kill_sdk = 1;
                break;

            case 'f':
                /* File Path*/
                imgFilename = optarg;
                break;

            case 'i':
                /* caller specifies crash state checking */
                g_crashStateIgnore = (BOOL)atoi(optarg);
                break;

            case 'l':
                /* slot id */
                slot = atoi(optarg);
                break;

            case -1:
                /* Done with options list */
                break;

            default:
                quit_app(eFWDWL_SUCCESS);
                break;
        }
    }
    while( next_option != -1 );
}

/*
 * Name:     InitParams
 *
 * Purpose:  Initialize the parameters after validation.
 *
 * Return:   none
 *
 * Notes:    none
 */
void InitParams()
{
    ULONG len;

    /* First check for Device type  */
    if( NULL != devType )
    {
        len = strlen(devType);

        /* Check if device length is too long */
        if ( len > MAX_DEV_LEN )
        {
            fprintf( stderr, "\nError: Invalid Device Type\n" );
            quit_app(eFWDWL_ERR_INVALID_DEV);
        }

        /* Initialize device type */
        if( 0 == strcmp( devType,"9x00" ) )
        {
            userOptData.devType = (ULONG) eDEV_MDM9x00;
        }
        else if( 0 == strcmp( devType, "9x15" ) )
        {
            userOptData.devType = (ULONG) eDEV_MDM9x15;
        }
        else if( 0 == strcmp(devType,"9x30") )
        {
            userOptData.devType = (ULONG) eDEV_MDM9x30;
        }
        else if( 0 == strcmp( devType, "g3k" ) )
        {
            userOptData.devType = (ULONG) eDEV_GOBI3K;
        }
        else if( 0 == strcmp(devType,"9x07") )
        {
            userOptData.devType = (ULONG) eDEV_MDM9x07;
        }
        else
        {
            /* Invalid device type specified*/
            fprintf( stderr, "\nError: Invalid Device Type\n" );
            quit_app(eFWDWL_ERR_INVALID_DEV);
        }
    }
    else
    {
        userOptData.devType = (ULONG) eDEV_MDM9x15;
    }

    /* Next check for path  */
    if( NULL != imgPath )
    {
        len = strlen(imgPath);

        /* Check if path length is too long */
        if ( len > MAX_PATH_LEN )
        {
            fprintf( stderr, "\nError: Length of Path too long (> 255) \n" );
            quit_app(eFWDWL_ERR_PATH_TOO_LONG);
        }

        /* Copy Image Path */
        strncpy(userOptData.path, imgPath, len);
    }
    else
    {
        if( NULL != imgFilename)
        {
            len = strlen(imgFilename);
            /* Check if path length is too long */
            if ( len > MAX_PATH_LEN-2 )
            {
                fprintf( stderr, "\nError: Length of Path and Filename too long (> 255) \n" );
                quit_app(eFWDWL_ERR_PATH_TOO_LONG);
            }
            /* Copy Image Path */
            strncpy(userOptData.path, imgFilename, len);
            userOptData.path[len] = '\0';
        }else
        {
            /* File path is mandatory */
            fprintf( stderr, "\nError: Path is a mandatory parameter \n" );
            quit_app(eFWDWL_ERR_PATH_NOT_SPECIFIED);
        }
    }
    /* TBD: Add validation of physical path and files */
}

/*
 * Name:     ExecDownload
 *
 * Purpose:  Register call back and invoke download.
 *
 * Return:   none
 *
 * Notes:    none
 */
void ExecDownload()
{
    BYTE  prevdevicemode;
    ULONG rc;

    switch( userOptData.devType )
    {
        case eDEV_MDM9x00:
            FwDloader_9x00( userOptData.path );
            break;
        case eDEV_MDM9x15:
        case eDEV_MDM9x30:
            FwDloader_9x15( userOptData.path, slot );
            break;
        case eDEV_GOBI3K:
            FwDloader_G3K( userOptData.path );
            break;
        case eDEV_MDM9x07:
            FwDloader_9x07( userOptData.path);
        default:
            /* We should never have reached here */
            quit_app(eFWDWL_ERR_INVALID_DEV);
            break;
    }
    prevdevicemode = devmode;
    rc = SLQSGetDeviceMode ((BYTE *)&devmode);

    if(eQCWWAN_ERR_NONE != rc)
    {
        fprintf( stderr, "ERROR: SLQSGetDeviceMode Failure Code: %u\n", rc );
        syslog (LOG_DEBUG, "%s: ERROR: SLQSGetDeviceMode Failure Code: %u", __func__, rc);
        quit_app(eFWDWL_ERR_SDK);
        return ;
    }

    /* If device was originally in boot mode, and is now in app mode,
     * re-connect the device to the SDK so that QMI services are properly set up.
     */
    if ( prevdevicemode == DEVICE_STATE_BOOT &&
         devmode == DEVICE_STATE_READY)
    {
        /* Start the SDk */
        rc = StartSDK(g_modem_index,g_sn);
        if( eQCWWAN_ERR_NONE != rc )
        {
            /* Display the failure reason */
            fprintf( stderr, "ERROR: Failed to start SDK: Exiting App\n"\
                             "Failure Code: %u\n", rc );

            /* Failed to start SDK, exit the application */
            syslog (LOG_DEBUG, "%s: ERROR: Failed to start SDK: %u", __func__, rc);
            quit_app( eFWDWL_ERR_SDK_NOT_STARTED );
         }
    }
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
    ULONG rc;

    /* Initialize the data structure before parsing switches */
    userOptData.devType = (ULONG)eDEV_INVALID;
    memset( (void*)&userOptData.path, 0, MAX_PATH_LEN);
    devmode = DEVICE_STATE_DISCONNECTED;

    /* Parse the command line switches  */
    parseSwitches( argc, argv, &userOptData );

    fprintf(stdout, "Detecting USB of the target\n");
    /* Start the SDK */
    rc = StartSDK(g_modem_index,g_sn);
    if ( rc == eQCWWAN_ERR_NONE)
    {
        fprintf(stdout, "DONE\n");
    }
    else
    {
        fprintf(stdout, "No device connected\n");
        quit_app(eFWDWL_ERR_INVALID_DEV);
    }

    /* Validate the parameters and formalize them for usage */
    InitParams();

    /* Execute firmware Download */
    ExecDownload();

    /* Finished, now exit */
    QCWWANDisconnect();
    quit_app(eFWDWL_SUCCESS);

    return 0; //not reached, just for compliation
}

int quit_app(int err)
{
    syslog (LOG_DEBUG, "%s: Exit Code: %d", __func__, err);
    if (g_kill_sdk)
    {
        /* if no qcqmi interface, QMI mode API SLQSKillSDKProcess() cannot be used  */
        if (devmode == DEVICE_STATE_DISCONNECTED ||
            devmode == DEVICE_STATE_SERIAL_ONLY )
        {
            system("killall -9 slqssdk");
        }
        else
        {
            SLQSKillSDKProcess();
        }
    }
    exit(err);
}
