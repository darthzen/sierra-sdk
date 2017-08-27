/**
 * \ingroup dms
 *
 * \file    qaGobiApiDms.h
 *
 * \brief   Device Management Service API function prototypes
 */

/*
 * Copyright: © 2011-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_DMS_H__
#define __GOBI_API_DMS_H__

#define MAX_FSN_LENGTH    255

#define MAX_CUST_ID_LEN   64
#define MAX_CUST_VALUE_LEN 8

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Returns the device manufacturer name
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain.
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetManufacturer(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device model ID
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetModelID(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device firmware revision
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetFirmwareRevision(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device firmware revisions (AMSS, boot, and PRI)
 *
 *  \param  amssSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the AMSS string array can contain
 *
 *  \param  pAMSSString[OUT]
 *          - NULL-terminated AMSS revision string
 *
 *  \param  bootSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the boot string array can contain
 *
 *  \param  pBootString[OUT]
 *          - NULL-terminated boot code revision string
 *
 *  \param  priSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the PRI string array can contain
 *
 *  \param  pPRIString[OUT]
 *          - NULL-terminated PRI revision string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetFirmwareRevisions(
    BYTE   amssSize,
    CHAR * pAMSSString,
    BYTE   bootSize,
    CHAR * pBootString,
    BYTE   priSize,
    CHAR * pPRIString );

/**
 *  Returns the version of the active Preferred Roaming List (PRL) in use by
 *  the device.
 *
 *  \param  pPRLVersion[OUT]
 *          - PRL version number
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetPRLVersion(
    WORD * pPRLVersion );

/**
 *  Returns the device IMSI. This API is deprecated on MC73xx/EM73xx modules
 *  since firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use SLQSUIMReadTransparent()(EF ID: 3F00 7F20 6F07 for 2G card and
 *  3F00 7FFF 6F07 for 3G card) instead for new firmware versions and new modules.
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetIMSI(
    BYTE   stringSize,
    CHAR * pString );

/**
 *  Returns all the serial numbers assigned to the device. These serial numbers
 *  include the ESN (Electronic serial number of the device), the IMEI
 *  (International Mobile Equipment Identity) and MEID (Mobile Equipment
 *  Identifier).
 *
 *  \param  esnSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the ESN string array can contain
 *
 *  \param  pESNString[OUT]
 *          - NULL-terminated ESN string. Empty string is returned when ESN is
 *            not supported/programmed
 *
 *  \param  imeiSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the IMEI string array can contain
 *
 *  \param  pIMEIString[OUT]
 *          - NULL terminated IMEI string. Empty string is returned when IMEI
 *            is not supported/programmed
 *
 *  \param  meidSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the MEID string array can contain
 *
 *  \param  pMEIDString[OUT]
 *          - NULL-terminated MEID string. Empty string is returned when MEID
 *            is not supported/programmed
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   For CDMA devices that use a RUIM, the MEID of the RUIM (if present)
 *          will be returned. Use SLQSSwiGetSerialNoExt() to get MEID of CDMA modems.
 *          Timeout: 2 seconds
 *
 */
ULONG GetSerialNumbers(
   BYTE     esnSize,
   CHAR    *pESNString,
   BYTE     imeiSize,
   CHAR    *pIMEIString,
   BYTE     meidSize,
   CHAR    *pMEIDString );

/**
 *  Returns all the serial numbers assigned to the device. These serial numbers
 *  include the ESN (Electronic serial number of the device), the IMEI
 *  (International Mobile Equipment Identity) and MEID (Mobile Equipment
 *  Identifier).
 *
 *  \param  esnSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the ESN string array can contain
 *
 *  \param  pESNString[OUT]
 *          - NULL-terminated ESN string. Empty string is returned when ESN is
 *            not supported/programmed
 *
 *  \param  imeiSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the IMEI string array can contain
 *
 *  \param  pIMEIString[OUT]
 *          - NULL terminated IMEI string. Empty string is returned when IMEI
 *            is not supported/programmed
 *
 *  \param  meidSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the MEID string array can contain
 *
 *  \param  pMEIDString[OUT]
 *          - NULL-terminated MEID string. Empty string is returned when MEID
 *            is not supported/programmed
 *
 *  \param  imeiSvnSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the IMEI SVN string array can contain.
 *
 *  \param  pImeiSvnString[OUT]
 *          - NULL-terminated IMEI SVN string. Empty string is returned when IMEI
 *            SVN is not supported/programmed.
 */
typedef struct serialNumbersInfo
{
    BYTE     esnSize;
    CHAR    *pESNString;
    BYTE     imeiSize;
    CHAR    *pIMEIString;
    BYTE     meidSize;
    CHAR    *pMEIDString;
    BYTE    imeiSvnSize;
    CHAR    *pImeiSvnString;
}serialNumbersInfo;

/**
 *  Returns all the serial numbers assigned to the device. These serial numbers
 *  include the ESN (Electronic serial number of the device), the IMEI
 *  (International Mobile Equipment Identity), MEID (Mobile Equipment
 *  Identifier) and IMEI SVN (IMEI software version number).
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   For CDMA devices that use a RUIM, the MEID of the RUIM (if present)
 *          will be returned. Use SLQSSwiGetSerialNoExt() to get MEID of CDMA modems.
 *          Timeout: 2 seconds
 */
ULONG SLQSGetSerialNumbers(
       serialNumbersInfo *pSerialNumbersInfo);

/**
 *  Returns the hardware revision of the device
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetHardwareRevision(
    BYTE stringSize,
    CHAR *pString );

/**
 *  Returns the current time of the device based on the value supported by the
 *  network.
 *
 *  \param  pTimeStamp[OUT]
 *          - Count of 1.25 ms that have elapsed from the start of GPS time
 *            (Jan 6, 1980)
 *
 *  \param  pTimeSource[OUT]
 *          - Source of timestamp\n
 *            0 - 32 kHz device clock\n
 *            1 - CDMA network\n
 *            2 - cdma2000 1xEV-DO network
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 *  \note   The source of the timestamp provided specifies how the timestamp
 *          was determined. The first network time that is available will be
 *          returned. If no network time is available, the timestamp is taken
 *          from the 32 kHz slow-clock of the device.
 *
 */
ULONG GetNetworkTime(
    ULONGLONG *pTimeStamp,
    ULONG     *pTimeSource );

/**
 *  Enables or disables protection of SIM contents for a given PIN, This API
 *  is deprecated on MC73xx/EM73xx modules since firmware version 
 *  SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions. Please use API
 *  SLQSUIMSetPinProtection() for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  bEnable[IN]
 *          - Enable/disable PIN protection, 0 = Disable
 *
 *  \param  pValue[IN]
 *          - PIN value of the PIN to be enabled/disabled
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Optional parameter
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Optional parameter
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked
 *            i.e. UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMSetPINProtection(
    ULONG id,
    ULONG bEnable,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Unblocks a blocked SIM. This API is deprecated on MC73xx/EM73xx modules since
 *  firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use API SLQSUIMUnblockPin() for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pPUKValue[IN]
 *          - PUK value of PIN to unblock
 *
 *  \param  pNewValue[IN]
 *          - New PIN value of PIN to unblock
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMUnblockPIN(
    ULONG id,
    CHAR  *pPUKValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Verifies a SIM PIN. This API is deprecated on MC73xx/EM73xx modules since
 *  firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use API SLQSUIMVerifyPin()() for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pValue[IN]
 *          - Value of PIN to verify
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMVerifyPIN(
    ULONG id,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Changes the PIN value for a given PIN. This API is deprecated on MC73xx/EM73xx modules
 *  since firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use API SLQSUIMChangePin() for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pOldValue[IN]
 *          - Old PIN value of PIN to change
 *
 *  \param  pNewValue[IN]
 *          - New PIN value of PIN to change
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Optional Parameter
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMChangePIN(
    ULONG id,
    CHAR  *pOldValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns the voice number in use by the device
 *
 *  \param  voiceNumberSize
 *          - Maximum number of characters (including NULL terminator) that the
 *            voice number array can contain.
 *
 *  \param  pVoiceNumber[OUT]
 *          - Voice number string: MDN or MS ISDN
 *
 *  \param  minSize
 *          - Maximum number of characters (including NULL terminator) that the
 *            MIN array can contain.
 *
 *  \param  pMIN[OUT]
 *          - Optional Parameter
 *          - MIN string: Empty string returned when MIN is not supported/
 *            programmed.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetVoiceNumber(
    BYTE voiceNumberSize,
    CHAR *pVoiceNumber,
    BYTE minSize,
    CHAR *pMIN );

/**
 *  Sets the operating mode of the device.
 *
 *  \param  powerMode[IN]
 *          - Selected operating mode
 *             - See qaGobiApiTablePowerModes.h for power modes
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SetPower(
    ULONG powerMode );

/**
 *  Returns the operating mode of the device.
 *
 *  \param  pPowerMode[OUT]
 *          - Selected operating mode
 *              - See qaGobiApiTablePowerModes.h for power modes
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetPower(
    ULONG *pPowerMode );

/**
 *  Returns the status of the specified UIM facility control key. This API is
 *  deprecated on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Pleaes use SLQSUIMGetConfiguration instead for
 *  new firmware versions and new modules.
 *
 *  \param  id[IN]
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  pStatus[OUT]
 *          - Control key status
 *              - 0 - Deactivated
 *              - 1 - Activated
 *              - 2 - Blocked
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - The number of retries left, after which the control key will be
 *            blocked
 *              - 0xFFFFFFFF - Unknown
 *  \param  pUnblockRetriesLeft[OUT]
 *          - The number of unblock retries left, after which the control key
 *            will be permanently blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMGetControlKeyStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns the UIM ICCID. This API is deprecated on MC73xx/EM73xx modules
 *  since firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use SLQSUIMReadTransparent()(EF ID: 3F00 2FE2) instead for new firmware
 *  versions and new modules.
 *  
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain.
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG UIMGetICCID(
    BYTE stringSize,
    CHAR *pString );

/**
 *  Gets the status of the SIM PINs. This API is deprecated on MC73xx/EM73xx modules
 *  since firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use API SLQSUIMGetCardStatus() for new firmware versions and new modules
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pStatus[OUT]
 *          - PIN status(0xFFFFFFFF - Unknown)
 *              - 0 - PIN not initialized
 *              - 1 - PIN enabled, not verified
 *              - 2 - PIN enabled, verified
 *              - 3 - PIN disabled
 *              - 4 - PIN blocked
 *              - 5 - PIN permanently blocked
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e., UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMGetPINStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns reason why the operating mode of the device is currently offline.
 *
 *  \param  pReasonMask[OUT]
 *          - Optional parameter
 *          - Bitmask of offline reasons
 *              - 0x00000001 - Host image configuration issue
 *              - 0x00000002 - PRI image configuration issue
 *              - 0x00000004 - PRI version incompatible
 *              - 0x00000008 - PRI copy issue
 *              - All others - Reserved
 *
 *  \param  pbPlatform[OUT]
 *          - Optional parameter
 *          - Is the device offline due to a platform restriction?
 *              - 0 - No
 *              - 1 - Yes
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetOfflineReason(
    ULONG *pReasonMask,
    ULONG *pbPlatform );

/**
 *  Changes the specified UIM facility control key. This API is deprecated
 *  on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSUIMDepersonalization()
 *  for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  status[IN]
 *          - Control key status
 *              - 0 - Deactivated
 *              - 1 - Activated
 *              - 2 - Blocked
 *
 *  \param  pValue[IN]
 *          - Control key de-personalization string (maximum length of 8
 *            characters)
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Optional parameter
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the control key will be blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMSetControlKeyProtection(
    ULONG id,
    ULONG status,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft );

/**
 *  Unblocks the specified UIM facility control key. This API is deprecated
 *  on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSUIMDepersonalization()
 *  for new firmware versions and new modules
 *
 *  \param  id[IN]
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  pValue[IN]
 *          - Control key de-personalization string (maximum length of 8
 *            characters)
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Optional parameter
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the control key will be blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG UIMUnblockControlKey(
    ULONG id,
    CHAR  *pValue,
    ULONG *pUnblockRetriesLeft );

/**
 *  Gets the device capabilities
 *
 *  \param  pMaxTXChannelRate[OUT]
 *          - Maximum transmission rate (in bps) supported by the device
 *          - In multi-technology devices, this value will be the greatest rate
 *            among all supported technologies
 *
 *  \param  pMaxRXChannelRate[OUT]
 *          - Maximum reception rate (in bps) supported by the device
 *          - In multi-technology devices, this value will be the greatest
 *            rate among all supported technologies
 *
 *  \param  pDataServiceCapability[OUT]
 *          - CS/PS data service capability
 *              - 0 - No data services supported
 *              - 1 - Only Circuit Switched (CS) services supported
 *              - 2 - Only Packet Switched (PS) services supported
 *              - 3 - Simultaneous CS and PS
 *              - 4 - Non-simultaneous CS and PS
 *
 *  \param  pSimCapability[OUT]
 *          - Device SIM capability
 *              - 0 - SIM not supported
 *              - 1 - SIM supported
 *
 *  \param  pRadioIfacesSize[IN/OUT]
 *          - Upon input, the maximum number of elements that the radio
 *            interface array can contain
 *          - Upon successful output, actual number of elements in the radio
 *            interface array
 *
 *  \param  pRadioIfaces[OUT]
 *          - Radio interface array. This is a structure of array containing the
 *            elements below.\n
 *            ULONG radioInterface
 *              - See qaGobiApiTableRadioInterfaces.h for Radio Interfaces
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetDeviceCapabilities(
    ULONG *pMaxTXChannelRate,
    ULONG *pMaxRXChannelRate,
    ULONG *pDataServiceCapability,
    ULONG *pSimCapability,
    ULONG *pRadioIfacesSize,
    BYTE  *pRadioIfaces );

/**
 *  Resets to default factory settings of the device
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing a six-digit service
 *          programming code
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 minutes
 *
 */
ULONG ResetToFactoryDefaults(
    CHAR *pSPC );

/**
 * This function Validates Service Programming code of the device
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing a six-digit service
 *          programming code
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds
 *
 */
ULONG ValidateSPC(
    CHAR *pSPC );

/**
 *  Requests the device to perform automatic service activation
 *
 *  \param  pActivationCode[IN]
 *          - NULL-terminated string representing activation code
 *          (maximum string length of 12);
 *          specific carrier requirements may dictate actual
 *          activation code that is applicable, e.g., "*22899"
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Minutes
 *
 */
ULONG ActivateAutomatic(
    CHAR *pActivationCode );

/**
 *  This structure contains Extended Roaming Indicator(ERI) file parameters
 *
 *  \param  pFileSize[IN/OUT]
 *          - Upon input, the maximum number of bytes that file
 *            contents array can contain.\n
 *          - Upon successful output, actual number of bytes written to
 *            file contents array
 *
 *  \param  pFile[OUT]
 *          - ERI data read from persistent storage( Max size is 1024 )
 *
 */
typedef struct ERIFileparams
{
    WORD  *pFileSize;
    BYTE  *pFile;
}ERIFileparams;

/**
 *  Returns the Extended Roaming Indicator (ERI) file that
 *  is stored in EFS on the device at a predetermined location.
 *  See the carrier requirements for specific details.
 *
 *  \param  pERIFileparams
 *          - Pointer to structure ERIFileparams
 *          - See \ref ERIFileparams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 Seconds
 *
 */
ULONG SLQSGetERIFile(
    ERIFileparams *pERIFileparams );

/**
 *  Returns the device activation state.
 *
 *  \param  pActivationState[OUT]
 *          -  Service Activation Code\n
 *              0 - Service not activated\n
 *              1 - Service activated\n
 *              2 - Activation connecting\n
 *              3 - Activation connected\n
 *              4 - OTASP security authenticated\n
 *              5 - OTASP NAM downloaded\n
 *              6 - OTASP MDN downloaded\n
 *              7 - OTASP IMSI downloaded\n
 *              8 - OTASP PRL downloaded\n
 *              9 - OTASP SPC downloaded\n
 *              10 - OTASP settings committed
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetActivationState(
    ULONG *pActivationState);

/**
 *  Returns the UIM state. This API is deprecated on MC73xx/EM73xx modules
 *  since firmware version SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions.
 *  Please use API SLQSUIMGetCardStatus() for new firmware versions and new modules
 *
 *  \param  pUIMState[OUT]
 *          -  UIM state:\n
 *             - 0x00 - UIM initialization completed\n
 *             - 0x01 - UIM locked or failed\n
 *             - 0x02 - UIM not present\n
 *             - 0x03 - 0xFE - Reserved\n
 *             - 0xFF - UIM state currently unavailable\n
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSUIMGetState(
    ULONG *pUIMState);

/**
 *  Returns the band capability of the device.
 *
 *  \param  pBandCapability[OUT]
 *          Bitmask of bands supported by the device
 *              - Bit 0 - Band class 0, A-system
 *              - Bit 1 - Band class 0, B-system
 *              - Bit 2 - Band class 1, all blocks
 *              - Bit 3 - Band class 2
 *              - Bit 4 - Band class 3, A-system
 *              - Bit 5 - Band class 4, all blocks
 *              - Bit 6 - Band class 5, all blocks
 *              - Bit 7 - GSM DCS band (1800)
 *              - Bit 8 - GSM Extended GSM (E-GSM) band (900)
 *              - Bit 9 - GSM Primary GSM (P-GSM) band (900)
 *              - Bit 10 - Band class 6
 *              - Bit 11 - Band class 7
 *              - Bit 12 - Band class 8
 *              - Bit 13 - Band class 9
 *              - Bit 14 - Band class 10
 *              - Bit 15 - Band class 11
 *              - Bit 16 - GSM 450 band
 *              - Bit 17 - GSM 480 band
 *              - Bit 18 - GSM 750 band
 *              - Bit 19 - GSM 850 band
 *              - Bit 20 - GSM railways GSM band (900)
 *              - Bit 21 - GSM PCS band (1900)
 *              - Bit 22 - WCDMA (Europe, Japan, and China) 2100 band
 *              - Bit 23 - WCDMA US PCS 1900 band
 *              - Bit 24 - WCDMA (Europe and China) DCS 1800 band
 *              - Bit 25 - WCDMA US 1700 band
 *              - Bit 26 - WCDMA US 850 band
 *              - Bit 27 - WCDMA Japan 800 band
 *              - Bit 28 - Band class 12
 *              - Bit 29 - Band class 14
 *              - Bit 30 - Reserved
 *              - Bit 31 - Band class 15
 *              - Bits 32 through 47 - Reserved
 *              - Bit 48 - WCDMA Europe 2600 band
 *              - Bit 49 - WCDMA Europe and Japan 900 band
 *              - Bit 50 - WCDMA Japan 1700 band
 *              - Bits 51 through 55 - Reserved
 *              - Bit 56 - Band class 16
 *              - Bit 57 - Band class 17
 *              - Bit 58 - Band class 18
 *              - Bit 59 - Band class 19
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSGetBandCapability( ULONGLONG *pBandCapability );

/**
 *  This structure contains current settings of custom features
 *
 *  \param  GpsEnable[OUT]
 *          - describes if GPS is enabled or disabled
 *          - values:
 *              - 0x00 - GPS is disabled
 *              - 0x01 - GPS is enabled
 *          - function SLQSGetCustFeatures() returns a default value FFFFFFFF
 *            if no value is returned by the modem
 *
 *  \param  pDisableIMSI[OUT]
 *          - optional 1 byte parameter
 *          - describes if IMSI display is enabled or disabled
 *          - values:
 *              - 0x00 - Allow display of IMSI
 *              - 0x01 - Do not display IMSI
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 *  \param  pIPFamSupport[OUT]
 *          - optional 2 byte BitMask
 *          - bitmask representing the IP families supported
 *          - values:
 *              - 0x01 - IPv4
 *              - 0x02 - IPv6
 *              - 0x04 - IPv4v6
 *          - function SLQSGetCustFeatures() returns a default value FFFF
 *            if no value is returned by the modem
 *
 *  \param  pRMAutoConnect[OUT]
 *          - optional 1 byte parameter
 *          - QMI Mode RM Net Auto Connect Support
 *          - values:
 *              - 0x00 - Not Supported
 *              - 0x01 - Supported
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 *  \param  pGPSSel[OUT]
 *          - optional 1 byte parameter
 *          - GPS Antenna Select
 *          - values:
 *              - 0x00 - Dedicated GPS Port
 *              - 0x01 - GPS Rx over AUX Port
 *              - 0x02 - GPS Rx over dedicated GPS port with no bias
 *                       voltage applied
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 *  \param  pSMSSupport[OUT]
 *          - optional 1 byte parameter
 *          - SMS support
 *          - values:
 *              - 0x00 - Not supported
 *              - 0x01 - supported
 *          - Used to determine whether or not to hide SMS from user
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem. In this case assume,
 *            SMS is supported.
 *
 *  \param  pIsVoiceEnabled[OUT]
 *          - optional 1 byte parameter
 *          - Voice support
 *          - values:
 *              - 0x00 - Enable voice on both AT and QMI interface (default)
 *              - 0x01 - Reserved
 *              - 0x02 - Disable voice on both AT and QMI interface
 *
 *  \param  pDHCPRelayEnabled[OUT]
 *          - optional 1 byte parameter
 *          - DHCP Relay support
 *          - values:
 *              - 0x00 - Disable DHCP relay
 *              - 0x01 - Enable DHCP relay
 *
 *  \param  pGPSLPM[OUT]
 *          - optional 1 byte parameter
 *          - GPSLPM support
 *          - values:
 *              - 0x00 - Enable GPS in Low Power Mode
 *              - 0x01 - Disable GPS in Low Power Mode
 *
 */
typedef struct custFeaturesInfo
{
    ULONG GpsEnable;
    BYTE  *pDisableIMSI;
    WORD  *pIPFamSupport;
    BYTE  *pRMAutoConnect;
    BYTE  *pGPSSel;
    BYTE  *pSMSSupport;
    BYTE  *pIsVoiceEnabled;
    BYTE  *pDHCPRelayEnabled;
    BYTE  *pGPSLPM;
}custFeaturesInfo;

/**
 *  This API fetches the current settings of custom features. This API is deprecated for EM74xx/MC74xx,
 *  please use SLQSGetCustFeaturesV2() instead for EM74xx/MC74xx.
 *
 *  \param  pCustFeaturesInfo
 *          - Structure containing settings of custom features.
 *          - See \ref custFeaturesInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSGetCustFeatures(
    custFeaturesInfo *pCustFeaturesInfo );

/**
 *  This structure contains settings to be used for custom features
 *
 *  \param  pGPSSel
 *          - optional 1 byte parameter
 *          - GPS Antenna Select
 *          - values:
 *              - 0x00 - Dedicated GPS Port
 *              - 0x01 - GPS Rx over AUX Port
 *              - 0x02 - GPS Rx over dedicated GPS port with no bias
 *                       voltage applied
 *
 *  \param  pGPSEnable
 *          - optional 4 byte parameter
 *          - GPS Enable/Disable
 *          - values: The value of 7 least significant bits:
 *              - 0 - Disabled
 *              - 1 - MT & MO enabled
 *              - 2 - MO enabled
 *              - 3 - MT enabled
 *              - 4 - MT & MO enabled if GPS_DISABLE pin is not asserted
 *              - 5 - MO GPS enabled if GPS_DISABLE pin is not asserted
 *              - 6 - MT GPS enabled if GPS_DISABLE pin is not asserted
 *
 *  \note   Only MC7750 3.5.x firmware supports above 0x04, 0x05 and 0x06
 *          settings. To disable GLONASS, set the most significant bit - 0x80.
 *          This setting is only applicable if GPS is not Disabled.
 *
 *  \param  pIsVoiceEnabled
 *          - optional 1 byte parameter
 *          - voice enabled/disabled
 *          - values:
 *              - 0 - Enable voice on both AT and QMI interface (default)
 *              - 1 - Reserved
 *              - 2 - Disable voice on both AT and QMI interface
 *
 *  \param  pDHCPRelayEnabled
 *          - optional 1 byte parameter
 *          - DHCPRELAYENABLE support
 *          - values:
 *              - 0 - Disable DHCP relay
 *              - 1 - Enable DHCP relay
 *
 *  \param  pGPSLPM
 *          - optional 1 byte parameter
 *          - GPSLPM support
 *          - values:
 *              - 0 - Enable GPS in Low Power Mode
 *              - 1 - Disable GPS in Low Power Mode
 */
typedef struct custFeaturesSetting
{
    BYTE  *pGPSSel;
    ULONG *pGPSEnable;
    BYTE  *pIsVoiceEnabled;
    BYTE  *pDHCPRelayEnabled;
    BYTE  *pGPSLPM;
}custFeaturesSetting;

/**
 *  This API changes the settings of custom features, a reset is required
 *  for any settings that are changed to take effect. This API is deprecated
 *  for EM74xx/MC74xx, please use SLQSSetCustFeaturesV2() for EM74xx/MC74xx.
 *
 *  \param  pCustFeaturesSetting[IN]
 *          - Structure containing settings of custom features.
 *          - See \ref custFeaturesSetting for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Secs
 *
 */
ULONG SLQSSetCustFeatures(
    custFeaturesSetting *pCustFeaturesSetting );

/**
 *  This structure contains GetCurrentPRLInfo response parameter
 *
 *  \param  pPRLVersion[OUT] - Optional
 *          - PRL version of device.
 *
 *  \param  pPRLPreference [OUT]- Optional
 *          - PRL Preference
 *              - 0 - Unset
 *              - 1 - Set
 */
typedef struct dmsCurrentPRLInfo
{
    WORD *pPRLVersion;
    BYTE *pPRLPreference;
} dmsCurrentPRLInfo;

/**
 *  This API get the currently active PRL information of the device.
 *
 *  \param  pCurrentPRLInfo
 *          - Pointer to structure dmsCurrentPRLInfo
 *          - See \ref dmsCurrentPRLInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 Secs
 *
 */
ULONG SLQSGetCurrentPRLInfo(
    dmsCurrentPRLInfo *pCurrentPRLInfo );

/**
 *  This structure used to store Factory Sequence Number parameter
 *
 *  \param  FSNumber[OUT]
 *          - Facorty Sequence Number
 *          - Maximum Length is 255 Bytes
 *
 */
typedef struct
{
    BYTE FSNumber[MAX_FSN_LENGTH];
} FactorySequenceNumber;

/**
 *  This API get the Factory Sequence Number of the device.
 *
 *  \param  pFSNumber
 *          - Pointer to structure FactorySequenceNumber
 *          - See \ref FactorySequenceNumber for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiGetFSN( FactorySequenceNumber *pFSNumber);


/**
 *  This structure is used to store Firmware Update Status
 *
 *  \param  ResCode[OUT]
 *          - FW Update Result Code
 *          - Values:
 *              - 0x00000001 - Successful
 *              - 0xFFFFFFFF - Unknown (due to power off reset after firmware
 *                             update )
 *              - 0x100000nn - File update errors while nn will be the exact
 *                             error number:
 *                             - 00 - General error
 *              - 0x200000nn - NVUP update errors while nn will be the exact
 *                             error number:
 *                             - 00 - General error
 *              - 0x40000nnn - FOTA update agent errors while nnn will be the
 *                             exact error number:
 *                             - 000 ~ 0FF - Insignia defined error code
 *                             - 100 ~ 1FF - Sierra defined error code
 *                             - See qaGobiApiTableFwDldErrorCodes.h for more
 *                               detailed information
 *              - 0x800000nn - FDT/SSDP reported errors while nn will be the
 *                             exact error number
 *                             - See qaGobiApiTableFwDldErrorCodes.h for more
 *                               detailed information
 *
 *  \param  pImgType[OUT]
 *          - Optional parameter
 *          - Firmware image type that failed the update
 *
 *  \param  pRefData[OUT]
 *          - Optional parameter
 *          - Failed image reference data
 *          - This is normally the offset of the image that caused the failure
 *
 *  \param  pRefStringLen[IN/OUT]
 *          - Length of Reference String parameter to follow
 *          - As input parameter specifies length assigned to
 *            pRefString parameter
 *          - As output parameter specifies length of actual value retrieved
 *            from the device
 *
 *  \param  pRefString[OUT]
 *          - Optional parameter
 *          - Failed image reference string. This is normally the partition
 *            name of the image that caused the failure if applicable.
 *
 *  \param  pLogStringLen[IN/OUT]
 *          - Length of Reference String parameter to follow
 *          - As input parameter specifies length assigned to
 *            pRefString parameter
 *          - As output parameter specifies length of actual value retrieved
 *            from the device
 *
 *  \param  pLogString[OUT]
 *          - Optional parameter
 *          - Failed image reference string. This is normally the partition
 *            name of the image that caused the failure if applicable.

 */
typedef struct
{
    ULONG ResCode;
    BYTE  *pImgType;
    ULONG *pRefData;
    BYTE  *pRefStringLen;
    BYTE  *pRefString;
    BYTE  *pLogStringLen;
    BYTE  *pLogString;
} FirmwareUpdatStat;

/**
 *  This API will be used to query last firmware update status. The firmware
 *  status is stored in RAM and can be retained over warm resets but not power
 *  off resets.
 *
 *  \param  pFirmwareUpdatStat
 *          - Pointer to structure FirmwareUpdatStat
 *          - See \ref FirmwareUpdatStat for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Device Supported: MC73xx\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiGetFwUpdateStatus( FirmwareUpdatStat *pFirmwareUpdatStat );

/**
 *  This structure is used to store retrieved USB Composition
 *
 *  \param  pUSBComp[OUT]
 *          - Current USB Composition(optional parameter)
 *          - Values:
 *              - 0..5 - Reserved (non-QMI)
 *              - 6 - DM NMEA AT QMI
 *              - 7 - DM NMEA AT QMI1 QMI2 QMI3
 *              - 8 - DM NMEA AT MBIM
 *              - 9 - MBIM
 *              - 10 - NMEA MBIM
 *              - 11 - DM MBIM
 *              - 12 - DM NMEA MBIM \n
 *          13-22 are combined compositions. One is for Win8 MBIM interfaces,
 *          another is for legacy QMI interfaces
 *              - 13 - 6 for QMI, 8 for MBIM
 *              - 14 - 6 for QMI, 9 for MBIM
 *              - 15 - 6 for QMI, 10 for MBIM
 *              - 16 - 6 for QMI, 11 for MBIM
 *              - 17 - 6 for QMI, 12 for MBIM
 *              - 18 - 7 for QMI, 8 for MBIM
 *              - 19 - 7 for QMI, 9 for MBIM
 *              - 20 - 7 for QMI, 10 for MBIM
 *              - 21 - 7 for QMI, 11 for MBIM
 *              - 22 - 7 for QMI, 12 for MBIM
 *
 *  \param  pNumSupUSBComps[OUT]
 *          - Number of supported USB compositions in the parameter to follow
 *          - Range - 0-255
 *
 *  \param  pSupUSBComps[OUT]
 *          - Optional parameter
 *          - List of supported USB compositions( 1 Byte each - Max 255 )
 *          - Total length is defined by pNumSupUSBComps parameter
 *          - Values:
 *              - 0..5 - Reserved (non-QMI)
 *              - 6 - DM NMEA AT QMI
 *              - 7 - DM NMEA AT QMI1 QMI2 QMI3
 *              - 8 - DM NMEA AT MBIM
 *              - 9 - MBIM
 *              - 10 - NMEA MBIM
 *              - 11 - DM MBIM
 *              - 12 - DM NMEA MBIM \n
 *          13-22 are combined compositions. One is for Win8 MBIM interfaces,
 *          another is for legacy QMI interfaces
 *              - 13 - 6 for QMI, 8 for MBIM
 *              - 14 - 6 for QMI, 9 for MBIM
 *              - 15 - 6 for QMI, 10 for MBIM
 *              - 16 - 6 for QMI, 11 for MBIM
 *              - 17 - 6 for QMI, 12 for MBIM
 *              - 18 - 7 for QMI, 8 for MBIM
 *              - 19 - 7 for QMI, 9 for MBIM
 *              - 20 - 7 for QMI, 10 for MBIM
 *              - 21 - 7 for QMI, 11 for MBIM
 *              - 22 - 7 for QMI, 12 for MBIM
 *
 */
typedef struct
{
    BYTE  *pUSBComp;
    BYTE  *pNumSupUSBComps;
    BYTE  *pSupUSBComps;
} USBCompParams;

/**
 *  This API queries the modem's USB interface configuration and supported
 *  configuration parameters.
 *
 *  \param  pUSBCompParams
 *          - Pointer to structure USBCompParams
 *          - See \ref USBCompParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiGetUSBComp( USBCompParams *pUSBCompParams );

/**
 *  This structure is used to store USB composition information
 *
 *  \param  pUSBComp[IN]
 *          - Current USB Composition
 *          - Values:
 *              - 0..5 - Reserved (non-QMI)
 *              - 6 - DM NMEA AT QMI
 *              - 7 - DM NMEA AT QMI1 QMI2 QMI3
 *              - 8 - DM NMEA AT MBIM
 *              - 9 - MBIM
 *              - 10 - NMEA MBIM
 *              - 11 - DM MBIM
 *              - 12 - DM NMEA MBIM \n
 *          13-22 are combined compositions. One is for Win8 MBIM interfaces,
 *          another is for legacy QMI interfaces
 *              - 13 - 6 for QMI, 8 for MBIM
 *              - 14 - 6 for QMI, 9 for MBIM
 *              - 15 - 6 for QMI, 10 for MBIM
 *              - 16 - 6 for QMI, 11 for MBIM
 *              - 17 - 6 for QMI, 12 for MBIM
 *              - 18 - 7 for QMI, 8 for MBIM
 *              - 19 - 7 for QMI, 9 for MBIM
 *              - 20 - 7 for QMI, 10 for MBIM
 *              - 21 - 7 for QMI, 11 for MBIM
 *              - 22 - 7 for QMI, 12 for MBIM
 *
 */
typedef struct
{
    BYTE *pUSBComp;
} USBCompConfig;

/**
 *  This API is used to change the modem's USB interface configuration thus
 *  allowing a device to have multiple USB compositions. Devices will,
 *  by default, be configured to support a minimal set of interfaces to reduce
 *  end user modem installation time. Developers and some customers, however,
 *  require access to a custom set of interfaces. A reset is required for any
 *  change in the USB composition to take effect.
 *
 *  \param  pUSBCompConfig
 *          - Pointer to structure USBCompConfig
 *          - See \ref USBCompConfig for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiSetUSBComp( USBCompConfig *pUSBCompConfig );

/**
 *  This structure is used to store Crash Information
 *
 *  \param  numCrashes[OUT]
 *          - Number of instances of the remaining fields
 *
 *  \param  crashId[OUT]
 *          - Random crash id assigned at crash
 *
 *  \param  crashData[OUT]
 *          - Crash Data
 *
 *  \param  crashStrLen[IN/OUT]
 *          - Length of pCrashString as an INPUT,  length of the pCrashString field returned by API as an OUTPUT
 *
 *  \param  pCrashString[OUT]
 *          - Pointer to store crash string
 *
 *  \param  gcDumpStrLen[IN/OUT]
 *          - Length of pGCDumpString as an INPUT, Length of the pGCDumpString field returned by API as an OUTPUT
 *
 *  \param  pGCDumpString[OUT]
 *          - gcdump string for the crash
 */
typedef struct
{
    WORD  numCrashes;
    ULONG crashId;
    ULONG crashData;
    WORD  crashStrLen;
    CHAR  *pCrashString;
    WORD  gcDumpStrLen;
    CHAR  *pGCDumpString;

} CrashInfo;

/**
 *  This structure is used to store Crash Information
 *
 *  \param  pDevCrashStatus[OUT]
 *          - Device Crash Status
 *          - 0 - no crash
 *          - 1 - crash has occurred
 *
 *  \param  pCrashInfo[OUT]
 *          - Pointer to structure CrashInfo (Optional parameter)
 *          - See \ref CrashInfo for more information
 */
typedef struct
{
    BYTE      *pDevCrashStatus;
    CrashInfo *pCrashInfo;
} CrashInfoParams;

/**
 *  This API queries the Crash Information from the device.
 *
 *  \param  pClear[IN]
 *          - request parameter Clear(Optional parameter)
 *          - Values:
 *                0 - Do not clear crash data after response
 *                1 - Clear crash data after response
 *
 *  \param  pCrashInfoParams[Out]
 *          - Pointer to structure CrashInfoParams
 *          - See \ref CrashInfoParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiGetCrashInfo( BYTE *pClear,CrashInfoParams *pCrashInfoParams );

/**
 *  This API queries the Crash State from the device.
 *
 *  \param  pDevCrashState[OUT]
 *          - Device Crash State
 *          - Values:
 *               - 0 - USB Memory Download
 *                     Modem will reset after a crash and will stay in
 *                     USB download mode with only ttyUSB0 enumerated.
 *                     ramdump tool is to be used to recover the crash dump.
 *                     Modem needs to be reset again to come back in ONLINE mode.
 *               - 1 - Reset
 *                     Modem will reset and come back in ONLINE mode.
 *                     Minimal crash data will be available and can be extracted
 *                     with at!gcdump? AT command or SLQSSwiGetCrashInfo() SDK API
 *               - 2 - No action
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *             Please free two buffers after get crash report successfully 
 *                 1. pCrashInfoParams->pCrashInfo->pCrashString
 *                 2. pCrashInfoParams->pCrashInfo->pGCDumpString
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiGetCrashAction( BYTE *pDevCrashState );

/**
 *  This API set the Crash Action to the device.
 *
 *  \param  crashActionParams[IN]
 *          - Crash Action
 *          - Values:
 *               - 0 - USB Memory Download
 *                     Modem will reset after a crash and will stay in
 *                     USB download mode with only ttyUSB0 enumerated.
 *                     ramdump tool is to be used to recover the crash dump.
 *                     Modem needs to be reset again to come back in ONLINE mode.
 *               - 1 - Reset
 *                     Modem will reset and come back in ONLINE mode.
 *                     Minimal crash data will be available and can be extracted
 *                     with at!gcdump? AT command or SLQSSwiGetCrashInfo() SDK API
 *               - 2 - No action
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 5 Secs
 *
 */
ULONG SLQSSwiSetCrashAction( BYTE crashActionParams);
/**
 *  This structure is used to Get Host Device Information
 *
 *  \param  bManSize[IN/OUT]
 *          - Host Device Manufacturer String Size
 *
 *  \param  pManString[OUT]
 *          - Host Device Manufacturer Name(Optional parameter)
 *          - Null terminated ASCII String
 *
 *  \param  bModelSize[IN/OUT]
 *          - Host Device Model String Size
 *
 *  \param  pModelString[OUT]
 *          - Host Device Model String(Optional parameter)
 *          - Null terminated ASCII string.
 *
 *  \param  bSWVerSize[IN/OUT]
 *          - Host Device Software Version String Size
 *
 *  \param  pSWVerString[OUT]
 *          - Host Device Software Version String(Optional parameter)
 *          - Null terminated ASCII string
 *
 *  \param  bPlasmaIDSize[IN/OUT]
 *          - Host Device Plasma ID String Size
 *
 *  \param  pPlasmaIDString[OUT]
 *          - Host Device Plasma ID String(Optional parameter)
 *          - Null terminated alphanumeric ASCII String.
 *
 */
typedef struct _SLQSSwiGetHostDevInfoParams
{
   BYTE     bManSize;
   CHAR    *pManString;
   BYTE     bModelSize;
   CHAR    *pModelString;
   BYTE     bSWVerSize;
   CHAR    *pSWVerString;
   BYTE     bPlasmaIDSize;
   CHAR    *pPlasmaIDString;
} SLQSSwiGetHostDevInfoParams;

/**
 *  This API Get Host Information from the device.
 *
 *  \param  pGetHostDevInfoParams
 *          -  See \ref SLQSSwiGetHostDevInfoParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 2 Secs
 */

ULONG SLQSSwiGetHostDevInfo( SLQSSwiGetHostDevInfoParams *pGetHostDevInfoParams );


/**
 *  This structure is used to Set Host Device Information
 *
 *  \param  bManSize[IN]
 *          - Host Device Manufacturer String Size
 *
 *  \param  pManString[IN]
 *          - Host Device Manufacturer Name(Optional parameter)
 *          - Null terminated ASCII String
 *
 *  \param  bModelSize[IN]
 *          - Host Device Model String Size
 *
 *  \param  pModelString[IN]
 *          - Host Device Model String(Optional parameter)
 *          - Null terminated ASCII string.
 *
 *  \param  bSWVerSize[IN]
 *          - Host Device Software Version String Size
 *
 *  \param  pSWVerString[IN]
 *          - Host Device Software Version String(Optional parameter)
 *          - Null terminated ASCII string
 *
 *  \param  bPlasmaIDSize[IN]
 *          - Host Device Plasma ID String Size
 *
 *  \param  pPlasmaIDString[IN]
 *          - Host Device Plasma ID String(Optional parameter)
 *          - Null terminated alphanumeric ASCII String.
 *
 */
typedef struct _SLQSSwiSetHostDevInfoParams
{
   BYTE     bManSize;
   CHAR    *pManString;
   BYTE     bModelSize;
   CHAR    *pModelString;
   BYTE     bSWVerSize;
   CHAR    *pSWVerString;
   BYTE     bPlasmaIDSize;
   CHAR    *pPlasmaIDString;
} SLQSSwiSetHostDevInfoParams;

/**
 *  This API Sets the host device info configured on the modem for OMA-DM reporting
 *
 *  \param  pSetHostDevInfoParams
 *          -  See \ref SLQSSwiSetHostDevInfoParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 2 Secs
 */

ULONG SLQSSwiSetHostDevInfo( SLQSSwiSetHostDevInfoParams *pSetHostDevInfoParams );
/**
 *  This structure is used to Get OS Information
 *
 *  \param  bNameSize[IN/OUT]
 *          - Size of Operating System Name
 *
 *  \param  pNameString[OUT]
 *          - Operating System Name(Optional parameter)
 *          - Null terminated ASCII string
 *
 *  \param  bVersionSize[IN/OUT]
 *          - Operating System Version Size
 *
 *  \param  pVersionString[OUT]
 *          - Operating System Version String(Optional parameter)
 *          - Null terminated ASCII string.
 *
 */
typedef struct _SLQSSwiGetOSInfoParams
{
   BYTE     bNameSize;
   CHAR    *pNameString;
   BYTE     bVersionSize;
   CHAR    *pVersionString;
} SLQSSwiGetOSInfoParams;
/**
 *  This API queries the device operating system info configured on the modem for OMA-DM reporting
 *
 *  \param  pParams
 * *          -  See \ref SLQSSwiGetOSInfoParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 2 Secs
 
 */
ULONG SLQSSwiGetOSInfo( SLQSSwiGetOSInfoParams *pParams );

/**
 *  This structure is used to Set OS Information
 *
 *  \param  bNameSize[IN]
 *          - Size of Operating System Name
 *
 *  \param  pNameString[IN]
 *          - Operating System Name(Optional parameter)
 *          - Null terminated ASCII string
 *
 *  \param  bVersionSize[IN]
 *          - Operating System Version Size
 *
 *  \param  pVersionString[IN]
 *          - Operating System Version String(Optional parameter)
 *          - Null terminated ASCII string.
 *
 */
typedef struct _SLQSSwiSetOSInfoParams
{
   BYTE     bNameSize;
   CHAR    *pNameString;
   BYTE     bVersionSize;
   CHAR    *pVersionString;
} SLQSSwiSetOSInfoParams;

/**
 *  This API Set OS Information to the device.
 *
 *  \param  pParams
 *          -  See \ref SLQSSwiSetOSInfoParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 2 Secs
 *
 */

ULONG SLQSSwiSetOSInfo( SLQSSwiSetOSInfoParams *pParams );

/**
 *  This structure is used to store MEID Information
 *
 *  \param  meidLength[OUT]
 *          - String length of the of MEID received
 *
 *  \param  pMeidString[OUT]
 *          - Optional parameter
 *          - Pointer to receive String containing the  Mobile Equipment Identifier(MEID)
 *            of the device.
 */

typedef struct _SLQSSwiGetSerialNoExtParams
{
   BYTE     meidLength;
   CHAR    *pMeidString;
} SLQSSwiGetSerialNoExtParams;

/**
 *  This API is used to get the MEID of the modem. For CDMA devices
 *  that use a RUIM, the MEID of the modem will always be returned.
 *
 *  \param  SLQSSwiGetSerialNoExtParams
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: NA\n
 *          Timeout: 5 Secs
 */

ULONG SLQSSwiGetSerialNoExt( SLQSSwiGetSerialNoExtParams *pParams );

/**
 *  This structure contains customization settings set to modem
 *
 *  \param  cust_id[IN]
 *          - Customization ID (Maximum 64 bytes)
 *
 *  \param  value_length[IN]
 *          - length of cust_value field
 *
 *  \param  cust_value[IN]
 *          - Customization Setting Value (Maximum 8 bytes)
 */

typedef struct
{
    CHAR cust_id[MAX_CUST_ID_LEN+1];
    WORD value_length;
    BYTE cust_value[MAX_CUST_VALUE_LEN+1];
} setCustomSettingV2;

/**
 *  This function sets the modem for a list of supported features. This function is for
 *  firmware version 2.0 and newer. Currently supported customization features: 
 *     - GPIOSARENABLE
 *     - GPSSEL
 *     - IMSWITCHHIDE
 *     - IPV6ENABLE
 *     - WAKEHOSTEN
 *
 *  \param  pSetCustSetting
 *          - Optional parameter
 *          - See \ref setCustomSettingV2 for more information
 *
 */
ULONG SLQSSetCustFeaturesV2(
    setCustomSettingV2 *pSetCustSetting );

/**
 *  This structure contains which customization id or the list type want to retrieve
 *  from modem. This TLV is only applicable for 9x30 modules so far
 *
 *  \param  cust_id
 *          - Customization ID (Maximum 64 bytes)
 * 
 *  \param  list_type
 *          - list type requested
 *
 */
typedef struct
{
    CHAR cust_id[MAX_CUST_ID_LEN+1];
    BYTE list_type;
} getCustomInput;

 /**
 *  This structure contains information about Customization Setting.
 *  This TLV is only applicable for 9x30 modules so far
 *
 *  \param  id_length
 *          - length of cust_id field
 *
 *  \param  cust_id
 *          - Customization ID (Maximum 64 bytes)
 *
 *  \param  value_length
 *          - length of cust_value field
 *
 *  \param  cust_value
 *          - Customization Setting Value (Maximum 8 bytes)
 *
 *  \param  cust_attr
 *          - Customization Setting attribute through QMI
 *              - bit 0: Values:
 *                  - 0 - read only
 *                  - 1 - read/write 
 *
 */
typedef struct
{
    WORD id_length;
    CHAR cust_id[MAX_CUST_ID_LEN+1];
    WORD value_length;
    BYTE cust_value[MAX_CUST_VALUE_LEN+1];
    WORD cust_attr;
} custSettingInfo;

/**
 *  This structure contains the fields of TLV Customization Setting List. 
 *  This TLV is only applicable for 9x30 modules so far
 *
 *  \param  list_type
 *          - list type requested
 *
 *  \param  num_instances
 *          - number of instances of customization setting
 *
 *  \param  custSetting
 *          -  See \ref custSettingInfo for more information
 *
 */
typedef struct
{
    BYTE list_type;
    WORD num_instances;
    custSettingInfo custSetting[256];
} custSettingList;

/**
 *  This struture contains the TLV required to get the Customization Info and
 *  customization list.
 *
 *  \param pGetCustomInput[IN]
*              - Optional parameter
*              - See \ref getCustomInput for more information
 *
 *  \param pCustSettingInfo[OUT]
 *               - Optional parameter
 *               - See \ref custSettingInfo for more information
 *
 *  \param pCustSettingList[OUT]
 *              - Optional parameter
 *              - See \ref custSettingList for more information
 *
 */
typedef struct
{
    getCustomInput *pGetCustomInput;
    custSettingInfo *pCustSettingInfo;
    custSettingList *pCustSettingList;
} getCustomFeatureV2;

/**
 *  This function queries the modem for a list of supported features. This function is for
 *  firmware version 2.0 and newer. Currently supported Customization features: 
 *     - GPIOSARENABLE
 *     - GPSSEL
 *     - IMSWITCHHIDE
 *     - IPV6ENABLE
 *     - WAKEHOSTEN
 *
 *  \param  pGetCustomFeatureV2
 *          - See \ref getCustomFeatureV2 for more information of the inpurt structure
 *
 */
ULONG SLQSGetCustFeaturesV2(
    getCustomFeatureV2 *pGetCustomFeatureV2 );

#define MAX_DYING_GASP_CFG_SMS_CONTENT_LENGTH 160
#define MAX_DYING_GASP_CFG_SMS_NUMBER_LENGTH 20

/**
 *  This struture contains the TLV required to get the Dysing GASP Config.
 *
 *  \param pDestSMSNum[OUT]
 *               - SMS Destination Number as string of 8 bit ASCII Characters Max 20 chars.
 *               - Optional parameter.
 *
 *  \param pDestSMSContent[OUT]
 *              - SMS COntent as a string of 8 bit ASCII text characters Max 160 chars.
 *              - Optional parameter.
 *
 */
typedef struct
{
    BYTE *pDestSMSNum;
    BYTE *pDestSMSContent;
} getDyingGaspCfg;

/**
 *  This function queries Dying GASP Config.
 *
 *  \param  pGetCustomFeatureV2
 *          - See \ref getDyingGaspCfg for more information of the inpurt structure *
 */

ULONG SLQSSwiGetDyingGaspCfg(
    getDyingGaspCfg *pConfig );

/**
 *  This struture contains the TLV required to get the Dysing GASP Config.
 *
 *  \param pDestSMSNum[OUT]
 *               - SMS Destination Number as string of 8 bit ASCII Characters Max 20 chars.
 *               - Optional parameter.
 *
 *  \param pDestSMSContent[OUT]
 *              - SMS COntent as a string of 8 bit ASCII text characters Max 160 chars.
 *              - Optional parameter.
 *
 */

typedef struct
{
    BYTE *pDestSMSNum;
    BYTE *pDestSMSContent;
} setDyingGaspCfg;

/**
 *  This function set Dying GASP Config.
 *
 *  \param  pConfig
 *          - See \ref setDyingGaspCfg for more information of the inpurt structure
 *
 */

ULONG SLQSSwiSetDyingGaspCfg(
    setDyingGaspCfg *pConfig );

/**
 *  This struture contains the TLV required to get the Dysing GASP Statistics.
 *
 *  \param  pTimeStamp[OUT]
 *               - Timestamp of the last time power loss was detected and 
 *                 Dying Gasp feature was triggered. 
 *               - UTC time in seconds since Jan 06, 1980 (GPS Epoch).
 *
 *  \param  pSMSAttemptedFlag[OUT]
 *              - Indicates whether device attempted to send SMS in the last power loss event.
 *              - 0 - SMS not attempted
 *              - 1 - SMS attempted
 *              - This only indicates device sent the SMS but does not guarantee network delivery.
 *
 */
typedef struct
{
    ULONG *pTimeStamp;
    BYTE  *pSMSAttemptedFlag;
} getDyingGaspStatistics;

/**
 *  This function queries Dying GASP Statistics.
 *
 *  \param  pStatistics
 *          - See \ref getDyingGaspStatistics for more information of the inpurt structure *
 */

ULONG SLQSSwiGetDyingGaspStatistics(
    getDyingGaspStatistics *pStatistics );

/**
 *  This function Clear Dying GASP Statistics.
 */

ULONG SLQSSwiClearDyingGaspStatistics();

/**
 *  This structure contains the SLQSDmsSwiIndicationRegister request
 *  parameters.
 *
 *  \param pGetResetInd [IN,Optional]
 *          - Get Reset Info indication registration. The following
 *            callbacks would not be invoked if the indication is disabled.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \note 'NULL' value confirms that the indication value is not sent.
 */
typedef struct
{
    BYTE *pSwiGetResetInd;
} dmsIndicationRegisterReq;

/**
 *  This function used to set Swi Indication Register
 *
 *  \param  pConfig
 *          - See \ref dmsIndicationRegisterReq for more information of the inpurt structure
 *
 */

ULONG SLQSDmsSwiIndicationRegister(
    dmsIndicationRegisterReq *pIndicationRegisterReq );

/**
 *  This structure contains the TLV required to Get Reset Info.
 *
 *  \param[OUT]  type[OUT]
 *               - type of reset or power down, possible values listed below:
 *                   - 0 - unknown
 *                   - 1 - warm
 *                   - 2 - hard
 *                   - 3 - crash
 *                   - 4 - power down
 *
 *  \param[OUT]  source[OUT]
 *               - entity which initiated the reset or power down, possible values listed below:
 *                   - 0 - unknown    
 *                   - 1 - user requested
 *                   - 2 - hardware switch
 *                   - 3 - temperature critical
 *                   - 4 - voltage critical
 *                   - 5 - configuration update
 *                   - 6 - LWM2M
 *                   - 7 - OMA-DM
 *                   - 8 - FOTA
 */
typedef struct
{
    BYTE type;
    BYTE source;
} dmsSwiGetResetInfo;

/**
 *  This function is used to get reset info
 *
 *  \param  pGetResetInfoResp
 *          - See \ref dmsSwiGetResetInfo for more information of the input structure
 *
 */
ULONG SLQSDmsSwiGetResetInfo(
    dmsSwiGetResetInfo *pGetResetInfoResp );
/**
 *  This structure contains the TLV required to Get Band Capability.
 *
 *  \param  bandCapability[OUT]
 *          Bitmask of bands supported by the device
 *              - Bit 0 - Band class 0, A-system
 *              - Bit 1 - Band class 0, B-system
 *              - Bit 2 - Band class 1, all blocks
 *              - Bit 3 - Band class 2
 *              - Bit 4 - Band class 3, A-system
 *              - Bit 5 - Band class 4, all blocks
 *              - Bit 6 - Band class 5, all blocks
 *              - Bit 7 - GSM DCS band (1800)
 *              - Bit 8 - GSM Extended GSM (E-GSM) band (900)
 *              - Bit 9 - GSM Primary GSM (P-GSM) band (900)
 *              - Bit 10 - Band class 6
 *              - Bit 11 - Band class 7
 *              - Bit 12 - Band class 8
 *              - Bit 13 - Band class 9
 *              - Bit 14 - Band class 10
 *              - Bit 15 - Band class 11
 *              - Bit 16 - GSM 450 band
 *              - Bit 17 - GSM 480 band
 *              - Bit 18 - GSM 750 band
 *              - Bit 19 - GSM 850 band
 *              - Bit 20 - GSM railways GSM band (900)
 *              - Bit 21 - GSM PCS band (1900)
 *              - Bit 22 - WCDMA (Europe, Japan, and China) 2100 band
 *              - Bit 23 - WCDMA US PCS 1900 band
 *              - Bit 24 - WCDMA (Europe and China) DCS 1800 band
 *              - Bit 25 - WCDMA US 1700 band
 *              - Bit 26 - WCDMA US 850 band
 *              - Bit 27 - WCDMA Japan 800 band
 *              - Bit 28 - Band class 12
 *              - Bit 29 - Band class 14
 *              - Bit 30 - Reserved
 *              - Bit 31 - Band class 15
 *              - Bits 32 through 47 - Reserved
 *              - Bit 48 - WCDMA Europe 2600 band
 *              - Bit 49 - WCDMA Europe and Japan 900 band
 *              - Bit 50 - WCDMA Japan 1700 band
 *              - Bits 51 through 55 - Reserved
 *              - Bit 56 - Band class 16
 *              - Bit 57 - Band class 17
 *              - Bit 58 - Band class 18
 *              - Bit 59 - Band class 19
 *
 *  \param[OUT]  pLteBandCapability[OUT]
 *               Bitmask of LTE bands supported by the device
 *                   - Bit 0 - LTE EUTRAN Band 1 UL:1920-1980; DL: 2110-2170
 *                   - Bit 1 - LTE EUTRAN Band 2 UL:1850-1910; DL: 1930-1990
 *                   - Bit 2 - LTE EUTRAN Band 3 UL:1710-1785; DL: 1805-1880
 *                   - Bit 3 - LTE EUTRAN Band 4 UL:1710-1755; DL: 2110-2155
 *                   - Bit 4 - LTE EUTRAN Band 5 UL: 824-849; DL: 869-894
 *                   - Bit 5 - LTE EUTRAN Band 6 UL: 830-840; DL: 875-885
 *                   - Bit 6 - LTE EUTRAN Band 7 UL:2500-2570; DL: 2620-2690
 *                   - Bit 7 - LTE EUTRAN Band 8 UL: 880-915; DL: 925-960
 *                   - Bit 8 - LTE EUTRAN Band 9 UL:1749.9-1784.9; DL: 1844.9-1879.9
 *                   - Bit 9 - LTE EUTRAN Band 10 UL:1710-1770; DL: 2110-2170
 *                   - Bit 10 - LTE EUTRAN Band 11 UL:1427.9-1452.9; DL: 1475.9-1500.9
 *                   - Bit 11 - LTE EUTRAN Band 12 UL:698-716; DL: 728-746
 *                   - Bit 12 - LTE EUTRAN Band 13 UL: 777-787; DL: 746-756
 *                   - Bit 13 - LTE EUTRAN Band 14 UL: 788-798; DL: 758-768
 *                   - Bits 14 and 15 - Reserved
 *                   - Bit 16 - LTE EUTRAN Band 17 UL: 704-716; DL: 734-746
 *                   - Bit 17 - LTE EUTRAN Band 18 UL: 815-830; DL: 860-875
 *                   - Bit 18 - LTE EUTRAN Band 19 UL: 830-845; DL: 875-890
 *                   - Bit 19 - LTE EUTRAN Band 20 UL: 832-862; DL: 791-821
 *                   - Bit 20 - LTE EUTRAN Band 21 UL: 1447.9-1462.9; DL: 1495.9-1510.9
 *                   - Bit 21 - Reserved
 *                   - Bit 22 - LTE EUTRAN Band 23 UL: 2000-2020; DL: 2180-2200
 *                   - Bit 23 - LTE EUTRAN Band 24 UL: 1626.5-1660.5; DL: 1525-1559
 *                   - Bit 24 - LTE EUTRAN Band 25 UL: 1850-1915; DL: 1930-1995
 *                   - Bit 25 - LTE EUTRAN Band 26 UL: 814-849; DL: 859-894
 *                   - Bit 26 - Reserved
 *                   - Bit 27 - LTE EUTRAN Band 28 UL: 703-748; DL: 758-803
 *                   - Bit 28 - LTE EUTRAN Band 29 UL: 1850-1910 or 1710-1755; DL: 716-728
 *                   - Bits 29 through 31 - Reserved
 *                   - Bit 32 - LTE EUTRAN Band 33 UL: 1900-1920; DL: 1900-1920
 *                   - Bit 33 - LTE EUTRAN Band 34 UL: 2010-2025; DL: 2010-2025
 *                   - Bit 34 - LTE EUTRAN Band 35 UL: 1850-1910; DL: 1850-1910
 *                   - Bit 35 - LTE EUTRAN Band 36 UL: 1930-1990; DL: 1930-1990
 *                   - Bit 36 - LTE EUTRAN Band 37 UL: 1910-1930; DL: 1910-1930
 *                   - Bit 37 - LTE EUTRAN Band 38 UL: 2570-2620; DL: 2570-2620
 *                   - Bit 38 - LTE EUTRAN Band 39 UL: 1880-1920; DL: 1880-1920
 *                   - Bit 39 - LTE EUTRAN Band 40 UL: 2300-2400; DL: 2300-2400
 *                   - Bit 40 - LTE EUTRAN Band 41 UL: 2496-2690; DL: 2496-2690
 *                   - Bit 41 - LTE EUTRAN Band 42 UL: 3400-3600; DL: 3400-3600
 *                   - Bit 42 - LTE EUTRAN Band 43 UL: 3600-3800; DL: 3600-3800
 *                   - Bits 43 through 64 - Reserved
 *
 *  \param[OUT]  pTdsBandCapability[OUT]
 *               Bitmask of TDS bands supported by the device.
 *                   - Bit 0 - TDS Band A 1900 to 1920 MHz, 2010 to 2020 MHz
 *                   - Bit 1 - TDS Band B 1850 to 1910 MHz, 1930 to 1990 MHz
 *                   - Bit 2 - TDS Band C 1910 to 1930 MHz
 *                   - Bit 3 - TDS Band D 2570 to 2620 MHz
 *                   - Bit 4 - TDS Band E 2300 to 2400 MHz
 *                   - Bit 5 - TDS Band F 1880 to 1920 MHz
 */
typedef struct
{
    ULONGLONG bandCapability;
    ULONGLONG *pLteBandCapability;
    ULONGLONG *pTdsBandCapability;
} BandCapabilityResp;
/**
 *  Returns the band capability of the device.
 *
 *  \param  pBandCapability[OUT]
 *
 *  See \ref BandCapabilityResp for more information of the inpurt structure
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSGetBandCapabilities( BandCapabilityResp *pBandCapability );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_DMS_H__ */
