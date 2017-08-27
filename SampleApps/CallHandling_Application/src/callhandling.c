/*************
 *
 * Filename:    callhandling.c
 *
 * Purpose:     Voice Sample Application
 *
 * Copyright:   © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "callhandling.h"

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/
/* path to sdk binary */
static char *sdkbinpath = NULL;

/* Device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];
static BOOL g_first_call = TRUE;

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
                pdev->deviceKey
                );
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

void SetCallBack(void)
{
    ULONG nRet;
    nRet = SLQSVoiceSetAllCallStatusCallBack (&DisplayCallInfo);
    if( nRet != eQCWWAN_ERR_NONE )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe SetAllCallStatusCallBack\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
        #endif
    }

    nRet = SLQSVoiceInfoRecCallback (&DisplayRecInfo);
    if( nRet != eQCWWAN_ERR_NONE )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe SLQSVoiceInfoRecCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
        #endif
    }

    nRet = SLQSVoiceSetSUPSNotificationCallback(&DisplayNotificationInfo);
    if( nRet != eQCWWAN_ERR_NONE )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe SetSUPSNotificationCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
        #endif
    }
}

void ClearCallBack(void)
{
    ULONG nRet;
    nRet = SLQSVoiceSetAllCallStatusCallBack (NULL);
    if( nRet != eQCWWAN_ERR_NONE )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe SetAllCallStatusCallBack\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
        #endif
    }

    nRet = SLQSVoiceSetSUPSNotificationCallback(NULL);
    if( nRet != eQCWWAN_ERR_NONE )
    {
        #ifdef DBG
        fprintf( stderr, "Failed to subscribe SetSUPSNotificationCallback\n" );
        fprintf( stderr, "Failure Code : %u\n", nRet );
        #endif
    }
}

void QuitApplication(void)
{
    free( sdkbinpath );
    fprintf( stderr, "\nExiting Application!!!\n" );
    ClearCallBack();
    QCWWANDisconnect();
    exit( EXIT_SUCCESS );
}

void VoiceDialCall(void)
{
    int count;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    CHAR VoiceDialNumber[MAX_CALL_NO_LEN];
    BYTE userChoice;
    BYTE calltype;
    BYTE clirtypeSuprs = 0x01;
    BYTE clirtypeInvoc = 0x02;
    ULONG nRet;

    voiceCallRequestParams  voicecallreq;
    voiceCallResponseParams voicecallresp;

    memset( &voicecallreq, 0, sizeof(voicecallreq) );
    memset( &voicecallresp, 0, sizeof(voicecallresp) );

    while(1)
    {
        fprintf( stderr, "Do You Want To Display Your Phone Number "\
                         "Or Press<Enter> To Go To Main Menu:\n"\
                         "1. Yes\n"\
                         "2. No\n" );
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

        /* Convert the option added by user into integer */
        userChoice = atoi( selOption );

        if ( userChoice < 1 || userChoice > 2 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        switch ( userChoice )
        {
            case eOPTION_1:
                voicecallreq.pCLIRType = &clirtypeSuprs;
                break;
            case eOPTION_2:
                voicecallreq.pCLIRType = &clirtypeInvoc;
                break;
        }
        break;
    }
    while(1)
    {
        fprintf( stderr, "Please Select One Of The Following Call Types "\
                         "Or Press<Enter> To Go To Main Menu:\n"\
                         "1. Voice\n"\
                         "2. Voice call over IP\n" );
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
        /* Convert the option added by user into integer */
        userChoice = atoi( selOption );
        if ( userChoice < 1 || userChoice > 2 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        switch ( userChoice )
        {
            case eOPTION_1:
                calltype = eVOICE;
                break;
            case eOPTION_2:
                calltype = eCALL_OVER_IP;
                break;
        }
        break;
    }
    voicecallreq.pCallType = &calltype;

    fprintf( stderr, "Enter the Phone Number Or Press<Enter> "
                     "To Go To Main Menu: " );
    fgets( VoiceDialNumber, MAX_CALL_NO_LEN, stdin );

    for (count = 0; VoiceDialNumber[count] != ENTER_KEY; count++)
    {
        voicecallreq.callNumber[count] = VoiceDialNumber[count];
    }

    nRet = SLQSVoiceDialCall( &voicecallreq, &voicecallresp );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Not Able To Dial Call\n" );
    }
}

void VoiceEndAllCall()
{
    WORD FailCause;
    voiceManageCallsReq  req;
    voiceManageCallsResp resp;

    req.SUPSType = eST_END_ALL_CALLS;
    req.pCallID = NULL;
    resp.pFailCause = &FailCause;

    SLQSVoiceManageCalls( &req, &resp );
}

void VoiceEndCall()
{
    unsigned int len = 0;
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE userChoice;

    fprintf( stderr, "\nWhich Call ID Do You Want To End:" );

    /* Receive the input from the user */
    fgets( selCallId, ( OPTION_LEN ), stdin );

    /* flush the input stream */
    pEndOfLine = strchr( selCallId, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
         FlushStdinStream();
    }
    len = strlen( selCallId );

    /* Convert the option added by user into integer */
    selCallId[ len - 1 ] = '\0';
    userChoice = atoi( selCallId );

    ULONG nRet = SLQSVoiceEndCall( &userChoice );
    sleep(2);

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "\nThere Is No Call In Progress\n" );
    }
    return;
}

void VoiceAnswerCall()
{
    unsigned int len = 0;
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE userChoice;
    voiceAnswerCall objVoiceAnswerCall;

    fprintf( stderr, "\nWhich Call ID Do You Want To Answer: " );

    /* Receive the input from the user */
    fgets( selCallId, ( OPTION_LEN ), stdin );

    /* flush the input stream */
    pEndOfLine = strchr( selCallId, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
         FlushStdinStream();
    }
    len = strlen( selCallId );

    /* Convert the option added by user into integer */
    selCallId[ len - 1 ] = '\0';
    userChoice = atoi( selCallId );

    objVoiceAnswerCall.pCallId = &userChoice;

    ULONG nRet = SLQSVoiceAnswerCall( &objVoiceAnswerCall );
    sleep(2);

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "\nThere Is No Incoming Call In Progress\n" );
    }
    return;
}

void ManageCalls( BYTE supstype )
{
    unsigned int len = 0;
    WORD FailCause;
    ULONG nRet;
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE userChoice;
    voiceManageCallsReq  req;
    voiceManageCallsResp resp;

    req.SUPSType = supstype;
    req.pCallID = NULL;
    resp.pFailCause = &FailCause;

    if( eST_HOLD_ALL_EXCEPT_SPECIFIED_CALL == supstype ||
        eST_RELEASE_SPECIFIED_CALL == supstype )
    {
        fprintf( stderr, "\nIf Any Call In Progress, Please Specify Call Id :" );

        /* Receive the input from the user */
        fgets( selCallId, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selCallId, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
             FlushStdinStream();
        }
        len = strlen( selCallId );
        /* Convert the option added by user into integer */
        selCallId[ len - 1 ] = '\0';
        userChoice = atoi( selCallId );
        req.pCallID = &userChoice;
    }

    nRet = SLQSVoiceManageCalls( &req, &resp );
    sleep(1);

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        if( *(resp.pFailCause) == CALL_ERROR_CODE )
        {
            fprintf( stderr, "\nNo Active Or Held Or Waiting Call"\
                             " In Progress\n" );
        }
        else
        {
            fprintf( stderr, "Failure Cause : %d \n", *(resp.pFailCause) );
            fprintf( stderr, "\nThere Should Be More Than One Call "\
                             "In Progress\n" );
        }
    }
    return;
}

void CallStateCBK(BOOL first_call)
{
    g_first_call = first_call;
}

int GetWirelessTechInfo(void)
{
    ULONG nRet;
    qaQmiServingSystemParam ServingSystemInfo;
    int tech = -1;

    /* Check if device connected is 3GPP or 3GPP2 */
    nRet = SLQSGetServingSystem( &ServingSystemInfo );
    if ( nRet == 0 && ServingSystemInfo.ServingSystem.numRadioInterfaces > 0)
    {
        switch(ServingSystemInfo.ServingSystem.radioInterface[0])
        {
            case 1: /* CDMA 1x */
            case 2: /* CDMA 1xEVDO */
                tech = TECH_3GPP2; /* 3GPP2 */
                break;

            case 4: /* GSM*/
            case 5: /* UMTS */
            case 8: /* LTE */
                tech = TECH_3GPP; /* 3 GPP */
                break;
        }
    }
    return tech;
}

int main( int argc, const char *argv[])
{
   ULONG resultCode = 0;
   CHAR selOption[OPTION_LEN];
   CHAR *pEndOfLine = NULL;
   unsigned int len = 0;
   ULONG userOption;
   BYTE SUPSType;
   int  wirelessTech;

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


   /* display menu */
   if( eQCWWAN_ERR_NONE != resultCode )
   {
        fprintf( stderr,
                 "\nRunning with device in boot and hold"\
                 " mode or disconnected\n" );
   }

   wirelessTech = GetWirelessTechInfo();
   SetCallBack();
   SetCallStateReportCallback(CallStateCBK);
   while(1)
   {
       fprintf( stderr, "\nPlease Select One Of The Following Options "\
               "Or Press<Enter> To Exit:\n"\
               "-----------CALL HANDLING-------------\n"\
               "1.  Dial a Call\n"\
               "2.  Answer Call\n"\
               "3.  End a Call\n"\
               "4.  End Held or Waiting Call\n"\
               "5.  End Active Call and Accept Held or Waiting Call\n"\
               "6.  Hold Active Call and Accept Waiting or Held Call\n"\
               "7.  Hold All Calls Except a Specified One\n"\
               "8.  Make a Conference Call\n"\
               "9.  End All Calls\n"\
               "10. Release a Specified Call \n"\
               "\n"\
               "------------SERVICES--------------\n"\
               "11. Set Supplementary Service\n"\
               "12. Start DTMF On Call In Progress\n"\
               "13. Stop DTMF On Call In Progress\n"\
               "14. Get CLIR Service Status\n"\
               "15. Get CLIP Service Status\n"\
               "16. Send Flash[ONLY FOR CDMA]\n" );

       fprintf( stderr, "Option:" );
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
           VoiceEndAllCall();
           QuitApplication();
           return ENTER_KEY_PRESSED;
       }

       /* Convert the option added by user into integer */
       selOption[ len - 1 ] = '\0';

       /* Special handling for 'y' and 'n' */
       if( 'y' == selOption[0] || 'Y' == selOption[0] )
       {
           /* if it is the first CDMA voice call, accept the call by VoiceAnswerCall */
           if (g_first_call == TRUE)
           {
               userOption = eANSWER_CALL;
               g_first_call = FALSE;
           }
           /* if there is already one call taking in progress, accept the incoming call by SendFlash */
           else
           {
               userOption = (ULONG)selOption[0];
           }
       }
       else if( 'n' == selOption[0] || 'N' == selOption[0] )
       {
           userOption = (ULONG)selOption[0];
       }
       else
       {
           userOption = atoi( selOption );
       }

       /* Process user input */
       switch( userOption )
       {
            case eDIAL_CALL:
                VoiceDialCall();
                g_first_call = FALSE;
                break;

            case eANSWER_CALL:
                VoiceAnswerCall();
                break;

            case eEND_CALL:
                VoiceEndCall();
                g_first_call = TRUE;
                break;

            case eRELEASE_HELD_OR_WAITING:
                SUPSType = eST_RELEASE_HELD_OR_WAITING;
                ManageCalls(SUPSType);
                break;

            case eRELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING:
                SUPSType = eST_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING;
                ManageCalls(SUPSType);
                break;

            case eHOLD_ACTIVE_ACCEPT_WAITING_OR_HELD:
                if(wirelessTech == TECH_3GPP)
                {
                    SUPSType = eST_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD;
                    ManageCalls(SUPSType);
                }
                else if(wirelessTech == TECH_3GPP2)
                {
                    SendFlashForWaitingCall();
                }
                break;

            case eHOLD_ALL_EXCEPT_SPECIFIED_CALL:
                SUPSType = eST_HOLD_ALL_EXCEPT_SPECIFIED_CALL;
                ManageCalls(SUPSType);
                break;

            case eMAKE_CONFERENCE_CALL:
                SUPSType = eST_MAKE_CONFERENCE_CALL;
                ManageCalls(SUPSType);
                break;

            case eEND_ALL_CALLS:
                SUPSType = eST_END_ALL_CALLS;
                ManageCalls(SUPSType);
                g_first_call = TRUE;
                break;

            case eRELEASE_SPECIFIED_CALL:
                SUPSType = eST_RELEASE_SPECIFIED_CALL;
                ManageCalls(SUPSType);
                break;

            case eSET_SUPS_SERVICE:
                SetVoiceSUPSService();
                break;

            case eSTART_DTMF:
                StartDTMFServices();
                break;

            case eSTOP_DTMF:
                StopDTMFServices();
                break;

            case eGET_CLIR:
                GetCLIRServiceStatus();
                break;

            case eGET_CLIP:
                GetCLIPServiceStatus();
                break;

            case eSEND_FLASH:
                SendFlash();
                break;

            case eRECEIVE_WAITING_CALL:
            case eRECV_WAITING_CALL:
                SendFlashForWaitingCall();
                break;

            case eREJECT_WAITING_CALL:
            case eREJ_WAITING_CALL:
               /* Do nothing*/
               break;

            default:
                fprintf( stderr, "Main Menu : Invalid Option!\n" );
                break;
       }
   }
}
