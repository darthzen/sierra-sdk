/*************
 *
 * Filename:    supsservices.c
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
 BYTE timerValue = 15;
 BYTE ServiceClass = 0x01;
 callFwdTypeAndPlan CallFwdTypePlan = {0x02, 0x08};
 BYTE CallFwdNu[] = "0123456789";

/****************************************************************************
*                       Functions
*****************************************************************************/
void CallBarringServices(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    BYTE CallBarringPwd[] = "0000"; /* carrier dependent, need to check with the operator for password */
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;

    memset( &req, 0, sizeof(req));
    memset( &resp, 0, sizeof(resp));

    while(1)
    {
        req.pCallBarringPasswd = CallBarringPwd;
        req.pServiceClass = &ServiceClass;

        fprintf( stderr, "Please Select One Of The Following Option "\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Activate\n"\
                         "2.De-Activate\n"\
                         "3.Register\n"\
                         "4.Erase\n" );
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
        if ( userOption < 1 || userOption > 4 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }

        switch( userOption )
        {
            case eCALL_SUPS_ACTIVATE:
                req.voiceSvc = eCALL_SUPS_ACTIVATE;
                break;

            case eCALL_SUPS_DEACTIVATE:
                req.voiceSvc = eCALL_SUPS_DEACTIVATE;
                break;

            case eCALL_SUPS_REGISTER:
                req.voiceSvc = eCALL_SUPS_REGISTER;
                break;

            case eCALL_SUPS_ERASE:
                req.voiceSvc = eCALL_SUPS_ERASE;
                break;
        }
        break;
    }
    while(1)
    {
        fprintf( stderr, "Please Select Type Of Call Barring "\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.All outgoing\n"\
                         "2.Outgoing internal\n"\
                         "3.Outgoing external to home\n"\
                         "4.All incoming\n"\
                         "5.Roaming incoming\n"\
                         "6.All calls are barred\n"\
                         "7.All outgoing calls are barred\n"\
                         "8.All incoming calls are barred\n" );
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
        if ( userOption < 1 || userOption > 8 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        switch(userOption)
        {
            case eCALL_BARR_ALLOUTGOING:
                req.reason = eBARR_ALLOUTGOING;
                break;
            case eCALL_BARR_OUTGOINGINT:
                req.reason = eBARR_OUTGOINGINT;
                break;
            case eCALL_BARR_OUTGOINGINTEXTHOME:
                req.reason = eBARR_OUTGOINGINTEXTOHOME;
                break;
            case eCALL_BARR_ALLINCOMING:
                req.reason = eBARR_ALLINCOMING;
                break;
            case eCALL_BARR_INCOMINGROAMING:
                req.reason = eBARR_INCOMINGROAMING;
                break;
            case eCALL_BARR_ALLBARRING:
                req.reason = eBARR_ALLBARRING;
                break;
            case eCALL_BARR_ALLOUTGOINGBARR:
                req.reason = eBARR_ALLOUTGOINGBARRING;
                break;
            case eCALL_BARR_ALLINCOMINGBARR:
                req.reason = eBARR_ALLINCOMINGBARRING;
                break;
        }
        break;
    }
    nRet = SLQSVoiceSetSUPSService( &req , &resp );

    if( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "\nNot Able To Provide Call Barring Services\n" );
    }
    else
    {
        fprintf( stderr, "\nCall Barring Services Successful\n" );
    }
}

void CallForwardingServices(void)
{
    unsigned int len = 0;
    unsigned int numlen = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    WORD digitcount;
    BYTE CallForwardNumber[MAX_CALL_NO_LEN];
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;

    memset( &req, 0, sizeof(req));
    memset( &resp, 0, sizeof(resp) );

#if 0
    req.pCallFwdTypeAndPlan = &CallFwdTypePlan;
#else
    /* need to initialize the pCallFwdTypeAndPlan to NULL so that the corresponding TLV will not be built.
       It seems that the firmware base line does not support this TLV and will return error if this TLV was
       sent and hence caused the failure of call forward set up.
    */
    req.pCallFwdTypeAndPlan = NULL;
#endif

    req.pServiceClass = &ServiceClass;
    req.pTimerVal = &timerValue;

    while(1)
    {
        fprintf( stderr, "Please Select One Of The Following Option "\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Activate\n"\
                         "2.Deactivate\n"\
                         "3.Register\n"\
                         "4.Erase\n" );
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
        if ( userOption < 1 || userOption > 4 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        else
        {
            switch( userOption )
            {
                case eCALL_SUPS_ACTIVATE:
                    req.voiceSvc = eCALL_SUPS_ACTIVATE;
                    break;

                case eCALL_SUPS_DEACTIVATE:
                    req.voiceSvc = eCALL_SUPS_DEACTIVATE;
                    break;

                case eCALL_SUPS_REGISTER:
                    req.pCallForwardingNumber = CallForwardNumber;
                    req.voiceSvc = eCALL_SUPS_REGISTER;
                    fprintf( stderr, "Enter the Number For Forwarding "\
                                     "a Call: " );
                    fgets( (char *)CallForwardNumber,MAX_CALL_NO_LEN,stdin);
                    fprintf( stderr, "\n" );

                    numlen = strlen( (char*)CallForwardNumber );
                    CallForwardNumber[ numlen - 1 ] = '\0';

                    fprintf( stderr, "\n" );

                    for ( digitcount = 0 ; CallForwardNumber[digitcount]
                                                 != '\0'; digitcount++ )
                    {
                        req.pCallForwardingNumber[digitcount] =
                                           CallForwardNumber[digitcount];
                    }

                    break;

                case eCALL_SUPS_ERASE:
                    req.voiceSvc = eCALL_SUPS_ERASE;
                    break;
            }
            break;
        }
    }
    while(1)
    {
        fprintf( stderr, "Please Select Type Of Call Forwarding "\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Unconditional\n"\
                         "2.Mobile is Busy\n"\
                         "3.No Reply\n"\
                         "4.Unreachable\n" );
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
        if ( userOption < 1 || userOption > 4 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }

        switch( userOption )
        {
            case eCALL_FWD_UNCONDITIONAL:
                req.reason = eCALL_FWD_UNCONDITIONAL;
                break;

            case eCALL_FWD_MOBILEBUSY:
                req.reason = eCALL_FWD_MOBILEBUSY;
                break;

            case eCALL_FWD_NOREPLY:
                req.reason = eCALL_FWD_NOREPLY;
                break;

            case eCALL_FWD_UNREACHABLE:
                req.reason = eCALL_FWD_UNREACHABLE;
                break;
        }
        break;
    }
    nRet = SLQSVoiceSetSUPSService( &req , &resp );

    if( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr , "\nNot Able To Provide Call Forwarding Services\n" );
    }
    else
    {
         fprintf( stderr, "\nCall Forwarding Services Successful\n" );
    }
}

void CallWaitingServices(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    ULONG nRet;
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;

    memset( &req, 0, sizeof(req));
    memset( &resp, 0, sizeof(resp) );

    req.pCallBarringPasswd = NULL;
    req.pCallForwardingNumber = CallFwdNu;
    req.pCallFwdTypeAndPlan = &CallFwdTypePlan;
    req.pServiceClass = &ServiceClass;
    req.pTimerVal = &timerValue;
    req.reason = CALL_WAITING_REASON;

    while(1)
    {
        fprintf( stderr, "Please Select One Of The Following Option\n"\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Activate\n"\
                         "2.Deactivate\n" );
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
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        switch( userOption )
        {
            case eCALL_SUPS_ACTIVATE:
                req.voiceSvc = eCALL_SUPS_ACTIVATE;
                break;

            case eCALL_SUPS_DEACTIVATE:
                req.voiceSvc = eCALL_SUPS_DEACTIVATE;
                break;
        }
        nRet = SLQSVoiceSetSUPSService( &req , &resp );

        if( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr , "\nNot Able To Provide Call Waiting Service\n" );
        }
        else
        {
            fprintf( stderr, "\nCall Waiting Services Successful\n" );
        }
        return;
    }

}

void AutoAnswerService(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    BYTE autoAnsEnable = AUTO_ANSWER_ENABLE;
    BYTE autoAnsDisable = AUTO_ANSWER_DISABLE;
    voiceSetConfigReq  req;
    voiceSetConfigResp resp;

    memset( &req, 0, sizeof(req) );
    memset( &resp, 0, sizeof(resp) );

    while(1)
    {
        fprintf( stderr, "Please Select One Of Following Options For Auto "
                   "Answer Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Enable\n"\
                         "2.Disable\n" );
        fprintf( stderr, "Option:" );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );
        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return ;

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
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        /* Process user input */
        switch( userOption )
        {
            case eOPTION_1:
                req.pAutoAnswer = &autoAnsEnable;
                break;

            case eOPTION_2:
                req.pAutoAnswer = &autoAnsDisable;
                break;
        }
        break;
    }
    ULONG nRet = SLQSVoiceSetConfig( &req , &resp );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "\nNot Able To Set Auto Answer Service\n" );
    }
    else
    {
         fprintf( stderr, "\nAuto Answer Service Successful\n" );
    }
}

void SetVoiceSUPSService(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    while(1)
    {
        fprintf( stderr, "Please select One Of Following Option "\
                         "Or Press<Enter> To Exit:\n"\
                         "1.Call Forwarding Services\n"\
                         "2.Call Barring Services\n"\
                         "3.Call Waiting Services\n"\
                         "4.Auto Answer\n"\
                         "5.Preferred Privacy[ONLY FOR CDMA]\n" );
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
        if ( userOption < 1 || userOption > 5 )
        {
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }

        /* Process user input */
        switch( userOption )
        {
            case eCALL_FORWARD_SERVICES:
                CallForwardingServices();
                break;

            case eCALL_BARRING_SEVICES:
                CallBarringServices();
                break;

            case eCALL_WAITING_SEVICES:
                CallWaitingServices();
                break;

            case eAUTO_ANSWER:
                 AutoAnswerService();
                 break;

            case ePREFERRED_PRIVACY:
                SetPreferredPrivacy();
                break;
        }
        /* exit the loop */
        break;
    }
}

void SetPreferredPrivacy(void)
{
    unsigned int len = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    ULONG userOption;
    voiceSetPrefPrivacy req;
    while(1)
    {
        fprintf( stderr, "Please Select One Of Following Option\n"\
                         "Or Press<Enter> To Go To Previous Menu:\n"\
                         "1.Standard privacy\n"\
                         "2.Enhanced privacy\n" );
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
            fprintf( stderr, "Invalid Choice!\n" );
            continue;
        }
        /* Process user input */
        switch( userOption )
        {
            case eOPTION_1:
                req.privacyPref = VOICE_STANDARD_PRIVACY;
                break;
            case eOPTION_2:
                req.privacyPref = VOICE_ENHANCED_PRIVACY;
                break;
        }
        break;
    }
    ULONG nRet = SLQSVoiceSetPreferredPrivacy( &req );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf(stderr, "Not Able To Provide\n" );
    }
    else
    {
         fprintf( stderr, "\nProvided Preferred Privacy\n" );
    }
}

void SendFlash(void)
{
    unsigned int len = 0;
    char flashPayLoad[FLASH_PAYLOAD];
    CHAR selOption[OPTION_LEN];
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE userChoice;
    BYTE *inChar = NULL;
    BYTE simpleFlash = 0;
    BYTE actAnsHold = 1;
    BYTE deActAnsHold = 2;
    ULONG nRet;
    voiceFlashInfo req;

    memset( &req, 0, sizeof(req) );

    fprintf( stderr, "\nOn Which Call ID Do You Want To Send Flash:" );

    /* Receive the input from the user */
    fgets( selCallId, ( OPTION_LEN ), stdin );

    /* flush the input stream */
    pEndOfLine = strchr( selCallId, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
         FlushStdinStream();
    }
    len = strlen( selCallId );
    if(len==0)
    {
        fprintf( stderr, "\nError Call ID length\n" );
        return ;
    }
    /* Convert the option added by user into integer */
    selCallId[ len - 1 ] = '\0';
    userChoice = atoi( selCallId );
    req.pCallID = &userChoice;

    while(1)
    {
        fprintf( stderr, "\nPlease Select One Of The Flash Type "\
                         "Or Press<Enter> To Go To Main Menu:\n"\
                         "1.Simple\n"\
                         "2.Activate Answer Hold\n"\
                         "3.Deactivate Answer Hold\n" );
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
        userChoice = atoi( selOption );
        if ( userChoice < 1 || userChoice > 3 )
        {
            fprintf( stderr, "Invalid Flash Type!\n" );
            continue;
        }
        switch ( userChoice )
        {
            case eOPTION_1:
                req.pFlashType = &simpleFlash;
                break;
            case eOPTION_2:
                req.pFlashType = &actAnsHold;
                break;
            case eOPTION_3:
                req.pFlashType = &deActAnsHold;
                break;
        }
        break;
    }

    fprintf( stderr, "\nPlease enter the Flash Message: " );
    inChar = (BYTE *) fgets(flashPayLoad, FLASH_PAYLOAD, stdin);

    if ( NULL == inChar )
    {
         fprintf( stderr, "Entered Message Can Not Be Read");
    }
    else
    {
        req.pFlashPayLd = inChar;
        fprintf(stderr, "\n");

        nRet = SLQSVoiceSendFlash( &req );
        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr, "\nNot Able To Send Flash\n" );
        }
        else
        {
            fprintf( stderr, "\nFlash Has Been Sent\n" );
        }
    }
}

void StartDTMFServices(void)
{
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;
    voiceContDTMFinfo objvoiceStartDTMFinfo;
    CHAR selOption[OPTION_LEN];

    fprintf( stderr, "\nOn Which Call ID Do You Want To Start DTMF:" );

    /* Receive the input from the user */
    fgets( selCallId, ( OPTION_LEN ), stdin );

    /* flush the input stream */
    pEndOfLine = strchr( selCallId, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
         FlushStdinStream();
    }
    len = strlen( selCallId );
    if(len==0)
    {
        fprintf( stderr, "\nError Cell ID length\n" );
        return ;
    }
    /* Convert the option added by user into integer */
    selCallId[ len - 1 ] = '\0';
    userChoice = atoi( selCallId );

    objvoiceStartDTMFinfo.pCallID     = &userChoice;

    while(1)
    {
        fprintf( stderr, "Please select a digit\n"\
                         "Digit:");


        memset(selOption, 0, OPTION_LEN);
        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );
        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == selOption[0] )
            return;

        /* Process user input */
        switch( selOption[0] )
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '#':
            case '*':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
                objvoiceStartDTMFinfo.DTMFdigit   = selOption[0];
                break;
            default:
                fprintf( stderr, "Invalid Digit!\n" );
                continue;
        }

        ULONG nRet = SLQSVoiceStartContDTMF( &objvoiceStartDTMFinfo );
    
        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf( stderr, "\nNot Able To Provide DTMF Services\n" );
        }
        else
        {
             fprintf( stderr, "\nDTMF Started\n" );
        }
    }
}

void StopDTMFServices(void)
{
    CHAR selCallId[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE userChoice;
    voiceStopContDTMFinfo objvoiceStopDTMFinfo;

    fprintf( stderr, "\nOn Which Call ID Do You Want To Stop DTMF:" );

    /* Receive the input from the user */
    fgets( selCallId, ( OPTION_LEN ), stdin );

    /* flush the input stream */
    pEndOfLine = strchr( selCallId, ENTER_KEY );
    if( NULL == pEndOfLine )
    {
         FlushStdinStream();
    }
    len = strlen( selCallId );
    if(len==0)
    {
        fprintf( stderr, "\nError Cell ID length\n" );
        return ;
    }
    /* Convert the option added by user into integer */
    selCallId[ len - 1 ] = '\0';
    userChoice = atoi( selCallId );

    objvoiceStopDTMFinfo.callID = userChoice;

    ULONG nRet = SLQSVoiceStopContDTMF( &objvoiceStopDTMFinfo );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "\nNot Able To Stop DTMF Services\n" );
    }
    else
    {
         fprintf( stderr, "\nDTMF Stopped\n" );
    }
}

void GetCLIRServiceStatus(void)
{
    CLIRResp clirResp;
    voiceGetCLIRResp resp;

    memset( &resp, 0, sizeof(resp) );

    resp.pCLIRResp = &clirResp;

    ULONG nRet = SLQSVoiceGetCLIR( &resp );

    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Failure\n" );
    }
    if ( NULL != resp.pCLIRResp )
    {
        fprintf( stderr, "CLIR Response \n" );
        fprintf( stderr, "Activation Status  : %x\n",
                        resp.pCLIRResp->ActiveStatus );
        fprintf( stderr, "Provision Status   : %x\n",
                        resp.pCLIRResp->ProvisionStatus );
    }
    else
        fprintf( stderr, "CLIR Response Is Not Available\n" );
}

void GetCLIPServiceStatus(void)
{
    CLIPResp    clipResp;
    voiceGetCLIPResp resp;

    memset( &resp, 0, sizeof(resp) );

    resp.pCLIPResp    = &clipResp;
    ULONG nRet = SLQSVoiceGetCLIP( &resp );
    if ( eQCWWAN_ERR_NONE != nRet )
    {
        fprintf( stderr, "Failure\n" );
    }

    if ( NULL != resp.pCLIPResp )
    {
        fprintf( stderr, "CLIP Response \n");
        fprintf( stderr, "Activation Status  : %x\n",
                     resp.pCLIPResp->ActiveStatus );
        fprintf( stderr, "Provision Status   : %x\n",
                     resp.pCLIPResp->ProvisionStatus );
     }
    else
        fprintf( stderr, "CLIP Response Is Not Available\n" );
}

void SendFlashForWaitingCall(void)
{
    ULONG nRet;
    voiceFlashInfo req;
    BYTE userChoice = 1;
    memset( &req, 0, sizeof(req) );
    req.pCallID = &userChoice;
    nRet = SLQSVoiceSendFlash( &req );
    if(nRet == 0)
        fprintf(stderr,"Waiting Call Retrieved \n");
    else
        fprintf(stderr,"Unable to retrieve \n ");
}

