/**
 * \ingroup ims
 *
 * \file    qaGobiApiImsa.h
 *
 * \brief   IMSA Service API function prototypes
 */

/*
 * Copyright: © 2014-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_IMSA_H__
#define __GOBI_API_IMSA_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  This structure contains parameters of IMSA Config Indication Register
 *
 *  \param  pRegStatusConfig(optional)
 *          - Register Indication For Registration status.
 *          - When this registration is enabled, the device learns of Registration
 *            status via the QMI_IMSA_REGISTRATION_STATUS_IND
 *            indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pServiceStatusConfig(optional)
 *          - Register Indication For Service status
 *            Events.
 *          - When this registration is enabled, the device learns of Service 
 *            status via the QMI_IMSA_SERVICE_STATUS_IND indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pRatHandoverStatusConfig(optional)
 *          - Registration Indication For RAT handover status.
 *          - When this registration is enabled, the device learns of RAT handover
 *            status via the QMI_IMSA_RAT_HANDOVER_STATUS_IND indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pPdpStatusConfig(optional)
 *          - PDP Status Configuration.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \note   One of the optional parameter is mandatory to be present in the
 *          request.
 */
typedef struct
{
    BYTE *pRegStatusConfig;
    BYTE *pServiceStatusConfig;
    BYTE *pRatHandoverStatusConfig;
    BYTE *pPdpStatusConfig;
} IMSAIndRegisterInfo;

/**
 *  Sets the registration state for different QMI_IMSA indications for the
 *  requesting control point
 *
 *  \param  pImsaIndRegisterInfo[IN]
 *          - Structure containing Indication Register Information.
 *              - See \ref IMSAIndRegisterInfo for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 *
 *          This API is used by a device to register/deregister for different
 *          QMI IMSA indications.
 *          The device's registration state variables that control registration
 *          for indications will be modified to reflect the settings indicated in the
 *          request message.
 *          At least one optional parameter must be present in the request.
 *
 */
ULONG SLQSRegisterIMSAIndication (
    IMSAIndRegisterInfo *pImsaIndRegisterInfo );
/**
 * This structure contains the Supported Messages List Information
 *
 * \param supportedMsgLen
 *        - Number of sets of the supported messages
 *
 * \param supportedMsgs
 *        - Array of uint8 is a bitmask where each bit represents
 *          a message ID.
 *        - Starting with the LSB, bit 0 represents message ID 0,
 *          bit 1 represents message ID 1.
 */
struct SupportedMsgList{
    WORD supportedMsgLen;
    BYTE supportedMsgs[256];
};

/**
 *  This structure contains Queries the set of messages implemented by the currently running software.
 *
 *  \param  pSupportedMsgList
 *          - List of Supported Messages.
 *          - See \ref SupportedMsgList for more information
 *
 */
typedef struct
{
    struct SupportedMsgList *pSupportedMsgList;
} IMSASupportedMsgInfo;

/**
 *  Queries the set of messages implemented by the currently running software.
 *
 *  \param  pIMSASupportedMsgInfo[OUT]
 *          - Structure containing Supported Messages Information.
 *              - See \ref IMSASupportedMsgInfo for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 *
 *          This API is used by a device to query the set of messages implemented by
 *          the currently running software
 *
 */
ULONG SLQSGetIMSASupportedMsg (
       IMSASupportedMsgInfo *pIMSASupportedMsgInfo );

/**
 * This structure contains the Supported Request Fields List Information
 *
 * \param requestFieldsLen
 *        - Number of sets of the request fields.
 *
 * \param requestFields
 *        - Describes which optional field IDs are supported in QMI Request.
 *        - Array of uint8 is a bitmask where each bit represents
 *          a field ID.
 *        - Field 0-15 are mandatory, First Bit represents field ID 16,
 *        - Starting with the LSB, bit 0 represents Field ID 16,
 *          bit 1 represents  ID 17.
 */
struct ReqFieldsList{
    BYTE requestFieldsLen;
    BYTE requestFields[256];
};
/**
 * This structure contains the Supported Response Fields List Information
 *
 * \param responseFieldsLen
 *        - Number of sets of the response fields.
 *
 * \param responseFields
 *        - Describes which optional field IDs are supported in QMI Response.
 *        - Format is same as request field.
 */
struct RespFieldsList{
    BYTE responseFieldsLen;
    BYTE responseFields[256];
};
/**
 * This structure contains the Supported Indication Fields List Information
 *
 * \param indicationFieldsLen
 *        - Number of sets of the indication fields.
 *
 * \param indicationFields
 *        - Describes which optional field IDs are supported in QMI indication.
 *        - Format is same as request field.
 */
struct IndFieldsList{
    BYTE indicationFieldsLen;
    BYTE indicationFields[256];
};
/**
 *  This structure contains response of supported fields by the currently running software.
 *
 *  \param  pReqFieldsList
 *          - List of Supported Request Fields.
 *          - See \ref ReqFieldsList for more information
 *
 *  \param  pRespFieldsList
 *          - List of Supported Request Fields.
 *          - See \ref RespFieldsList for more information
 *
 *  \param  pIndFieldsList
 *          - List of Supported Request Fields.
 *          - See \ref IndFieldsList for more information
 *
 */
typedef struct
{
    struct ReqFieldsList        *pReqFieldsList;
    struct RespFieldsList       *pRespFieldsList;
    struct IndFieldsList        *pIndFieldsList;
} IMSASupportedFieldsResp;

/**
 *  Queries the set of supported fields implemented by the currently running software.
 *
 *  \param  messageID[IN]
 *          - Service Message ID.
 *
 *  \param  pIMSASupportedFieldsResp[OUT]
 *          - Structure containing Supported Fields Response.
 *              - See \ref IMSASupportedFieldsResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 *
 *          This API is used by a device to query the fields supported for a
 *          single command as implemented by the currently running software.
 *
 */
ULONG SLQSGetIMSASupportedFields (
        WORD                    messageID,
        IMSASupportedFieldsResp *pIMSASupportedFieldsResp );

/**
 *  This structure contains response parameters of registration status.
 *
 *  \param  pImsRegStatus
 *          - IMS Registration Status (Deprecated).
 *          - Values
 *             - TRUE  - UE is registered on the IMS network
 *             - FALSE - UE is not registered on the IMS network
 *
 *  \param  pImsRegErrCode
 *          - IMS Registration Error Code.
 *          - An error code is returned when the IMS registration
 *            status is IMSA_STATUS_NOT_REGISTERED.
 *          -Values
 *            - 3xx – Redirection responses
 *            - 4xx – Client failure responses
 *            - 5xx – Server failure responses
 *            - 6xx – Global failure responses
 *
 *  \param  pNewImsRegStatus
 *          - New IMS Registration Status
 *          - Values
 *             - 0 - Not registered for IMS
 *             - 1 - Registering for IMS
 *             - 2 - Registered for IMS
 *
 */
typedef struct
{
    BYTE  *pImsRegStatus;
    WORD  *pImsRegErrCode;
    ULONG *pNewImsRegStatus;
} IMSARegistrationStatus;

/**
 *  Queries the set of messages implemented by the currently running software.
 *
 *  \param  pIMSARegistrationStatus[OUT]
 *          - Structure containing response parameters for registration status.
 *              - See \ref IMSARegistrationStatus for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 *
 *          This API is used by a device to get the registration status for
 *          various IMS services for the requesting control point.
 *
 */
ULONG SLQSGetIMSARegStatus (
        IMSARegistrationStatus *pIMSARegistrationStatus );

/**
 *  This structure contains response parameters of service status for various IMS services.
 *
 *  \param  pSmsServiceStatus
 *          - SMS Service Status.
 *          - Values
 *             - 0 - IMS SMS service is not available
 *             - 1 - IMS SMS is in limited service
 *             - 2 - IMS SMS is in full service
 *
 *  \param  pVoipSeriveStatus
 *          - VoIP Service Status.
 *          -Values
 *            - 0 - IMS VoIP service is not available
 *            - 2 - IMS VoIP is in full service
 *
 *  \param  pVtServiceStatus
 *          - VT Service Status
 *          - Values
 *             - 0 - IMS VT service is not available
 *             - 2 - IMS VT is in full service
 *
 *  \param  pSmsServiceRat
 *          - SMS service RAT
 *          - Values
 *             - 0 - IMS service is registered on WLAN
 *             - 1 - IMS service is registered on WWAN
 *             - 2 - IMS service is registered on interworking WLAN
 *
 *  \param  pVoipServiceRat
 *          - VoIP service RAT.
 *          - Values
 *             - 0 - IMS service is registered on WLAN
 *             - 1 - IMS service is registered on WWAN
 *             - 2 - IMS service is registered on interworking WLAN
 *
 *  \param  pVtServiceRat
 *          - VT service RAT.
 *          - Values
 *             - 0 - IMS service is registered on WLAN
 *             - 1 - IMS service is registered on WWAN
 *             - 2 - IMS service is registered on interworking WLAN
 *
 *  \param  pUtServiceStatus
 *          - UT service Status.
 *          - Values
 *             - 0 - IMS UT service is not available
 *             - 2 - IMS UT is in full service
 *
 *  \param  pUtServiceRat
 *          - UT service RAT.
 *          - Values
 *             - 0 - IMS service is registered on WLAN
 *             - 1 - IMS service is registered on WWAN
 *             - 2 - IMS service is registered on interworking WLAN
 *
 *  \param  pVsServiceStatus
 *          - VS service Status.
 *          - Values
 *             - 0 - IMS UT service is not available
 *             - 2 - IMS UT is in full service
 *
 *  \param  pVsServiceRat
 *          - VS service RAT.
 *          - Values
 *             - 0 - IMS service is registered on WLAN
 *             - 1 - IMS service is registered on WWAN
 *             - 2 - IMS service is registered on interworking WLAN
 */
typedef struct
{
    ULONG  *pSmsServiceStatus;
    ULONG  *pVoipServiceStatus;
    ULONG  *pVtServiceStatus;
    ULONG  *pSmsServiceRat;
    ULONG  *pVoipServiceRat;
    ULONG  *pVtServiceRat;
    ULONG  *pUtServiceStatus;
    ULONG  *pUtServiceRat;
    ULONG  *pVsServiceStatus;
    ULONG  *pVsServiceRat;
} IMSAServiceStatus;

/**
 *  Gets the service status for various IMS services for the requesting control point.
 *
 *  \param  pIMSAServiceStatus[OUT]
 *          - Structure containing response parameters for service status.
 *              - See \ref IMSAServiceStatus for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 *
 *          This API is used by a device to Gets the service status for various
 *          IMS services for the requesting control point.
 *
 */
ULONG SLQSGetIMSAServiceStatus (
        IMSAServiceStatus *pIMSAServiceStatus );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_IMSA_H__ */
