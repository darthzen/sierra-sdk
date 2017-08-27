/*************
 *
 * Filename:    displaymgmnt.c
 *
 * Purpose:     Voice Sample Application
 *
 * Copyright:   © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include "callhandling.h"

static tFNcallStateInfo fnCbk = NULL;

/****************************************************************************
*                       Functions
*****************************************************************************/
void DisplayCallInfo( voiceSetAllCallStatusCbkInfo *pCallBackInfo )
{
    voiceSetAllCallStatusCbkInfo *pInfo;
    callInfoVb dCallInfo[MAX_NO_OF_CALLS];
    int w10 = -10, w20 = -20;
    WORD lC,lC1,lC2;

    /* Initialise the callback pointer */
    pInfo = pCallBackInfo;

    /* Intialize the Image info buffer */
    memset( (void *)dCallInfo, 0, sizeof( dCallInfo[MAX_NO_OF_CALLS] ) );
    fprintf( stderr, "\n%3$*1$s%4$*2$s%5$*1$s%6$*1$s%7$*1$s%8$*2$s\n",
             w10, w20,
             "Call Id",
             "Call State",
             "Type",
             "Direction",
             "Mode",
             "Call Number" );

    for ( lC = 0; lC < pInfo->arrCallInfomation.numInstances ; lC++ )
    {
        getAllCallInformation *pCallInfo =
                       &pInfo->arrCallInfomation.getAllCallInfo[lC];

        /* Get the Call Id */
        dCallInfo[lC].callId = pCallInfo->Callinfo.callID;

        /* Get the Call State */
        switch ( pCallInfo->Callinfo.callState )
        {
            case eCALL_STATE_ORIGINATION:
                strcpy(dCallInfo[lC].callState, "Originating");
                break;

            case eCALL_STATE_INCOMING:
                strcpy(dCallInfo[lC].callState, "Incoming");
                break;

            case eCALL_STATE_CONNECTED:
                strcpy(dCallInfo[lC].callState, "Active");
                break;

            case eCALL_STATE_RINGING:
                strcpy(dCallInfo[lC].callState, "Ringing");
                break;

            case eCALL_STATE_HOLD:
                strcpy(dCallInfo[lC].callState, "Hold");
                break;

            case eCALL_STATE_WAITING:
                strcpy(dCallInfo[lC].callState, "Waiting");
                break;

            case eCALL_STATE_DISCONNECTED:
                strcpy(dCallInfo[lC].callState, "Disconnected");
                /* report the call state to the main application */
                fnCbk(TRUE);
                break;

            case eCALL_STATE_END:
                strcpy(dCallInfo[lC].callState, "End Call");
                /* report the call state to the main application */
                fnCbk(TRUE);
                break;
        }

        /* Get the Call Type */
        switch ( pCallInfo->Callinfo.callType )
        {
            case CALL_TYPE_VOICE:
                strcpy(dCallInfo[lC].callType, "Voice");
                break;
        }

        /* Get the Call Direction */
        switch ( pCallInfo->Callinfo.direction )
        {
            case MOBILE_ORIGINATED:
                strcpy(dCallInfo[lC].direction, "MO");
                break;

            case MOBILE_TERMINATED:
                strcpy(dCallInfo[lC].direction, "MT");
                break;
        }

        /* Get the Mode */
        switch ( pCallInfo->Callinfo.mode )
        {
            case eCALL_MODE_CDMA:
                strcpy(dCallInfo[lC].mode, "CDMA");
                break;

            case eCALL_MODE_GSM:
                strcpy(dCallInfo[lC].mode, "GSM");
                break;

            case eCALL_MODE_UMTS:
                strcpy(dCallInfo[lC].mode, "UMTS");
                break;

            case eCALL_MODE_LTE:
                strcpy(dCallInfo[lC].mode, "LTE");
                break;

            case eCALL_MODE_TD_SCDMA:
                strcpy(dCallInfo[lC].mode, "TD-SCDMA");
                break;
        }

        /* Get the Call Number */
        if ( pInfo->pArrRemotePartyNum )
        {
            for ( lC1 = 0;lC1 < pInfo->pArrRemotePartyNum->numInstances;lC1++ )
            {
                getAllCallRmtPtyNum *pRmtPtyNum =
                             &pInfo->pArrRemotePartyNum->RmtPtyNum[lC1];

                if( dCallInfo[lC].callId == pRmtPtyNum->callID )
                {
                    for ( lC2=0 ;lC2 < pRmtPtyNum->RemotePartyNum.numLen;lC2++)
                    {
                        dCallInfo[lC].number[lC2] =
                               pRmtPtyNum->RemotePartyNum.remPartyNumber[lC2];
                    }
                    dCallInfo[lC].number[lC2] = EOS;
                }
            }
        }

        fprintf( stderr, "%3$*1$x%4$*2$s%5$*1$s%6$*1$s%7$*1$s%8$*2$s\n",
                 w10, w20,
                 dCallInfo[lC].callId,
                 dCallInfo[lC].callState,
                 dCallInfo[lC].callType,
                 dCallInfo[lC].direction,
                 dCallInfo[lC].mode,
                 dCallInfo[lC].number );
    }

    fprintf( stderr, "______________________________________"\
                     "_________________________________\n");

    return;
}

void DisplayRecInfo( voiceInfoRec *pCallBackInfo )
{
    voiceInfoRec *pInfo;
    BYTE count = 0;
    int w10 = -10, w20 = -20;
    BYTE len = 0;

    pInfo = pCallBackInfo;
    fprintf( stderr, "\n%3$*1$s%4$*2$s%5$*1$s%6$*1$s%7$*1$s%8$*2$s\n",
             w10, w20,
             "Call Id",
             "Call State",
             "Type",
             "Direction",
             "Mode",
             "Call Number" );
    fprintf( stderr, "%3$*1$x%4$*2$s%5$*1$s%6$*1$s%7$*1$s",
             w10, w20,
             pInfo->callID,
             "Waiting",
             "Voice",
             "--",
             "CDMA" );

    /*Print the call number*/
    if (pInfo->pCallingPartyInfo != NULL)
    {
        len = pInfo->pCallingPartyInfo->callerIDLen;
        pInfo->pCallingPartyInfo->callerID[len] = EOS;
        for(count =0; count<pInfo->pCallingPartyInfo->callerIDLen;count++)
        {
        	  if (pInfo->pCallingPartyInfo->callerID[count] >= ASCII_0)
        	  {
               fprintf( stderr, "%x",pInfo->pCallingPartyInfo->callerID[count]-ASCII_0);
        	  }
        }
    }

    fprintf( stderr, "\n______________________________________"\
                     "_________________________________\n");

    fprintf(stderr,
            "\nAn incoming Call is waiting ,"
            " Enter your option (y - accept or n - reject ) : ");

}

void DisplayNotificationInfo(voiceSUPSNotification *pNotificationTypeInfo )
{
    voiceSUPSNotification *pNInfo = pNotificationTypeInfo;

    if( eCALL_IS_ON_HOLD == pNInfo->notifType )
    {
        fprintf( stderr, "\nCall Id %d Is On Hold ..\n", pNInfo->callID );
    }

    if( eCALL_IS_RETRIEVED == pNInfo->notifType )
    {
        fprintf( stderr, "\nCall Id %d is Retrieved ..\n", pNInfo->callID );
    }
}

void SetCallStateReportCallback(tFNcallStateInfo pCallback)
{
     fnCbk = pCallback;
}
