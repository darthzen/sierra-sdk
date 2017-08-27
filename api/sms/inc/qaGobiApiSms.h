/**
 *  \ingroup sms
 *
 *  \file    qaGobiApiSms.h
 *
 *  \brief   Short Message Service API function prototypes
 */

/*
 *  Copyright: © 2011-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_SMS_H__
#define __GOBI_API_SMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_LEN         0x05
#define TIME_STAMP_BUF     0x08
#define ABSOLUTE_VALIDITY  0x08
#define TIME_DATE_BUF      0x09
#define MAX_SMS_ROUTES     0x0A
#define NUM_OF_SET         0xFF

/**
 * Deletes one or more SMSs from device memory. If both of the optional
 * parameters, messageIndex and messageTag, are NULL, all messages are
 * deleted from the storage location specified in the mandatory storageType
 * parameter. The optional index and tag parameters narrow the range of
 * messages being deleted. If an index is specified, the single message
 * with the index from the specified memory store will be deleted. If a
 * tag is specified, all messages in the specified memory store whose
 * tag matches that specified will be deleted.\n
 *
 * There are three ways to use this message:
 *  - Specify storageType only
 *      - Deletes all messages from memory storage
 *  - Specify storageType and a tag
 *      - Deletes all messages from memory storage that match the given
 *        message tag
 *  - Specify storageType and an index
 *      - Deletes only the message with the given index from memory storage
 *
 *  \param  storageType
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  pMessageIndex[IN]
 *          - (Optional) message index
 *
 *  \param  pMessageTag[IN]
 *          - (Optional) message tag
 *              - 0 - Read
 *              - 1 - Not read
 *              - 2 - Mobile originated and sent
 *              - 3 - Mobile originated but not yet sent
 *
 *  \param  pMessageMode[IN]
 *          - (Optional) messgae mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 10 seconds
 *
 */
ULONG SLQSDeleteSMS(
    ULONG storageType,
    ULONG *pMessageIndex,
    ULONG *pMessageTag,
    BYTE  *pMessageMode );

/**
 *  Returns an SMS from device memory.
 *
 *  \param  storageType
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  messageIndex
 *          - Message index
 *
 *  \param  pMessageTag[OUT]
 *          - Message tag
 *              - 0 - Read
 *              - 1 - Not read
 *              - 2 - Mobile originated and sent
 *              - 3 - Mobile originated but not yet sent
 *
 *  \param  pMessageFormat[OUT]
 *          - Message format
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  pMessageSize[IN/OUT]
 *          - Upon input the maximum number of bytes that can be written to the
 *            message array.
 *          - Upon successful output the actual number of bytes written to the
 *            message array.
 *
 *  \param  pMessage[OUT]
 *          - The message contents array
 *
 *  \param  pMessageMode[IN]
 *          - (Optional) Message Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 *
 */
ULONG SLQSGetSMS(
    ULONG storageType,
    ULONG messageIndex,
    ULONG *pMessageTag,
    ULONG *pMessageFormat,
    ULONG *pMessageSize,
    BYTE  *pMessage,
    BYTE  *pMessageMode );

/**
 *  Sends an SMS message for immediate over-the-air transmission
 *
 *  \param  messageFormat[IN]
 *          - Message format
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  messageSize[IN]
 *          - The length of the message contents in bytes
 *
 *  \param  pMessage[IN]
 *          - The message contents in PDU format contains SMS header and payload message
 *
 *  \param  pSmsOnIms[IN]
 *          - (Optional) SMS on IMS
 *          - The message is to be sent on IMS.
 *             - 0x00 Message is not to be sent on IMS.
 *             - 0x01 Message is to be sent on IMS.
 *             - 0x02 to 0xFF Reserved.
 *
 *  \param  pMessageFailureCode[OUT]
 *          - (Optional) Message Failure Code
 *          - pointer to message failure code. If cause code is not provided,
 *            then value will be 0xFFFFFFFF
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 minutes
 *
 */
ULONG SendSMS(
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage,
    ULONG *pMessageFailureCode,
    BYTE  *pSmsOnIms);

/**
 *  This structure contains SMS parameters
 *
 *  \param  messageFormat[IN]
 *          - Message format
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  messageSize[IN]
 *          - The length of the message contents in bytes
 *
 *  \param  pMessage[IN]
 *          - The message contents in PDU format contains SMS header and payload message
 *
 *  \param  pMessageID[OUT]
 *          - message reference ID
 *
 *  \param  pMessageFailureCode[OUT]
 *          - (Optional) Message Failure Code
 *          - If cause code is not provided, then value will
 *            be 0xFFFFFFFF
 *
 *  \param  pSmsOnIms[IN]
 *          - (Optional) SMS on IMS
 *          - Indicates whether the message is to be sent on IMS.
 *          - Values:
 *              - 0x00 - Message is not to be sent on IMS
 *              - 0x01 - Message is to be sent on IMS
 *              - 0x02 to 0xFF - Reserved
 *
 *  \param  pLinktimer[IN]
 *          - (Optional) Link Timer
 *          - Keeps the GW SMS link open for the specified number of seconds;
 *            can be enabled if more messages are expected to follow
 */
typedef struct
{
    ULONG  messageFormat;
    ULONG  messageSize;
    BYTE   *pMessage;
    USHORT messageID;
    ULONG  messageFailureCode;
    BYTE   *pLinktimer;
    BYTE   *pSmsOnIms;
}slqssendsmsparams_s;

/**
 *  Sends an SMS message for immediate over-the-air transmission
 *
 *  \param  pSendSmsParams
 *          - structure containing the SMS parameters. Refer slqssendsmsparams_s
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 minutes
 *
 */
ULONG SLQSSendSMS( slqssendsmsparams_s *pSendSmsParams );

/**
 *  Gets the SMS center address.
 *
 *  \param  addressSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the SMS center address array can contain.
 *
 *  \param  pSMSCAddress[0UT]
 *          - The SMS center address represented as a NULL terminated string.
 *
 *  \param  typeSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the SMS center address type array can contain.
 *
 *  \param  pSMSCType[0UT]
 *          - The SMS center address type represented as a NULL terminated
 *            string.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetSMSCAddress(
    BYTE addressSize,
    CHAR *pSMSCAddress,
    BYTE typeSize,
    CHAR *pSMSCType );

/**
 *  Sets the SMS center address.
 *
 *  \param  pSMSCAddress[IN]
 *          - The SMS center address represented as a NULL terminated string
 *
 *  \param  pSMSCType[IN]
 *          - The SMS center address type represented as a NULL terminated
 *          string (optional).
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 *
 */
ULONG SetSMSCAddress(
    CHAR *pSMSCAddress,
    CHAR *pSMSCType );

/**
 *  Saves an SMS message to device memory
 *
 *  \param  storageType[IN]
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  messageFormat[IN]
 *          - Message format
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  messageSize
 *          - The length of the message contents in bytes
 *
 *  \param  pMessage[IN]
 *          - The message contents
 *
 *  \param  pMessageIndex[OUT]
 *          - The message index assigned by the device
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 10 seconds
 *
 */
ULONG SaveSMS(
    ULONG storageType,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage,
    ULONG *pMessageIndex );

/**
 *  Returns the list of SMS messages stored on the device.
 *
 *  \param  storageType[IN]
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  pRequestedTag[IN]
 *          - (Optional) Message tag
 *              - 0 - Read
 *              - 1 - Not read
 *              - 2 - Mobile originated and sent
 *              - 3 - Mobile originated but not yet sent
 *
 *  \param  pMessageListSize[IN/OUT]
 *          - Upon input the maximum number of elements that the message list
 *            array can contain.
 *          - Upon successful output the actual number of elements in the
 *            message list array.
 *
 *  \param  pMessageList[OUT]
 *          - The message list array
 *
 *  \param  pMessageMode[IN]
 *          - (Optional) Message Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 *
 */
ULONG SLQSGetSMSList(
    ULONG storageType,
    ULONG *pRequestedTag,
    ULONG *pMessageListSize,
    BYTE  *pMessageList,
    BYTE  *pMessageMode );

/**
 *  Modifies the status of an SMS message saved in storage on the device.
 *
 *  \param  storageType[IN]
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  messageIndex[IN]
 *          - Message index
 *
 *  \param  messageTag[IN]
 *          - Message tag
 *              - 0 - Read
 *              - 1 - Not read
 *
 *  \param  pMessageMode[IN]
 *          - (Optional) Message Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 *
 */
ULONG SLQSModifySMSStatus(
    ULONG storageType,
    ULONG messageIndex,
    ULONG messageTag,
    BYTE  *pMessageMode );

/**
 *  This structure contains BroadcastConfig parameters
 *
 *  \param  fromServiceId
 *          - Starting point of range of CBM message identifiers
 *
 *  \param  toServiceId
 *          - Ending point of range of CBM message identifiers
 *
 *  \param  selected
 *          - Range of CBM message identifiers indicated by
 *            from_service_id and to_service_id
 *               - 0x00 – Not selected
 *               - 0x01 – Selected
 *
 */
struct BroadcastConfig
{
    WORD fromServiceId;
    WORD toServiceId;
    BYTE selected;
};

/**
 *  This structure contains the 3GPP Broadcast Configuration Information
 *  parameters
 *
 *  \param  activated_ind
 *          - Broadcast SMS
 *              - 0x00 - Deactivated
 *              - 0x01 - Activated
 *
 *  \param  num_instances
 *          - Number of sets (N) of parameters
 *            Following each set describes one entry in the broadcast
 *            configuration table.
 *              - fromServiceId
 *              - toServiceId
 *              - selected
 *
 *  \param  broadcastConfig
 *          - A BroadcastConfig structure array.
 *          - Further defined by the structure BroadcastConfig
 *
 */
typedef struct _qaQmi3GPPBroadcastCfgInfo {
    BYTE                   activated_ind;
    WORD                   num_instances;
    struct BroadcastConfig broadcastConfig[CONFIG_LEN];
} qaQmi3GPPBroadcastCfgInfo;

/**
 *  This structure contains CDMABroadcastConfig parameters
 *
 *  \param  serviceCategory
 *          - Service category
 *
 *  \param  language
 *          - Language
 *
 *  \param  selected
 *          - Specified service_category and language
 *               - 0x00 – Not selected
 *               - 0x01 – Selected
 *
 */
struct CDMABroadcastConfig
{
    WORD serviceCategory;
    WORD language;
    BYTE selected;
};

/**
 *  This structure contains the 3GPP2 Broadcast Configuration Information
 *  parameters
 *
 *  \param  activated_ind
 *          - Broadcast SMS
 *              - 0x00 - Deactivated
 *              - 0x01 - Activated
 *
 *  \param  num_instances
 *          - Number of sets (N) of parameters
 *            Following each set describes one entry in the broadcast
 *            configuration table.
 *              - serviceCategory
 *              - language
 *              - selected
 *
 *  \param  broadcastConfig
 *          - A CDMABroadcastConfig structure array.
 *          - Further defined by the structure CDMABroadcastConfig
 *
 */
typedef struct _qaQmi3GPP2BroadcastCfgInfo
{
    BYTE                       activated_ind;
    WORD                       num_instances;
    struct CDMABroadcastConfig CDMABroadcastConfig[CONFIG_LEN];
} qaQmi3GPP2BroadcastCfgInfo;

/**
 *  Provides Information about the SMS BroadcastConfiguration
 *
 * \param  mode[IN]
 *         - Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 * \param  pBroadcastConfig[OUT]
 *         - The data for 3GPP Broadcast Information(Optional).
 *
 * \param  pCDMABroadcastConfig[OUT]
 *         - The data for 3GPP2 Broadcast Information(Optional).
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 */
ULONG SLQSGetSmsBroadcastConfig(
    BYTE mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig );

/**
 *  Sets the information about the SMS BroadcastConfiguration
 *
 * \param  mode[IN]
 *         - Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 * \param  pBroadcastConfig[IN]
 *         - The data for 3GPP Broadcast Information(Optional).
 *
 * \param  pCDMABroadcastConfig[IN]
 *         - The data for 3GPP2 Broadcast Information(Optional).
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 */
ULONG SLQSSetSmsBroadcastConfig(
    BYTE                       mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig );

/**
 *  Enables or disables the reception of broadcast SMS messages.
 *
 *  \param  Mode[IN]
 *          - Mode
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \param  broadcastActivate[IN]
 *          - 0x00 - Disable broadcast
 *          - 0x01 - Activate broadcast
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 *
 */
ULONG SLQSSetSmsBroadcastActivation(
    BYTE mode,
    BYTE broadcastActivate );

/**
 *  Structure contains parameters for message to be encoded
 *
 *  \param  pMessageSize[IN/OUT]
 *          - Upon input, specifies the total number of bytes that the
 *            given pMessage buffer can hold (a buffer of length 240
 *            is recommended).Upon successful output, specifies the
 *            length of the constructed message placed in the pMessage
 *            buffer (in bytes)
 *
 *  \param  pMessage[OUT] - The constructed raw message
 *
 *  \param  messageId[IN]
 *          - The message reference number for this message.
 *            This value should be incremented for every message the host
 *            application sends
 *
 * \param  pDestAddr[IN]
 *         - Gives NULL-terminated ASCII String containing a destination
 *           address.International number will be prepended with a '+'
 *           character
 *
 * \param   pCallbackAddr[IN]
 *          - Gives NULL-terminated ASCII String containing a callback
 *            address.International number will be prepended with a '+'
 *            character
 *
 *  \param  textMsgLength[IN]
 *          - Number of UCS2 characters in the text message(excluding NULL)
 *
 *  \param  pTextMsg[IN]
 *          - Text message to be encoded
 *
 *  \param  pPriority[IN](optional parameter)
 *          - Gives the priority of the outgoing message:
 *            0 - normal (default if NULL pointer is given)
 *            1 - interactive
 *            2 - urgent
 *            3 - emergency
 *            64  - 64 is decoded value for URGENT VZAM Support interactive.
 *            128 - 128 is decoded value for URGENT VZAM Support urgent.
 *            192 - 128 is decoded value for URGENT VZAM Support emergency.
 *
 *  \param  pEncodingAlphabet[IN/OUT](optional parameter)
 *          - Upon input, specifies the alphabet the text message
 *            should be encoded in
 *            0 - 8bit ASCII (not supported at this time)
 *            1 - IS91EP (not supported at this time)
 *            2 - 7bit ASCII (default if NULL pointer is given)
 *            3 - IA5 (not supported at this time)
 *            4 - unicode (not supported at this time)
 *            5 - shift JIS (not supported at this time)
 *            6 - korean (not supported at this time)
 *            7 - latin hebrew (not supported at this time)
 *            8 - latin (not supported at this time)
 *            9 - GSM 7 bit default
 *            Upon successful output, specifies the alphabet
 *            used to encode the message.
 *
 *  \param  pRelValidity[IN](optional parameter)
 *          - Gives the relative validity period of the outgoing message
 *            0 - Set Relative validity to 11
 *            1 - Set Relative validity to 71
 *            2 - Set Relative validity to 167
 *            3 - Set Relative validity to 169
 *            4 - Set Relative validity to 171
 *            Values have the following meanings:
 *            0 to 143: validity period = (value + 1)* 5 minutes
 *            144 to 167: validity period = 12 hours + (value - 143)*30
 *            minutes
 *            168 to 196: validity period = (value - 166) * 1 day
 *            197 to 244: validity period = (value - 192) * 1 week
 *            245: validity period = indefinite
 *
 *  \Notes   Currently only encoding of 7bit ASCII messages is supported.
 *
 */
struct cdmaMsgEncodingParams
{
    BYTE    *pMessageSize;
    BYTE    *pMessage;
    BYTE     messageId;
    CHAR    *pDestAddr;
    CHAR    *pCallbackAddr;
    ULONG    textMsgLength;
    WORD    *pTextMsg;
    BYTE    *pPriority;
    BYTE    *pEncodingAlphabet;
    BYTE    *pRelValidity;
};

/**
 *  Encodes text message to CDMA PDU message.
 *
 *  \param  pMsgToBeEncodedCDMA[IN/OUT]
 *          - SLQS Runtime Settings Information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: None
 */
ULONG SLQSCDMAEncodeMOTextMsg(
    struct cdmaMsgEncodingParams *pCdmaMsgEncodingParams );

/**
 * Structure contains parameters which need to be decoded from
 * message
 *
 *  \param  messageLength[IN]
 *          - Length of the message to be decoded in bytes
 *
 *  \param  pMessage[IN]
 *          - Message read off the device via GetSMS
 *
 *  \param  pSenderAddrLength[IN/OUT]
 *          - Upon input, indicates the maximum number of ASCII
 *            characters (including NULL termination) that the
 *            pSenderAddr buffer can accommodate.  Note that a
 *            length of 14 is reasonable.Upon successful output,
 *            returns the length of originating address string
 *           (including the NULL termination)
 *
 *  \param  pSenderAddr[OUT]
 *          - Returns NULL-terminated ASCII String containing the
 *            originating address.International number will be
 *            prepended with a '+'character
 *
 *  \param  pTextMsgLength[IN/OUT]
 *          - Upon input, specifies the number of UCS2 characters the
 *            given text message buffer can accommodate.Upon successful
 *            output, returns the number of UCS2 characters returns in
 *            the given text messagebuffer(including NULL-terminator)
 *
 *  \param  pTextMsg[OUT]
 *          - Returns the text message as  NULL-terminated UCS2 string
 *
 *  \param  pPriority[OUT] (optional parameter)
 *          - Returns the priority setting of the message
 *            0x00 - normal
 *            0x01 - interactive
 *            0x02 - urgent
 *            0x03 - emergency
 *            0xFF - unavailable setting
 *
 *  \param  pPrivacy[OUT](optional parameter)
 *          - Returns the privacy setting of the message
 *            0x00 - not restricted
 *            0x01 - restricted
 *            0x02 - confidential
 *            0x03 - secret
 *            0xFF - unavailable setting
 *
 *  \param  pLanguage[OUT] (optional parameter )
 *          - Returns the language setting of the message
 *            0x00 - unspecified
 *            0x01 - english
 *            0x02 - french
 *            0x03 - spanish
 *            0x04 - japanese
 *            0x05 - korean
 *            0x06 - chinese
 *            0x07 - hebrew
 *            0xFF - unavailable setting
 *
 *  \param  mcTimeStamp[8][OUT] (optional parameter)
 *          - Returns the message center timestamp which takes the form:
 *            YYMMDDHHMMSSTZ where
 *            YY - year
 *            MM - month
 *            DD - day
 *            HH - hour
 *            MM - minute
 *            SS - second
 *            TZ - timezone
 *            All values are in decimal. Timezone is in relation to GMT,
 *            one unit is equal to 15 minutes and MSB indicates a
 *            negative value.If this information is unavailable for
 *            message then this field will be filled with 0xFF
 *
 *  \param  absoluteValidity[8][OUT] (optional parameter)
 *          - Returns the absolute validity period setting for this
 *            message.This field takes the same form as mcTimeStamp
 *
 *  \param  pRelativeValidity[OUT] (optional parameter)
 *          - Returns the relative validity period.Values have the
 *            following meanings:
 *            0 to 143: validity period =(value + 1)* 5 minutes
 *            144 to 167: validity period =12 hours+(value - 143)*30
 *            minutes
 *            168 to 196: validity period = (value - 166) * 1 day
 *            197 to 244: validity period = (value - 192) * 1 week
 *            245: validity period = indefinite
 *            246: validity period = immediate
 *            247: validity period = valid until mobile becomes inactive
 *            248: validity period = valid until registration area
 *            changes
 *            249 to 254: reserved
 *            255: unavailable information
 *
 *  \param  pDisplayMode[OUT] (optional parameter)
 *          - Returns the display mode parameter
 *            0x00 - immediate display
 *            0x01 - mobile default setting
 *            0x02 - user invoked
 *            0x03 - reserved
 *            0xFF - unavailable parameter
 *
 *  \param  pUserAcknowledgementReq[OUT] (optional parameter)
 *          - Returns the user (manual) acknowledgment request parameter
 *            TRUE - means the user is requested to manually acknowledge
 *                   the delivery of the message.
 *            FALSE - means no such user acknowledgement is requested
 *
 *  \param  pReadAcknowledgementReq[OUT] (optional parameter)
 *          - Returns the read acknowledgement request parameter
 *            TRUE - means acknowledgment of the message being viewed is
 *                   requested.
 *            FALSE - means no such read acknowledgement is requested
 *
 *  \param  pAlertPriority[OUT] (optional parameter)
 *          - Returns the alerting parameter setting
 *            0x00 - use default alert
 *            0x01 - use low priority alert
 *            0x02 - use medium priority alert
 *            0x03 - use high priority alert
 *            0xFF - unavailable parameter
 *
 *  \param  pCallbkAddrLength[OUT] (optional parameter)
 *          - returns the length of Callback
 *            address string (including the NULL termination)
 *
 *  \param  pCallbkAddr[OUT] (optional parameter)
 *          - returns NULL-terminated ASCII String containing callback
 *            address String containing the Call Back number with a 32
 *            maximum characters.
 *
 */
struct cdmaMsgDecodingParams
{
    ULONG  messageLength;
    BYTE  *pMessage;
    ULONG *pMessageID;
    BYTE  *pSenderAddrLength;
    CHAR  *pSenderAddr;
    BYTE  *pTextMsgLength;
    WORD  *pTextMsg;
    BYTE  *pPriority;
    BYTE  *pPrivacy;
    BYTE  *pLanguage;
    BYTE  mcTimeStamp[TIME_STAMP_BUF];
    BYTE  absoluteValidity[ABSOLUTE_VALIDITY];
    BYTE  *pRelativeValidity;
    BYTE  *pDisplayMode;
    BOOL  *pUserAcknowledgementReq;
    BOOL  *pReadAcknowledgementReq;
    BYTE  *pAlertPriority;
    BYTE  *pCallbkAddrLength;
    CHAR  *pCallbkAddr;
};

/**
 * Decodes text message to CDMA PDU message
 *
 *  \param  pMsgToBeEncodedCDMA[IN/OUT]
 *          - Pointer to structure containing parameters needed for decoding
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: None
 */

ULONG SLQSCDMADecodeMTTextMsg(
    struct cdmaMsgDecodingParams *pCdmaMsgDecodingParams );

/**
 *  Structure contains parameters which need to encoded with message
 *
 *  \param  messageSize
 *          - The length of the message contents in bytes
 *
 *  \param  pDestAddr[IN]
 *          - Gives NULL-terminated ASCII String containing destination address
 *
 *  \param  pTextMsg[IN]
 *          - Text message to be encoded, maximum limit is 160 charaters
 *
 *  \param  pPDUMessage[OUT]
 *          - Encoded PDU message
 *
 *  \param  alphabet[IN]
 *          - Encoding method to generate the PDU
 *              - 0 - 7 bit encoding
 *              - 4 - 8 bit encoding
 *              - 8 - 16 bit UCS2 encoding
 *              - others value will be treated as default 7 bit encoding
 *
 */
struct wcdmaMsgEncodingParams
{
    ULONG messageSize;
    CHAR  *pDestAddr;
    CHAR  *pTextMsg;
    CHAR  *pPDUMessage;
    BYTE  alphabet;
};

/**
 *  Returns the encoded WCDMA PDU message.
 *
 *  \param  pwcdmaMsgEncodingParams[IN/OUT]
 *          - Pointer to parameters Required for encoding
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: None
 */
ULONG SLQSWCDMAEncodeMOTextMsg(
    struct wcdmaMsgEncodingParams *pWcdmaMsgEncodingParams);

/**
 *  Structure contains parameters which need to be decoded from message
 *
 *  \param  pMessage[IN]
 *          - Message read off the device via SLQSGetSMS
 *
 *  \param  pSenderAddrLength[IN/OUT]
 *          - Upon input, indicates the maximum number of ASCII characters
 *            (including NULL termination) that the pSenderAddr buffer can
 *            accommodate. A length with 24 will be much safe
 *            since this address filed can be up to 12 octets (24 bytes)
 *            Upon successful output, returns the length of destination
 *            address string.
 *
 *  \param  pSenderAddr[OUT]
 *          - Note that a length with 24 bytes will be much safe. Returns
 *            NULL-terminated ASCII String containing destination address
 *
 *
 *  \param  pTextMsgLength[IN/OUT]
 *          - Upon input, specifies the number of characters the given text
 *            message buffer can accommodate. Upon successful output, returns
 *            the number of characters returns in the given text message
 *            buffer.
 *
 *  \param  pTextMsg[OUT]
 *          - Encoded PDU message
 *
 *  \param  pScAddrLength[IN/OUT]
 *          - A length with 24 will be much safe since this address filed can
 *            be up to 12 octets (24 bytes) Returns NULL-terminated ASCII
 *            String containing destination address
 *
 *  \param  pScAddr[OUT]
 *          - Note that a length with 24 bytes will be much safe. Returns
 *            NULL-terminated ASCII String containing service center address.
 *            This SMSC field contains the Type of Address. To get the exact
 *            SMSC address, skip the first two bytes. e.g, 9085290100334, 90
 *            is the Type of Address, indicates international format of phone number
 *
 *  \param  pTime[OUT]
 *          - Time fecthed from message
 *
 *  \param  pDate
 *          - Date fecthed from message
 *
 */
struct wcdmaMsgDecodingParams
{
    BYTE  *pMessage;
    BYTE  *pSenderAddrLength;
    CHAR  *pSenderAddr;
    BYTE  *pTextMsgLength;
    CHAR  *pTextMsg;
    BYTE  *pScAddrLength;
    CHAR  *pScAddr;
    BYTE   Time[TIME_DATE_BUF];
    BYTE   Date[TIME_DATE_BUF];
};

/**
 * Decodes WCDMA PDU message, returns structure filled with decoded parameters
 *
 *  \param  pwcdmaMsgDecodingParams[IN/OUT]
 *          - Pointer to parameters required for decoding
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: none
 */
ULONG SLQSWCDMADecodeMTTextMsg(
    struct wcdmaMsgDecodingParams *pWcdmaMsgDecodingParams);

/**
 *  Structure contains parameters which need to be decoded from message
 *
 *  \param  pMessage[IN]
 *          - Message read off the device via SLQSGetSMS
 *
 *  \param  pSenderAddrLength[IN/OUT]
 *          - Upon input, indicates the maximum number of ASCII characters
 *            (including NULL termination) that the pSenderAddr buffer can
 *            accommodate. A length with 24 will be much safe
 *            since this address filed can be up to 12 octets (24 bytes)
 *            Upon successful output, returns the length of destination
 *            address string.
 *
 *  \param  pSenderAddr[OUT]
 *          - Note that a length with 24 bytes will be much safe. Returns
 *            NULL-terminated ASCII String containing destination address
 *
 *
 *  \param  pTextMsgLength[IN/OUT]
 *          - Upon input, specifies the number of characters the given text
 *            message buffer can accommodate. Upon successful output, returns
 *            the number of characters returns in the given text message
 *            buffer.
 *
 *  \param  pTextMsg[OUT]
 *          - Encoded PDU message
 *
 *  \param  pScAddrLength[IN/OUT]
 *          - A length with 24 will be much safe since this address filed can
 *            be up to 12 octets (24 bytes) Returns NULL-terminated ASCII
 *            String containing destination address
 *
 *  \param  pScAddr[OUT]
 *          - Note that a length with 24 bytes will be much safe. Returns
 *            NULL-terminated ASCII String containing service center address.
 *            This SMSC field contains the Type of Address. To get the exact
 *            SMSC address, skip the first two bytes. e.g, 9085290100334, 90
 *            is the Type of Address, indicates international format of phone number
 *
 *  \param  pTime[OUT]
 *          - Time fecthed from message
 *
 *  \param  pReferenceNum[OUT]
 *          - Reference number of the sms
 *
 *  \param  pTotalNum[OUT]
 *          - Total number of the concatenated message
 *
 *  \param  pPartNum[OUT]
 *          - Sequence number of the current message
 *
 *  \param  pIsUDHPresent
 *          - Is User Data Header Prensent in the PDU? If yes, it means it is a
 *          - concatenated SMS.
 *
 */
struct wcdmaLongMsgDecodingParams
{
    BYTE  *pMessage;
    BYTE  *pSenderAddrLength;
    CHAR  *pSenderAddr;
    BYTE  *pTextMsgLength;
    CHAR  *pTextMsg;
    BYTE  *pScAddrLength;
    CHAR  *pScAddr;
    BYTE  Time[TIME_DATE_BUF];
    BYTE  Date[TIME_DATE_BUF];
    BYTE  *pReferenceNum;
    BYTE  *pTotalNum;
    BYTE  *pPartNum;
    BOOL  *pIsUDHPresent;
};

/**
 * Decodes WCDMA Long SMS PDU message, returns structure filled with decoded parameters
 *
 *  \param  pwcdmaMsgDecodingParams[IN/OUT]
 *          - Pointer to parameters required for decoding
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: none
 */
ULONG SLQSWCDMADecodeLongTextMsg(
    struct wcdmaLongMsgDecodingParams *pWcdmaLongMsgDecodingParams);

/**
 *  This structure contains Transport Layer Information
 *
 *  \param  TransType
 *          - Transport Type
 *              - 0x00 - IMS
 *
 *  \param  TransCap
 *          - Transport Capability
 *          - Values:
 *              - 0x00 - CDMA
 *              - 0x01 - GW
 */
typedef struct _transLayerinfo
{
    BYTE TransType;
    BYTE TransCap;
} transLayerInfo;

/**
 *  This structure contains Get Transport Layer Info Response parameters
 *
 *  \param  pRegInd -
 *          - Optional parameter indicating if transport layer is registered
 *          - Values:
 *              - 0x00 - Transport layer is not registered
 *              - 0x01 - Transport layer is registered
 *          - function SLQSGetTransLayerInfo() returns a default value 0xFF
 *            if no response is received from the device.
 *
 *  \param  pTransLayerInfo
 *          - Pointer to structure of transLayerInfo.
 *              - Optional parameter
 *              - See \ref transLayerInfo for more information
 *          - function SLQSGetTransLayerInfo() returns a default value 0xFF
 *            for parameter values if no response is received from the device.
 *
 */
typedef struct _getTransLayerInfoResp
{
    BYTE           *pRegInd;
    transLayerInfo *pTransLayerInfo;
} getTransLayerInfoResp;

/**
 *  This API provides information about the transport layer.
 *
 *  \param  pGetTransLayerInfoResp [OUT]
 *          - Pointer to structure of getTransLayerInfoResp
 *              - See \ref getTransLayerInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSGetTransLayerInfo (
    getTransLayerInfoResp  *pGetTransLayerInfoResp );

/**
 *  This structure contains transport network registration info
 *  parameter
 *
 *  \param  pRegStatus -
 *          - Optional 1 BYTE parameter indicating transport layer network
 *            registration status
 *          - Values:
 *              - 0x00 - No service
 *              - 0x01 - In progress
 *              - 0x02 - Failed
 *              - 0x03 - Limited Service
 *              - 0x04 - Full Service
 *          - function SLQSGetTransNWRegInfo() returns a default value 0xFF
 *            if no response is received from the device.
 *
 *
 */
typedef struct _getTransNWRegInfoResp
{
    BYTE *pRegStatus;
 } getTransNWRegInfoResp;

/**
 *  This API provides transport layer network registration info.
 *
 *  \param  pGetTransNWRegInfoResp [OUT]
 *          - Pointer to structure of getTransNWRegInfoResp
 *              - See \ref getTransNWRegInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSGetTransNWRegInfo (
    getTransNWRegInfoResp *pGetTransNWRegInfoResp );

/**
 *  This structure contains Get Indication Register Response parameters
 *
 *  \param  pRegTransLayerInfoEvt -
 *          - Optional 1 BYTE parameter indicating registration status of
 *            transport layer information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *          - function SLQSGetIndicationRegister() returns a default value 0xFF
 *            if this parameter is allocated memory in the structure and no
 *            response is received from the device.
 *
 *  \param  pRegTransNWRegInfoEvt -
 *          - Optional 1 BYTEparameter indicating registration status of
 *            transport network registration information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *          - function SLQSGetIndicationRegister() returns a default value 0xFF
 *            if this parameter is allocated memory in the structure and no
 *            response is received from the device.
 *
 *  \param  pRegCallStatInfoEvt -
 *          - Optional 1 BYTE parameter indicating registration status of call
 *            status information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *          - function SLQSGetIndicationRegister() returns a default value 0xFF
 *            if this parameter is allocated memory in the structure and no
 *            response is received from the device.
 *
 */
typedef struct _getIndicationRegResp
{
    BYTE *pRegTransLayerInfoEvt;
    BYTE *pRegTransNWRegInfoEvt;
    BYTE *pRegCallStatInfoEvt;
 } getIndicationRegResp;

/**
 *  This API provides registration state of different WMS indications.
 *
 *  \param  pGetIndicationRegInfo [OUT]
 *          - Pointer to structure of getIndicationRegResp
 *              - See \ref getIndicationRegResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSGetIndicationRegister (
    getIndicationRegResp *pGetIndicationRegInfo );

/**
 *  This structure contains Indication Register request parameters
 *
 *  \param  pRegTransLayerInfoEvt -
 *          - Optional 1 BYTE parameter indicating registration status of
 *            transport layer information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *              - NULL - No change - specifying NULL indicates that the device
 *                will continue to use the existing setting (disable/enable)
 *                which has been previously set for the device
 *
 *  \param  pRegTransNWRegInfoEvt -
 *          - Optional 1 BYTE parameter indicating registration status of
 *            transport network registration information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *              - NULL - No change - specifying NULL indicates that
 *                the device will continue to use the existing setting
 *                (disable/enable) which has been previously set for the device
 *
 *  \param  pRegCallStatInfoEvt -
 *          - Optional 1 BYTE parameter indicating registration status of call
 *            status information events
 *          - Values:
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *              - NULL - No change - specifying NULL indicates that the device
 *                will continue to use the existing setting (disable/enable)
 *                which has been previously set for the device
 *
 */
typedef struct _setIndicationRegReq
{
    BYTE *pRegTransLayerInfoEvt;
    BYTE *pRegTransNWRegInfoEvt;
    BYTE *pRegCallStatInfoEvt;
 } setIndicationRegReq;

/**
 *  This API sets the registration state of different WMS indications.
 *
 *  \param  pSetIndicationRegReq [IN]
 *          - Pointer to structure of indicationRegReqParams
 *              - See \ref setIndicationRegReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 */
ULONG SLQSSetIndicationRegister (
    setIndicationRegReq *pSetIndicationRegReq );

/**
 *  This structure contains SMS route entry details
 *
 *  \param  messageType -
 *          - Message type matching this route
 *          - Values:
 *              - 0x00 - MESSAGE_TYPE_POINT_TO_POINT
 *
 *  \param  messageClass -
 *          - Message Class
 *          - Values:
 *              - 0x00 - MESSAGE_CLASS_0
 *              - 0x01 - MESSAGE_CLASS_1
 *              - 0x02 - MESSAGE_CLASS_2
 *              - 0x03 - MESSAGE_CLASS_3
 *              - 0x04 - MESSAGE_CLASS_NONE
 *              - 0x05 - MESSAGE_CLASS_CDMA
 *
 *  \param  routeStorage -
 *          - If the receiptAction is store where to store the message
 *          - Values:
 *              - 0x00 - STORAGE_TYPE_UIM
 *              - 0x01 - STORAGE_TYPE_NV
 *              - 0xFF - STORAGE_TYPE_NONE
 *
 *  \param  receiptAction -
 *          - Action to be taken on receipt of a message matching the specified
 *            type and class for this route
 *          - Values:
 *              - 0x00 - DISCARD (discarded without notification)
 *              - 0x01 - STORE AND NOTIFY (stored and notified to the
 *                registered clients)
 *              - 0x02 - TRANSFER ONLY (transferred to the client, client
 *                expected to send the ACK)
 *              - 0x03 - TRANSFER AND ACK (transferred to the client, device
 *                expected to send the ACK)
 */
typedef struct
{
    BYTE messageType;
    BYTE messageClass;
    BYTE routeStorage;
    BYTE receiptAction;
} smsRouteEntry;

/**
 *  This structure contains SMS route request parameters
 *
 *  \param  numOfRoutes -
 *          - Number of sets of the following element
 *
 *  \param  routeList -
 *          - Array containing the set of \ref smsRouteEntry
 *
 *  \param  pTransferStatusReport -
 *          - 0x01 - Status reoprt are transferred to the client (optional)
 */
typedef struct
{
    WORD          numOfRoutes;
    smsRouteEntry routeList[MAX_SMS_ROUTES];
    BYTE          *pTransferStatusReport;
} smsSetRoutesReq;

/**
 *  This API sets the action performed on SMS message receipt for specified
 *  message routes. It also specifies the action performed on SMS receipt of
 *  status reports.
 *
 *  \param  pSetRoutesReq [IN]
 *          - Pointer to structure of smsSetRoutesReq
 *              - See \ref smsSetRoutesReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 */
ULONG SLQSSmsSetRoutes (smsSetRoutesReq *pSetRoutesReq);

/**
 *  This structure contains get message protocol response parameters
 *
 *  \param  msgProtocol -
 *          - Message Protocol
 *          - Values:
 *              - 0x00 - MESSAGE_PROTOCOL_CDMA
 *              - 0x01 - MESSAGE_PROTOCOL_WCDMA
 */
typedef struct
{
    BYTE msgProtocol;
} smsMsgprotocolResp;

/**
 *  This API queries the message protocol currently in use for the WMS client.
 *
 *  \param  pMessageProtocol [OUT}
 *          - Pointer to smsMsgprotocolResp
 *              - See \ref smsMsgprotocolResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 */
ULONG SLQSSmsGetMessageProtocol ( smsMsgprotocolResp *pMessageProtocol );

/**
 *  This structure contains get store max size resquest parameters
 *
 *  \param  storageType
 *          - SMS message storage type
 *              - 0 - UIM - Invalid in case of CDMA device that does not
 *                          require SIM
 *              - 1 - NV
 *
 *  \param  pMessageMode(optional parameter)
 *          - 0x00 - CDMA, LTE (if network type is CDMA)
 *          - 0x01 - GW, LTE (if network type is UMTS)
 *
 *  \note   The Message Mode TLV must be included if the device is capable of
 *          supporting more than one protocol
 */
typedef struct
{
    BYTE storageType;
    BYTE *pMessageMode;
} smsMaxStorageSizeReq;


/**
 *  This structure contains get store max size response parameters
 *
 *  \param  maxStorageSize -
 *          - Memory Store Size
 *
 *  \param  freeSlots -
 *          - Optional parameter indicating how much Memory is available
 *          - function SLQSSmsGetMaxStorageSize() returns a default value
 *            0xFFFFFFFF for parameter values if no response is received from
 *            the device.
 */
typedef struct
{
    ULONG maxStorageSize;
    ULONG freeSlots;
} smsMaxStorageSizeResp;

/**
 *  This API provides the maximum number of messages that can be stored in the
 *  specified memory storage. Also it provides the number of slots currently
 *  available
 *
 *  \param  pMaxStorageSizeReq[IN]
 *          - Request parameters for SmsSLQSGetMaxStorageSize
 *              - See \ref smsMaxStorageSizeReq for more information
 *
 *  \param  pMaxStorageSizeResp[OUT]
 *          - Response parameters for SmsSLQSGetMaxStorageSize
 *              - See \ref smsMaxStorageSizeResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 */
ULONG SLQSSmsGetMaxStorageSize(
    smsMaxStorageSizeReq *pMaxStorageSizeReq,
    smsMaxStorageSizeResp *pMaxStorageSizeResp );

/**
 *  This structure contains message waiting information per instance
 *
 *  \param  msgType
 *          - Message type
 *              - 0x00 - MWI_MESSAGE_TYPE_VOICMAIL - Voicemail
 *              - 0x01 - MWI_MESSAGE_TYPE_FAX - Fax
 *              - 0X02 - MWI_MESSAGE_TYPE_EMAIL - Email
 *              - 0x03 - MWI_MESSAGE_TYPE_OTHER - Other
 *              - 0x04 - MWI_MESSAGE_TYPE_VIDEOMAIL - Videomail
 *
 *  \param  activeInd
 *          - Indicates whether the indication is active
 *              - 0x00 - Inactive
 *              - 0x01 - Active
 *
 *  \param  msgCount
 *          - Number of messages
 */
typedef struct
{
    BYTE msgType;
    BYTE activeInd;
    BYTE msgCount;
} messageWaitingInfoContent;

/**
 *  This structure contains Get Message Waiting Info Response parameters
 *
 *  \param  numInstances
 *          - Number of sets of the elements in structure messageWaitingInfoContent
 *
 *  \param  pMsgWaitInfo
 *          - Pointer to structure of messageWaitingInfoContent.
 *              - See \ref messageWaitingInfoContent for more information.
 *
 */
typedef struct
{
    BYTE                      numInstances;
    messageWaitingInfoContent msgWaitInfo[NUM_OF_SET];
} getMsgWaitingInfo;

/**
 *  This API provides information about the message waiting information.
 *
 *  \param  pGetMsgWaitingInfoResp [OUT]
 *          - Pointer to structure of getMsgWaitingInfoResp
 *              - See \ref getMsgWaitingInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSGetMessageWaiting (
    getMsgWaitingInfo  *pGetMsgWaitingInfoResp );

/**
 *  This structure contains SMS parameters
 *
 *  \param  messageFormat
 *          - Message format
 *          - Values:
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  messageSize
 *          - The length of the message contents in bytes
 *
 *  \param  pMessage
 *          - The message contents
 *
 *  \param  pForceOnDC
 *          - Force the message to be sent on the CDMA dedicated channel.
 *          - Values:
 *              - 0x00 - Do not care about the channel on which the message is sent
 *              - 0x01 - Request to send the message over the dedicated channel
 *
 *  \param  pServiceOption
 *          - Service option:
 *          - Values:
 *              - 0x00 - SO_AUTO - AUTO (choose the best service option)
 *              - 0x06 - SO_6 - Service option 6
 *              - 0x0E - SO_14 - Service option 14
 *
 *  \param  pFollowOnDC
 *          - Flag to request not to disconnect the CDMA dedicated channel
 *            after the send operation is complete.
 *          - This TLV can be included if more messages are expected to follow.
 *          - Values:
 *              - 0x01 - FOLLOW_ON_DC_ON - On (don't disconnect after send operation)
 *                Any value other than 0x01 is treated as an absence of this TLV.
 *
 *  \param  pLinktimer
 *          - Keeps the GW SMS link open for the specified number of seconds;
 *            can be enabled if more messages are expected to follow
 *
 *  \param  pSmsOnIms
 *          - Indicates whether the message is to be sent on IMS.
 *          - Values:
 *              - 0x00 - Message is not to be sent on IMS
 *              - 0x01 - Message is to be sent on IMS
 *              - 0x02 to 0xFF - Reserved
 *
 *  \param  pRetryMessage
 *          - Indicates this message is a retry message.
 *          - Values:
 *              - 0x01 - WMS_MESSAGE_IS_A_RETRY - Message is a retry message
 *              Note: Any value other than 0x01 in this field is treated
 *              as an absence of this TLV.
 *
 *
 *  \param  pRetryMessageId
 *          - Message ID to be used in the retry message.
 *          - The message ID specified here is used instead of the messsage ID
 *            encoded in the raw message.
 *
 *  \param  pUserData
 *          - Enables the control point to associate the request with the
 *            corresponding indication.
 *          - The control point might send numerous requests.
 *          - This TLV will help the control point to identify the request
 *            for which the received indication belongs.
 */
typedef struct
{
    ULONG messageFormat;
    ULONG messageSize;
    BYTE  *pMessage;
    BYTE  *pForceOnDC;
    BYTE  *pServiceOption;
    BYTE  *pFollowOnDC;
    BYTE  *pLinktimer;
    BYTE  *pSmsOnIms;
    BYTE  *pRetryMessage;
    ULONG *pRetryMessageId;
    ULONG *pUserData;
}slqssendasyncsmsparams_s;

/**
 *  Sends an SMS message for immediate over-the-air transmission
 *
 *  \param  pSendSmsParams
 *          - structure containing the SMS parameters. Refer slqssendasyncsmsparams_s
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 minutes
 *
 */
ULONG SLQSSendAsyncSMS( slqssendasyncsmsparams_s *pSendSmsParams );

/**
 *  Sets the SMS Storage on the device
 *
 * \param  smsStorage[IN]
 *         - SMS Storage
 *          - 0x01 - device’s permanent memory
 *          - 0x02 - UICC
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: ALL\n
 *          Timeout: 5 seconds
 */
ULONG SLQSSetSmsStorage( BYTE smsStorage );

/**
 *  This API queries the device to return current
 *  SMS configuration that is applied to all incoming and outgoing messages.
 *
 *  \param  pSmsStorage[OUT]
 *          - Values:
 *              - 0x01 - device’s permanent memory
 *              - 0x02 - UICC
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Secs
 */
ULONG SLQSSwiGetSMSStorage( ULONG *pSmsStorage );

/**
 *  Sends a long SMS message for immediate over-the-air transmission, a short SMS
 *  can be sent by this API as well, the input message is text string without any
 *  encoding
 *
 *  \param  messageFormat[IN] 
 *          - Message format
 *              - 0 - CDMA (IS-637B)
 *              - 1 - 5 (Reserved)
 *              - 6 - GSM/WCDMA PP
 *
 *  \param  messageSize[IN] 
 *          - Message size of the input message text
 *
 *  \param  pMessage[IN] 
 *          - Original message text 
 *
 *  \param  encodingScheme[IN] 
 *          - Encoding method to generate the PDU
 *              - 0 - 7 bit encoding
 *              - 4 - 8 bit encoding
 *              - 8 - 16 bit UCS2 encoding
 *              - others value will be treated as default 7 bit encoding
 *
 *  \param  pMessageFailureCode[OUT] 
 *          - message failure code. If cause code is not provided,
 *            then value will be 0xFFFFFFFF
 * 
 *  \param  pMobileNum[IN] 
 *          - Mobile number of the receiver 
 *
 *  \param  pSmsOnIMS[IN] 
 *          -  A flag indicates whether SMS was sent through IMS
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: 3GPP and 3GPP2, but 3GPP2 does not support multiple sms\n
 *          Timeout: 5 minutes
 *
 */
ULONG SLQSSendLongSMS(
    ULONG messageFormat,
    ULONG messageSize,
    CHAR  *pMessage,
    BYTE  encodingScheme,
    ULONG *pMessageFailureCode,
    CHAR  *pMobileNum ,
    BYTE  *pSmsOnIMS);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SMS_H__ */
