/*************
 *
 * Filename:    locservice.h
 *
 * Purpose:     Provide LOC Services
 *
 * Copyright:   © 2016 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "locservice.h"
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
            fprintf(stderr, "Failed to set the SDK Image path (0x%x)\n",\
                        resultCode);
            return resultCode;
     }

     if( eQCWWAN_ERR_SWICM_SOCKET_IN_USE == (resultCode = SLQSStart(0, NULL)) )
     {
            fprintf(stderr, "Another APP is currently using the SDK (0x%x)\n",
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
                     "-------LOC SERVICES-------\n"\
                     "1.  SET LOC Operation Mode\n"\
                     "2.  START LOC\n"\
                     "3.  STOP  LOC\n"\
                     "______________________________________________\n" );
                     fprintf( stderr, "Option:" );
}
void SetCallBack( void )
{
    ULONG nRet;
    nRet = SetLocEventPositionCallback(  &LocEventPositionCB );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetLocEventPositionCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void ClearCallBack( void )
{
    ULONG nRet;
    nRet = SetLocEventPositionCallback( NULL );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failed to subscribe SetPDSStateCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void LocEventPositionCB( QmiCbkLocPositionReportInd *pMsg)
{
        if (pMsg != NULL)
        {
             fprintf(stderr, "Event Position Report\n");
             fprintf( stderr, "Session Status: %u\n",
                          pMsg->sessionStatus);
             fprintf( stderr, "Session ID: %d\n",
                          pMsg->sessionId);
             IFPRINTF( stderr, "Latitude: %lf\n",
                          (double*)pMsg->pLatitude);
             IFPRINTF( stderr, "Longitude: %lf\n",
                          (double*)pMsg->pLongitude);
             IFPRINTF( stderr, "Horizontal position uncertainty: %f\n",
                          (float*)pMsg->pHorUncCircular);
             IFPRINTF( stderr, "Semi-minor axis of horizontal elliptical uncertainty: %f\n",
                          (float*)pMsg->pHorUncEllipseSemiMinor);
             IFPRINTF( stderr, "Semi-major axis of horizontal elliptical uncertainty: %f\n",
                          (float*)pMsg->pHorUncEllipseSemiMajor);
             IFPRINTF( stderr, "Elliptical Horizontal Uncertainty Azimuth: %f\n",
                          (float*)pMsg->pHorUncEllipseOrientAzimuth);
             IFPRINTF( stderr, "Horizontal Confidence: %d\n",
                           pMsg->pHorConfidence);
             IFPRINTF( stderr, "Horizontal Reliability: %u\n",
                           (pMsg->pHorReliability));
             IFPRINTF( stderr, "Horizontal Speed: %f\n",
                           (float*)pMsg->pSpeedHorizontal);
             IFPRINTF( stderr, "Speed Uncertainty: %f\n",
                           (float*)pMsg->pSpeedUnc);
             IFPRINTF( stderr, "Altitude With Respect to Ellipsoid: %f\n",
                           (float*)pMsg->pAltitudeWrtEllipsoid);
             IFPRINTF( stderr, "Altitude With Respect to Sea Level: %f\n",
                           (float*)pMsg->pAltitudeWrtMeanSeaLevel);
             IFPRINTF( stderr, "Vertical Uncertainty: %f\n",
                           (float*)pMsg->pVertUnc);
             IFPRINTF( stderr, "Vertical Confidence: %d\n",
                            pMsg->pVertConfidence);
             IFPRINTF( stderr, "Vertical Reliability: %u\n",
                            pMsg->pVertReliability);
             IFPRINTF( stderr, "Vertical Speed: %f\n",
                           (float*)pMsg->pSpeedVertical);
             IFPRINTF( stderr, "Heading: %f\n",
                           (float*)pMsg->pHeading);
             IFPRINTF( stderr, "Heading Uncertainty: %f\n",
                           (float*)pMsg->pHeadingUnc);
             IFPRINTF( stderr, "Magnetic Deviation: %f\n",
                           (float*)pMsg->pMagneticDeviation);
             IFPRINTF( stderr, "Technology Mask: %u\n",
                            pMsg->pTechnologyMask);
             if (pMsg->pPrecisionDilution != NULL)
             {
                 fprintf( stderr, "Position dilution of precision: %f\n",
                              *((float*)&pMsg->pPrecisionDilution->PDOP));
                 fprintf( stderr, "Horizontal dilution of precision: %f\n",
                              *((float*)&pMsg->pPrecisionDilution->HDOP));
                 fprintf( stderr, "Vertical dilution of precision: %f\n",
                              *((float*)&pMsg->pPrecisionDilution->VDOP));
             }
             IFPRINTF( stderr, "UTC Timestamp: %lf\n",
                           (double*)pMsg->pTimestampUtc);
             IFPRINTF( stderr, "Leap Seconds: %d\n",
                            pMsg->pLeapSeconds);
             if (pMsg->pGpsTime != NULL)
             {
                 fprintf( stderr, "GPS Week: %d\n",
                              (pMsg->pGpsTime->gpsWeek));
                 fprintf( stderr, "Amount of time into the current GPS week: %u\n",
                              (pMsg->pGpsTime->gpsTimeOfWeekMs));
             }
             IFPRINTF( stderr, "Time Uncertainty: %f\n",
                          (float*)pMsg->pTimeUnc);
             IFPRINTF( stderr, "Time Source: %u\n",
                            pMsg->pTimeSrc);
             if (pMsg->pSensorDataUsage != NULL)
             {
                 fprintf( stderr, "Usage Mask: %u\n",
                              (pMsg->pSensorDataUsage->usageMask));
                 fprintf( stderr, "Aided Indicator Mask: %u\n",
                              (pMsg->pSensorDataUsage->aidingIndicatorMask));
             }
             IFPRINTF( stderr, "Fix ID: %u\n",
                            pMsg->pFixId);
             if (pMsg->pSvUsedforFix != NULL)
             {
                  BYTE i = 0;
                  fprintf( stderr, "GNSS SV Used List Length: %d\n",
                              (pMsg->pSvUsedforFix->gnssSvUsedList_len));
                 for ( i = 0; i < pMsg->pSvUsedforFix->gnssSvUsedList_len; i++ )
                 {
                       fprintf( stderr, "GNSS SVs Used List: %d\n",
                              pMsg->pSvUsedforFix->gnssSvUsedList[i]);
                 }
             }
             IFPRINTF( stderr, "Altitude Assumed: %x\n",
                           pMsg->pAltitudeAssumed);
             }
        PrintMainMenu();
}

void StartLOCService(void)
{
    ULONG nRet;
    LOCStartReq req;
    req.SessionId=1;
    req.pRecurrenceType = NULL;
    req.pHorizontalAccuracyLvl = NULL;
    req.pIntermediateReportState = NULL;
    req.pMinIntervalTime = NULL;
    req.pApplicationInfo = NULL;
    req.pConfigAltitudeAssumed = NULL;

    nRet = SLQSLOCStart(&req );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void StopLOCService(void)
{
    ULONG nRet;
    LOCStopReq req;
    req.sessionId=1;

    nRet = SLQSLOCStop( &req );
    if( nRet != eQCWWAN_ERR_NONE )
    {
        fprintf( stderr, "Failure Code : %u\n", nRet );
    }
}

void SetOperationMode(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    ULONG value =1 ; /*Set default value*/

    while(1)
    {
        fprintf( stderr, "\nPlease select one of the option for GPS Service "\
                         "Or Press<Enter> to go to previous menu:\n"\
                         "1.Use the default engine mode\n"\
                         "2.Use the MS-based mode\n"\
                         "3.Use the MS-assisted mode\n"\
                         "4.Use Standalone mode\n" \
                         "5.Use cell ID; this mode is only valid for GSM/UMTS networks\n"\
                         "6.Use WWAN measurements to calculate the position\n" );
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

        if ( userOption < 1 || userOption > 6 )
        {
            fprintf( stderr, "Invalid Option!!!!!\n" );
            continue;
        }

        break;
    }

    nRet = SLQSLOCSetOpMode( value );
    if ( !nRet )
    {
            fprintf( stderr, "\nSet Loc operation mode successfully\n" );
    }
    else
    {
        fprintf( stderr, "\nSet Loc operation mode failed\n" );
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
            case eQMI_LOC_SET_OP_MODE:
                SetOperationMode();
                break;
            case eQMI_LOC_START:
                StartLOCService();
                break;
            case eQMI_LOC_STOP:
                StopLOCService();
                break;
            default:
                fprintf( stderr, "Main Menu : Invalid Option!!!!!\n" );
                break;
        }
    }
}
