/*************
 *
 * Filename:    SWIOMASampleApp.c
 *
 * Purpose:     SWIOMA Sample Application
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include "SWIOMASampleApp.h"

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/
/* path to sdk binary */
static char *sdkbinpath = NULL;
BYTE FwStatusNotify = 0xFF;

/* Device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

/****************************************************************************
*                       Functions
*****************************************************************************/
void printMenu( bool print_status )
{
    if (print_status)
    {
        fprintf( stderr, "\nPlease select one of the following options or press"\
                         "<Enter> to exit:\n"\
                         "1.  Start Session\n"\
                         "2.  Cancel Session\n"\
                         "3.  Get Session Info\n"\
                         "4.  Get OMADM Setting\n"\
                         "5.  Set OMADM Setting\n"\
                         "_____________________________________________________"
                         "______________\n" );

        fprintf( stderr, "Option:" );
    }
}

ULONG StartSDK()
{
    ULONG resultCode = 0;
    BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE !=
         (resultCode = SetSDKImagePath(sdkbinpath)) )
    {
        fprintf(stderr, "Failed to set the SDK Image path (%x)\n",\
                        resultCode);
        return resultCode;
    }

    if( eQCWWAN_ERR_SWICM_SOCKET_IN_USE == (resultCode = SLQSStart(0, NULL)) )
    {
           fprintf(stderr, "Another APP is currently using the SDK (%x)\n",
                      resultCode);
           return resultCode;
    }
    else if( eQCWWAN_ERR_NONE != resultCode )
    {
         return resultCode;
    }

    /* Enumerate the device */
    while (QCWWAN2kEnumerateDevices(&devicesSize, (BYTE *)pdev) != 0)
    {
        printf ("\nUnable to find device..\n");
        sleep(1);
    }

    #ifdef DBG
        fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
                 devicesSize,
                 pdev->deviceNode,
                 pdev->deviceKey );
    #endif

    /* Connect to the SDK */
    resultCode = QCWWANConnect( pdev->deviceNode,
                                pdev->deviceKey );
    return resultCode;
}

void FlushStdinStream( )
{
    int inputChar;

    /* keep on reading until a <New Line> or end of file is received */
    do
    {
        inputChar = getchar();

        #ifdef DBG
        fprintf( stderr,  "inputChar: 0x%x\n", inputChar );
        #endif
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != EOF ) );
}

void cbkSetSLQSOMADMAlertCallback(
    ULONG sessionType,
    BYTE* psessionTypeFields )
{
    struct omaDmFotaTlv *sessionInfoFota;
    static BYTE downloading = 0, updating = 0;
    if ( sessionType == SESSION_TYPE_FOTA )
    {
        if ( psessionTypeFields )
        {
            sessionInfoFota = (struct omaDmFotaTlv *)psessionTypeFields;

            /* Update notification variable */
            FwStatusNotify = sessionInfoFota->state;

            switch( FwStatusNotify )
            {
                case eNO_FIRMWARE_AVAILABLE:
                    fprintf ( stderr, "\n\nNo Firmware Available!!!\n" );
                    printMenu(true);
                    break;

                case eQUERY_FIRMWARE_DOWNLOAD:
                    fprintf ( stderr, "\nPlease select following option or press"\
                                      "<Enter> to exit:\n"\
                                      "6. Proceed to Download firmware \n"\
                                      "___________________________________________"
                                      "______________\n" );
                                      fprintf( stderr, "Option:" );
                    break;

                case eFIRMWARE_DOWNLOADING:
                    downloading++;
                    if (downloading == 1)
                    {
                        fprintf( stderr, "\n\nDownloading Firmware" );
                    }
                    else
                    {
                        fprintf( stderr, "... Please wait!!" );
                    }
                    break;

                case eFIRMWARE_DOWNLOADED:
                    fprintf ( stderr, "\nFirmware Downloaded\n" );
                    downloading = 0;
                    break;

                case eQUERY_FIRMWARE_UPDATE:
                    fprintf ( stderr, "\nPlease select following option or press"\
                                      "<Enter> to exit:\n"\
                                      "7. Proceed to Update Firmware \n"\
                                      "____________________________________________"
                                      "______________\n" );
                                      fprintf( stderr, "Option:" );
                    break;

                case eFIRMWARE_UPDATING:
                    updating++;
                    if (updating == 1)
                    {
                        fprintf( stderr, "\n\nUpdating Firmware" );
                    }
                    fprintf( stderr, " ... Plesae wait!!" );
                    break;

                case eFIRMWARE_UPDATED:
                    updating = 0;
                    fprintf ( stderr, "\nFirmware Updated\n" );
                    printMenu(true);
                    break;

                default:
                    fprintf ( stderr, "\n\nInvalid Response from Callback!! \n" );
                    FwStatusNotify = 0xFF;
                    printMenu(true);
                    break;
            }
        }
    }
    else if( sessionType == SESSION_TYPE_CONFIG )
    {
        if ( psessionTypeFields )
        {
            sessionInfoFota = (struct omaDmFotaTlv *)psessionTypeFields;
            FwStatusNotify = sessionInfoFota->state;
            switch( FwStatusNotify )
            {
                case eREAD_REQUEST:
                    fprintf ( stderr, "\nPlease select following option or press"\
                                      "<Enter> to exit:\n"\
                                      "8. Proceed to Read Request\n"\
                                      "____________________________________________"
                                      "______________\n" );
                                      fprintf( stderr, "Option:" );
                    break;

                case eCHANGE_REQUEST:
                    fprintf ( stderr, "\nPlease select following option or press"\
                                      "<Enter> to exit:\n"\
                                      "9. Proceed to Change Request\n"\
                                      "____________________________________________"
                                      "______________\n" );
                                      fprintf( stderr, "Option:" );
                    break;

                case eCONFIG_COMPLETE:
                    fprintf ( stderr, "\nConfig Completed\n" );
                    printMenu(true);
                    break;

                default:
                    fprintf ( stderr, "Invalid Response from Callback \n" );
                    FwStatusNotify = 0xFF;
                    printMenu(true);
                    break;
            }
        }
    }
}

void SetCallBack(void)
{
    ULONG rc;
    rc = SetSLQSOMADMAlertCallback( &cbkSetSLQSOMADMAlertCallback );

    if ( rc != eQCWWAN_ERR_NONE )
        fprintf( stderr, "SetCallBack: Failed : %u\r\n", rc );
    else
        fprintf( stderr, "SetCallback: Enabled callback\r\n" );
}

void ClearCallBack(void)
{
    ULONG rc;
    rc = SetSLQSOMADMAlertCallback( NULL );

    if ( rc != eQCWWAN_ERR_NONE )
        fprintf( stderr, "Callback: Failed : %u\n", rc );
    else
        fprintf( stderr, "Callback: Disabled\n" );
}

void QuitApplication(void)
{
    free( sdkbinpath );
    ClearCallBack();
    fprintf( stderr, "\nExiting Application!!!\n" );
    QCWWANDisconnect();
    exit( EXIT_SUCCESS );
}

bool StartSession(void)
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;
    ULONG sessionType, startSessionResponse;

    while(1)
    {
        fprintf( stderr, "\nEnter the Session Type or <ENTER KEY> to return to Main Menu\n"\
                         "1. FOTA\n"\
                         "2. DM\n"\
                         "3. PRL\n");
        fprintf( stderr, "Option : ");

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
             FlushStdinStream();
        }

        /* If only <ENTER> is pressed by the user */
        if( ENTER_KEY == selOption[0])
        {
            return true;
        }
        len = strlen(selOption);
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userChoice = atoi( selOption );

        if ( userChoice > 3 || userChoice < 1 )
        {
            fprintf( stderr, "\nINVALID INDEX!!!!\n ");
            continue;
        }

        sessionType = userChoice;
        ULONG nRet  = SLQSOMADMStartSession2( sessionType,  &startSessionResponse);

        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr, "Not Able To Start Session\n" );
            fprintf( stderr, "Error Code : %d \n",nRet );
            return true;
        }
        else
        {
            fprintf( stderr, "Session Started\n"\
                     "Firmware Availiability: %u\n", startSessionResponse );
            return true;
        }
    }
}

void CancelSession(void)
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;
    ULONG sessionType;

    while(1)
    {
        fprintf( stderr, "\nEnter the Session Type or <ENTER KEY> to return to Main Menu\n"\
                         "1. FOTA\n"\
                         "2. DM\n");
        fprintf( stderr, "Option : ");

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
             FlushStdinStream();
        }

        /*If only <ENTER> is pressed by the user */
        if( ENTER_KEY == selOption[0])
        {
            return;
        }
        len = strlen(selOption);
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userChoice = atoi( selOption );

        if ( userChoice > 2 || userChoice < 1 )
        {
            fprintf( stderr, "\nINVALID INDEX!!!!\n " );
            continue;
        }

        sessionType = userChoice;
        ULONG nRet  = SLQSOMADMCancelSession( sessionType );

        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr, "Not Able To Cancel Session\n" );
            fprintf( stderr, "Error Code : %d \n",nRet );
        }
        else
        {
            fprintf( stderr, "Session Cancelled\n" );
        }
        return;
    }
}

void SendSelection(void)
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;
    ULONG selection;
    ULONG deferTime;
    ULONG rejectReason;
    ULONG nRet;

    while(1)
    {
        fprintf( stderr, "\nChoose one of following option for operation or"\
                         " <ENTER KEY> to return to Main Menu\n"\
                         "1. Accept \n"\
                         "2. Reject\n" \
                         "3. Defer\n" );
        fprintf( stderr, "Option : " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
             FlushStdinStream();
        }

        /*If only <ENTER> is pressed by the user */
        if( ENTER_KEY == selOption[0])
        {
            return;
        }
        len = strlen(selOption);
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userChoice = atoi( selOption );

        if ( userChoice > 3 || userChoice < 1 )
        {
            fprintf( stderr, "\nINVALID INDEX!!!!\n ");
            continue;
        }
        selection = userChoice;
        switch (selection)
        {
            case 1:  
                 nRet = SLQSOMADMSendSelection2(selection, NULL, NULL);
                 break;
            case 2:
                 /* provide the reason of reject, 0 - user, 1 - enterprise policy */
                 rejectReason = 0;
                 nRet = SLQSOMADMSendSelection2(selection, NULL, &rejectReason);
                 break;            
            case 3:
                 /* specify the defer time in minutes, A value of 0 will cause the prompt to be resent immediately
                    here we defer it 10 minutes */
                 deferTime = 10;
                 nRet = SLQSOMADMSendSelection2(selection, &deferTime, NULL);
                 break;
        }
        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr, "Not Able To Send Selection\n" );
            fprintf( stderr, "Error Code : %d \n",nRet );
        }
        else
        {
            fprintf( stderr, "Selection Sent Successfully\n");
        }
        return;
    }
}

void GetSessionInfo(void)
{
    ULONG sessionType;
    BYTE Status        = 0xFF;
    SLQSOMADMSessionInfo sessionState;

    WORD CompStatus    = 0xFFFF;
    BYTE Severity      = 0xFF;
    WORD SourceLength  = 0xFF;
    BYTE Source[255];
    WORD PkgNameLength = 0xFF;
    BYTE PkgName[255];
    WORD PkgDescLength = 0xFF;
    BYTE PkgDesc[255];
    WORD DateLength    = 0xFF;
    BYTE Date[255];
    WORD TimeLength    = 0xFF;
    BYTE Time[255];
    BYTE SessionType   = 0xFF;
    BYTE SessionState  = 0xFF;
    WORD RetryCount    = ~0;

    sessionType = SESSION_TYPE_FOTA;
    memset( &sessionState, 0, sizeof(sessionState) );
    sessionState.pStatus = &Status;
    sessionState.pUpdateCompleteStatus = &CompStatus;
    sessionState.pSeverity = &Severity;
    sessionState.pSourceLength = &SourceLength;
    sessionState.pSource = Source;
    sessionState.pPkgNameLength = &PkgNameLength;
    sessionState.pPkgName = PkgName;
    sessionState.pPkgDescLength = &PkgDescLength;
    sessionState.pPkgDescription = PkgDesc;
    sessionState.pDateLength = &DateLength;
    sessionState.pDate = Date;
    sessionState.pTimeLength = &TimeLength;
    sessionState.pTime = Time;
    sessionState.pSessionType  = &SessionType;
    sessionState.pSessionState = &SessionState;
    sessionState.pRetryCount   = &RetryCount;
    ULONG nRet  = SLQSOMADMGetSessionInfo( &sessionType, &sessionState );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Not Able To Get Session Info\n" );
        fprintf( stderr, "Error Code : %d \n",nRet );
        return;
    }

    fprintf( stderr, "\nSession Type : %d \n",sessionType );
    fprintf( stderr, "Session State : %d \n", *sessionState.pStatus );
    return;
}

void SetSwiOMADMSetting(void)
{
    ULONG bFOTAdownload;
    ULONG bFOTAUpdate;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;

    while(1)
    {
        fprintf( stderr, "\nSelect the AutoDownload Mode or <ENTER KEY> to return to Main Menu\n"\
                         "1. Enable \n"\
                         "2. Disable\n" );
        fprintf( stderr, "Option : " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
             FlushStdinStream();
        }

        /*If only <ENTER> is pressed by the user */
        if( ENTER_KEY == selOption[0])
        {
            return;
        }
        len = strlen(selOption);
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userChoice = atoi( selOption );

        if ( userChoice > 2 || userChoice < 1 )
        {
            fprintf( stderr, "\nINVALID INDEX!!!!\n " );
            continue;
        }

        switch(userChoice)
        {
        case eENABLE:
            bFOTAdownload = 1;
            break;

        case eDISABLE:
            bFOTAdownload = 0;
            break;
        }
        break;
    }

    while(1)
    {
        fprintf( stderr, "\nSelect the AutoUpdate Mode or <ENTER KEY> to return to Main Menu\n"\
                          "1. Enable \n"\
                          "2. Disable\n" );
        fprintf( stderr, "Option : " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        /*If only <ENTER> is pressed by the user */
        if( ENTER_KEY == selOption[0])
        {
            return;
        }
        len = strlen(selOption);
        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userChoice = atoi( selOption );

        if ( userChoice > 2 || userChoice < 1 )
        {
            fprintf( stderr, "\nINVALID INDEX!!!!\n ");
            continue;
        }

        switch(userChoice)
        {
            case eENABLE:
                 bFOTAUpdate = 1;
                 break;

            case eDISABLE:
                 bFOTAUpdate = 0;
             break;
        }
        break;
    }

    ULONG nRet  = SLQSOMADMSetSettings( bFOTAdownload, bFOTAUpdate );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Not Able To Set SWIOMADM Setting\n" );
        fprintf( stderr, "Error Code : %d \n",nRet );
        return;
    }

    fprintf(stderr, "SetSwiOmaDMSetting Successful\n");
    return;
}

void GetSwiOMADMSetting(void)
{
    SLQSOMADMSettings  slqsOMADMSettings;
    ULONG bOMADMEnabled;
    BYTE bFOTADownload;
    BYTE bFOTAUpdate;
    BYTE autoSdm; 
    BYTE autoCheck;

    slqsOMADMSettings.pOMADMEnabled = &bOMADMEnabled;
    slqsOMADMSettings.pFOTAdownload = &bFOTADownload;
    slqsOMADMSettings.pFOTAUpdate = &bFOTAUpdate;
    slqsOMADMSettings.pAutosdm = &autoSdm;
    slqsOMADMSettings.pFwAutoCheck = &autoCheck;

    ULONG nRet  = SLQSOMADMGetSettings2( &slqsOMADMSettings );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Not Able To Get SWIOMADM Setting\n" );
        fprintf( stderr, "Error Code : %d \n",nRet );
        return;
    }

    fprintf(stderr, "GetSwiOmaDMSetting Successful\n");
    fprintf(stderr, "OMA-DM Enabled : 0x%08x\n", *slqsOMADMSettings.pOMADMEnabled);
    fprintf(stderr, "FOTA Download : 0x%02x\n", *slqsOMADMSettings.pFOTAdownload);
    fprintf(stderr, "FOTA Update: 0x%02x\n", *slqsOMADMSettings.pFOTAUpdate);
    fprintf(stderr, "Auto UI Alert Response: 0x%02x\n", *slqsOMADMSettings.pAutosdm);
    return;
}

int main( int argc, const char *argv[])
{
    ULONG resultCode = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    ULONG userOption;
    bool status = true;

    if( argc < 2 )
    {
        fprintf( stderr, "usage: %s <path to sdk binary>\n", argv[0] );
        exit( EXIT_SUCCESS );
    }

    if( NULL == (sdkbinpath = (char *)malloc(strlen(argv[1]) + 1)) )
    {
        perror(__func__);
        exit( EXIT_FAILURE );
    }

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1 );

    /* Start the SDK */
    resultCode = StartSDK();
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        free(sdkbinpath);

        /* Display the failure reason */
        fprintf( stderr,
                 "Failed to start SDK: Exiting App\n"\
                 "Failure Code: %u\n", resultCode );

        /* Failed to start SDK, exit the application */
        exit( EXIT_FAILURE );
    }

    if( eQCWWAN_ERR_NONE != resultCode )
    {
         fprintf( stderr,
                  "\nRunning with device in boot and hold"\
                  " mode or disconnected\n" );
    }

    SetCallBack();

    while(1)
    {
        printMenu(status);

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }
        len = strlen( selOption );

        /*If only <ENTER> is pressed by the user quit Application*/
        if( ENTER_KEY == selOption[0])
        {
            QuitApplication();
            return ENTER_KEY_PRESSED;
        }

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userOption = atoi( selOption );

        /* Process user input */
        switch( userOption )
        {
            case eSTART_SESSION:
                status = StartSession();
                break;

            case eCANCEL_SESSION:
                CancelSession();
                status = true;
                break;

            case eGET_SESSION_INFO:
                GetSessionInfo();
                status = true;
                break;

            case eGET_OMADM_SETTING:
                GetSwiOMADMSetting();
                status = true;
                break;

            case eSET_OMADM_SETTING:
                SetSwiOMADMSetting();
                status = true;
                break;

            case eSEL_DOWNLOAD_FIRMWARE:
                SendSelection();
                status = false;
                break;

            case eSEL_UPDATE_FIRMWARE:
                SendSelection();
                status = false;
                break;

            case eSEL_READ_REQUEST:
                SendSelection();
                status = false;
                break;

            case eSEL_CHANGE_REQUEST:
                SendSelection();
                status = false;
                break;

            default:
                fprintf( stderr, "INVALID OPTION!!!!!\n" );
                status = true;
                break;
        }
    }
}
