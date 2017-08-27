/**
 * \ingroup swioma
 *
 * \file    qaGobiApiSwiOmadms.h
 *
 * \brief   SWI Open Mobile Alliance Device Management Service API function
 *          prototypes
 *
 * Copyright: © 2013-2105 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_SWIOMADMSEXT_H__
#define __GOBI_API_SWIOMADMSEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Starts an OMA-DM session.
 *
 *  \param  sessionType[IN]
 *          - Session type
 *          0: Client Initiated Device Configuration (CIDC)
 *          1: Client initiated FUMO
 *          2: Device initiated FUMO
 *          3: Device Initiated DM
 *          4. Client Initiated PRL
 *          5. User Initiated HFA
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMStartSessionExt(
    ULONG sessionType );

/**
 *  Cancels an ongoing OMA-DM session.
 *
 *  \param  session[IN]
 *          - Session
 *          0: Client Initiated Device Configuration (CIDC)
 *          1: Client initiated FUMO
 *          2: Device initiated FUMO
 *          3: Device Initiated DM
 *          4. Client Initiated PRL
 *          5. User Initiated HFA
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMCancelSessionExt(
    ULONG session );


/**
 *  Sends the specified OMA-DM selection for the current network initiated
 *  session.
 *
 *  \param  selection[IN]
 *          - OMA-DM NIA Selection
 *              0 - User selects NO
 *              1 - User selects "YES/OK"
 *              2 - User selects "Delay"
 *              3 - User selects "Delay one hour"
 *              4 - User selects "Delay four hours"
 *              5 - User selects "Delay until 2:00 AM"
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMSendSelectionExt(
    ULONG selection );

/**
 *  Structure containing the OMA DM settings to be set on the device
 *
 *  \param  FOTAdownload
 *          - 1 Byte parameter indicating support for FOTA Automatic download
 *              - 0x00 - Firmware autodownload FALSE
 *              - 0x01 - Firmware autodownload TRUE
 *
 *  \param  FOTAUpdate
 *          - 1 byte parameter indicating FOTA Automatic update
 *              - 0x00 - Firmware autoupdate FALSE
 *              - 0x01 - Firmware autoupdate TRUE
 *
 *  \param  OMADMEnable[IN]
 *          -  1 byte parameter indicating OMA DM
 *              - 0x00 - OMA DM Disabled
 *              - 0x01 - OMA DM Enabled
 *
 *  \param  OMADMLogEnable[IN]
 *          -  1 byte parameter indicating OMA DM Logs
 *              - 0x00 - OMA DM Logs Disabled
 *              - 0x01 - OMA DM Logs Enabled
 *
 *  \param  FUMOEnable[IN]
 *          -  1 byte parameter indicating FUMO enabled
 *              - 0x00 - FUMO Disabled
 *              - 0x01 - FUMO enabled
 *
 *  \param  PRLEnable[IN]
 *          -  1 byte parameter indicating PRL Enabled
 *              - 0x00 - PRL Disabled
 *              - 0x01 - PRL Enabled
 *              - 0x02 - Launch a CI PRL
 *              - 0x03 - PRL Enabled and PRL automatic update every 45 days
 *              - 0x04 - PRL Enabled and PRL update ever 90 days
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 */
typedef struct _SLQSOMADMSettingsReqParamsExt
{
    BYTE  FOTAdownload;
    BYTE  FOTAUpdate;
    BYTE  OMADMEnable;
    BYTE  OMADMLogEnable;
    BYTE  FUMOEnable;
    BYTE  PRLEnable;
} SLQSOMADMSettingsReqParamsExt;


/**
 * Sets the settings related to OMADM. These settings are saved on the modem
 * across power cycles.
 *
 *  \param pSLQSOMADMSettingsReqParams[IN]
 *         - See \ref SLQSOMADMSettingsReqParamsExt for more information
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMSetSettingsExt(
    SLQSOMADMSettingsReqParamsExt *pSLQSOMADMSettingsReqParams);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SWIOMADMS_H__ */
