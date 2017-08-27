/**************
 *
 * Filename: qaQmiNotify.h
 *
 * Purpose:  Header file for QMI Notify interface
 *
 * Copyright: © 2011-2014 Sierra Wireless, Inc. all rights reserved
 *
 **************/
#ifndef __QMI_NOTIFY_H__
#define __QMI_NOTIFY_H__

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include "qmerrno.h"
#include "qaCbkDmsEventReportInd.h"
#include "qaCbkDmsSwiEventReportInd.h"
#include "qaCbkDmsSwiGetResetInfoInd.h"
#include "qaCbkWdsEventReportInd.h"
#include "qaCbkWdsGetPktSrvcStatusInd.h"
#include "qaCbkNasServingSystemInd.h"
#include "qaCbkNasEventReportInd.h"
#include "qaCbkNasSystemSelectionPreferenceInd.h"
#include "qaCbkNasSysInfoInd.h"
#include "qaCbkNasNetworkTimeInd.h"
#include "qaCbkPdsEventReportInd.h"
#include "qaCbkPdsGpsServiceStateInd.h"
#include "qaCbkWmsEventReportInd.h"
#include "qaCbkCatEventReportInd.h"
#include "qaCbkSwiOmaDmEventReportInd.h"
#include "qaCbkDcsEventReportInd.h"
#include "qaCbkFmsEventReportInd.h"
#include "qaCbkOmaDmEventReportInd.h"
#include "qaCbkVoiceUssdInd.h"
#include "qaCbkVoiceSUPSNotificationInd.h"
#include "qaCbkVoiceSLQSVoiceAllCallStatusInd.h"
#include "qaCbkWmsTransLayerInfoInd.h"
#include "qaCbkWmsTransNWRegInfoInd.h"
#include "qaCbkUimSLQSUimSetStatusChangeInd.h"
#include "qaCbkUimSLQSUimSetRefreshInd.h"
#include "qaCbkVoiceSLQSVoicePrivacyInd.h"
#include "qaCbkVoiceSLQSVoiceDTMFInd.h"
#include "qaCbkVoiceSLQSVoiceSUPSInd.h"
#include "qaCbkWmsMemoryFullInd.h"
#include "qaCbkVoiceSLQSVoiceSetOTASPStatusInd.h"
#include "qaCbkVoiceSLQSVoiceInfoRecInd.h"
#include "qaCbkWmsMessageWaitingInd.h"
#include "qaCbkQosNetworkStatusInd.h"
#include "qaCbkQosEventReportInd.h"
#include "qaCbkQosFlowStatusInd.h"
#include "qaCbkQosPriEventInd.h"
#include "qaCbkNasSigInfoInd.h"
#include "qaCbkNasSwiHDRPersonalityInd.h"
#include "qaCbkImsSLQSSIPCfgInd.h"
#include "qaCbkImsSLQSRegMgrCfgInd.h"
#include "qaCbkImsSLQSSMSCfgInd.h"
#include "qaCbkImsSLQSUserCfgInd.h"
#include "qaCbkImsSLQSVoIPCfgInd.h"
#include "qaCbkWdsDUNCallInfoInd.h"
#include "qaCbkWmsAsyncRawSendInd.h"
#include "qaCbkNasSwiOTAMessageInd.h"
#include "qaCbkWdsSWIRMTransferStatisticsInd.h"
#include "qaCbkLocCradleMountInd.h"
#include "qaCbkLocEventTimeSyncInd.h"
#include "qaCbkLocInjectTimeInd.h"
#include "qaCbkLocSensorStreamInd.h"
#include "qaCbkLocSensorDataInd.h"
#include "qaCbkLocPositionRptInd.h"
#include "qaCbkImsaSLQSRatStatusInd.h"
#include "qaCbkImsaSLQSRegStatusInd.h"
#include "qaCbkImsaSLQSSvcStatusInd.h"
#include "qaCbkImsaSLQSPdpStatusInd.h"
#include "qaCbkNasLteCPHYCAInd.h"
#include "qaCbkNasSwiRankIndicatorInd.h"
#include "qaCbkWdsSWIDHCPv4ClientLeaseInd.h"
#include "qaCbkUimSlotStatusChangeInd.h"
#include "qaCbkNasTimerInd.h"

/*
 * Name:    eQMICallbackIndex
 *
 * Purpose: Enumeration used to list Callback Index in the List. The enum values
 *          has been used in service levels to categorize better.
 * Note:    This listing should match array qaCallbackList
 */
enum eQMICallbackIndex{
    eQMI_CB_START = 0,

    /*---------------------
      WDS Service Callbacks
     ----------------------*/

    /* WDS_PKT_SRVC_STATUS_IND */
    eQMI_CB_SESSION_STATE = eQMI_CB_START, /* Data Bearer Callback */
    /* WDS_EVENT_REPORT_IND */
    eQMI_CB_WDS_START = eQMI_CB_SESSION_STATE,
    eQMI_CB_PKT_SERV_STATUS,              /* Packet Service Stat Cbk */
    eQMI_CB_MOBILE_IP,                    /* Mobile IP Callback */
    eQMI_CB_WDS_EVENT_0,                  /* 1st inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_1,                  /* 2nd inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_2,                  /* 3rd inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_3,                  /* 4th inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_4,                  /* 5th inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_5,                  /* 6th inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_6,                  /* 7th inst. Byte Totals Callback */
    eQMI_CB_WDS_EVENT_7,                  /* 8th inst. Byte Totals Callback */
    eQMI_CB_WDS_XFER_STATS_0,             /* 1st xfer stats callback */
    eQMI_CB_WDS_XFER_STATS_1,             /* 2nd xfer stats callback */
    eQMI_CB_WDS_DUN_CALL_INFO,            /* DUN Call Info Callback */
    eQMI_CB_WDS_DATA_SYS_STAT,            /* Data System Status Callback */
    eQMI_CB_WDS_DHCPV4_CLIENT_LEASE_0,    /* 1st inst. DHCPv4 Lease Status Callback */
    eQMI_CB_WDS_DHCPV4_CLIENT_LEASE_1,    /* 2nd inst. DHCPv4 Lease Status Callback */
    eQMI_CB_WDS_END = eQMI_CB_WDS_DHCPV4_CLIENT_LEASE_1,

    /*---------------------
      DMS Service Callbacks
     ----------------------*/

    /* DMS_EVENT_REPORT_IND */
    eQMI_CB_ACTIVATION_STATUS,                  /* Activation Status Callback */
    eQMI_CB_DMS_START =
        eQMI_CB_ACTIVATION_STATUS,
    eQMI_CB_POWER,                              /* Power Callback */
    eQMI_CB_MODEM_TEMP,                         /* Modem temperature Callback */
    eQMI_CB_SWI_GET_RESET_INFO_IND,             /* Get Reset Info Callback*/
    eQMI_CB_DMS_END = eQMI_CB_SWI_GET_RESET_INFO_IND,

    /*---------------------
      NAS Service Callbacks
     ----------------------*/

    /* NAS_SERVING_SYSTEM_IND */
    eQMI_CB_ROAMING_INDICATOR,                  /* Roaming Indicator Callback */
    eQMI_CB_NAS_START =
        eQMI_CB_ROAMING_INDICATOR,
    eQMI_CB_DATA_CAPABILITIES,                  /* Data Capabilities Callback */
    /* NAS_EVENT_REPORT_IND */
    eQMI_CB_SIGNAL_STRENGTH,                    /* Signal Strength Callback */
    eQMI_CB_RF_INFO,                            /* RFT Info Callback */
    eQMI_CB_RSSI_INFO,                          /* Received Signal Strength */
    eQMI_CB_LU_REJECT,                          /* LU Reject Callback */
    eQMI_CB_NAS_SERV_SYS,                       /* Serving System Callback */
    eQMI_CB_NAS_BAND_PREF,                      /* Band Preference Callback */
    eQMI_CB_NAS_SYS_SEL_PREFERENCE,             /* System Selection Pref. */
    eQMI_CB_NAS_SYS_INFO_IND,                   /* System Info Indication */
    eQMI_CB_NAS_NETWORK_TIME_IND,               /* Network Time Indication */
    eQMI_CB_NAS_SIG_INFO_IND,                   /* Signal Strength Indication */
    eQMI_CB_NAS_HDR_PERS_IND,                   /* HDR Personality Indication*/
    eQMI_CB_NAS_END = eQMI_CB_NAS_HDR_PERS_IND,
    eQMI_CB_NAS_SWI_OTA_MESSAGE_IND,            /* Swi OTA message indication */
    eQMI_CB_NAS_LTE_CPHY_CA_INFO_IND,           /* Previous Carrier Aggregation event  indication */
    eQMI_CB_NAM_SWI_RANK_INDICATOR_IND,         /* Swi Rank Indicator indication */
    eQMI_CB_NAS_TIMER_IND,                      /* Nas Timer Indication */
    eQMI_CB_NAS_NETWORK_REJECT_IND,             /* Network Reject Cause Indication */
    /*---------------------
      QOS Service Callbacks
     ----------------------*/

    /* QOS_NETWORK_STATUS_IND */
    eQMI_CB_QOS_NETWORK_STATUS,                 /* QOS Network status Callback */

    /* QOS_EVENT_REPORT_IND */
    eQMI_CB_QOS_EVENT_0,                  /* 1st inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_1,                  /* 2nd inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_2,                  /* 3rd inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_3,                  /* 4th inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_4,                  /* 5th inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_5,                  /* 6th inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_6,                  /* 7th inst. Set Qos Event Report Callback */
    eQMI_CB_QOS_EVENT_7,                  /* 8th inst. Set Qos Event Report Callback */

    /* QMI_QOS_STATUS_IND */
    eQMI_CB_QOS_FLOW_STATUS_EVENT_0,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_1,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_2,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_3,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_4,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_5,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_6,
    eQMI_CB_QOS_FLOW_STATUS_EVENT_7,

    /* QMI_QOS_PRIMARY_QOS_EVENT_IND */
    eQMI_CB_QOS_PRI_FLOW,

    /*---------------------
      PDS Service Callbacks
     ----------------------*/

    /* PDS_SERVING_SYSTEM_IND */
    eQMI_CB_NMEA,                               /* NMEA Callback */
    eQMI_CB_PDS_START = eQMI_CB_NMEA,
    eQMI_CB_NMEA_PLUS,                          /* NMEA Plus Callback */
    /* PDS_GPS_SERVICE_STATE_IND */
    eQMI_CB_PDS_STATE,                          /* PDS State Callback */
    eQMI_CB_PDS_END = eQMI_CB_PDS_STATE,

    /*---------------------
      WMS Service Callbacks
     ----------------------*/

    /* WMS_EVENT_REPORT_IND */
    eQMI_CB_NEW_SMS,                         /* New SMS Callback */
    eQMI_CB_WMS_START = eQMI_CB_NEW_SMS,
    eQMI_CB_WMS_TRANS_LAYER_INFO_IND,        /* Trans Layer info callback */
    eQMI_CB_WMS_TRANS_NW_REG_INFO_IND,       /* Trans NW Reg info callback */
    eQMI_CB_SMS_EVENT,                       /* SMS Event Callback */
    eQMI_CB_WMS_MEMORY_FULL,                 /* SMS Memory Full Callback*/
    eQMI_CB_WMS_MESSAGE_WAITING_IND,         /* Message Waiting Callback*/
    eQMI_CB_WMS_ASYNC_RAW_SEND_IND,          /* Async Raw Send Callback*/
    eQMI_CB_WMS_END = eQMI_CB_WMS_ASYNC_RAW_SEND_IND,

    /*---------------------
      CAT Service Callbacks
     ----------------------*/

    /* CAT_EVENT_REPORT_IND */
    eQMI_CB_CAT_EVENT,                          /* CAT Event Callback */
    eQMI_CB_CAT_START = eQMI_CB_CAT_EVENT,
    eQMI_CB_CAT_END = eQMI_CB_CAT_START,

    /*---------------------
      OMA Service Callbacks
     ----------------------*/

    /* OMA_EVENT_REPORT_IND */
    eQMI_CB_OMADM_ALERT,                        /* Roaming Indicator Callback */
    eQMI_CB_OMADM_START = eQMI_CB_OMADM_ALERT,
    eQMI_CB_OMADM_STATE,                        /* Roaming Indicator Callback */
    eQMI_CB_OMADM_END = eQMI_CB_OMADM_STATE,

    /*------------------------
      SWIOMA Service Callbacks
     ------------------------*/

    /* SWIOMA_EVENT_REPORT_IND */
    eQMI_CB_SWIOMADM_ALERT,                    /* OMADM Alert Callback */
    eQMI_CB_SWIOMADM_START = eQMI_CB_SWIOMADM_ALERT,
    eQMI_CB_SWIOMADM_STATE,                    /* OMADM State Callback */
    eQMI_CB_SWIOMADM_END = eQMI_CB_SWIOMADM_STATE,

    /*------------------------
      DCS Service Callbacks
     ------------------------*/

    /* DCS_EVENT_REPORT_IND */
    eQMI_CB_DCS_DEVICE_STATE_CHANGE,            /* Device State Callback */
    eQMI_CB_DCS_START = eQMI_CB_DCS_DEVICE_STATE_CHANGE,
    eQMI_CB_DCS_SDK_TERMINATED,                 /* SDK terminated Callback */
    eQMI_CB_DCS_NET_0,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_1,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_2,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_3,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_4,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_5,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_6,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_NET_7,                            /* Net Tx Queue Callback */
    eQMI_CB_DCS_END = eQMI_CB_DCS_NET_7,
    /*------------------------
      FMS Service Callbacks
     ------------------------*/

    /* FMS_EVENT_REPORT_IND */
    eQMI_CB_FMS_FW_DWLD_STATUS,
    eQMI_CB_FMS_START = eQMI_CB_FMS_FW_DWLD_STATUS,
    eQMI_CB_FMS_END   = eQMI_CB_FMS_FW_DWLD_STATUS,

    /*-----------------------
      Voice Service Callbacks
     ------------------------*/
    /* VOICE_USSD_IND */
    eQMI_CB_VOICE_USSD_RELEASE_IND,          /* USSD Release callback */
    eQMI_CB_VOICE_START = eQMI_CB_VOICE_USSD_RELEASE_IND,
    eQMI_CB_VOICE_USSD_IND,                  /* USSD Notification callback */
    eQMI_CB_VOICE_USSD_NO_WAIT_IND,          /* USSD Indication No Wait call back */
    eQMI_CB_VOICE_SUPS_NOTIFICATION_IND,     /* SUPS Notification callback */
    eQMI_CB_VOICE_ALL_CALL_STATUS,           /* Call Status Indication */
    eQMI_CB_VOICE_PRIVACY_IND,               /* Voice Privacy Indication */
    eQMI_CB_VOICE_DTMF_IND,                  /* DTMF Event Indication */
    eQMI_CB_VOICE_SUPS_IND,                  /* SUPS Indication */
    eQMI_CB_VOICE_OTASP_STATUS_IND,          /* OTASP/OTAPA Event Indication */
    eQMI_CB_VOICE_INFO_REC_IND,              /* Info Record Indication */
    eQMI_CB_USSD_END = eQMI_CB_VOICE_INFO_REC_IND,

    /*-----------------------
     UIM Service Callbacks
    ------------------------*/
    eQMI_CB_UIM_STATUS_CHANGE_IND,          /* UIM Status Change Callback */
    eQMI_CB_UIM_REFRESH_IND,                /* UIM Refresh CAllback */
    eQMI_CB_UIM_SLOT_STATUS_CHANGE_IND,     /* UIM Slot Status Change Callback */
    eQMI_CB_UIM_END = eQMI_CB_UIM_SLOT_STATUS_CHANGE_IND,

    /*-----------------------
      IMS Service Callbacks
     ------------------------*/
    /* VOICE_USSD_IND */
    eQMI_CB_IMS_SIP_CONFIG_IND,          /* SIP Configuration callback */
    eQMI_CB_IMS_START = eQMI_CB_IMS_SIP_CONFIG_IND,
    eQMI_CB_IMS_REG_MGR_CONFIG_IND,
    eQMI_CB_IMS_SMS_CONFIG_IND,
    eQMI_CB_IMS_USER_CONFIG_IND,
    eQMI_CB_IMS_VOIP_CONFIG_IND,
    eQMI_CB_IMS_END = eQMI_CB_IMS_VOIP_CONFIG_IND,
    eQMI_CB_GPS, 
    eQMI_CB_RM_TRANSFER_STATISTICS,

    /*---------------------
      LOC Service Callbacks
     ----------------------*/
    eQMI_CB_LOC_CRADLE_MOUNT_IND,          /* Cradle Mount Configuration callback */
    eQMI_CB_LOC_EVENT_TIME_SYNC_IND,       /* Event Time Sync callback */
    eQMI_CB_LOC_INJECT_TIME_IND,           /* Inject Time  callback */
    eQMI_CB_LOC_SENSOR_STREAMING_IND,      /* Sensor Streaming callback */
    eQMI_CB_LOC_SENSOR_DATA_IND,           /* Inject Sensor Data callback */
    eQMI_CB_LOC_POSITION_REPORT_IND,       /* Event Position Report callback */
    eQMI_CB_LOC_GNSS_SV_INFO_IND,          /* GNSS SV Info callback */
    eQMI_CB_LOC_OP_MODE_IND,               /* Set Operation mode callback */
    eQMI_CB_LOC_DELETE_ASSIST_DATA_IND,    /* Delete Assist Data callback */
    eQMI_CB_LOC_INJECT_UTC_TIME_IND,       /* Inject UTC Time callback */
    eQMI_CB_LOC_INJECT_POSITION_IND,       /* Inject Position callback */
    eQMI_CB_LOC_ENGINE_STATE_IND,          /* Engine State callback */
    eQMI_CB_LOC_SET_EXT_POWER_CONFIG_IND,  /* Set External Power Status Callback*/
    eQMI_CB_LOC_BEST_AVAIL_POS_IND,        /* Set best available position callback */
    eQMI_CB_LOC_EVENT_NMEA_IND,            /* LOC NMEA Event callback*/
    eQMI_CB_LOC_GET_SERVER_IND,           /* Get Server callback */
    /*-----------------------
      IMSA Service Callbacks
     ------------------------*/
    eQMI_CB_IMSA_REG_STATUS_IND,          /* Registration Status callback */
    eQMI_CB_IMSA_START = eQMI_CB_IMSA_REG_STATUS_IND,
    eQMI_CB_IMSA_SVC_STATUS_IND,          /* Service Status callback */
    eQMI_CB_IMSA_RAT_HANDOVER_STATUS_IND, /* RAT Handover Status callback */
    eQMI_CB_IMSA_PDP_STATUS_IND,             /* PDP Status Callback   */
    eQMI_CB_IMSA_END = eQMI_CB_IMSA_PDP_STATUS_IND,
    /*-----------------------
      TMD Service Callbacks
     ------------------------*/
    eQMI_CB_TMD_MITIGATION_LVL_RPT_IND,          /* Mitigation level report callback */
    /*----------------------
      SWI AVMS Service Callback
    --------------------------*/
    eQMI_CB_SWI_AVMS_REPORT_IND,                /* AVMS Event Report */
    eQMI_CB_END                                 /* End of Callback list */
};

/*
 * Name:    eQmiCbkSetStatus
 *
 * Purpose: Enumeration used to identify if the parameters for a callback
 *          function need to be set.
 */
enum eQmiCbkSetStatus{
    QMI_CBK_PARAM_RESET = 0,
    QMI_CBK_PARAM_SET   = 1,
    QMI_CBK_PARAM_NOCHANGE
};

/*
 * Name:    eQMICbkState
 *
 * Purpose: Enumeration used to track the state of Callback service.
 */
enum eQMICbkState{
    eQMI_CBK_INIT,
    eQMI_CBK_LISTENING
};

/* Set<>Callback function prototype */
typedef ULONG (* pCbkType)(void *);

/*************
 *
 * Name:    qaCallbackInfo
 *
 * Purpose: Structure for storing callback function data
 *
 * Members: pCallback      - Callback pointer
 *          pSetCallback   - Function pointer to enable the callback
 *          pCallbackCache - Function pointer to save callback provided by the
 *                           user in case device resets
 *
 * Notes:
 *
 **************/
struct qaCallbackInfo{
    void     *pCallback;
    pCbkType pSetCallback;
    void     *pCallbackCache;
};

/*
 * Name:    QmiNotification
 *
 * Purpose: Structure used to store all QMI Notification parameters.
 *
 * Members: eCbkIndex                            - Index to the Callback List
 *          QmiInd                               - Union Containing the QMI
 *                                                 indications
 *          QmiWdsEventStatusReportInd           - WDS Evevt Indication
 *                                                 Parameters
 *          qaQmiWdsSessionStateInd              - WDS Session State Indication
 *                                                 Parameters
 *          qaQmiCbkDmsEventStatusReportInd      - DMS Event Indication
 *                                                 Parameters
 *          qaQmiCbkNasServingSystemInd          - NAS Serving System Indication
 *                                                 Parameters
 *          qaQmiCbkNasEventStatusReportInd      - NAS Event Indication
 *                                                 Parameters
 *          qaQmiCbkNasSystemSelPrefInd          - NAS System Selection
 *                                                 Preference Indication
 *                                                 parameters
 *          QmiCbkQosNetworkStatusInd            - QOS Evevt Indication
 *                                                 Parameters
 *          qaQmiCbkPdsEventStatusReportInd      - PDS Event Indication
 *                                                 Parameters
 *          qaQmiCbkPdsGpsServiceStatusInd       - PDS GPS Service State
 *                                                 Indication Parameters
 *          qaQmiCbkWmsEventReportInd            - WMS Event Indication
 *                                                 parameters
 *          qaQmiCbkCatEventStatusReportInd      - CAT Event Indication
 *                                                 parameters
 *          qaQmiCbkSwiOmaDmEventStatusReportInd - SWIOMA Event Indication
 *                                                 parameters
 *          qaQmiCbkDcsEventStatusReportInd      - DCS Event Indication
 *                                                 parameters
 *          qaQmiCbkFmsEventStatusReportInd      - FMS Event Indication
 *                                                 parameters
 *          qaQmiCbkOmaDmEventStatusReportInd    - OMA Event Indication
 *                                                 parameters
 *          qaQmiCbkVoiceUssdInd                 - VOICE Event Indication
 *                                                 parameters
 *          qaQmiCbkVoiceSUPSNotificationInd     - SUPS Notification Indication
 *                                                 parameters
 *          qaQmiCbkVoiceAllCallStatusInd        - VOICE All Call Status
 *                                                 Indication parameters
 *          qaQmiCbkTransLayerNotificationInd    - WMS Transport Layer Info
 *                                                 Indication parameters
 *          qaQmiCbkTransNWRegNotificationInd    - WMS Transport Network Reg
 *                                                 notification indication
 *
 *          qaQmiCbkUIMRefreshInd                - UIM refresh Indication
 *
 *          qaQmiCbkUIMStatusChangeInd           - UIM status Change Indication
 *
 *          qaQmiCbkVoicePrivacyInd              - Voice Privacy Change
 *                                                 Indication parameters
 *
 *          qaQmiCbkVoiceDTMFEventInd            - Voice DTMF Event Indication
 *
 *          qaQmiCbkVoiceSupsInd                 - Voice SUPS Indication
 *
 *          qaQmiCbkNasSysInfo                   - System Info Indication
 *
 *          qaQmiCbkNasNetworkTime               - Network Time Indication
 *
 *          qaQmiCbkWmsMemoryFull                - Memory Full Indication
 *
 *          qaQmiCbkVoiceOTASPStatusInd          - OTASP Status Indication
 *
 *          qaQmiCbkVoiceInfoRecInd              - Info Record Indication
 *
 *          qaQmiCbkWmsMessageWaitingInd         - Message Waiting Info Indication
 *
 *          qaQmiCbkNasSigInfo                   - Signal Info Indication
 *
 *          qaQmiCbkDmsSwiEventStatusReportInd   - SWI DMS Event Indication
 *                                                 parameters
 *
 *          qaQmiCbkNasSwiHDRPersonalityInd      - SWI HDR Personality Indication
 *
 *          qaQmiCbkImsSIPConfigInfo             - SIP Config Indication
 *
 *          qaQmiCbkImsRegMgrConfigInfo          - Reg Mgr Config Indication
 *
 *          qaQmiCbkImsSMSConfigInfo             - SMS Config Indication
 *
 *          qaQmiCbkImsUserConfigInfo            - User Config Indication
 *
 *          qaQmiCbkImsVoIPConfigInfo            - VoIP Config Indication
 *
 *          qaQmiCbkWdsDunCallInfo               - DUN Call Info
 *
 *          qaQmiCbkSwiAvmsEventReportInd        - AVMS Info Indication
 *
 * Note:    None
 */
struct QmiNotification
{
    qaQmiInterfaceInfo qmiInterfaceInfo;
    enum   eQMICallbackIndex  eCbkIndex;
    union
    {
        struct QmiCbkWdsEventStatusReportInd
                                    qaQmiCbkWdsEventStatusReportInd;

        struct QmiCbkWdsSessionStateInd  qaQmiCbkWdsSessionStateInd;

        struct QmiCbkDmsEventStatusReportInd
                                    qaQmiCbkDmsEventStatusReportInd;

        struct QmiCbkNasServingSystemInd qaQmiCbkNasServingSystemInd;

        struct QmiCbkNasEventStatusReportInd
                                    qaQmiCbkNasEventStatusReportInd;

        struct QmiCbkNasSystemSelPrefInd
                                    qaQmiCbkNasSystemSelPrefInd;

        struct QmiCbkQosNetworkStatusInd  qaQmiCbkQosNetworkStatusInd;

        struct QmiCbkQosEventReportInd  qaQmiCbkQosEventReportInd;

        struct QmiCbkQosFlowStatusInd  qaQmiCbkQosFlowStatusInd;
        struct QmiCbkQosPriEventInd  qaQmiCbkQosPriEventInd;

        struct QmiCbkPdsEventStatusReportInd
                                    qaQmiCbkPdsEventStatusReportInd;

        struct QmiCbkPdsGpsServiceStatusInd
                                    qaQmiCbkPdsGpsServiceStatusInd;

        struct QmiCbkWmsEventReportInd
                                    qaQmiCbkWmsEventReportInd;

        struct QmiCbkCatEventStatusReportInd
                                    qaQmiCbkCatEventStatusReportInd;

        struct QmiCbkSwiOmaDmEventStatusReportInd
                                    qaQmiCbkSwiOmaDmEventStatusReportInd;

        struct QmiCbkDcsEventStatusReportInd
                                    qaQmiCbkDcsEventStatusReportInd;

        struct QmiCbkFmsEventStatusReportInd
                                    qaQmiCbkFmsEventStatusReportInd;

        struct QmiCbkOmaDmEventStatusReportInd
                                    qaQmiCbkOmaDmEventStatusReportInd;

        struct QmiCbkVoiceUssdInd   qaQmiCbkVoiceUssdInd;

        voiceSUPSNotification       qaQmiCbkVoiceSUPSNotificationInd;

        voiceSetAllCallStatusCbkInfo
                                    qaQmiCbkVoiceAllCallStatusInd;

        transLayerNotification      qaQmiCbkTransLayerNotificationInd;

        transNWRegInfoNotification  qaQmiCbkTransNWRegNotificationInd;

        UIMRefreshEvent             qaQmiCbkUIMRefreshInd;

        UIMStatusChangeInfo         qaQmiCbkUIMStatusChangeInd;
        voicePrivacyInfo            qaQmiCbkVoicePrivacyInd;

        voiceDTMFEventInfo          qaQmiCbkVoiceDTMFEventInd;

        voiceSUPSInfo               qaQmiCbkVoiceSupsInd;

        nasSysInfo                  qaQmiCbkNasSysInfo;

        nasNetworkTime              qaQmiCbkNasNetworkTime;

        SMSMemoryInfo               qaQmiCbkWmsMemoryFull;

        voiceOTASPStatusInfo        qaQmiCbkVoiceOTASPStatusInd;

        voiceInfoRec                qaQmiCbkVoiceInfoRecInd;

        msgWaitingInfo              qaQmiCbkWmsMessageWaitingInd;

        nasSigInfo                  qaQmiCbkNasSigInfo;

        struct QmiCbkDmsSwiEventStatusReportInd
                                    qaQmiCbkDmsSwiEventStatusReportInd;

        struct QmiCbkNasSwiHDRPersonalityInd
                                    qaQmiCbkNasSwiHDRPersonalityInd;

        struct QmiCbkImsSIPConfigInd
                                    qaQmiCbkImsSIPConfigInfo;

        struct QmiCbkImsRegMgrConfigInd
                                    qaQmiCbkImsRegMgrConfigInfo;

        struct QmiCbkImsSMSConfigInd
                                    qaQmiCbkImsSMSConfigInfo;

        struct QmiCbkImsUserConfigInd
                                    qaQmiCbkImsUserConfigInfo;

        struct QmiCbkImsVoIPConfigInd
                                    qaQmiCbkImsVoIPConfigInfo;

        struct QmiCbkWdsDUNCallInfoInd
                                    qaQmiCbkWdsDunCallInfo;
        QmiCbkWdsStatisticsIndState
                                    qmQmiCbkWdsTransfterStaticsInfo;
        QmiCbkWdsDHCPv4ClientLeaseInd
                                    qmQmiCbkWdsSWIDHCPv4ClientLeaseInd;

        SMSAsyncRawSend             qaQmiCbkWdsAsyncRawSendInd;
        struct QmiCbkNasSwiOTAMessageInd
                                    qaQmiCbkNasSwiOTAMessageInd;
        QmiCbkLocCradleMountInd
                                    qaQmiCbkLocCradleMountInd;
        QmiCbkLocEventTimeSyncInd
                                    qaQmiCbkLocEventTimeSyncInd;
        QmiCbkLocInjectTimeInd
                                    qaQmiCbkLocInjectTimeInd;
        QmiCbkLocSensorStreamingInd
                                    qaQmiCbkLocSensorStreamingInd;
        QmiCbkLocInjectSensorDataInd
                                    qaQmiCbkLocInjectSensorDataInd;
        QmiCbkLocPositionReportInd
                                    qaQmiCbkLocPositionReportInd;
        QmiCbkLocInjectUTCTimeInd
                                    qaQmiCbkLocInjectUTCTimeInd;
        QmiCbkLocInjectPositionInd
                                    qaQmiCbkLocInjectPositionInd;
        QmiCbkLocEngineStateInd
                                    qaQmiCbkLocEngineStateInd;
        QmiCbkLocSetExtPowerConfigInd
                                    qaQmiCbkLocSetExtPowerConfigInd;
        struct QmiCbkImsaRegStatusInd
                                    qaQmiCbkImsaRegStatusInfo;
        struct QmiCbkImsaSvcStatusInd
                                    qaQmiCbkImsaSvcStatusInfo;
        struct QmiCbkImsaRatStatusInd
                                    qaQmiCbkImsaRatStatusInfo;
        struct QmiCbkImsaPdpStatusInd
                                    qaQmiCbkImsaPdpStatusInfo;
        gnssSvInfoNotification      qaQmiCbkLocGnssSvInfoInd;
        QmiCbkNasLTECphyCaInfo            qaQMCbkNasLTECphyCaInfo;
        QmiCbkNasSwiRankIndicatorInd qaQmiCbkNasSwiRankIndicatorInd;
        UIMSlotStatusChangeInfo qaQmiCbkUimSlotStatusChangeInd;
        struct QmiCbkDmsSwiGetResetInfoInd qaQmiCbkDmsSwiGetResetInfoInd;
        QmiCbkTmdMitiLvlRptInd
                                    qaQmiCbkTmdMitiLvlRptInd;
        QmiCbkLocBestAvailPosInd
                                    qaQmiCbkLocBestAvailPosInd;
        nasTimers qaQmiCbkNasTimerInd;
        nasNetworkReject qaQmiCbkNasNetRejectInd;
        SwiAvmsEventInd     qaQmiCbkSwiAvmsEventReportInd;
        QmiCbkLocGetServerInd qaQmiCbkLocGetServerInd;
    } QmiInd;
};

package enum eQCWWANError UpkQmiCbkWdsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp,
    BYTE                    instance );

package enum eQCWWANError UpkQmiCbkNasNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkDmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkQosNotification(
    USHORT                  QmiMsgID,
    BYTE                    *pMdmResp,
    struct QmiNotification  *pNotifResp,
    BYTE                    instance );

package enum eQCWWANError UpkQmiCbkPdsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkCatNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkWmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkSwiOmaDmNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

/* Added support for SL9090 */
package enum eQCWWANError UpkQmiCbkSwiOmaDmNotificationExt(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkDcsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp,
    BYTE                    instance );

package enum eQCWWANError UpkQmiCbkFmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkOmaDmNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkVoiceNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkUIMNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkIMSNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkLOCNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkIMSANotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkContorlNotification(
   USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp);

package enum eQCWWANError UpkQmiCbkTMDNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkSwiAVMSNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

/* Function to get a notification callback */
package void *qaQmiGetCallback(
    enum eQMICallbackIndex eCbkIndex );

/* Function to set a notification callback */
package void qaQmiSetCallback(
    enum eQMICallbackIndex eCbkIndex,
    void                   *pCallback );

void qaNotifyInit(void);

extern void qaQmiRemoveAllCallbacks(void);

/* Function to receive device state change event */
extern void qaQmiDeviceStateChangeCbk(
    BYTE deviceState );

/* Maps intance to callback index
 *
 *  \param  instance
 *         - PDP instance
 *
 *  \return  callback index
 *
 *  \note    none
 *
 */

enum eQMICallbackIndex qmiInst2WdsEvent( BYTE instance );
enum eQMICallbackIndex qmiInst2XferStatsEvent( BYTE instance );

enum eQMICallbackIndex qmiInst2DcsNetEvent( BYTE instance );

enum eQMICallbackIndex qmiInst2QosEvent( BYTE instance );

enum eQMICallbackIndex qmiInst2QosFlowStatusEvent( BYTE instance );

enum eQMICallbackIndex qmiInst2DHCPv4ClientLeaseEvent( BYTE instance );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /*__QMI_NOTIFY_H__ */

