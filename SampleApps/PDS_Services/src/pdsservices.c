/*************
 *
 * Filename:    pdsservices.h
 *
 * Purpose:     Provide PDS Services
 *
 * Copyright:   © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "pdsservices.h"

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/
/* path to sdk binary */
static char *sdkbinpath = NULL;

/* Device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

/****************************************************************************
*                       Functions
*****************************************************************************/
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

     while (QCWWAN2kEnumerateDevices(&devicesSize, (BYTE *)pdev) != 0)
     {
         printf ("\nUnable to find device..\n");
         sleep(1);
     }

     #ifdef DBG
         fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
                  devicesSize,
                  pdev->deviceNode,
                  pdev->deviceKey
                );
     #endif

     /* Connect to the SDK */
     resultCode = QCWWANConnect( pdev->deviceNode, pdev->deviceKey );
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

void QuitApplication( void )
{
    free( sdkbinpath );
    fprintf( stderr, "\nExiting Application!!!\n" );
    ClearCallBack();
    QCWWANDisconnect();
    exit( EXIT_SUCCESS );
}

void PrintMainMenu( void )
{
    fprintf( stderr, "\nPlease select one of the following options "\
                     "Or Press<Enter> To Exit:\n"\
                     "-------GPS SERVICES-------\n"\
                     "1.  Set GPS Service State\n"\
                     "2.  Get GPS Service State\n"\
                     "3.  Start Tracking Session\n"\
                     "4.  End Tracking Session\n"\
                     "5.  Set SERVICE Auto Tracking\n"\
                     "6.  Set PORT Auto Tracking\n"
                     "______________________________________________\n" );
    fprintf( stderr, "Option:" );
}

void SetCallBack( void )
{
    ULONG nRet;
    nRet = SetPDSStateCallback( &PDSStateCallBack );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetPDSStateCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }

    nRet = SetNMEACallback( &NMEACallBack );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetNMEACallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void ClearCallBack( void )
{
    ULONG nRet;
    nRet = SetPDSStateCallback( NULL );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetPDSStateCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }

    nRet = SetNMEACallback( NULL );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetNMEACallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void PDSStateCallBack( ULONG enabledStatus, ULONG trackingStatus )
{
    if ( DISABLE == enabledStatus )
    {
        fprintf( stderr, "\n\nGPS Service is disabled!!!\n" );
    }
    if ( ENABLE == enabledStatus )
    {
        fprintf( stderr, "\n\nGPS Service is enabled!!!\n" );
    }
    if( eUNKNOWN == trackingStatus )
    {
        fprintf( stderr, "\nGPS Tracking Session is unknown!!!\n" );
    }
    if( eACTIVE == trackingStatus )
    {
        fprintf( stderr, "\nGPS Tracking Session is active!!!\n" );
    }
    if( eINACTIVE == trackingStatus )
    {
        fprintf( stderr, "\nGPS Tracking Session is inactive!!!\n" );
    }
    PrintMainMenu();
}

void NMEACallBack( const char *pNMEAData )
{
    FILE *fp;

    fp = fopen("./NMEACallBackData.txt", "a" );
    if( fp )
    {
        fprintf ( fp, "NMEA Data follows:\r\n\n" );
        fprintf ( fp, "%s\r\n", pNMEAData );
    }
    if(fp)
        fclose(fp);
}

void SetGPSServiceState(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    ULONG value;

    while(1)
    {
        fprintf( stderr, "\nPlease select one of the option for GPS Service "\
                         "Or Press<Enter> to go to previous menu:\n"\
                         "1.Enable\n"\
                         "2.Disable\n" );
        fprintf( stderr, "Option:" );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return;

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        len = strlen( selOption );

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userOption = atoi( selOption );

        if ( userOption < 1 || userOption > 2 )
        {
            fprintf( stderr, "Invalid Option!!!!!\n" );
            continue;
        }

        switch( userOption )
        {
            case eOPTION_1:
                value = ENABLE;
                break;
            case eOPTION_2:
                value = DISABLE;
                break;
        }
        break;
    }

    nRet = SetPDSState( value );
    if ( !nRet )
    {
        if ( DISABLE == value )
        {
            fprintf( stderr, "\nGPS State disabled\n" );
        }
        else if  ( ENABLE == value )
        {
            fprintf( stderr, "\nGPS State enabled\n" );
        }
    }
    else
    {
        fprintf( stderr, "\nGPS state enable/disable operation failed\n" );
    }
}

void GetGPSServiceState(void)
{
    ULONG enabledStatus;
    ULONG trackingStatus;
    ULONG nRet;
    nRet = GetPDSState( &enabledStatus,
                        &trackingStatus );

    if( nRet )
    {
        fprintf( stderr, "\nNot able to Get GPS State\n" );
    }
    else
    {
        fprintf( stderr, "\nGPS enabled status : %x\n", enabledStatus );
        fprintf( stderr, "GPS Tracking status : %x\n",trackingStatus );
    }
}

void StartTrackingSession(void)
{
    starttrackingsession req;

    /* Set values of parameters which are mandatory to start tracking session */
    req.fixAccuracy = 10; /* Time between fixes in ms */
    req.fixCount = 10; /* Count of position fix requests for the session */
    req.fixInterval = 1; /* interval between position fix requests in sec */
    req.fixTimeout = 255; /* Maximum time to work on each fix in sec max 255 */
    req.sessionControl = HEXZERO; /* Control method - Manual */
    req.sessionOperation = HEXZERO; /* Session operation - Standalone */
    req.sessionServerOption = HEXZERO; /* Default */
    req.sessionType = HEXZERO; /* Session type - New */


    ULONG nRet = StartPDSTrackingSessionExt( req.sessionControl,
                                             req.sessionType,
                                             req.sessionOperation,
                                             req.sessionServerOption,
                                             req.fixTimeout,
                                             req.fixInterval,
                                             req.fixCount,
                                             req.fixAccuracy );

    if( nRet )
    {
        fprintf( stderr, "\nNot able to Start Tracking Session\n" );
    }
    else
    {
        fprintf( stderr,"\nNMEA Data will be saved in current directory "\
                        "In NMEACallBackData.txt file\n");
        fprintf( stderr, "\nTracking Session Started\n" );
    }
}

void EndTrackingSession(void)
{
    ULONG nRet = StopPDSTrackingSession();
    if( nRet )
    {
        fprintf( stderr, "\nNot able to stop GPS Session\n" );
    }
    else
    {
        fprintf( stderr, "\nEnded Tracking Session\n" );
    }
}

void SrvAutoTrackingsession()
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    ULONG value;

    while(1)
    {
        fprintf( stderr, "\nPlease select one of the option for service auto "\
                         " tracking Or Press<Enter> to go to previous Menu:\n"\
                         "1.Enable\n"\
                         "2.Disable\n" );
        fprintf( stderr, "Option:" );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return;

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        len = strlen( selOption );

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userOption = atoi( selOption );

        if ( userOption < 1 || userOption > 2 )
        {
            fprintf( stderr, "Invalid Option!!!!!\n" );
            continue;
        }

        switch( userOption )
        {
            case eOPTION_1:
                value = ENABLE;
                break;
            case eOPTION_2:
                value = DISABLE;
                break;
        }
        break;
    }

    nRet = SetServiceAutomaticTracking( value );
    if ( !nRet )
    {
        if ( DISABLE == value )
        {
            fprintf( stderr, "\nService Auto Tracking disabled\n" );
        }
        else if  ( ENABLE == value )
        {
            fprintf( stderr, "\n Service Auto tracking enabled\n" );
        }
    }
    else
    {
        fprintf( stderr, "\n Auto Tracking enable/disable operation failed\n");
    }
}

void PortAutoTrackingsession()
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    ULONG value;

    while(1)
    {
        fprintf( stderr, "\nPlease select one of the option for port auto "\
                         " tracking Or Press<Enter> to go to previous menu:\n"\
                         "1.Enable\n"\
                         "2.Disable\n" );
        fprintf( stderr, "Option:" );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return;

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        len = strlen( selOption );

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        userOption = atoi( selOption );

        if ( userOption < 1 || userOption > 2 )
        {
            fprintf( stderr, "Invalid Option!!!!!\n" );
            continue;
        }

        switch( userOption )
        {
            case eOPTION_1:
                value = ENABLE;
                break;
            case eOPTION_2:
                value = DISABLE;
                break;
        }
        break;
    }

    nRet = SetPortAutomaticTracking( value );
    if ( !nRet )
    {
        if ( DISABLE == value )
        {
            fprintf( stderr, "\n Port Auto Tracking disabled\n" );
        }
        else if  ( ENABLE == value )
        {
            fprintf( stderr, "\n Port Auto tracking enabled\n" );
        }
    }
    else
    {
        fprintf( stderr, "\n Port Auto Tracking enable/disable operation failed\n" );
    }
}

void GetAutoTrackingState()
{
    ULONG autotrackState = 0;
    ULONG nRet;
    nRet = GetPortAutomaticTracking( &autotrackState );
    if ( !nRet )
    {
        if ( autotrackState )
        {
             fprintf(stderr,"\n<<<Port Autotracking is enabled!!!>>>\n"
                            "- Only option 6 can be used to disable Port"
                            " auto tracking\n");
        }
        else
        {
            fprintf(stderr,"\n<<<Port Autotracking is disabled!!!>>>\n");
        }
    }
    else
    {
        fprintf(stderr,"\n<<<Port Autotracking can not be retrived!!!>>>\n"
                       "- Continue with Normal opertaion\n");
    }
}

int main( int argc, const char *argv[] )
{
    ULONG resultCode = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    ULONG userOption;

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

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1);

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

    SetCallBack();
    GetAutoTrackingState();

    while(1)
    {
        PrintMainMenu();
    	
    	/* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }
        len = strlen( selOption );

        /* If only <ENTER> is pressed by the user quit Application */
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
            case eSET_GPS_SERVICE_STATE:
                SetGPSServiceState();
                break;

            case eGET_GPS_SERVICE_STATE:
                GetGPSServiceState();
                break;

            case eSTART_TRACKING_SESSION:
                StartTrackingSession();
                break;

            case eEND_TRACKING_SESSION:
                EndTrackingSession();
                break;

            case eSRV_AUTO_TRACKING_SESSION:
                SrvAutoTrackingsession();
                break;

            case ePORT_AUTO_TRACKING_SESSION:
                PortAutoTrackingsession();
                break;
            default:
                fprintf( stderr, "Main Menu : Invalid Option!!!!!\n" );
                break;
        }
    }
}
