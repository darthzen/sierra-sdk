/**
 * \ingroup nas
 *
 * \file    qaGobiApiNas.h
 *
 * \brief   Network Access Service API function prototypes
 */

/*
 * Copyright: © 2011-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_NAS_H__
#define __GOBI_API_NAS_H__

#define SLQS_SS_INFO_LIST_MAX_ELEMENTS   18
#define MAX_DESCRIPTION_LENGTH           255
#define SLQS_SYSTEM_ID_SIZE              16
#define PLMN_LENGTH                      3
#define MAX_SERV_SYSTEM_RADIO_INTERFACES 0x0A
#define MAX_DATA_SRV_CAPABILITIES        0x20
#define NAM_NAME_LENGTH                  12
#define IMSI_M_S1_LENGTH                 7
#define IMSI_M_S2_LENGTH                 3
#define MAX_PILOT_SETS                   0xFF
#define UATISIZE                         16
#define NAS_SIG_INFO_MAX_TDSCDMA_THRESHOLDS_LIST_SIZE 16
#define NAS_SIG_INFO_MIN_dBm_FLOAT_VALUE -125.0 
#define NAS_SIG_INFO_MIN_dB_FLOAT_VALUE  -10.0
#define NAS_MAX_SCC_RX_INFO_INSTANCES     255

#ifdef __cplusplus
extern "C" {
#endif


/**
 *  Returns the available signal strengths ( in dBm ) as measured by the
 *  device in an array. The API also provides the corresponding radio
 *  radio interfaces in an array.
 *
 *  \param  pArraySizes[IN/OUT]
 *          - Upon input maximum number of elements that each array can
 *            contain.
 *          - Upon successful output actual number of elements
 *            in the array.
 *
 *  \param  pSignalStrength[OUT]
 *          - Received signal strength array (in dBm)
 *
 *  \param  pRadioInterface[OUT]
 *          - Radio interface technology array of the signal being measured
 *              - See qaGobiApiTableRadioInterfaces.h for Radio Interface info
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetSignalStrengths(
    ULONG *pArraySizes,
    INT8  *pSignalStrength,
    ULONG *pRadioInterface );

/**
 *  Performs scan for available networks.
 *
 *  \param  pInstanceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the network info
 *            instance array can contain.
 *          - Upon successful output, the actual number of elements in the
 *            network info instance array.
 *
 *  \param  pInstances[OUT]
 *          - Network info instance array
 *              - See \ref QmiNas3GppNetworkInfo
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: 5 minutes\n
 *
 */
ULONG PerformNetworkScan(
    BYTE *pInstanceSize,
    BYTE *pInstances );

/**
 *  Initiates the network registration process. This API is deprecated on
 *  MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx and
 *  all EM74xx firmware versions. Please use API SLQSSetSysSelectionPref()
 *  and SLQSSetBandPreference() for new firmware versions and new modules
 *
 *  \param  regType
 *          - Registration type\n
 *              - 1 - Automatic
 *              - 2 - Manual
 *
 *  \param  mcc
 *          - Mobile country code
 *
 *  \param  mnc
 *          - Mobile network code
 *
 *  \param  rat
 *          - Radio access technology\n
 *              - 4 - GSM
 *              - 5 - UMTS
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: 30 seconds\n
 *
 */
ULONG InitiateNetworkRegistration(
    ULONG regType,
    WORD  mcc,
    WORD  mnc,
    ULONG rat );

/**
 *  Provides information about the system that provides service to the device.
 *  This API is deprecated on MC73xx/EM73xx modules since firmware version
 *  SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions. Please use API
 *  SLQSNasGetSysInfo() for new firmware versions and new modules
 *
 *  \param  pRegistrationState[OUT]
 *          - Registration state:
 *              - 0 - Not registered
 *              - 1 - Registered
 *              - 2 - Searching/Not Registered
 *              - 3 - Registration Denied
 *              - 4 - Unknown
 *
 *  \param  pCSDomain[OUT]
 *          - Circuit switch domain status:
 *              - 0 - Unknown/Not Applicable
 *              - 1 - Attached
 *              - 2 - Detached
 *
 *  \param  pPSDomain[OUT]
 *          - Packet switch domain status
 *              - 0 - Unknown/Not Applicable
 *              - 1 - Attached
 *              - 2 - Detached
 *
 *  \param  pRAN[OUT]
 *          - Type of radio access network on which mobile is registered:\n
 *              - 0 - Unknown
 *              - 1 - cdma2000 network
 *              - 2 - UMTS network
 *
 *  \param  pRadioIfacesSize[IN/OUT]
 *          - Upon input, maximum number of elements that the radio interface
 *            array contain.
 *          - Upon successful output, actual number of elements in the radio
 *            interface array.
 *
 *  \param  pRadioIfaces[OUT]
 *          - An array of Radio Interface Technology
 *              - See qaGobiApiTableRadioInterfaces.h for the Radio Interface Technologies
 *
 *  \param  pRoaming[OUT]
 *          - Roaming indicator
 *
 *  \param  pMCC[OUT]
 *          - Mobile country code
 *
 *  \param  pMNC[OUT]
 *          - Mobile network code
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator)
 *          that network name array can contain; applicable only for
 *          UMTS networks
 *
 *  \param  pName[OUT]
 *          - Network name or description represented as a NULL terminated
 *          string; empty string is returned when unknown; applicable only
 *          for UMTS networks
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetServingNetwork(
    ULONG *pRegistrationState,
    ULONG *pCSDomain,
    ULONG *pPSDomain,
    ULONG *pRAN,
    BYTE  *pRadioIfacesSize,
    BYTE  *pRadioIfaces,
    ULONG *pRoaming,
    WORD  *pMCC,
    WORD  *pMNC,
    BYTE  nameSize,
    CHAR  *pName );

/**
 *  Retrieves information about the home network of the device.
 *  For 3GPP2 home network information use GetHomeNetwork3GPP2.
 *
 *  \param  pMCC[OUT]
 *          - Mobile country code (UMTS only).
 *
 *  \param  pMNC[OUT]
 *          - Mobile network code (UMTS only).
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator) that
 8            network name array can contain (UMTS only).
 *
 *  \param  pName[OUT]
 *          - Network name or description represented as a NULL terminated
 *            string (empty string returned when unknown) (UMTS only).
 *
 *  \param  pSID[OUT]
 *          - Home network system ID
 *              - 0xFFFF - Unknown.
 *              - Only applies to cdma2000
 *
 *  \param  pNID[OUT]
 *          - Home network ID\n
 *              - 0xFFFF - Unknown.
 *              - Only applies to cdma2000
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetHomeNetwork(
    WORD *pMCC,
    WORD *pMNC,
    BYTE nameSize,
    CHAR *pName,
    WORD *pSID,
    WORD *pNID);

/**
 *  Returns information regarding the data capabilities of the system
 *  that currently provides service to the device.
 *
 *  \param  pDataCapsSize[IN/OUT]
 *          - Upon input, the maximum number of elements the data capabilities
 *            array can contain.
 *          - Upon output, the actual number of elements in the data
 *            capabilities array.
 *
 *  \param  pDataCaps[OUT]
 *          - Data capabilities array of unsigned long type
 *              - 1  - GPRS\n
 *              - 2  - EDGE\n
 *              - 3  - HSDPA\n
 *              - 4  - HSUPA\n
 *              - 5  - WCDMA\n
 *              - 6  - CDMA 1xRTT\n
 *              - 7  - CDMA 1xEV-DO Rev 0\n
 *              - 8  - CDMA 1xEV-DO Rev. A\n
 *              - 9  - GSM\n
 *              - 10 - EVDO Rev. B\n
 *              - 11 - LTE\n
 *              - 12 - HSDPA Plus\n
 *              - 13 - Dual Carrier HSDPA Plus\n
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetServingNetworkCapabilities(
    BYTE *pDataCapsSize,
    BYTE *pDataCaps );

/**
 *  Sets the network registration preference. This API is deprecated
 *  on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSSetSysSelectionPref()
 *  for new firmware versions and new modules
 *
 *  \param  technologyPref[IN]
 *          - 2 Byte Bitmask representing radio technology preference
 *              - No bits set indicates device to automatically determine the
 *                technology to use.
 *              - Type of technology
 *                  - Bit 0 - Technology is 3GPP2
 *                  - Bit 1 - Technology is 3GPP
 *              - Technology-specific protocol bitmask
 *                  - Bit 2 - Analog
 *                      - AMPS if 3GPP2, GSM if 3GPP
 *                  - Bit 3 - Digital
 *                      - CDMA if 3GPP2, WCDMA if 3GPP
 *                  - Bit 4 - HDR
 *                  - Bit 5 - LTE
 *                  - All other bits are reserved.
 *
 *  \param  duration[IN]
 *          - Duration of active preference
 *              - 0 - Persistent
 *              - 1 - Power cycle
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SetNetworkPreference(
    ULONG technologyPref,
    ULONG duration );

/**
 *  Returns the network registration preference. This API is deprecated
 *  on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSGetSysSelectionPref()
 *  for new firmware versions and new modules
 *
 *  \param  pTechnologyPref[OUT]
 *          - Bitmask representing the radio technology preference set.
 *          - No bits set indicates to the device to automatically
 *            determine the technology to use
 *          - Values:
 *              - Bit 0 - Technology is 3GPP2
 *              - Bit 1 - Technology is 3GPP
 *          - Any combination of the following may be returned:
 *              - Bit 2 - Analog - AMPS if 3GPP2, GSM if 3GPP
 *              - Bit 3 - Digital - CDMA if 3GPP2, WCDMA if 3GPP
 *              - Bit 4 - HDR
 *              - Bit 5 - LTE
 *              - Bits 6 to 15 - Reserved
 *
 *  \param  pDuration[OUT]
 *          - Duration of active preference
 *              - 0 - Permanent
 *              - 1 - Power cycle
 *              - 2 - Until the end of the next call or a power cycle
 *              - 3 - Until the end of the next call, a specified time, or a power cycle
 *              - 4 to 6 - Until the end of the next call
 *
 *  \param  pPersistentTechnologyPref[OUT]
 *          - Bit field representing persistent radio technology preference
 *              - Same representation as the pTechnologyPref parameter
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetNetworkPreference(
    ULONG *pTechnologyPref,
    ULONG *pDuration,
    ULONG *pPersistentTechnologyPref );

/**
 * This structure contains the RFBandInfo response parameters.
 *
 *  \param  radioInterface
 *          - Radio interface technology
 *              - See \ref Tables for Radio Interface
 *
 *  \param  activeBandClass
 *          - Active Band Class
 *              - See \ref Tables for Band Classes
 *
 *  \param  activeChannel
 *          - Active channel (0 if channel is not relevant to the
 *            reported technology)
 */
struct RFBandInfoElements
{
    BYTE radioInterface;
    WORD activeBandClass;
    WORD activeChannel;
};

/**
 *  Sets the RFInfoList
 *
 *  \param  pInstanceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the RF info instances
 *            array can contain.
 *          - Upon successful output, actual number of elements in RF info
 *            instances array.
 *
 *  \param  pInstances[OUT]
 *          - RF info instances array
 *              - See \ref RFBandInfoElements for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetRFInfo(
    BYTE *pInstanceSize,
    struct RFBandInfoElements *pRFBandInfo );

/**
 *  Initiates a domain attach/detach of the device. This API is deprecated on
 *  MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx and all
 *  EM74xx firmware versions. Please use API SLQSSetSysSelectionPref() for new
 *  firmware versions and new modules
 *
 *  \param  action[IN]
 *          - Domain action to attempt\n
 *          1 - Attach\n
 *          2 - Detach
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 30 seconds\n
 *
 */
ULONG InitiateDomainAttach(
    ULONG action );

/**
 *  Retrieves information about the access overload class (ACCOLC)
 *
 *  \param  pACCOLC[OUT]
 *          - ACCOLC : Valid range is 0 to 15
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG GetACCOLC(
    BYTE *pACCOLC );

/**
 *  Sets the access overload class (ACCOLC)
 *
 *  \param  spc[IN]
 *          - service programming code NULL-terminated string of six digit
 *
 *  \param  ACCOLC[IN]
 *          - ACCOLC : Valid range is 0 to 15
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 *
 */
ULONG SetACCOLC(
    CHAR *spc,
    BYTE accolc );

/**
 *  Sets the CDMA network parameters. Currently the modified settings
 *  will not be utilized until the device has been reset. For this reason,
 *  the recommended approach when using SetCDMANetworkParameters is for
 *  the application to perform the following steps:\n
 *   1 - Call SetCDMANetworkParameters()
 *   2 - Call SetPower( 5 )
 *   3 - Call QCWWANDisconnect()
 *   4 - Reconnect after the device power cycles
 *
 *  \param  pSPC[IN]
 *          - Six digit service programming code (not necessary
 *            when only the roaming field is being set)
 *
 *  \param  pForceRev0[IN]
 *          - (Optional)Force CDMA 1x-EV-DO Rev. 0 mode\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *              Note: Enabled can only be specified if pCustomSCP
 *                    state is set to Disabled
 *
 *  \param  pCustomSCP[IN]
 *          - (Optional)Use a custom config for CDMA 1x-EV-DO SCP\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *              Note: Enabled can only be specified if pForceRev0
 *                    is set to Disabled
 *
 *  \param  pProtocol[IN]
 *          - Protocol mask for custom SCP config\n
 *            - 0x00000001 - Subtype 2 Physical Layer
 *            - 0x00000002 - Enhanced CCMAC
 *            - 0x00000004 - Enhanced ACMAC
 *            - 0x00000008 - Enhanced FTCMAC
 *            - 0x00000010 - Subtype 3 RTCMAC
 *            - 0x00000020 - Subsystem 1 RTCMAC
 *            - 0x00000040 - Enhanced Idle
 *            - 0x00000080 - Generic Multimode Capable Disc Port
 *            - 0xFFFFFFFF - Unknown\n
 *
 *  \param  pBroadcast[IN]
 *          - Broadcast mask for custom SCP config
 *              - 0x00000001 - Generic broadcast enabled
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pApplication[IN]
 *          - Application mask for custom SCP config\n
 *            - 0x00000001 - SN Multiflow Packet Application
 *            - 0x00000002 - SN Enhanced Multiflow Packet Application
 *            - 0xFFFFFFFF - Unknown
 *
 *  \param  pRoaming[IN]
 *          - Roaming preference\n
 *            - 0 - Automatic
 *            - 1 - Home Only
 *            - 2 - Affiliated Roaming Only
 *            - 3 - Home and Affiliated Roaming
 *            - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 *
 */
ULONG SetCDMANetworkParameters(
   CHAR  *pSPC,
   BYTE  *pForceRev0,
   BYTE  *pCustomSCP,
   ULONG *pProtocol,
   ULONG *pBroadcast,
   ULONG *pApplication,
   ULONG *pRoaming );

/**
 *  Gets the current CDMA network parameters
 *
 *  \param  pSCI[OUT]
 *          - Slot cycle index\n
 *             - 0xFF-Unknown
 *
 *  \param  pSCM[OUT]
 *          - Station class mark\n
 *             - 0xFF-Unknown
 *
 *  \param  pRegHomeSID[OUT]
 *          - Register on home SID\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *            - 0xFF - Unknown
 *
 *  \param  pRegForeignSID[OUT]
 *          - Register on foreign SID\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *            - 0xFF - Unknown
 *
 *  \param  pRegForeignNID[OUT]
 *          - Register on foreign NID\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *            - 0xFF - Unknown
 *
 *  \param  pForceRev0[OUT]
 *          - Force CDMA 1x-EV-DO Rev. 0 mode\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *            - 0xFF - Unknown
 *
 *  \param  pCustomSCP[OUT]
 *          - Use a custom config for CDMA 1x-EV-DO SCP\n
 *            - 0 - Disabled
 *            - 1 - Enabled
 *            - 0xFF - Unknown
 *
 *  \param  pProtocol[OUT]
 *          - Protocol mask for custom SCP config\n
 *            - 0x00000001 - Subtype 2 Physical Layer
 *            - 0x00000002 - Enhanced CCMAC
 *            - 0x00000004 - Enhanced ACMAC
 *            - 0x00000008 - Enhanced FTCMAC
 *            - 0x00000010 - Subtype 3 RTCMAC
 *            - 0x00000020 - Subsystem 1 RTCMAC
 *            - 0x00000040 - Enhanced Idle
 *            - 0x00000080 - Generic Multimode Capable Disc Port
 *            - 0xFFFFFFFF - Unknown\n
 *
 *  \param  pBroadcast[OUT]
 *          - Broadcast mask for custom SCP config
 *              - 0x00000001 - Generic broadcast enabled
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pApplication[OUT]
 *          - Application mask for custom SCP config\n
 *            - 0x00000001 - SN Multiflow Packet Application
 *            - 0x00000002 - SN Enhanced Multiflow Packet Application
 *            - 0xFFFFFFFF - Unknown
 *
 *  \param  pRoaming[OUT]
 *          - Roaming preference\n
 *            - 0 - Automatic
 *            - 1 - Home Only
 *            - 2 - Affiliated Roaming Only
 *            - 3 - Home and Affiliated Roaming
 *            - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 *
 */
ULONG GetCDMANetworkParameters(
   BYTE  *pSCI,
   BYTE  *pSCM,
   BYTE  *pRegHomeSID,
   BYTE  *pRegForeignSID,
   BYTE  *pRegForeignNID,
   BYTE  *pForceRev0,
   BYTE  *pCustomSCP,
   ULONG *pProtocol,
   ULONG *pBroadcast,
   ULONG *pApplication,
   ULONG *pRoaming );

/**
 *  AN-AAA  authentication status of the device.
 *
 *  \param  pStatus[OUT]
 *          - Status of last AN-AAA authentication attempt\n
 *            - 0 - Failure
 *            - 1 - Success
 *            - 2 - Not Requested
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetANAAAAuthenticationStatus(
    ULONG *pStatus );

/**
 * This structure contains the Serving System parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param regState
 *        - Registration state - Registration state of the mobile
 *        - Values:
 *            - 0 - Not Registered; mobile is not currently searching
 *                  for a new network to provide service
 *            - 1 - Registered with a network
 *            - 2 - Not registered, but mobile is currently searching
 *                  for a new network to provide service
 *            - 3 - Registration denied by visible network
 *            - 4 - Registration state is unknown
 *
 * \param csAttachState
 *        - CS Attach State - Circuit-switched domain attach state of the mobile
 *        - Values:
 *            - 0 - Unknown or not applicable
 *            - 1 - Attached
 *            - 2 - Detached
 *
 * \param psAttachState
 *        - PS Attach State - Packet-switched domain attach state of the mobile
 *        - Values:
 *            - 0 - Unknown or not applicable
 *            - 1 - Attached
 *            - 2 - Detached
 *
 * \param selNetwork
 *        - Selected Network - Type of selected radio access network
 *        - Values:
 *            - 0 - Unknown
 *            - 1 - 3GPP2 network
 *            - 2 - 3GPP network
 *
 * \param numRadioInterfaces
 *        - In Use Radio Interfaces Number
 *            - Number of radio interfaces currently in use
 *            - defaults to zero
 *
 * \param radioInterface
 *        - Radio Interface(s) modem discovered 
 *        - Values:
 *            - 0x00 - RADIO_IF_NO_SVC - None(no service)
 *            - 0x01 - RADIO_IF_CDMA_1X - cdma2000 1X
 *            - 0x02 - RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *            - 0x03 - RADIO_IF_AMPS - AMPS
 *            - 0x04 - RADIO_IF_GSM - GSM
 *            - 0x05 - RADIO_IF_UMTS - UMTS
 *            - 0x08 - RADIO_IF_LTE - LTE
 *
 */
typedef struct {
    BYTE regState;
    BYTE csAttachState;
    BYTE psAttachState;
    BYTE selNetwork;
    BYTE numRadioInterfaces;
    BYTE radioInterface[MAX_SERV_SYSTEM_RADIO_INTERFACES];
}servSystem;

/**
 * This structure contains the data services capability
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param dataCapabilitiesLen
 *        - Length of data capabilities list
 *        - Defaults to zero
 *
 * \param dataCapabilities
 *        - List of data capabilities
 *        - Values:
 *            - 0x01 - GPRS
 *            - 0x02 - EDGE
 *            - 0x03 - HSDPA
 *            - 0x04 - HSUPA
 *            - 0x05 - WCDMA
 *            - 0x06 - CDMA
 *            - 0x07 - EV-DO Rev0
 *            - 0x08 - EV-DO RevA
 *            - 0x09 - GSM
 *            - 0x0A - EV-DO Rev B
 *            - 0x0B - LTE
 *            - 0x0C - HSDPA+
 *            - 0x0D - DC-HSDPA+
 *
 */
typedef struct {
    BYTE dataCapabilitiesLen;
    BYTE dataCapabilities[MAX_DATA_SRV_CAPABILITIES];
}dataSrvCapabilities;

/**
 * This structure contains the current PLMN parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param MCC
 *        - mobile country code
 *            - A 16 bit representation of MCC
 *            - Range 0 to 999
 *
 * \param MNC
 *        - mobile network code
 *            - A 16 bit representation of MNC
 *            - Range 0 to 999
 *
 * \param netDescrLength
 *        - Length of Network description field
 *        - Defaults to zero
 *
 * \param netDescr
 *        - Network Description
 *            - optional string containing network name or description
 *
 */
typedef struct {
    WORD MCC;
    WORD MNC;
    BYTE netDescrLength;
    BYTE netDescr[255];
}currentPLMN;

/**
 * This structure contains the Roaming Indicator List
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param numInstances
 *        - number of sets of radio interface currently in use and roaming
 *          indicator
 *            - defaults to zero
 *
 * \param radioInterface
 *        - Radio Interface currently in use
 *        - Values:
 *            - 0x01 - RADIO_IF_CDMA_1X - cdma2000 1X
 *            - 0x02 - RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *            - 0x03 - RADIO_IF_AMPS - AMPS
 *            - 0x04 - RADIO_IF_GSM - GSM
 *            - 0x05 - RADIO_IF_UMTS - UMTS
 *            - 0x08 - RADIO_IF_LTE - LTE
 *
 * \param roamIndicator
 *        - Roaming Indicator
 *        - Values:
 *            - 0x00 - Roaming
 *            - 0x01 - Home
 *
 */
typedef struct {
    BYTE numInstances;
    BYTE radioInterface[MAX_SERV_SYSTEM_RADIO_INTERFACES];
    BYTE roamIndicator[MAX_SERV_SYSTEM_RADIO_INTERFACES];
}roamIndList;

/**
 * This structure contains the 3GPP2TimeZone parameters
 *
 * \param leapSeconds
 *        - leap seconds - Number of leap seconds since the start of
 *                          CDMA system time.
 *
 * \param localTimeOffset
 *        - Local Time Offset - Offset of system time in units of 30 minutes;
 *                             the value in this field conveys as 8 bit 2's
 *                             compliment number.
 *
 * \param daylightSavings
 *        - Day Light Savings Indicator
 *            - 0x00 - OFF (daylight savings not in effect)
 *            - 0x01 - ON (daylight savings in effect)
 *
 */
typedef struct {
    BYTE leapSeconds;
    BYTE localTimeOffset;
    BYTE daylightSavings;
}qaQmi3Gpp2TimeZone;

/**
 * This structure contains Detailed Service information
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param srvStatus
 *        - Service status
 *        - Values:
 *            - 0x00 - No service
 *            - 0x01 - Limited service
 *            - 0x02 - Service available
 *            - 0x03 - Limited regional service
 *            - 0x04 - MS in power save or deep sleep
 *
 * \param srvCapability
 *        - System's service capability
 *        - Values:
 *            - 0x00 - No Service
 *            - 0x01 - Circuit-switched only
 *            - 0x02 - Packet-switched only
 *            - 0x03 - Circuit-switched and packet-switched
 *            - 0x04 - MS found the right system but not yet
 *                     registered/attached
 *
 * \param hdrSrvStatus
 *        - HDR service status
 *        - Values:
 *            - 0x00 - No service
 *            - 0x01 - Limited service
 *            - 0x02 - Service available
 *            - 0x03 - Limited regional service
 *            - 0x04 - MS in power save or deep sleep
 *
 * \param hdrHybrid
 *        - HDR hybrid information
 *        - Values:
 *            - 0x00 - System is not hybrid
 *            - 0x01 - System is hybrid
 *
 * \param isSysForbidden
 *        - Forbidden system information
 *        - Values:
 *            - 0x00 - System is not a forbidden system
 *            - 0x01 - System is a forbidden system
 *
 */
typedef struct {
    BYTE srvStatus;
    BYTE srvCapability;
    BYTE hdrSrvStatus;
    BYTE hdrHybrid;
    BYTE isSysForbidden;
}detailSvcInfo;

/**
 * This structure contains CDMA system information extension
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param MCC
 *        - Mobile Country Code
 *
 * \param imsi_11_12
 *        - IMSI_11_12
 */
typedef struct {
    WORD MCC;
    BYTE imsi_11_12;
}CDMASysInfoExt;

/**
 * This structure contains Call Barring Status.
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 * \param csBarStatus
 *        - Call Barring Status for circuit-switched calls.
 *        - Values:
 *        - NAS_CELL_ACCESS_NORMAL_ONLY - Cell access is allowed for normal
 *                                        calls only
 *        - NAS_CELL_ACCESS_EMERGENCY_ONLY - Cell access is allowed for
 *                                           emergency calls only
 *        - NAS_CELL_ACCESS_NO_CALLS - Cell access is not allowed for any
 *                                     call type
 *        - NAS_CELL_ACCESS_ALL_CALLS - Cell access is allowed for all call
 *                                      types
 *        - NAS_CELL_ACCESS_UNKNOWN - Cell access type is unknown
 *
 * \param psBarStatus
 *        - Call Barring Status for packet-switched calls.
 *        - Values:
 *            - NAS_CELL_ACCESS_NORMAL_ONLY - Cell access is allowed for normal
 *                                            calls only
 *            - NAS_CELL_ACCESS_EMERGENCY_ONLY - Cell access is allowed for
 *                                               emergency calls only
 *            - NAS_CELL_ACCESS_NO_CALLS - Cell access is not allowed for any
 *                                         call type
 *            - NAS_CELL_ACCESS_ALL_CALLS - Cell access is allowed for all call
 *                                          types
 *            - NAS_CELL_ACCESS_UNKNOWN - Cell access type is unknown
 */
typedef struct {
    ULONG csBarStatus;
    ULONG psBarStatus;
}callBarStatus;

/**
 * This structure contains the Serving System parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  ServingSystem
 *          - Serving System
 *          - See \ref servSystem for more information
 *
 *  \param  roamIndicatorVal
 *          - Optional parameter indicating Roaming Indicator value
 *          - Values:
 *              - 0x00 - Roaming
 *              - 0x01 - Home
 *              - 0x02 - Flashing
 *              - 0x03 and above - Operator defined values
 *
 *  \param  DataSrvCapabilities
 *          - Optional parameter indicating Data services capability
 *          - See \ref dataSrvCapabilities for more information
 *
 *  \param  CurrentPLMN
 *          - Optional parameter indicating Current PLMN
 *          -  See \ref currentPLMN for more information
 *
 *  \param  SystemID
 *          - Optional parameter indicating System ID
 *
 *  \param  NetworkID
 *          - Optional parameter indicating Network ID
 *
 *  \param  BaseStationID
 *          - Optional parameter indicating Base Station Identification Number
 *
 *  \param  BaseStationLatitude
 *          - Optional parameter indicating Base station latitude in units of
 *            0.25 sec,expressed as a two's complement signed number with
 *            positive numbers signifying North latitude
 *
 *  \param  BasestationLongitude
 *          - Optional parameter indicating Base station longitude in units of
 *            0.25 sec, expressed as a Two's complement signed number with
 *            positive numbers signifying East longitude
 *
 *  \param  RoamingIndicatorList
 *          - Optional parameter indicating Roaming Indicator List
 *          - See \ref roamIndList for more information
 *
 *  \param  defaultRoamInd
 *          - Optional parameter indicating Default Roaming Indicator
 *          - Values:
 *              - 0x00 - Roaming
 *              - 0x01 - Home
 *
 *  \param  Gpp2TimeZone
 *          - Optional parameter indicating 3GPP2 Time Zone
 *          - See \ref qaQmi3Gpp2TimeZone for more information
 *
 *  \param  CDMA_P_Rev
 *          - Optional parameter indicating CDMA P_Rev in use
 *
 *  \param  GppTimeZone
 *          - Optional parameter indicating Offset from Universal time, i.e.,
 *            difference between local time and Universal time, in increments
 *            of 15 min. (signed value).
 *
 *  \param  GppNetworkDSTAdjustment
 *          - Optional parameter indicating 3GPP network daylight
 *            saving adjustment
 *          - Values:
 *              - 0x00 - No adjustment for Daylight Saving Time
 *              - 0x01 - 1 hr adjustment for Daylight Saving Time
 *              - 0x02 - 2 hr adjustment for Daylight Saving Time
 *
 *  \param  Lac
 *          - Optional parameter indicating 3GPP Location Area Code
 *
 *  \param  CellID
 *          - Optional parameter indicating 3GPP Cell ID
 *
 *  \param  concSvcInfo
 *          - Optional parameter indicating 3GPP2 concurrent service Info
 *          - Values:
 *              - 0x00 - Concurrent service not available
 *              - 0x01 - Concurrent service available
 *
 *  \param  PRLInd
 *          - Optional parameter indicating 3GPP2 PRL Indicator
 *          - Values:
 *              - 0x00 - System not in PRL
 *              - 0x01 - System is in PRL
 *
 *  \param  DTMInd
 *          - Optional parameter indicating Dual Transfer Mode
 *            Indication(GSM Only)
 *          - Values:
 *              - 0x00 - DTM not supported
 *              - 0x01 - DTM supported
 *
 *  \param  DetailedSvcInfo
 *          - Optional parameter indicating Detailed service information
 *          - See \ref detailSvcInfo for more information
 *
 *  \param  CDMASystemInfoExt
 *          - Optional parameter indicating CDMA System Info Ext
 *          - See \ref CDMASysInfoExt for more information
 *
 *  \param  hdrPersonality
 *          - Optional parameter indicating HDR Personality Information
 *          - Values:
 *              - 0x00 - Unknown
 *              - 0x01 - HRPD
 *              - 0x02 - eHRPD
 *
 *  \param  trackAreaCode
 *          - Optional parameter indicating Tracking area code information
 *            for LTE
 *
 *  \param  CallBarStatus
 *          - Optional parameter indicating Call Barring Status
 *          - See \ref callBarStatus for more information
 */
typedef struct {
    servSystem              ServingSystem;
    BYTE                    roamIndicatorVal;
    dataSrvCapabilities     DataSrvCapabilities;
    currentPLMN             CurrentPLMN;
    WORD                    SystemID;
    WORD                    NetworkID;
    WORD                    BasestationID;
    ULONG                   BasestationLatitude;
    ULONG                   BasestationLongitude;
    roamIndList             RoamingIndicatorList;
    BYTE                    defaultRoamInd;
    qaQmi3Gpp2TimeZone      Gpp2TimeZone;
    BYTE                    CDMA_P_Rev;
    BYTE                    GppTimeZone;
    BYTE                    GppNetworkDSTAdjustment;
    WORD                    Lac;
    ULONG                   CellID;
    BYTE                    concSvcInfo;
    BYTE                    PRLInd;
    BYTE                    DTMInd;
    detailSvcInfo           DetailedSvcInfo;
    CDMASysInfoExt          CDMASystemInfoExt;
    BYTE                    hdrPersonality;
    WORD                    trackAreaCode;
    callBarStatus           CallBarStatus;
}qaQmiServingSystemParam;

/**
 *  Provides information about the system that provides service to the device.
 *  This API is deprecated on MC73xx/EM73xx modules since firmware version
 *  SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions. Please use API
 *  SLQSNasGetSysInfo() for new firmware versions and new modules. Also report
 *  available radio interface technology. If there are more than one radio interface, 
 *  please choose the right interface(usually the first pair).
 *  - See  \ref SLQSSetSysSelectionPref 
 *
 *  \param  pServingSystem[OUT]
 *          - serving system parameters obtained from the system
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   Timeout: 2 seconds\n
 */
ULONG SLQSGetServingSystem( qaQmiServingSystemParam *pServingSystem );

/**
 *  Provides information about the band preference.
 *
 *  \param  bandpreference[IN]
 *          - Bit mask representing the band preference to be set.
 *          - Bit position meanings:
 *              - 0 - BC0_A - Band Class 0, A-System
 *              - 1 - BC0_B - Band Class 0, B-System, Band Class 0
 *                    AB , GSM 850 Band
 *              - 2 - BC1 - Band Class 1, all blocks
 *              - 3 - BC2 - Band Class 2 place holder
 *              - 4 - BC3 - Band Class 3, A-System
 *              - 5 - BC4 - Band Class 4, all blocks
 *              - 6 - BC5 - Band Class 5, all blocks
 *              - 7 - GSM_DCS_1800 - GSM DCS band
 *              - 8 - GSM_EGSM_900 - GSM Extended GSM (E-GSM) band
 *              - 9 - GSM_PGSM_900 - GSM Primary GSM (P-GSM) band
 *              - 10 - BC6 - Band Class 6
 *              - 11 - BC7 - Band Class 7
 *              - 12 - BC8 - Band Class 8
 *              - 13 - BC9 - Band Class 9
 *              - 14 - BC10 - Band Class 10
 *              - 15 - BC11 - Band Class 11
 *              - 16 - GSM_450 - GSM 450 band
 *              - 17 - GSM_480 - GSM 480 band
 *              - 18 - GSM_750 - GSM 750 band
 *              - 19 - GSM_850 - GSM 850 band
 *              - 20 - GSM_RGSM_900 - GSM Railways GSM Band
 *              - 21 - GSM_PCS_1900 - GSM PCS band
 *              - 22 - WCDMA_I_IMT_2000 - WCDMA EUROPE JAPAN
 *                     and CHINA IMT 2100 band
 *              - 23 - WCDMA_II_PCS_1900 - WCDMA US PCS 1900 band
 *              - 24 - WCDMA_III_1700 - WCDMA EUROPE and CHINA DCS 1800 band
 *              - 25 - WCDMA_IV_1700 - WCDMA US 1700 band
 *              - 26 - WCDMA_V_850 - WCDMA US 850 band
 *              - 27 - WCDMA_VI_800 - WCDMA JAPAN 800 band
 *              - 28 - BC12 - Band Class 12
 *              - 29 - BC14 - Band Class 14
 *              - 30 - RESERVED_2 - Reserved 2
 *              - 31 - BC15 - Band Class 15
 *              - 32 - 47 - Reserved
 *              - 48 - WCDMA_VII_2600 - WCDMA EUROPE 2600 band
 *              - 49 - WCDMA_VIII_900 - WCDMA EUROPE and JAPAN 900 band
 *              - 50 - WCDMA_IX_1700 - WCDMA JAPAN 1700 band
 *              - 51 to 55 - Reserved
 *              - 56 - BBC16 - Band Class 16
 *              - 57 - BC17 - Band Class 17
 *              - 58 - BC18 - Band Class 18
 *              - 59 - BC19 - Band Class 19
 *              - 60 to 64 - Reserved
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   Timeout: 2 seconds\n
 */
ULONG SLQSSetBandPreference( ULONGLONG bandpreference );

/**
 *  Register/De-register from NAS (Network access service) broadcast
 *  indications. Some callbacks would not be invoked if the indications
 *  are not registered. The details are provided in the parameter description.
 *
 *
 *  \param  systemSelectionInd[IN]
 *          - system selection preference indication registration. The following
 *            callbacks would not be invoked if the indication is disabled.\n
 *            \ref tFNRoamingIndicator \ref tFNDataCapabilities and
 *            \ref tFNServingSystem
 *              - 0x00 - for disable
 *              - 0x01 - for enable
 *              - 0xFF - No change - Specifying this parameter indicates that
 *                the device will continue to use the existing setting
 *                (disable/enable) which has been previously set for the device
 *
 *  \param  DDTMInd[IN]
 *          - DDTM (Data Dedicated Transmission Mode) indication registration.
 *              - 0x00 - for disable
 *              - 0x01 - for enable
 *              - 0xFF - No change - Specifying this parameter indicates that
 *                the device will continue to use the existing setting
 *                (disable/enable) which has been previously set for the device
 *
 *  \param  servingSystemInd[IN]
 *          - Serving system indication registration. The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNBandPreference
 *              - 0x00 - for disable
 *              - 0x01 - for enable
 *              - 0xFF - No change - Specifying this parameter indicates that
 *                the device will continue to use the existing setting
 *                (disable/enable) which has been previously set for the device
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   Timeout: 2 seconds\n
 */
ULONG SLQSNasIndicationRegister(
    BYTE systemSelectionInd,
    BYTE DDTMInd,
    BYTE servingSystemInd);

/**
 * This structure contains the Received Signal Strength Information
 *
 *  \param  rxSignalStrength
 *          - Received signal strength in dBm
 *              - For CDMA and UMTS, this indicates forward link pilotEc.
 *              - For GSM, the received signal strength.
 *              - For LTE, this indicates the total received wideband power
 *                observed by UE.
 *
 *  \param  radioIf
 *          - Radio interface technology of the signal being radio_if measured
 *              - 0x00 - RADIO_IF_NO_SVC - None (no service)
 *              - 0x01 - RADIO_IF_CDMA_1X - cdma2000Â® 1X
 *              - 0x02 - RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *              - 0x03 - RADIO_IF_AMPS - AMPS
 *              - 0x04 - RADIO_IF_GSM - GSM
 *              - 0x05 - RADIO_IF_UMTS - UMTS
 *              - 0x08 - RADIO_IF_LTE - LTE
 *
 *  \note   First element of the RSSI list always contains the current Signal
 *          strength and Radio Interface.
 */
struct rxSignalStrengthListElement
{
    SHORT rxSignalStrength;
    BYTE  radioIf;
};

/**
 * This structure contains the ECIO Information
 *
 *  \param  ecio
 *          - ECIO value in dBm
 *
 *  \param  radioIf
 *          - Radio interface technology of the signal being measured
 *              - 0x00 - RADIO_IF_NO_SVC - None (no service)
 *              - 0x01 - RADIO_IF_CDMA_1X - cdma2000Â® 1X
 *              - 0x02 - RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *              - 0x03 - RADIO_IF_AMPS - AMPS
 *              - 0x04 - RADIO_IF_GSM - GSM
 *              - 0x05 - RADIO_IF_UMTS - UMTS
 */
struct ecioListElement
{
    SHORT ecio;
    BYTE  radioIf;
};

/**
 * This structure contains the Error Rate Information
 *
 *  \param  errorRate
 *          - Error rate value corresponds to the RAT that is currently
 *            registered.
 *              - For CDMA, the error rate reported is Frame Error Rate:
 *                  - Valid error rate values between 1 and 10000 are returned
 *                    to indicate percentage, e.g., a value of 300 means the
 *                    error rate is 3%
 *                  - A value of 0xFFFF indicates that the error rate is
 *                    unknown or unavailable
 *              - For HDR, the error rate reported is Packet Error Rate:
 *                  - Valid error rate values between 1 and 10000 are returned
 *                    to indicate percentage, e.g., a value of 300 means the
 *                    error rate is 3%
 *                  - A value of 0xFFFF indicates that the error rate is
 *                    unknown or unavailable
 *              - For GSM, the error rate reported is Bit Error Rate:
 *                  - Valid values are 0, 100, 200, 300, 400, 500, 600, and 700
 *                    The reported value divided by 100 gives the error rate as
 *                    an RxQual value, e.g.,a value of 300 represents an RxQual
 *                    value of 3.
 *                  - A value of 25500 indicates No Data
 *              - For WCDMA, the error rate reported is Block Error Rate (BLER):
 *                  - Valid values are 1 to 10000
 *                  - The reported value divided by 100 provides the error rate
 *                    in percentages, e.g., a value of 300 represents a BLER of
 *                    3%.
 *                  - A value of 0 indicates No Data
 *
 *  \param  radioIf
 *          - Radio interface technology of the signal being measured
 *              - 0x00 - RADIO_IF_NO_SVC - None (no service)
 *              - 0x01 - RADIO_IF_CDMA_1X - cdma2000Â® 1X
 *              - 0x02 - RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *              - 0x03 - RADIO_IF_AMPS - AMPS
 *              - 0x04 - RADIO_IF_GSM - GSM
 *              - 0x05 - RADIO_IF_UMTS - UMTS
 */
struct errorRateListElement
{
    USHORT errorRate;
    BYTE   radioIf;
};

/**
 * This structure contains the RSRQ Information
 *
 *  \param  rsrq
 *          - RSRQ value in dB (signed integer value); valid range is -3
 *            to -20 (-3 means -3 dB, -20 means -20 dB)
 *
 *  \param  radioIf
 *          - Radio interface technology of the signal being measured
 *              - 0x08 - LTE
 */
struct rsrqInformation
{
    INT8 rsrq;
    BYTE radioIf;
};

/**
 * This structure contains the LTE SNR Information
 *
 *  \param  snrlevel
 *          - LTE SNR level as a scaled integer in units of 0.1dB
 *             e.g. -16dB has a value of -160 and 24.6dB has value of 246.
 */
struct lteSnrinformation
{
    SHORT snrlevel;
};

/**
 * This structure contains the LTE RSRP Information
 *
 *  \param  rsrplevel
 *          - LTE RSRP in dBm as a mesaured by L1.
 *            Range: -44 to -140(-44 means -44dBm, -140 means -140dBm).
 */
struct lteRsrpinformation
{
    SHORT rsrplevel;
};

/**
 * This structure contains the Signal Strength Information
 *
 *  \param  signalStrengthReqMask[IN]
 *          - Request Mask\n
 *              - Request additional signal information for:
 *                Bit 0 - RSSI Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for RSSI\n
 *                        1 - Request Additional Info for RSSI\n
 *                Bit 1 - ECIO Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for ECIO\n
 *                        1 - Request Additional Info for ECIO\n
 *                Bit 2 - IO Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for IO\n
 *                        1 - Request Additional Info for IO\n
 *                Bit 3 - SINR Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for SINR\n
 *                        1 - Request Additional Info for SINR\n
 *                Bit 4 - ERROR RATE Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for Error Rate\n
 *                        1 - Request Additional Info for Error Rate\n
 *                Bit 5 - RSRQ Information bit\n
 *                        Valid values are:\n
 *                        0 - Do Not Request Additional Info for RSRQ\n
 *                        1 - Request Additional Info for RSRQ\n
 *                Bit 6 - LTE SNR information bit\n
 *                        Valid values are:\n
 *                        0 - Do not request additional information for LTE SNR\n
 *                        1 - Request additional information for LTE SNR
 *                Bit 7 - LTE RSRP Information bit\n
 *                        Valid values are:\n
 *                        0 - Do not request additional information for LTE RSRP\n
 *                        1 - Request additional information for LTE RSRP\n
 *
 *  \param  rxSignalStrengthListLen[OUT]
 *          - Number of elements in Receive Signal Strength List
 *
 *  \param  rxSignalStrengthList[OUT]
 *          - See \ref rxSignalStrengthListElement for more information
 *
 *  \param  ecioListLen[OUT]
 *          - Number of elements in ECIO List
 *
 *  \param  ecioList[OUT]
 *          - See \ref ecioListElement for more information
 *
 *  \param  Io[OUT]
 *          - Received Io in dBm; IO is only applicable for 1xEV-DO
 *
 *  \param  sinr[OUT]
 *          - SINR level
 *              - SINR is only applicable for 1xEV-DO; valid levels are 0 to 8
 *                where maximum value for
 *                0 - SINR_LEVEL_0 is -9 dB
 *                1 - SINR_LEVEL_1 is -6 dB
 *                2 - SINR_LEVEL_2 is -4.5 dB
 *                3 - SINR_LEVEL_3 is -3 dB
 *                4 - SINR_LEVEL_4 is -2 dB
 *                5 - SINR_LEVEL_5 is +1 dB
 *                6 - SINR_LEVEL_6 is +3 dB
 *                7 - SINR_LEVEL_7 is +6 dB
 *                8 - SINR_LEVEL_8 is +9 dB
 *
 *  \param  errorRateListLen[OUT]
 *          - Number of elements in Error Rate List
 *
 *  \param  errorRateList[OUT]
 *          - See \ref errorRateListElement for more information
 *
 *  \param  rsrqInfo[OUT]
 *          - See \ref rsrqInformation for more information
 *
 *  \param  ltesnr[OUT]
 *          - LTE SNR level as a scaled integer in units of 0.1 dB;
 *            e.g., -16 dB has a value of -160 and 24.6 dB has a
 *            value of 246. LTE SNR is included only when the current
 *            serving system is LTE
 *
 *  \param  ltersrp[OUT]
 *          - LTE SNR level as a scaled integer in units of 0.1 dB;
 *            e.g., -16 dB has a value of -160 and 24.6 dB has a
 *            value of 246. LTE SNR is included only when the current
 *            serving system is LTE
 */
struct slqsSignalStrengthInfo
{
    USHORT signalStrengthReqMask;
    USHORT rxSignalStrengthListLen;
    struct rxSignalStrengthListElement rxSignalStrengthList[SLQS_SS_INFO_LIST_MAX_ELEMENTS];
    USHORT ecioListLen;
    struct ecioListElement ecioList[SLQS_SS_INFO_LIST_MAX_ELEMENTS];
    INT32  Io;
    BYTE   sinr;
    USHORT errorRateListLen;
    struct errorRateListElement errorRateList[SLQS_SS_INFO_LIST_MAX_ELEMENTS];
    struct rsrqInformation rsrqInfo;
    SHORT  ltesnr;
    SHORT  ltersrp;
};

/**
 *  Queries the current signal strength as measured by the device. This API is
 *  deprecated on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSNasGetSigInfo() for new
 *  firmware versions and new modules
 *
 *  \param  pSignalInfo[IN/OUT]
 *          - See \ref slqsSignalStrengthInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG SLQSGetSignalStrength( struct slqsSignalStrengthInfo *pSignalInfo );

/**
 *  Contain the 3GPP network information.
 *
 *  \param  MCC
 *          - Mobile Country Code
 *
 *  \param  MNC
 *          - Mobile Network Code
 *
 *  \param  InUse
 *          - Is the Network the current serving Network
 *              - 0 - Unknown
 *              - 1 - Current serving network
 *              - 2 - Not current serving network, available
 *
 *  \param  Roaming
 *          - Home/Roam Status of the Network
 *              - 0 - Unknown
 *              - 1 - Home
 *              - 2 - Roam
 *
 *  \param  Forbidden
 *          - Is the Network in the forbidden network list
 *              - 0 - Unknown
 *              - 1 - Forbidden
 *              - 2 - Not Forbidden
 *
 *  \param  Preferred
 *          - Is the Network in the Preferred network list
 *              - 0 - Unknown
 *              - 1 - Preferred
 *              - 2 - Not Preferred
 *
 *  \param  Description
 *          - Network Name/Description
 *          - This is a NULL terminated string.
 */
struct SlqsNas3GppNetworkInfo
{
    WORD  MCC;
    WORD  MNC;
    ULONG InUse;
    ULONG Roaming;
    ULONG Forbidden;
    ULONG Preferred;
    CHAR  Description[MAX_DESCRIPTION_LENGTH];
};

/**
 *  Contain the 3GPP radio access technology information.
 *
 *  \param  MCC
 *          - Mobile Country Code
 *
 *  \param  MNC
 *          - Mobile Network Code
 *
 *  \param  RAT
 *          - Radio Access Technology
 *              - 0x04 - GERAN
 *              - 0x05 - UMTS
 *              - 0x08 - LTE
 *              - 0x09 - TD-SCDMA
 *
 */
typedef struct _SlqsNas3GppNetworkRAT_
{
    WORD MCC;
    WORD MNC;
    BYTE RAT;
}SlqsNas3GppNetworkRAT;

/**
 *  Contain the PCS Digit information
 *
 *  \param  MCC
 *          - Mobile Country Code
 *
 *  \param  MNC
 *          - Mobile Network Code
 *
 *  \param  includes_pcs_digit
 *          - this field is use to interpret the length of corresponding MNC reported
 *          - 0x01 - MNC is a three-digit value
 *          - 0x00 - MNC is a two-digit value
 *
 */
struct SlqsNasPcsDigit
{
    WORD MCC;
    WORD MNC;
    BYTE includes_pcs_digit;
};

/**
 *  Contain the network scan information.
 *
 *  \param  pNetworkInfoInstances[IN/OUT]
 *          - Upon input, maximum number of elements that the network info
 *            instance array can contain.
 *          - Upon successful output, the actual number of elements in the
 *            network info instance array.
 *
 *  \param  pNetworkInfo[OUT]
 *          - Network info instance array
 *              - See \ref SlqsNas3GppNetworkInfo for more information
 *
 *  \param  pRATInstances[IN/OUT]
 *          - Upon input, maximum number of elements that the RAT info
 *            instance array can contain.
 *          - Upon successful output, the actual number of elements in the
 *            RAT info instance array.
 *
 *  \param  pRATInfo[OUT]
 *          - RAT info instance array
 *              - See \ref SlqsNas3GppNetworkRAT for more information
 *
 *  \param  pPCSDigitInstances[IN/OUT]
 *          - Upon input, maximum number of elements that the PCS Digit info
 *            instance array can contain.
 *          - Upon successful output, the actual number of elements in the
 *            PCS Digit info instance array.
 *
 *  \param  pPCSDigitInfo[OUT]
 *          - PCS Digit info instance array
 *              - See \ref SlqsNasPcsDigit for more information
 *
 *  \param  pScanResult[OUT]
 *          - status of network scan
 *          - 0x00 - scan successful
 *          - 0x01 - scan was aborted
 *          - 0x02 - scan did not complete due to a radio link failure recovery in progress
 *
 */
typedef struct _slqsNetworkScanInfo
{
    BYTE                          *pNetworkInfoInstances;
    struct SlqsNas3GppNetworkInfo *pNetworkInfo;
    BYTE                          *pRATInstances;
    SlqsNas3GppNetworkRAT  *pRATInfo;
    BYTE                          *pPCSDigitInstances;
    struct SlqsNasPcsDigit        *pPCSDigitInfo;
    ULONG                         *pScanResult;
} slqsNetworkScanInfo;

/**
 *  Performs scan for available networks and scans for RAT info as well.
 *
 *  \param  pNetworkInfo[IN/OUT]
 *          - See \ref slqsNetworkScanInfo for more information
 *          - Valid pointers to the following structure members are mandatory
 *              - pNetworkInfoInstances
 *              - pNetworkInfo
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: 5 minutes\n
 *
 */
ULONG SLQSPerformNetworkScan( slqsNetworkScanInfo *pNetworkInfo );

/**
 *  Contain the network selection preference.
 *
 *  \param  netReg
 *          - specifies one of the following actions:
 *              - 0x00 - Automatic registration
 *                     - Device registers according to its provisioning; mcc
 *                       and mnc fields are ignored
 *              - 0x01 - Manual Registration
 *                     - Device registers to specified network; mcc and mnc
 *                       must contain valid values
 *
 *  \param  mcc
 *          - MCC value. Range 0 to 999
 *
 *  \param  mnc
 *          - MNC value. Range 0 to 999
 *
 */
struct netSelectionPref
{
    BYTE netReg;
    WORD mcc;
    WORD mnc;
};

/**
 *  Contain the Acquisition Order Preference.
 *
 *  \param  acqOrdeLen
 *          - Number of sets of the following elements.
 *
 *  \param  pAcqOrder
 *          - Acquisition order preference to be set. Values:
 *            - 0x01 - NAS_RADIO_IF_CDMA_1X - cdma2000 1X
 *            - 0x02 - NAS_RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *            - 0x04 - NAS_RADIO_IF_GSM - GSM
 *            - 0x05 - NAS_RADIO_IF_UMTS - UMTS
 *            - 0x08 - NAS_RADIO_IF_LTE - LTE
 *            - 0x09 - NAS_RADIO_IF_TDSCDMA - TD-SCDMA
 *
 */
struct acqOrderPref
{
    BYTE acqOrdeLen;
    BYTE *pAcqOrder;
};

/**
 *  Contain the CSGID.
 *
 *  \param  mcc
 *          - MCC value. Range 0 to 999
 *
 *  \param  mnc
 *          - MNC value. Range 0 to 999
 *
 *   \param  mncPcsDigits
 *           - TRUE - MNC is a three-digit value; e.g., a reported
 *                   value of 90 corresponds to an MNC value of 090
 *           - FALSE - MNC is a two-digit value; e.g., a reported
 *                   value of 90 corresponds to an MNC value of 90
 *   \param   id
 *            - Closed subscriber group identifier.
 *
 *   \param   rat
 *            - Radio interface technology of the CSG network.
 *              Values:
 *              - 0x04 - RADIO_IF_GSM - GSM
 *              - 0x05 - RADIO_IF_UMTS - UMTS
 *              - 0x08 - RADIO_IF_LTE - LTE
 *              - 0x09 - RADIO_IF_TDSCDMA - TDS
 *
 */
struct CSGID
{
    WORD  mcc;
    WORD  mnc;
    BYTE  mncPcsDigits;
    ULONG id;
    BYTE  rat;
};

/**
 *  Contain the system selection preferences.
 *
 *  \param  pEmerMode
 *          - Optional parameter specifying the emergency Mode
 *          - Values:
 *              - 0 - OFF (normal)
 *              - 1 - ON (Emergency)
 *
 *  \param  pModePref
 *          - Optional parameter
 *          - Bit Mask indicating the radio technology mode preference
 *          - Bit values:
 *              - Bit 0 - cdma2000 1x
 *              - Bit 1 - cdma2000 HRPD(1xEV-DO)
 *              - Bit 2 - GSM
 *              - Bit 3 - UMTS
 *              - Bit 4 - LTE
 *
 *  \param  pBandPref
 *          - Optional parameter
 *          - Bit mask representing the band preference
 *          - Bit values:
 *              - Bit 0 - Band Class 0, A-System
 *              - Bit 1 - Band Class 0, B-System, Band Class 0
 *                        AB, GSM 850 Band
 *              - Bit 2 - Band Class 1, all blocks
 *              - Bit 3 - Band Class 2 place holder
 *              - Bit 4 - Band Class 3, A-System
 *              - Bit 5 - Band Class 4, all blocks
 *              - Bit 6 - Band Class 5, all blocks
 *              - Bit 7 - GSM_DCS_1800 band
 *              - Bit 8 - GSM Extended GSM (E-GSM) 900 band
 *              - Bit 9 - GSM Primary GSM (P-GSM) 900 band
 *              - Bit 10 - Band Class 6
 *              - Bit 11 - Band Class 7
 *              - Bit 12 - Band Class 8
 *              - Bit 13 - Band Class 9
 *              - Bit 14 - Band Class 10
 *              - Bit 15 - Band Class 11
 *              - Bit 16 - GSM 450 band
 *              - Bit 17 - GSM 480 band
 *              - Bit 18 - GSM 750 band
 *              - Bit 19 - GSM 850 band
 *              - Bit 20 - GSM Railways GSM 900 Band
 *              - Bit 21 - GSM PCS 1900 band
 *              - Bit 22 - WCDMA Europe, Japan, and China IMT 2100 band
 *              - Bit 23 - WCDMA U.S. PCS 1900 band
 *              - Bit 24 - WCDMA Europe and China DCS 1800 band
 *              - Bit 25 - WCDMA U.S. 1700 band
 *              - Bit 26 - WCDMA U.S. 850 band
 *              - Bit 27 - WCDMA Japan 800 band
 *              - Bit 28 - Band Class 12
 *              - Bit 29 - Band Class 14
 *              - Bit 30 - Reserved
 *              - Bit 31 - Band Class 15
 *              - Bit 32 to 47 - Reserved
 *              - Bit 48 - WCDMA Europe 2600 band
 *              - Bit 49 - WCDMA Europe and Japan 900 band
 *              - Bit 50 - WCDMA Japan 1700 band
 *              - Bit 51 to 55 - Reserved
 *              - Bit 56 - Band Class 16
 *              - Bit 57 - Band Class 17
 *              - Bit 58 - Band Class 18
 *              - Bit 59 - Band Class 19
 *              - Bit 60 to 64 - Reserved
 *
 *  \param  pPRLPref
 *          - Optional parameter indicating the CDMA PRL Preference
 *          - Values:
 *              - 0x0001 - Acquire available system only on the A side
 *              - 0x0002 - Acquire available system only on the B side
 *              - 0x3FFF - Acquire any available systems
 *
 *  \param  pRoamPref
 *          - Optional parameter indicating the roaming Preference
 *          - Values:
 *              - 0x01 - Acquire only systems for which the roaming indicator
 *                       is off
 *              - 0x02 - Acquire a system as long as its roaming indicator is
 *                       not off
 *              - 0x03 - Acquire only systems for which the roaming indicator
 *                       is off or solid on, i.e. not flashing; CDMA only
 *              - 0xFF - Acquire systems, regardless of their roaming indicator
 *
 *  \param  pLTEBandPref
 *          - Optional parameter
 *          - Bit mask representing the LTE band preference
 *          - Bit Values
 *              - Bit 0 - E-UTRA Operating Band 1
 *              - Bit 1 - E-UTRA Operating Band 2
 *              - Bit 2 - E-UTRA Operating Band 3
 *              - Bit 3 - E-UTRA Operating Band 4
 *              - Bit 4 - E-UTRA Operating Band 5
 *              - Bit 5 - E-UTRA Operating Band 6
 *              - Bit 6 - E-UTRA Operating Band 7
 *              - Bit 7 - E-UTRA Operating Band 8
 *              - Bit 8 - E-UTRA Operating Band 9
 *              - Bit 9 - E-UTRA Operating Band 10
 *              - Bit 10 - E-UTRA Operating Band 11
 *              - Bit 11 - E-UTRA Operating Band 12
 *              - Bit 12 - E-UTRA Operating Band 13
 *              - Bit 13 - E-UTRA Operating Band 14
 *              - Bit 16 - E-UTRA Operating Band 17
 *              - Bit 17 - E-UTRA Operating Band 18
 *              - Bit 18 - E-UTRA Operating Band 19
 *              - Bit 19 - E-UTRA Operating Band 20
 *              - Bit 20 - E-UTRA Operating Band 21
 *              - Bit 22 - E-UTRA Operating Band 23
 *              - Bit 23 - E-UTRA Operating Band 24
 *              - Bit 24 - E-UTRA Operating Band 25
 *              - Bit 25 - E-UTRA Operating Band 26
 *              - Bit 27 - E-UTRA Operating Band 28
 *              - Bit 28 - E-UTRA Operating Band 29
 *              - Bit 29 - E-UTRA Operating Band 32
 *              - Bit 32 - E-UTRA Operating Band 33
 *              - Bit 33 - E-UTRA Operating Band 34
 *              - Bit 34 - E-UTRA Operating Band 35
 *              - Bit 35 - E-UTRA Operating Band 36
 *              - Bit 36 - E-UTRA Operating Band 37
 *              - Bit 37 - E-UTRA Operating Band 38
 *              - Bit 38 - E-UTRA Operating Band 39
 *              - Bit 39 - E-UTRA Operating Band 40
 *              - Bit 40 - E-UTRA Operating Band 41
 *              - Bit 41 - E-UTRA Operating Band 42
 *              - Bit 42 - E-UTRA Operating Band 43
 *              - Bit 60 - E-UTRA Operating Band 125
 *              - All other bits are reserved
 *
 *  \param  pNetSelPref - netSelectionPref
 *          - Optional parameter for specifying Network Selection Preference
 *          - Modem selects networks based on this parameter(if present).
 *          - Either of pNetSelPref or pCSGID can be set.
 *          - see \ref netSelectionPref for more information
 *
 *  \param  pChgDuration
 *          - Optional parameter specifying the duration of the change
 *          - At least one system selection setting to be set if pChgDuration is populated.
 *          - Values:
 *              - 0x00 - Power cycle - Remains active until the next device
 *                                     power cycle
 *              - 0x01 - Permanent - Remains active through power cycles until
 *                                   changed by client
 *              - Device will use "0x01 - permanent" as default if this
 *                parameter is omitted
 *
 *  \param  pMNCIncPCSDigStat
 *          - Optional parameter indicating if MNC includes PCS digit
 *          - pNetSelPref is expected if MNC includes PCS digit is set to 1.
 *          - Values:
 *              - TRUE - MNC is a 3 digit value; e.g., a reported value of
 *                       90 corresponds to an MNC value of 090
 *              - FALSE - MNC is a 2-digit value; e.g., a reported value of
 *                        90 corresponds to an MNC value of 90
 *
 *  \param  pSrvDomainPref
 *          - Optional parameter indicating Service domain preference
 *          - Values:
 *              - 0x00 - Circuit switched only
 *              - 0x01 - Packet switched only
 *              - 0x02 - Circuit switched and packet switched
 *              - 0x03 - Packet switched attach
 *              - 0x04 - Packet switched detach
 *
 *  \param  pGWAcqOrderPref
 *          - Optional parameter indicating GSM/WCDMA Acquisition
 *            order Preference
 *          - Values:
 *              - 0x00 - Automatic
 *              - 0x01 - GSM then WCDMA
 *              - 0x02 - WCDMA then GSM
 *
 *  \param  pTdscdmaBandPref
 *          - Optional parameter indicating bitmask representing the TD-SCDMA band
 *            preference to be set.
 *          - Values:
 *              - 0x01 - TD-SCDMA Band A
 *              - 0x02 - TD-SCDMA Band B
 *              - 0x04 - TD-SCDMA Band C
 *              - 0x08 - TD-SCDMA Band D
 *              - 0x10 - TD-SCDMA Band E
 *              - 0x20 - TD-SCDMA Band F
 *              - All other bits are reserved
 *
 *  \param  pAcqOrderPref - acqOrderPref
 *          - Optional parameter for specifying Acquisition Order Preference
 *          - see \ref acqOrderPref for more information
 *
 *  \param  pSrvRegRestriction
 *          - Optional parameter indicating Network Selection Registration Restriction Preference
 *          - Values:
 *              - 0x00 - Device follows the normal registration process
 *              - 0x01 - Device camps on the network according to its provisioning,
 *                       but does not register
 *              - 0x02 - Device selects the network for limited service
 *              - All other values are reserved.
 *
 *  \param  pCSGID - CSGID
 *          - Optional parameter for specifying CSG ID
 *          - Either of pNetSelPref or pCSGID can be set.
 *          - see \ref CSGID for more information
 *
 *  \param  pRAT
 *          - Optional parameter Radio Access Technology
 *            order Preference
 *          - Values:
 *              - 0x04 - GSM
 *              - 0x05 - UMTS
 *              - 0x08 - LTE
 *              - 0x09 - TDSCDMA
 */
typedef struct _sysSelectPrefParams
{
    BYTE                    *pEmerMode;
    WORD                    *pModePref;
    ULONGLONG               *pBandPref;
    WORD                    *pPRLPref;
    WORD                    *pRoamPref;
    ULONGLONG               *pLTEBandPref;
    struct netSelectionPref *pNetSelPref;
    BYTE                    *pChgDuration;
    BYTE                    *pMNCIncPCSDigStat;
    ULONG                   *pSrvDomainPref;
    ULONG                   *pGWAcqOrderPref;
    ULONGLONG               *pTdscdmaBandPref;
    struct acqOrderPref     *pAcqOrderPref;
    ULONG                   *pSrvRegRestriction;
    struct CSGID            *pCSGID;
    BYTE                    *pRAT;
} sysSelectPrefParams;

/**
 *  Sets the different system selection preferences of the device.
 *
 *  \param  pSysSelectPrefParams[IN]
 *          - See \ref sysSelectPrefParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSSetSysSelectionPref( sysSelectPrefParams *pSysSelectPrefParams );

/**
 *  Structure for storing the current preferred system selection settings
 *  for the device.
 *
 *  \param  pEmerMode
 *          - Optional parameter specifying the emergency Mode
 *          - Values:
 *              - 0 - OFF (normal)
 *              - 1 - ON (Emergency)
 *          - function SLQSGetSysSelectionPref() returns a default value FF
 *            if no value is returned by the device.
 *
 *  \param  pModePref
 *          - Optional parameter
 *          - Bit Mask indicating the radio technology mode preference
 *          - Bit values:
 *              - Bit 0 - cdma2000 1x
 *              - Bit 1 - cdma2000 HRPD(1xEV-DO)
 *              - Bit 2 - GSM
 *              - Bit 3 - UMTS
 *              - Bit 4 - LTE
 *          - function SLQSGetSysSelectionPref() returns a default value FF
 *            if no value is returned by the device.
 *
 *  \param  pBandPref
 *          - Optional parameter
 *          - Bit mask representing the band preference
 *          - Bit values:
 *              - Bit 0 - Band Class 0, A-System
 *              - Bit 1 - Band Class 0, B-System, Band Class 0
 *                        AB, GSM 850 Band
 *              - Bit 2 - Band Class 1, all blocks
 *              - Bit 3 - Band Class 2 place holder
 *              - Bit 4 - Band Class 3, A-System
 *              - Bit 5 - Band Class 4, all blocks
 *              - Bit 6 - Band Class 5, all blocks
 *              - Bit 7 - GSM_DCS_1800 band
 *              - Bit 8 - GSM Extended GSM (E-GSM) 900 band
 *              - Bit 9 - GSM Primary GSM (P-GSM) 900 band
 *              - Bit 10 - Band Class 6
 *              - Bit 11 - Band Class 7
 *              - Bit 12 - Band Class 8
 *              - Bit 13 - Band Class 9
 *              - Bit 14 - Band Class 10
 *              - Bit 15 - Band Class 11
 *              - Bit 16 - GSM 450 band
 *              - Bit 17 - GSM 480 band
 *              - Bit 18 - GSM 750 band
 *              - Bit 19 - GSM 850 band
 *              - Bit 20 - GSM Railways GSM 900 Band
 *              - Bit 21 - GSM PCS 1900 band
 *              - Bit 22 - WCDMA Europe, Japan, and China IMT 2100 band
 *              - Bit 23 - WCDMA U.S. PCS 1900 band
 *              - Bit 24 - WCDMA Europe and China DCS 1800 band
 *              - Bit 25 - WCDMA U.S. 1700 band
 *              - Bit 26 - WCDMA U.S. 850 band
 *              - Bit 27 - WCDMA Japan 800 band
 *              - Bit 28 - Band Class 12
 *              - Bit 29 - Band Class 14
 *              - Bit 30 - Reserved
 *              - Bit 31 - Band Class 15
 *              - Bit 32 to 47 - Reserved
 *              - Bit 48 - WCDMA Europe 2600 band
 *              - Bit 49 - WCDMA Europe and Japan 900 band
 *              - Bit 50 - WCDMA Japan 1700 band
 *              - Bit 51 to 55 - Reserved
 *              - Bit 56 - Band Class 16
 *              - Bit 57 - Band Class 17
 *              - Bit 58 - Band Class 18
 *              - Bit 59 - Band Class 19
 *              - Bit 60 to 64 - Reserved
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFFFFFFFFFFFFFF if no value is returned by the device.
 *
 *  \param  pPRLPref
 *          - Optional parameter indicating the CDMA PRL Preference
 *          - Values:
 *              - 0x0001 - Acquire available system only on the A side
 *              - 0x0002 - Acquire available system only on the B side
 *              - 0x3FFF - Acquire any available systems
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFF if no value is returned by the device.
 *
 *  \param  pRoamPref
 *          - Optional parameter indicating the roaming Preference
 *          - Values:
 *              - 0x01 - Acquire only systems for which the roaming indicator
 *                       is off
 *              - 0x02 - Acquire a system as long as its roaming indicator is
 *                       not off
 *              - 0x03 - Acquire only systems for which the roaming indicator
 *                       is off or solid on, i.e. not flashing; CDMA only
 *              - 0xFF - Acquire systems, regardless of their roaming indicator
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFF if no value is returned by the device.
 *
 *  \param  pLTEBandPref
 *          - Optional parameter
 *          - Bit mask representing the LTE band preference
 *          - Bit Values
 *              - Bit 0 - E-UTRA Operating Band 1
 *              - Bit 1 - E-UTRA Operating Band 2
 *              - Bit 2 - E-UTRA Operating Band 3
 *              - Bit 3 - E-UTRA Operating Band 4
 *              - Bit 4 - E-UTRA Operating Band 5
 *              - Bit 5 - E-UTRA Operating Band 6
 *              - Bit 6 - E-UTRA Operating Band 7
 *              - Bit 7 - E-UTRA Operating Band 8
 *              - Bit 8 - E-UTRA Operating Band 9
 *              - Bit 9 - E-UTRA Operating Band 10
 *              - Bit 10 - E-UTRA Operating Band 11
 *              - Bit 11 - E-UTRA Operating Band 12
 *              - Bit 12 - E-UTRA Operating Band 13
 *              - Bit 13 - E-UTRA Operating Band 14
 *              - Bit 16 - E-UTRA Operating Band 17
 *              - Bit 17 - E-UTRA Operating Band 18
 *              - Bit 18 - E-UTRA Operating Band 19
 *              - Bit 19 - E-UTRA Operating Band 20
 *              - Bit 20 - E-UTRA Operating Band 21
 *              - Bit 22 - E-UTRA Operating Band 23
 *              - Bit 23 - E-UTRA Operating Band 24
 *              - Bit 24 - E-UTRA Operating Band 25
 *              - Bit 25 - E-UTRA Operating Band 26
 *              - Bit 27 - E-UTRA Operating Band 28
 *              - Bit 28 - E-UTRA Operating Band 29
 *              - Bit 29 - E-UTRA Operating Band 32
 *              - Bit 32 - E-UTRA Operating Band 33
 *              - Bit 33 - E-UTRA Operating Band 34
 *              - Bit 34 - E-UTRA Operating Band 35
 *              - Bit 35 - E-UTRA Operating Band 36
 *              - Bit 36 - E-UTRA Operating Band 37
 *              - Bit 37 - E-UTRA Operating Band 38
 *              - Bit 38 - E-UTRA Operating Band 39
 *              - Bit 39 - E-UTRA Operating Band 40
 *              - Bit 40 - E-UTRA Operating Band 41
 *              - Bit 41 - E-UTRA Operating Band 42
 *              - Bit 42 - E-UTRA Operating Band 43
 *              - Bit 60 - E-UTRA Operating Band 125
 *              - All other bits are reserved
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFFFFFFFFFFFFFF if no value is returned by the device.
 *
 *  \param  pNetSelPref
 *          - Optional parameter indicating network selection preference
 *          - Values:
 *              - 0x00 - Automatic network selection
 *              - 0x01 - Manual network selection
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FF if no value is returned by the device.
 *
 *  \param  pSrvDomainPref
 *          - Optional parameter indicating Service domain preference
 *          - Values:
 *              - 0x00 - Circuit switched only
 *              - 0x01 - Packet switched only
 *              - 0x02 - Circuit switched and packet switched
 *              - 0x03 - Packet switched attach
 *              - 0x04 - Packet switched detach
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFFFFFF if no value is returned by the device.
 *
 *  \param  pGWAcqOrderPref
 *          - Optional parameter indicating GSM/WCDMA Acquisition
 *            order Preference
 *          - Values:
 *              - 0x00 - Automatic
 *              - 0x01 - GSM then WCDMA
 *              - 0x02 - WCDMA then GSM
 *          - function SLQSGetSysSelectionPref() returns a default value
 *            FFFFFFFF if no value is returned by the device.
 *
 * \note    None
 *
 */
typedef struct _sysSelectPrefInfo
{
    BYTE      *pEmerMode;
    WORD      *pModePref;
    ULONGLONG *pBandPref;
    WORD      *pPRLPref;
    WORD      *pRoamPref;
    ULONGLONG *pLTEBandPref;
    BYTE      *pNetSelPref;
    ULONG     *pSrvDomainPref;
    ULONG     *pGWAcqOrderPref;
} sysSelectPrefInfo;

/**
 *  Queries the different system selection preferences of the device.
 *
 *  \param  pSysSelectPrefInfo[OUT]
 *          - See \ref sysSelectPrefInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSGetSysSelectionPref( sysSelectPrefInfo *pSysSelectPrefInfo );

/**
 *  Structure for storing the service status information for CDMA and HDR
 *  networks.
 *
 *  \param  srvStatus
 *          - Service status of the system.
 *              - 0x00 - No service
 *              - 0x01 - Limited service
 *              - 0x02 - Service
 *              - 0x03 - Limited regional service
 *              - 0x04 - Power save
 *              - 0xFF - Not Available
 *
 *  \param  isPrefDataPath
 *          - Whether the RAT is the preferred data path.
 *              - 0x00 - Not preferred
 *              - 0x01 - Preferred
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE srvStatus;
    BYTE isPrefDataPath;
} SrvStatusInfo;

/**
 *  Structure for storing the service status information for GSM, WCDMA and
 *  LTE networks.
 *
 *  \param  srvStatus
 *          - Service status of the system.
 *              - 0x00 - No service
 *              - 0x01 - Limited service
 *              - 0x02 - Service
 *              - 0x03 - Limited regional service
 *              - 0x04 - Power save
 *              - 0xFF - Not Available
 *
 *  \param  trueSrvStatus
 *          - True service status of the system.
 *          - Not applicable to CDMA/HDR.
 *              - 0x00 - No service
 *              - 0x01 - Limited service
 *              - 0x02 - Service
 *              - 0x03 - Limited regional service
 *              - 0x04 - Power save
 *              - 0xFF - Not Available
 *
 *  \param  isPrefDataPath
 *          - Whether the RAT is the preferred data path.
 *              - 0x00 - Not preferred
 *              - 0x01 - Preferred
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE srvStatus;
    BYTE trueSrvStatus;
    BYTE isPrefDataPath;
} GSMSrvStatusInfo;

/**
 *  Structure for storing the System Information common to CDMA, HDR, GSM,
 *  WCDMA and LTE networks.
 *
 *  \param  srvDomainValid
 *          - Indicates whether the service domain is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  srvDomain
 *          - Service domain registered on the system.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *              - 0x04 - Camped
 *              - 0xFF - Not Available
 *
 *  \param  srvCapabilityValid
 *          - Indicates whether the service capability is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  srvCapability
 *          - Current system's service capability.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *              - 0x04 - Camped
 *              - 0xFF - Not Available
 *
 *  \param  roamStatusValid
 *          - Indicates whether the roaming status is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  roamStatus
 *          - Current roaming status.
 *              - 0x00 - Off
 *              - 0x01 - On
 *              - 0x02 - Blinking
 *              - 0x03 - Out of the neighborhood
 *              - 0x04 - Out of the building
 *              - 0x05 - Preferred system
 *              - 0x06 - Available system
 *              - 0x07 - Alliance partner
 *              - 0x08 - Premium partner
 *              - 0x09 - Full service
 *              - 0x0A - Partial service
 *              - 0x0B - Banner is on
 *              - 0x0C - Banner is off
 *              - 0x0D to 0x3F - Reserved for Standard Enhanced Roaming
 *                               Indicator Numbers
 *              - 0x40 to 0x7F - Reserved for Non-Standard Enhanced Roaming
 *                               Indicator Numbers
 *              - 0x40 to 0xFF - Reserved.
 *              - 0xFF - Not Available
 *          - Values from 0x02 onward are only applicable for 3GPP2
 *
 *  \param  isSysForbiddenValid
 *          - Indicates whether the forbidden system is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  isSysForbidden
 *          - Whether the system is forbidden.
 *              - 0x00 - Not forbidden
 *              - 0x01 - Forbidden
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE srvDomainValid;
    BYTE srvDomain;
    BYTE srvCapabilityValid;
    BYTE srvCapability;
    BYTE roamStatusValid;
    BYTE roamStatus;
    BYTE isSysForbiddenValid;
    BYTE isSysForbidden;
} sysInfoCommon;

/**
 *  Structure for storing the CDMA System Information.
 *
 *  \param  sysInfoCDMA
 *          - See \ref sysInfoCommon for more information.
 *
 *  \param  isSysPrlMatchValid
 *          - Indicates whether the system PRL match is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  isSysPrlMatch
 *          - Indicates whether the system is in a PRL.
 *          - Only applies to CDMA/HDR.
 *              - 0x00 - System is not in a PRL
 *              - 0x01 - System is in a PRL
 *              - 0xFF - Not Available
 *          - If the system is not in a PRL, roam_status carries the value from
 *            the default roaming indicator in the PRL.
 *          - If the system is in a PRL, roam_status is set to the value based
 *            on the standard specification.
 *
 *  \param  pRevInUseValid
 *          - Indicates whether the P_Rev in use is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  pRevInUse
 *          - The lesser of the base station P_Rev and mobile P_Rev
 *          - Only applicable for CDMA.
 *              - 0xFF - Not Available
 *
 *  \param  bsPRevValid
 *          - Indicates whether the base station P_Rev is valid
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  bsPRev
 *          - Base station P_Rev.
 *          - Only applicable for CDMA.
 *              - 0xFF - Not Available
 *
 *  \param  ccsSupportedValid
 *          - Indicates whether the supported concurrent service is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  ccsSupported
 *          - Whether concurrent service is supported.
 *          - Only applicable for CDMA.
 *              - 0x00 - Not supported
 *              - 0x01 - Supported
 *              - 0xFF - Not Available
 *
 *  \param  cdmaSysIdValid
 *          - Indicates whether the CDMA system ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  systemID
 *          - System ID.
 *              - 0xFFFF - Not Available
 *
 *  \param  networkID
 *          - Network ID.
 *              - 0xFFFF - Not Available
 *
 *  \param  bsInfoValid
 *          - Indicates whether the base station information is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  baseLat
 *          - Base station latitude in units of 0.25 sec.
 *          - Expressed as a two's complement signed number with positive
 *            numbers signifying North latitudes.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  baseLong
 *          - Base station longitude in units of 0.25 sec.
 *          - Expressed as a two's complement signed number with positive
 *            numbers signifying East latitudes.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  packetZoneValid
 *          - Indicates whether the packet zone is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  packetZone
 *          - Packet zone (8-bit).
 *              - 0xFFFF indicates no packet zone.
 *          - Only applicable for CDMA.
 *
 *  \param  networkIdValid
 *          - Indicates whether the network ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  MCC[PLMN_LENGTH]
 *          - Mobile Country Code.
 *          - MCC digits in ASCII characters
 *          - MCC wildcard value is returned as {'3', 0xFF, 0xFF}.
 *
 *
 *  \param  MNC[PLMN_LENGTH]
 *          - Mobile Network Code.
 *          - MNC digits in ASCII characters
 *          - An unused byte is set to 0xFF.
 *          - MNC wildcard value is returned as {'7', 0xFF, 0xFF}.
 *
 */
typedef struct
{
    sysInfoCommon sysInfoCDMA;
    BYTE          isSysPrlMatchValid;
    BYTE          isSysPrlMatch;
    BYTE          pRevInUseValid;
    BYTE          pRevInUse;
    BYTE          bsPRevValid;
    BYTE             bsPRev;
    BYTE             ccsSupportedValid;
    BYTE          ccsSupported;
    BYTE          cdmaSysIdValid;
    WORD               systemID;
    WORD               networkID;
    BYTE          bsInfoValid;
    WORD               baseId;
    ULONG              baseLat;
    ULONG              baseLong;
    BYTE          packetZoneValid;
    WORD               packetZone;
    BYTE          networkIdValid;
    BYTE          MCC[PLMN_LENGTH];
    BYTE          MNC[PLMN_LENGTH];
} CDMASysInfo;

/**
 *  Structure for storing the HDR System Information.
 *
 *  \param  sysInfoHDR
 *          - See \ref sysInfoCommon for more information.
 *
 *  \param  isSysPrlMatchValid
 *          - Indicates whether the system PRL match is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  isSysPrlMatch
 *          - Indicates whether the system is in a PRL.
 *          - Only applies to CDMA/HDR.
 *              - 0x00 - System is not in a PRL
 *              - 0x01 - System is in a PRL
 *              - 0xFF - Not Available
 *          - If the system is not in a PRL, roam_status carries the value from
 *            the default roaming indicator in the PRL.
 *          - If the system is in a PRL, roam_status is set to the value based
 *            on the standard specification.
 *
 *  \param  hdrPersonalityValid
 *          - Indicates whether the HDR personality is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  hdrPersonality
 *          - HDR personality information.
 *          - Only applicable for HDR.
 *              - 0x00 - None
 *              - 0x02 - HRPD
 *              - 0x03 - eHRPD
 *              - 0xFF - Not Available
 *
 *  \param  hdrActiveProtValid
 *          - Indicates whether the HDR active protocol revision information
 *            is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  hdrActiveProt
 *          - HDR active protocol revision information .
 *          - Only applicable for HDR.
 *              - 0x00 - None
 *              - 0x02 - HDR Rel 0
 *              - 0x03 - HDR Rel A
 *              - 0x04 - HDR Rel B
 *              - 0xFF - Not Available
 *
 *  \param  is856SysIdValid
 *          - Indicates whether the IS-856 system ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  is856SysId[SLQS_SYSTEM_ID_SIZE]
 *          - IS-856 system ID.
 *          - Only applicable for HDR.
 *
 */
typedef struct
{
   sysInfoCommon sysInfoHDR;
   BYTE          isSysPrlMatchValid;
   BYTE          isSysPrlMatch;
   BYTE          hdrPersonalityValid;
   BYTE          hdrPersonality;
   BYTE          hdrActiveProtValid;
   BYTE          hdrActiveProt;
   BYTE          is856SysIdValid;
   BYTE          is856SysId[SLQS_SYSTEM_ID_SIZE];
} HDRSysInfo;

/**
 *  Structure for storing the GSM System Information.
 *
 *  \param  sysInfoGSM
 *          - See \ref sysInfoCommon for more information.
 *
 *  \param  lacValid
 *          - Indicates whether the location area code is valid..
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  lac
 *          - Location area code.
 *          - Only applies to 3GPP.
 *              - 0xFFFF - Not Available
 *
 *  \param  cellIdValid
 *          - Indicates whether the cell ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  cellId
 *          - Cell ID.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  regRejectInfoValid
 *          - Indicates whether the registration reject information is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  rejectSrvDomain
 *          - Type of service domain in which the registration is rejected.
 *              - 0x00 - SYS_SRV_DOMAIN_NO_SRV - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *              - 0x04 - Camped
 *              - 0xFF - Not Available
 *
 *  \param  rejCause
 *          - Reject cause values sent are specified in
 *            [3GPP TS 24.008, Section 10.5.3.6].
 *              - 0xFF - Not Available
 *
 *
 *  \param  networkIdValid
 *          - Indicates whether the network ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  MCC[PLMN_LENGTH]
 *          - Mobile Country Code.
 *          - MCC digits in ASCII characters
 *
 *  \param  MNC[PLMN_LENGTH]
 *          - Mobile Network Code.
 *          - MNC digits in ASCII characters
 *          - An unused byte is set to 0xFF.
 *          - In case of two-digit MNC values, the third (unused) digit is set to 0xFF.
 *            For example, 15 (a two-digit MNC) is reported using the byte stream
 *            0x31 0x35 0xFF.
 *
 *  \param  egprsSuppValid
 *          - Indicates whether the EGPRS support is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  egprsSupp
 *          - EGPRS support indication.
 *          - Only applicable for GSM.
 *              - 0x00 - Not available
 *              - 0x01 - Available
 *              - 0xFF - Not Available
 *
 *  \param  dtmSuppValid
 *          - Indicates whether Dual Transfer mode support is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  dtmSupp
 *          - Dual Transfer mode support indication.
 *          - Only applicable for GSM.
 *              - 0x00 - Not available
 *              - 0x01 - Available
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
     sysInfoCommon sysInfoGSM;
     BYTE          lacValid;
     WORD          lac;
     BYTE          cellIdValid;
     ULONG         cellId;
     BYTE          regRejectInfoValid;
     BYTE          rejectSrvDomain;
     BYTE          rejCause;
     BYTE          networkIdValid;
     BYTE          MCC[PLMN_LENGTH];
     BYTE          MNC[PLMN_LENGTH];
     BYTE          egprsSuppValid;
     BYTE          egprsSupp;
     BYTE          dtmSuppValid;
     BYTE          dtmSupp;
} GSMSysInfo;

/**
 *  Structure for storing the WCDMA System Information.
 *
 *  \param  sysInfoWCDMA
 *          - See \ref sysInfoCommon for more information.
 *
 *  \param  lacValid
 *          - Indicates whether the location area code is valid..
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  lac
 *          - Location area code.
 *          - Only applies to 3GPP.
 *              - 0xFFFF - Not Available
 *
 *  \param  cellIdValid
 *          - Indicates whether the cell ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  cellId
 *          - Cell ID.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  regRejectInfoValid
 *          - Indicates whether the registration reject information is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  rejectSrvDomain
 *          - Type of service domain in which the registration is rejected.
 *              - 0x00 - SYS_SRV_DOMAIN_NO_SRV - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *              - 0x04 - Camped
 *              - 0xFF - Not Available
 *
 *  \param  rejCause
 *          - Reject cause values sent are specified in
 *            [3GPP TS 24.008, Section 10.5.3.6].
 *              - 0xFF - Not Available
 *
 *
 *  \param  networkIdValid
 *          - Indicates whether the network ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  MCC[PLMN_LENGTH]
 *          - Mobile Country Code.
 *          - MCC digits in ASCII characters
 *
 *  \param  MNC[PLMN_LENGTH]
 *          - Mobile Network Code.
 *          - MNC digits in ASCII characters
 *          - An unused byte is set to 0xFF.
 *          - In case of two-digit MNC values, the third (unused) digit is set to 0xFF.
 *            For example, 15 (a two-digit MNC) is reported using the byte stream
 *            0x31 0x35 0xFF.
 *
 *  \param  hsCallStatusValid
 *          - Indicates whether the high-speed call status is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  hsCallStatus
 *          - Call status on high speed.
 *          - Only applicable for WCDMA.
 *              - 0x00 - HSDPA and HSUPA are unsupported
 *              - 0x01 - HSDPA is supported
 *              - 0x02 - HSUPA is supported
 *              - 0x03 - HSDPA and HSUPA are supported
 *              - 0x04 - HSDPA+ is supported
 *              - 0x05 - HSDPA+ and HSUPA are supported
 *              - 0x06 - Dual-cell HSDPA+ is supported
 *              - 0x07 - Dual-cell HSDPA+ and HSUPA are supported
 *              - 0xFF - Not Available
 *
 *  \param  hsIndValid
 *          - Indicates whether high-speed service indication is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  hsInd
 *          - High-speed service indication
 *          - Only applicable for WCDMA.
 *              - 0x00 - HSDPA and HSUPA are unsupported
 *              - 0x01 - HSDPA is supported
 *              - 0x02 - HSUPA is supported
 *              - 0x03 - HSDPA and HSUPA are supported
 *              - 0x04 - HSDPA+ is supported
 *              - 0x05 - HSDPA+ and HSUPA are supported
 *              - 0x06 - Dual-cell HSDPA+ is supported
 *              - 0x07 - Dual-cell HSDPA+ and HSUPA are supported
 *              - 0xFF - Not Available
 *
 *  \param  pscValid
 *          - Indicates whether primary scrambling code is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  psc
 *          - Primary scrambling code.
 *              - 0xFFFF - Not Available
 *
 */
typedef struct
{
    sysInfoCommon sysInfoWCDMA;
    BYTE          lacValid;
    WORD          lac;
    BYTE          cellIdValid;
    ULONG         cellId;
    BYTE          regRejectInfoValid;
    BYTE          rejectSrvDomain;
    BYTE          rejCause;
    BYTE          networkIdValid;
    BYTE          MCC[PLMN_LENGTH];
    BYTE          MNC[PLMN_LENGTH];
    BYTE          hsCallStatusValid;
    BYTE          hsCallStatus;
    BYTE          hsIndValid;
    BYTE          hsInd;
    BYTE          pscValid;
    WORD          psc;
} WCDMASysInfo;

/**
 *  Structure for storing the LTE System Information.
 *
 *  \param  sysInfoLTE
 *          - See \ref sysInfoCommon for more information.
 *
 *  \param  lacValid
 *          - Indicates whether the location area code is valid..
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  lac
 *          - Location area code.
 *          - Only applies to 3GPP.
 *              - 0xFFFF - Not Available
 *
 *  \param  cellIdValid
 *          - Indicates whether the cell ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  cellId
 *          - Cell ID.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  regRejectInfoValid
 *          - Indicates whether the registration reject information is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  rejectSrvDomain
 *          - Type of service domain in which the registration is rejected.
 *              - 0x00 - SYS_SRV_DOMAIN_NO_SRV - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *              - 0x04 - Camped
 *              - 0xFF - Not Available
 *
 *  \param  rejCause
 *          - Reject cause values sent are specified in
 *            [3GPP TS 24.008, Section 10.5.3.6].
 *              - 0xFF - Not Available
 *
 *
 *  \param  networkIdValid
 *          - Indicates whether the network ID is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  MCC[PLMN_LENGTH]
 *          - Mobile Country Code.
 *          - MCC digits in ASCII characters
 *
 *  \param  MNC[PLMN_LENGTH]
 *          - Mobile Network Code.
 *          - MNC digits in ASCII characters
 *          - An unused byte is set to 0xFF.
 *          - In case of two-digit MNC values, the third (unused) digit is set to 0xFF.
 *            For example, 15 (a two-digit MNC) is reported using the byte stream
 *            0x31 0x35 0xFF.
 *
 *  \param  tacValid
 *          - Indicates whether tracking area code is valid.
 *              - 0x00 - Invalid
 *              - 0x01 - Valid
 *              - 0xFF - Not Available
 *
 *  \param  tac
 *          - Tracking area code.
 *          - Only applicable for LTE.
 *              - 0xFFFF - Not Available
 *
 */
typedef struct
{
    sysInfoCommon sysInfoLTE;
    BYTE          lacValid;
    WORD          lac;
    BYTE          cellIdValid;
    ULONG         cellId;
    BYTE          regRejectInfoValid;
    BYTE          rejectSrvDomain;
    BYTE          rejCause;
    BYTE          networkIdValid;
    BYTE          MCC[PLMN_LENGTH];
    BYTE          MNC[PLMN_LENGTH];
    BYTE          tacValid;
    WORD          tac;
} LTESysInfo;

/**
 *  Structure for storing the Additional CDMA System Information.
 *
 *  \param  geoSysIdx
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  regPrd
 *          - Registration period after the CDMA system is acquired.
 *          - When the CDMA registration period is not valid, 0xFFFF is used.
 *
 */
typedef struct
{
    WORD geoSysIdx;
    WORD regPrd;
} AddCDMASysInfo;

/**
 *  Structure for storing the Additional GSM and WCDMA System Information.
 *
 *  \param  geoSysIdx
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  cellBroadcastCap
 *          - Cell broadcast capability of the serving system.
 *          - When the CDMA registration period is not valid, 0xFFFF is used.
 *              - NAS_CELL_BROADCAST_CAP_UNKNOWN - Cell broadcast support is unknown
 *              - NAS_CELL_BROADCAST_CAP_OFF - Cell broadcast is not supported
 *              - NAS_CELL_BROADCAST_CAP_ON - Cell broadcast is supported
 *
 */
typedef struct
{
    WORD  geoSysIdx;
    ULONG cellBroadcastCap;
} AddSysInfo;

/**
 *  Structure for storing the GSM and WCDMA Call Barring System Information.
 *
 *  \param  csBarStatus
 *          - Call barring status for circuit-switched calls.
 *              - NAS_CELL_ACCESS_NORMAL_ONLY - Cell access is allowed for normal calls only
 *              - NAS_CELL_ACCESS_EMERGENCY_ONLY - Cell access is allowed for emergency calls only
 *              - NAS_CELL_ACCESS_NO_CALLS - Cell access is not allowed for any call type
 *              - NAS_CELL_ACCESS_ALL_CALLS - Cell access is allowed for all call types
 *              - NAS_CELL_ACCESS_UNKNOWN - Cell access type is unknown
 *
 *  \param  psBarStatus
 *          - Call barring status for packet-switched calls.
 *              - NAS_CELL_ACCESS_NORMAL_ONLY - Cell access is allowed for normal calls only
 *              - NAS_CELL_ACCESS_EMERGENCY_ONLY - Cell access is allowed for emergency calls only
 *              - NAS_CELL_ACCESS_NO_CALLS - Cell access is not allowed for any call type
 *              - NAS_CELL_ACCESS_ALL_CALLS - Cell access is allowed for all call types
 *              - NAS_CELL_ACCESS_UNKNOWN - Cell access type is unknown
 *
 */
typedef struct
{
    ULONG csBarStatus;
    ULONG psBarStatus;
} CallBarringSysInfo;

/**
 *  Structure for storing the SLQSNasGetSysInfo response parameters.
 *
 *  \param  pCDMASrvStatusInfo
 *          - See \ref SrvStatusInfo for more information.
 *
 *  \param  pHDRSrvStatusInfo
 *          - See \ref SrvStatusInfo for more information.
 *
 *  \param  pGSMSrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pWCDMASrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pLTESrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pCDMASysInfo
 *          - See \ref CDMASysInfo for more information.
 *
 *  \param  pHDRSysInfo
 *          - See \ref HDRSysInfo for more information.
 *
 *  \param  pGSMSysInfo
 *          - See \ref GSMSysInfo for more information.
 *
 *  \param  pWCDMASysInfo
 *          - See \ref WCDMASysInfo for more information.
 *
 *  \param  pLTESysInfo
 *          - See \ref LTESysInfo for more information.
 *
 *  \param  pAddCDMASysInfo
 *          - See \ref AddCDMASysInfo for more information.
 *
 *  \param  pAddHDRSysInfo
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  pAddGSMSysInfo
 *          - See \ref AddSysInfo for more information.
 *
 *  \param  pAddWCDMASysInfo
 *          - See \ref AddSysInfo for more information.
 *
 *  \param  pAddLTESysInfo
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  pGSMCallBarringSysInfo
 *          - See \ref CallBarringSysInfo for more information.
 *
 *  \param  pWCDMACallBarringSysInfo
 *          - See \ref CallBarringSysInfo for more information.
 *
 *  \param  pLTEVoiceSupportSysInfo
 *          - Indicates voice support status on LTE.
 *              - 0x00 - Voice is not supported
 *              - 0x01 - Voice is supported
 *
 *  \param  pGSMCipherDomainSysInfo
 *          - Ciphering on the service domain.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *
 *  \param  pWCDMACipherDomainSysInfo
 *          - Ciphering on the service domain.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *
 */
typedef struct
{
    SrvStatusInfo      *pCDMASrvStatusInfo;
    SrvStatusInfo      *pHDRSrvStatusInfo;
    GSMSrvStatusInfo   *pGSMSrvStatusInfo;
    GSMSrvStatusInfo   *pWCDMASrvStatusInfo;
    GSMSrvStatusInfo   *pLTESrvStatusInfo;
    CDMASysInfo        *pCDMASysInfo;
    HDRSysInfo         *pHDRSysInfo;
    GSMSysInfo         *pGSMSysInfo;
    WCDMASysInfo       *pWCDMASysInfo;
    LTESysInfo         *pLTESysInfo;
    AddCDMASysInfo     *pAddCDMASysInfo;
    WORD               *pAddHDRSysInfo;
    AddSysInfo         *pAddGSMSysInfo;
    AddSysInfo         *pAddWCDMASysInfo;
    WORD               *pAddLTESysInfo;
    CallBarringSysInfo *pGSMCallBarringSysInfo;
    CallBarringSysInfo *pWCDMACallBarringSysInfo;
    BYTE               *pLTEVoiceSupportSysInfo;
    BYTE               *pGSMCipherDomainSysInfo;
    BYTE               *pWCDMACipherDomainSysInfo;
} nasGetSysInfoResp;

/**
 *  Provides the system information. This API is preferred when trying to get
 *  the service status info and serving system info. The API SLQSGetServingSystem()
 *  reports similar NAS information, but it is deprecated. Please refer to the header
 *  description of API SLQSGetServingSystem() for more information.
 *
 *  \param  pGetSysInfoResp[OUT]
 *          - See \ref nasGetSysInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *          This API queries current serving system information, including
 *          registration information and system property. The registration
 *          information for all RATs specified in the mode capability setting
 *          are included regardless of registration status. The RAT-specific
 *          system property are included only for RATs that are specified in
 *          the mode capability setting and which are not in either No Service
 *          or Power Save modes.
 *
 */
ULONG SLQSNasGetSysInfo( nasGetSysInfoResp *pGetSysInfoResp );

/**
 *  Structure for storing the common information for the device.
 *
 *  \param  temperature
 *          - Temperature.
 *              - 8-bit signed integer
 *              - 0xFF - Not Available.
 *
 *  \param  modemMode
 *          - Modem Operating Mode.
 *              - 0x00 - POWERING OFF
 *              - 0x01 - FACTORY TEST
 *              - 0x02 - OFFLINE
 *              - 0x03 - OFFLINE_AMPS
 *              - 0x04 - OFFLINE_CDMA
 *              - 0x05 - ONLINE
 *              - 0x06 - LOW POWER MODE
 *              - 0x07 - RESETTING
 *              - 0x08 - NETWORK TEST
 *              - 0x09 - OFFLINE REQUEST
 *              - 0x0A - PSEUDO ONLINE
 *              - 0x0B - RESETTING MODEM
 *              - 0xFF - Unknown
 *
 *  \param  systemMode
 *          - System Acquisition Mode.
 *              - 0x00 - No service
 *              - 0x01 - AMPS
 *              - 0x02 - CDMA
 *              - 0x03 - GSM
 *              - 0x04 - HDR
 *              - 0x05 - WCDMA
 *              - 0x06 - GPS
 *              - 0x08 - WLAN
 *              - 0x09 - LTE
 *              - 0xFF - Unknown
 *
 *  \param  imsRegState
 *          - IMS Registration State.
 *              - 0x00 - NO SRV
 *              - 0x01 - IN PROG
 *              - 0x02 - FAILED
 *              - 0x03 - LIMITED
 *              - 0x04 - FULL SRV
 *              - 0xFF - Unknown
 *
 *  \param  psState
 *          - PS Attach State.
 *              - 0x00 - Attached
 *              - 0x01 - Detached
 *              - 0xFF - Unknown
 *
 */
typedef struct
{
    BYTE temperature;
    BYTE modemMode;
    BYTE systemMode;
    BYTE imsRegState;
    BYTE psState;
} CommInfo;

/**
 *  Structure for storing the LTE information for the device.
 *
 *  \param  band
 *          - LTE Band
 *              - 1 ~ 41 (Band in decimal)
 *              - 0xFF - Invalid
 *
 *
 *  \param  bandwidth
 *          - BandWidth.
 *              - 0x00 - 1.4 MHz
 *              - 0x01 - 3 MHz
 *              - 0x02 - 5 MHz
 *              - 0x03 - 10 MHz
 *              - 0x04 - 15 MHz
 *              - 0x05 - 20 MHz
 *              - 0x06 - Invalid
 *              - 0xFF - Unknown
 *
 *  \param  RXChan
 *          - RX channel number in decimal
 *              - 0xFFFF - Not Available
 *
 *  \param  TXChan
 *          - TX channel number in decimal
 *              - 0xFFFF - Not Available
 *
 *  \param  emmState
 *          - EMM State.
 *              - 0x00 - Deregistered
 *              - 0x01 - Reg Initiated
 *              - 0x02 - Registered
 *              - 0x03 - TAU Initiated
 *              - 0x04 - SR Initiated
 *              - 0x05 - Dereg Initiated
 *              - 0x06 - Invalid
 *              - 0xFF - Unknown
 *
 *  \param  emmSubState
 *          - EMM Sub State.
 *              - 0xFF - NOT Applicable
 *          - When EMM_state is 0x00:
 *              - 0x00 - No IMSI
 *              - 0x01 - PLMN Search
 *              - 0x02 - Attach Needed
 *              - 0x03 - No Cell
 *              - 0x04 - Attaching
 *              - 0x05 - Normal Service
 *              - 0x06 - Limited Service
 *              - 0x07 - Waiting for PDN
 *          - When EMM_state is 0x01:
 *              - 0x00 - Waiting for NW
 *              - 0x01 - Waiting for ESM
 *          - When EMM_state is 0x02:
 *              - 0x00 - Normal Service
 *              - 0x01 - Update Needed
 *              - 0x02 - Attempt Update
 *              - 0x03 - No Cell
 *              - 0x04 - PLMN Search
 *              - 0x05 - Limited Service
 *              - 0x06 - MM Update
 *              - 0x07 - IMSI Detach
 *              - 0x08 - Waiting for ESM
 *
 *  \param  emmConnState
 *          - EMM Connected Mode State.
 *              - 0x00 - RRC Idle
 *              - 0x01 - Waiting RRC Cfm
 *              - 0x02 - RRC Connected
 *              - 0x03 - RRC Releasing
 *              - 0xFF - Unknown
 *
 */
typedef struct
{
    BYTE band;
    BYTE bandwidth;
    WORD RXChan;
    WORD TXChan;
    BYTE emmState;
    BYTE emmSubState;
    BYTE emmConnState;
} LTEInfo;

/**
 *  Structure for storing the SLQS Nas Swi Modem Status response parameters.
 *
 *  \param  commonInfo (mandatory)
 *          - See \ref CommInfo for more information
 *
 *
 *  \param  pLTEInfo (optional)
 *          - See \ref LTEInfo for more information
 *
 */
typedef struct
{
    CommInfo commonInfo;
    LTEInfo  *pLTEInfo;
} swiModemStatusResp;

/**
 *  This API requests the device to return the current status of modem.
 *
 *  \param  pModemStatusResp[OUT]
 *          - See \ref swiModemStatusResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSNasSwiModemStatus( swiModemStatusResp *pModemStatusResp );

/**
 *  Structure for storing the current preferred system selection settings
 *  for the device.
 *
 *  \param  pColorCode [Optional]
 *          - Color code value
 *          - Color code corresponding to the sector to which the AT is
 *            sending the access probe
 *          - See 3GPP2 C.S0024-B V3.0, Section 7.11.6.2.1 for more information.
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE *pColorCode;
} nasGetHDRColorCodeResp;

/**
 *  This API retrieves the current HDR color code value.
 *
 *  \param  pGetHDRColorCodeResp[OUT]
 *          - See \ref nasGetHDRColorCodeResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSNasGetHDRColorCode( nasGetHDRColorCodeResp *pGetHDRColorCodeResp );

/**
 * This structure contains the GetTxRxInfoReq request parameters
 *
 *  \param  radio_if [Mandatory]
 *          - Radio interface technology of the signal being measured
 *          - Valid Values
 *              - 0x01 - NAS_RADIO_IF_CDMA_1X - CDMA
 *              - 0x02 - NAS_RADIO_IF_CDMA_1XEVDO - HDR
 *              - 0x04 - NAS_RADIO_IF_GSM - GSM
 *              - 0x05 - NAS_RADIO_IF_UMTS - UMTS
 *              - 0x08 - NAS_RADIO_IF_LTE - LTE
 */
typedef struct
{
    BYTE radio_if;
} nasGetTxRxInfoReq;

/**
 * This structure contains the Rx Information.
 *
 *  \param  isRadioTuned
 *          - Whether Rx is tuned to a channel:
 *            - 0x00 - Not tuned
 *            - 0x01 - Tuned
 *            - 0xFF - Not Available
 *          - If the radio is tuned, instantaneous values are set for the
 *            signal information fields below.
 *          - If the radio is not tuned, or is delayed or invalid, the values
 *            are set depending on each technology.
 *
 *  \param  rx_pwr
 *          - Rx power value in 1/10 dbm resolution.
 *
 *  \param  ecio
 *          - ECIO in 1/10 dbm; valid for CDMA, HDR, GSM, WCDMA, and LTE.
 *
 *  \param  rscp
 *          - Received signal code power in 1/10 dbm.
 *          - Valid for WCDMA.
 *
 *  \param  rsrp
 *          - Current reference signal received power in 1/10 dbm valid for LTE.
 *
 *  \param  phase
 *          - Phase in 1/100 degrees; valid for LTE.
 *          - When the phase is unknown, 0xFFFFFFFF is used.
 *
 */
typedef struct
{
    BYTE  isRadioTuned;
    INT32 rxPower;
    INT32 ecio;
    INT32 rscp;
    INT32 rsrp;
    ULONG phase;
} rxInfo;

/**
 * This structure contains the Tx Information.
 *
 *  \param  isInTraffic
 *          - Whether the device is in traffic.
 *            - 0x00 - not in traffic
 *            - 0x01 - in traffic
 *          - The txPower field is only meaningful when in the device is in
 *            traffic.
 *
 *  \param  txPower
 *          - Tx power value in 1/10 dbm.
 *
 */
typedef struct
{
    BYTE  isInTraffic;
    INT32 txPower;
} txInfo;

/**
 *  This structure contains the GetTxRxInfoResp response parameters.
 *
 *  \param  pRXChain0Info [Optional]
 *          - See \ref rxInfo for more information.
 *
 *  \param  pRXChain1Info [Optional]
 *          - See \ref rxInfo for more information.
 *
 *  \param  pTXInfo [Optional]
 *          - See \ref txInfo for more information.
 *
 */
typedef struct
{
    rxInfo *pRXChain0Info;
    rxInfo *pRXChain1Info;
    txInfo *pTXInfo;
} nasGetTxRxInfoResp;

/**
 * This API retrieves the detailed Tx/Rx information.
 *
 *  \param  pGetTxRxInfoReq[IN]
 *          - See \ref nasGetTxRxInfoReq for more information
 *
 *  \param  pGetTxRxInfoResp[OUT]
 *          - See \ref nasGetTxRxInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *          This command retrieves Tx/Rx information for a radio interface.
 *          The Rx chain are included in the response message only if they are
 *          enabled.
 *
 */
ULONG SLQSNasGetTxRxInfo(
    nasGetTxRxInfoReq  *pGetTxRxInfoReq,
    nasGetTxRxInfoResp *pGetTxRxInfoResp );

/**
 * This structure contains the parameters for CDMA/WCDMA Signal Strength
 * Information
 *
 *  \param  rssi
 *          - RSSI in dBm (signed value).
 *          - A value of -125 dBm or lower is used to indicate No Signal.
 *
 *  \param  ecio
 *          - ECIO value representing negative 0.5 dBm increments, i.e.,
 *            2 means -1 dBm (14 means -7 dBm, 63 means -31.5 dBm).
 *
 */
typedef struct
{
    INT8  rssi;
    SHORT ecio;
} CDMASSInfo;

/**
 * This structure contains the parameters for HDR Signal Strength Information
 *
 *  \param  rssi
 *          - RSSI in dBm (signed value).
 *          - A value of -125 dBm or lower is used to indicate No Signal.
 *
 *  \param  ecio
 *          - ECIO value representing negative 0.5 dBm increments, i.e.,
 *            2 means -1 dBm (14 means -7 dBm, 63 means -31.5 dBm).
 *
 *  \param  sinr
 *          - SINR level.
 *          - SINR is only applicable for 1xEV-DO.
 *          - Valid levels are 0 to 8, where the maximum value for:
 *              - 0 - SINR_LEVEL_0 is -9 dB
 *              - 1 - SINR_LEVEL_1 is -6 dB
 *              - 2 - SINR_LEVEL_2 is -4.5 dB
 *              - 3 - SINR_LEVEL_3 is -3 dB
 *              - 4 - SINR_LEVEL_4 is -2 dB
 *              - 5 - SINR_LEVEL_5 is +1 dB
 *              - 6 - SINR_LEVEL_6 is +3 dB
 *              - 7 - SINR_LEVEL_7 is +6 dB
 *              - 8 - SINR_LEVEL_8 is +9 dB
 *              - 0xFF - Not Available
 *
 *  \param  io
 *          - Received IO in dBm.
 *          - IO is only applicable for 1xEV-DO.
 *
 */
typedef struct
{
    INT8  rssi;
    SHORT ecio;
    BYTE  sinr;
    INT32 io;
} HDRSSInfo;

/**
 * This structure contains the parameters for LTE Signal Strength Information
 *
 *  \param  rssi
 *          - RSSI in dBm (signed value).
 *          - A value of -125 dBm or lower is used to indicate No Signal.
 *            - For CDMA and UMTS, this indicates forward link
 *              pilot Ec
 *            - For GSM, this indicates received signal strength
 *
 *  \param  rsrq
 *          - RSRQ value in dB (signed integer value) as measured by L1.
 *          - Range: -3 to -20 (-3 means -3 dB, -20 means -20 dB).
 *
 *  \param  rsrp
 *          - Current RSRP in dBm as measured by L1.
 *          - Range: -44 to -140 (-44 means -44 dBm, -140 means -140 dBm).
 *
 *  \param  snr
 *          - SNR level as a scaled integer in units of 0.1 dB. e.g., -16
 *            dB has a value of -160 and 24.6 dB has a value of 246,
 *
 */
typedef struct
{
    INT8  rssi;
    INT8  rsrq;
    SHORT rsrp;
    SHORT snr;
} LTESSInfo;

/**
 *  This structure contains the TDSCDMA Signal Strength Info Extended
 *
 *  \param  rssi
 *          - Measured RSSI in dB
 *
 *  \param  rscp[Optional]
 *          - Measured RSCP in dBm
 *
 *  \param  ecio[Optional]
 *          - Measured ECIO in dBm.
 *
 *  \param  sinr[Optional]
 *          - Measured SINR in dB. -15 dB is sent to clients if the
 *            actual SINR is less than -15 dB
 *
 */
typedef struct
{
    FLOAT rssi;
    FLOAT rscp;
    FLOAT ecio;
    FLOAT sinr;
} TDSCDMASigInfoExt;

/**
 *  This structure contains the SLQSNasGetSigInfo response parameters.
 *
 *  \param  pCDMASSInfo [Optional]
 *          - See \ref CDMASSInfo for more information
 *
 *  \param  pHDRSSInfo [Optional]
 *          - See \ref HDRSSInfo for more information
 *
 *  \param  pGSMSSInfo [Optional]
 *          - GSM signal strength is the RSSI in dBm (signed value).
 *          - A value of -125 dBm or lower is used to indicate No Signal.
 *
 *  \param  pWCDMASSInfo [Optional]
 *          - See \ref CDMASSInfo for more information
 *
 *  \param  pLTESSInfo [Optional]
 *          - See \ref LTESSInfo for more information
 *
 *  \param  pTDSCDMASigInfoRscp [Optional]
 *          - RSCP of the Primary Common Control Physical Channel (PCCPCH) in dBm. 
 *          - Measurement range: -120 dBm to -25 dBm.
 *
 *  \param  pTDSCDMASigInfoExt [Optional]
 *          - See \ref TDSCDMASigInfoExt for more information.
 */
typedef struct
{
    CDMASSInfo *pCDMASSInfo;
    HDRSSInfo  *pHDRSSInfo;
    INT8       *pGSMSSInfo;
    CDMASSInfo *pWCDMASSInfo;
    LTESSInfo  *pLTESSInfo;
    INT8       *pTDSCDMASigInfoRscp;
    TDSCDMASigInfoExt *pTDSCDMASigInfoExt;
} nasGetSigInfoResp;

/**
 *  This API queries information regarding the signal strength.
 *
 *  \param  pGetSigInfoResp[OUT]
 *          - See \ref nasGetSigInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *          This command queries the signal strength information for currently
 *          active RATs. Information is reported only if the corresponding
 *          RATs have signal strength values to be reported.
 *          If no signal strength information is available for any RAT, the
 *          response message contains only the mandatory response message
 *
 */
ULONG SLQSNasGetSigInfo( nasGetSigInfoResp *pGetSigInfoResp );

/**
 *  This structure contains the SLQSNasIndicationRegisterExt request
 *  parameters.
 *
 *  \param  pSystemSelectionInd [Optional]
 *          - System Selection Preference indication registration. The following
 *            callbacks would not be invoked if the indication is disabled.\n
 *            \ref tFNRoamingIndicator \ref tFNDataCapabilities and
 *            \ref tFNServingSystem
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pDDTMInd [Optional]
 *          - DDTM (Data Dedicated Transmission Mode) indication registration.
 *            The following callbacks would not be invoked if the indication
 *            is disabled.\n
 *            \ref tFNDDTM
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pServingSystemInd [Optional]
 *          - Serving System indication registration. The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNBandPreference
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pDualStandByPrefInd [Optional]
 *          - Dual Standby Preference indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNDualStandByPref
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pSubscriptionInfoInd [Optional]
 *          - Subscription Information indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNSubscriptionInfo
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pNetworkTimeInd [Optional]
 *          - Network Time indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNNetworkTime
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pSysInfoInd [Optional]
 *          - System Information indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNSysInfo
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pSignalStrengthInd [Optional]
 *          - Signal Strength indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNSigInfo
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pErrorRateInd [Optional]
 *          - Error Rate indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNErrRate
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pHDRNewUATIAssInd [Optional]
 *          - HDR New UATI Assigned indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNHDRUATIUpdate
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pHDRSessionCloseInd [Optional]
 *          - HDR Session Closed indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNHDRSessionClose
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pManagedRoamingInd [Optional]
 *          - Managed Roaming indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *            \ref tFNManagedRoaming
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pNetworkRejectInd [Optional]
 *          - Network reject information indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pSuppressSysInfoInd [Optional]
 *          - Controls the reporting of QMI_NAS_SYS_INFO_IND when only the
 *            reject_cause field has changed. Values:\n
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pLTECphyCa [Optional]
 *          - LTE Physical Carrier Aggregation Information indication registration.The following callbacks
 *            would not be invoked if the indication is disabled.\n
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \note Atleast one parameter must be provided as request. 'NULL' value
 *        confirms that the indication value is not sent.
 */
typedef struct
{
    BYTE *pSystemSelectionInd;
    BYTE *pDDTMInd;
    BYTE *pServingSystemInd;
    BYTE *pDualStandByPrefInd;
    BYTE *pSubscriptionInfoInd;
    BYTE *pNetworkTimeInd;
    BYTE *pSysInfoInd;
    BYTE *pSignalStrengthInd;
    BYTE *pErrorRateInd;
    BYTE *pHDRNewUATIAssInd;
    BYTE *pHDRSessionCloseInd;
    BYTE *pManagedRoamingInd;
    BYTE *pNetworkRejectInd;
    BYTE *pSuppressSysInfoInd;
    BYTE *pLTECphyCa;
} nasIndicationRegisterReq;

/**
 *  This API Registers/De-registers for different NAS (Network access service)
 *  indications.
 *
 *  \param  pIndicationRegisterReq[IN]
 *          - See \ref nasIndicationRegisterReq for more information
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   Timeout: 2 seconds\n
 *          This API is used by a control point to register/deregister for
 *          different QMI_NAS indications. The control point's registration
 *          state variables, controlling registration for indications, are
 *          modified to reflect the settings indicated in the parameters that
 *          are present in the request message. At least one optional parameter
 *          must be present in the request.
 *
 */
ULONG SLQSNasIndicationRegisterExt(
    nasIndicationRegisterReq *pIndicationRegisterReq );

/**
 *  Structure for storing the PLMN Name request parameters
 *
 *  \param  mcc
 *          - A 16-bit integer representation of MCC. Range: 0 to 999
 *
 *  \param  mnc
 *          - A 16-bit integer representation of MNC. Range: 0 to 999
 *
 *  \param  pMncPcsStatus
 *          - MNC PCS Digit Include Status
 *          - Used to interpret the length of the corresponding MNC reported
 *            in the PLMN TLV(0x01).
 *          - Values
 *            - TRUE -  MNC is a three-digit value. e.g. a reported value of 90
 *                      corresponds to an MNC value of 090
 *            - FALSE - MNC is a two-digit value. e.g. a reported value of 90
 *                      corresponds to an MNC value of 90
 *
 *  \note    If pMncPcsStatus is not present, an MNC smaller than 100
 *           is assumed to be a two-digit value, and an MNC greater than
 *           or equal to 100 is assumed to be a three digit value.
 *
 */
typedef struct
{
    WORD mcc;
    WORD mnc;
    BYTE *pMncPcsStatus;
} nasPLMNNameReq;

/**
 *  Structure for storing the PLMN Name response parameters
 *
 *  \param  spnEncoding
 *          - Coding scheme used for service provider name. This value is
 *            ignored if spn_len is zero
 *            Values:
 *              - 0x00 - SMS default 7-bit coded alphabet as defined in
 *                3GPP TS 23.038 with bit 8 set to 0
 *              - 0x01 - UCS2 (16 bit, little-endian) 3GPP TS 23.038
 *
 *  \param  spnLength
 *          - Length of SPN which follows
 *
 *  \param  spn
 *          - Service Provider name string
 *
 *  \param  shortNameEn
 *          - Coding scheme used for PLMN short name. This value is
 *            ignored if PLMN short name length is zero
 *            Values:
 *              - 0x00 - SMS default 7-bit coded alphabet as defined in
 *                3GPP TS 23.038 with bit 8 set to 0
 *              - 0x01 - UCS2 (16 bit, little-endian) 3GPP TS 23.038
 *
 *  \param  shortNameCI
 *          - Indicates whether the country initials are to be added to the
 *            shortName. This value is ignored if shortNameLen is zero. Values:
 *              - 0x00 - Do not add the letters for the countrys initials
 *                to the name
 *              - 0x01 - Add the countrys initials and a text string to
 *                the name
 *              - 0xFF - Not specified
 *
 *  \param  shortNameSB
 *          - PLMN short name spare bits. This value is ignored if
 *            shortNameLen is zero. Values:
 *              - 0x01 - Bit 8 is spare and set to 0 in octet
 *              - 0x02 - Bits 7 and 8 are spare and set to 0 in octet n
 *              - 0x03 - Bits 6 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x04 - Bits 5 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x05 - Bits 4 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x06 - Bits 3 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x07 - Bits 2 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x00 - Carries no information about the number of spare
 *                bits in octet n
 *
 *  \param  shortNameLen
 *          - Length of shortName which follows
 *
 *  \param  shortName
 *          - PLMN short name
 *
 *  \param  longNameEn
 *          - Coding scheme used for PLMN long name. This value is
 *            ignored if PLMN long name length is zero
 *            Values:
 *              - 0x00 - SMS default 7-bit coded alphabet as defined in
 *                3GPP TS 23.038 with bit 8 set to 0
 *              - 0x01 - UCS2 (16 bit, little-endian) 3GPP TS 23.038
 *
 *  \param  longNameCI
 *          - Indicates whether the country initials are to be added to the
 *            longName. This value is ignored if longNameLen is zero. Values:
 *              - 0x00 - Do not add the letters for the countrys initials
 *                to the name
 *              - 0x01 - Add the countrys initials and a text string to
 *                the name
 *              - 0xFF - Not specified
 *
 *
 *  \param  longNameSB
 *          - PLMN long name spare bits. This value is ignored if
 *            longNameLen is zero. Values:
 *              - 0x01 - Bit 8 is spare and set to 0 in octet
 *              - 0x02 - Bits 7 and 8 are spare and set to 0 in octet n
 *              - 0x03 - Bits 6 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x04 - Bits 5 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x05 - Bits 4 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x06 - Bits 3 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x07 - Bits 2 to 8 (inclusive) are spare and set to 0 in
 *                octet n
 *              - 0x00 - Carries no information about the number of spare
 *                bits in octet n
 *
 *  \param  longNameLen
 *          - Length of longName which follows
 *
 *  \param  longName
 *          - PLMN long name
 *
 *  \note    None
 *
 */
typedef struct
{
    BYTE spnEncoding;
    BYTE spnLength;
    BYTE spn[255];
    BYTE shortNameEn;
    BYTE shortNameCI;
    BYTE shortNameSB;
    BYTE shortNameLen;
    BYTE shortName[255];
    BYTE longNameEn;
    BYTE longNameCI;
    BYTE longNameSB;
    BYTE longNameLen;
    BYTE longName[255];
} nasPLMNNameResp;

/**
 *  Get the operator name data from the network.
 *
 *  \param  pPLMNNameReq[IN]
 *          - See \ref nasPLMNNameReq for more information
 *
 *  \param  pPLMNNameResp[OUT]
 *          - See \ref nasPLMNNameResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 10 seconds
 *
 */
ULONG SLQSGetPLMNName(
    nasPLMNNameReq  *pPLMNNameReq,
    nasPLMNNameResp *pPLMNNameResp );

/**
 *  This structure contains Operator PLMN Data from multiple sources.
 *
 *  \param  mcc
 *          - MCC in ASCII string (a value of D in any of the digits is to
 *            be used to indicate a "wild" value for that corresponding
 *            digit).
 *
 *  \param  mnc
 *          - MNC in ASCII string (a value of D in any of the digits is to
 *            be used to indicate a "wild" value for that corresponding
 *            digit; digit 3 in MNC is optional and when not present, will
 *            be set as ASCII F).
 *
 *  \param  lac1
 *          - Location area code 1.
 *
 *  \param  lac2
 *          - Location area code 1.
 *
 *  \param  PLMNRecID
 *          - PLMN network name record identifier.
 *
 */
typedef struct
{
    BYTE mcc[PLMN_LENGTH];
    BYTE mnc[PLMN_LENGTH];
    WORD lac1;
    WORD lac2;
    BYTE PLMNRecID;
} OperatorPLMNData;

/**
 *  This structure contains Operator PLMN List as defined in 3GPP TS 31.102
 *  (Section 4.2.59) from multiple sources.
 *
 *  \param  numInstance
 *          - Number of sets of the elements.
 *
 *  \param  PLMNData
 *          - Refer OperatorPLMNLData for details (Optional).
 */
typedef struct
{
    WORD numInstance;
    OperatorPLMNData PLMNData[MAX_DESCRIPTION_LENGTH];
} operatorPLMNList;

/**
 *  This structure contains Service Provider Name as defined in 3GPP TS 31.102
 *  (Section 4.2.12) from multiple sources.
 *
 *  \param  displayCondition
 *          - Display condition.
 *
 *  \param  spnLength
 *          - It provides length of spn.
 *
 *  \param  spn
 *          - Service provider name string must use:
 *            The SMS default 7-bit coded alphabet as defined in 3GPP TS 23.038
 *            with bit 8 set to 9.
 */
typedef struct
{
    BYTE displayCondition;
    BYTE spnLength;
    BYTE spn[MAX_DESCRIPTION_LENGTH];
} serviceProviderName;

/**
 *  This structure contains PLMN Network Name Data from multiple sources.
 *
 *  \param  codingScheme
 *          - Coding scheme:
 *              - 0 - CODING_SCHEME_CELL_BROADCAST_GSM -
 *                  Cell broadcast data coding scheme, GSM default alphabet,
 *                  language unspecified;defined in 3GPP TS 23.038.
 *              - 1 - CODING_SCHEME_UCS2 - UCS2 (16 bit);defined in ISO/IEC 10646
 *
 *  \param  countryInitials
 *          - Country's initials:
 *              - 0 - COUNTRY_INITIALS_DO_NOT_ADD - MS
 *                  should not add the letters for the country's initials to the
 *                  text string.
 *              - 1 - COUNTRY_INITIALS_ADD - MS should add the
 *                  letters for the country's initials and a separator, e.g., a
 *                  space, to the text string.
 *
 *  \param  longNameSpareBits
 *          - Long Name Spare Bits:
 *              - 1 - SPARE_BITS_8 - Bit 8 is spare and set to 0 in octet n
 *              - 2 - SPARE_BITS_7_TO_8 - Bits 7 and 8 are spare and
 *                  set to 0 in octet n.
 *              - 3 - SPARE_BITS_6_TO_8 - Bits 6 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 4 - SPARE_BITS_5_TO_8 - Bits 5 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 5 - SPARE_BITS_4_TO_8 - Bits 4 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 6 - SPARE_BITS_3_TO_8 - Bits 3 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 7 - SPARE_BITS_2_TO_8 - Bits 2 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 0 - SPARE_BITS_UNKNOWN - Carries no information
 *                  about the number of spare bits in octet n.
 *
 *  \param  shortNameSpareBits
 *          - Short Name Spare Bits:
 *              - 1 - SPARE_BITS_8 - Bit 8 is spare and set to 0 in octet n.
 *              - 2 - SPARE_BITS_7_TO_8 - Bits 7 and 8 are spare and
 *                  set to 0 in octet n.
 *              - 3 - SPARE_BITS_6_TO_8 - Bits 6 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 4 - SPARE_BITS_5_TO_8 - Bits 5 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 5 - SPARE_BITS_4_TO_8 - Bits 4 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 6 - SPARE_BITS_3_TO_8 - Bits 3 to 8 (inclusive) are
 *                  spare and set to 0 in octet n.
 *              - 7 - SPARE_BITS_2_TO_8 - Bits 2 to 8 (inclusive) are spare and set
 *                  to 0 in octet n.
 *              - 0 - SPARE_BITS_UNKNOWN - Carries no information
 *                  about the number of spare bits in octet n.
 *
 *  \param  longNameLen
 *          - It provides the length of long name.
 *
 *  \param  longName
 *          - Long name string in coding_scheme.
 *
 *  \param  shortNameLen
 *          - It provides the length of short name.
 *
 *  \param  shortName
 *          - Short name string in coding_scheme.
 */
typedef struct
{
    BYTE codingScheme;
    BYTE countryInitials;
    BYTE longNameSpareBits;
    BYTE shortNameSpareBits;
    BYTE longNameLen;
    BYTE longName[MAX_DESCRIPTION_LENGTH];
    BYTE shortNameLen;
    BYTE shortName[MAX_DESCRIPTION_LENGTH];
} PLMNNetworkNameData;

/**
 *  This structure contains PLMN Network Name as defined in 3GPP TS 24.008
 *  (Section 10.5.3.5a) from multiple sources.
 *
 *  \param  numInstance
 *          - Number of sets of the elements.
 *
 *  \param  PLMNNetName
 *          - Refer PLMNNetworkNameData for details (Optional).
 */
typedef struct
{
    BYTE numInstance;
    PLMNNetworkNameData PLMNNetName[MAX_DESCRIPTION_LENGTH];
} PLMNNetworkName;

/**
 *  This structure contains Operator Name String as defined in
 *  CPHS4_2.WW6(Feb 27, 1997) (Section B.4.1.2)from multiple sources.
 *
 *  \param  PLMNName
 *          - PLMN name must be coded in a default 7-bit alphabet with b8
 *            set to 0.
 */
typedef struct
{
    BYTE PLMNName[MAX_DESCRIPTION_LENGTH];
} operatorNameString;

/**
 *  This structure contains Operator Name Data related from multiple sources.
 *
 *  \param  pSrvcProviderName
 *          - Refer serviceProviderName for details (Optional).
 *          - Can provide NULL if this parameter is not required.
 *
 *  \param  pOperatorPLMNList
 *          - Refer operatorPLMNList for details (Optional).
 *          - Can provide NULL if this parameter is not required.
 *
 *  \param  pPLMNNetworkName
 *          - Refer PLMNNetworkName for details (Optional).
 *          - Can provide NULL if this parameter is not required.
 *
 *  \param  pOperatorNameString
 *          - Refer operatorNameString for details (Optional).
 *          - Can provide NULL if this parameter is not required.
 *
 *  \param  pNITZInformation
 *          - Refer PLMNNetworkNameData for details (Optional).
 *          - Can provide NULL if this parameter is not required.
 */
typedef struct
{
    serviceProviderName *pSrvcProviderName;
    operatorPLMNList    *pOperatorPLMNList;
    PLMNNetworkName     *pPLMNNetworkName;
    operatorNameString  *pOperatorNameString;
    PLMNNetworkNameData *pNITZInformation;
} nasOperatorNameResp;

/**
 *  Get the operator name data from the network. This API is deprecated
 *  on MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use API SLQSGetPLMNName()
 *  for new firmware versions and new modules
 *
 *  \param  pOperatorNameData[OUT]
 *          - See \ref nasOperatorNameResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 10 seconds
 *
 */
ULONG SLQSGetOperatorNameData( nasOperatorNameResp *pOperatorNameData );

/**
 * This structure contains the Get3GPP2SubscriptionInfo request parameters
 *
 *  \param  namID [Mandatory]
 *          - NAM ID of the information to be retrieved. The index
 *            starts from 0. A nam_id of 0xFF is used to retrieve
 *            information of current NAM.
 */
typedef struct
{
    BYTE namID;
} nasGet3GPP2SubscriptionInfoReq;

/**
 * This structure contains the parameters for NAM Name Information
 *
 *  \param  namNameLen
 *          - Number of sets of the following elements:
 *              - nam_name
 *          - If zero(0), then no information follows.
 *
 *  \param  namName
 *          - Name information in ASCII. The maximum length of
 *            nam_name is 12.
 *
 */
typedef struct
{
    BYTE namNameLen;
    BYTE namName[NAM_NAME_LENGTH];
} namName;

/**
 * This structure contains the parameters for Directory Number Information
 *
 *  \param  dirNumLen
 *          - Number of sets of the following elements:
 *              - dir_num
 *          - If zero(0), then no information follows.
 *
 *  \param  dirNum
 *          - Directory number in ASCII characters.
 *
 */
typedef struct
{
    BYTE dirNumLen;
    BYTE dirNum [MAX_DESCRIPTION_LENGTH];
} dirNum;

/**
 * This structure contains the parameters for SidNid Information
 *
 *  \param  nid
 *          - Network ID
 *
 *  \param  sid
 *          - System ID
 *
 */
typedef struct
{
    WORD nid;
    WORD sid;
} sidNid;

/**
 * This structure contains the parameters for Home SID/NID Information
 *
 *  \param  numInstances
 *          - Number of sets of the following elements:
 *              - sid
 *              - nid
 *          - If zero(0), then no information follows.
 *
 *  \param  SidNid
 *          - See \ref sidNid for more information
 *
 */
typedef struct
{
    BYTE   numInstances;
    sidNid SidNid[MAX_DESCRIPTION_LENGTH];
} homeSIDNID;

/**
 * This structure contains the parameters for Min based IMSI Information
 *
 *  \param  mccM
 *          - ASCII character representation of MCC_M
 *
 *  \param  imsiM1112
 *          - ASCII character representation of IMSI_M_11_12 value
 *              - 0xFFFF - Not Available
 *
 *  \param  imsiMS1
 *          - ASCII character representation of IMSI_M_S1 value
 *
 *  \param  imsiMS2
 *          - ASCII character representation of IMSI_M_S2 value
 *
 *
 */
typedef struct
{
    BYTE mccM[PLMN_LENGTH];
    WORD imsiM1112;
    BYTE imsiMS1[IMSI_M_S1_LENGTH];
    BYTE imsiMS2[IMSI_M_S2_LENGTH];
} minBasedIMSI;

/**
 * This structure contains the parameters for True IMSI Information
 *
 *  \param  mccT
 *          - ASCII character representation of MCC_T
 *
 *  \param  imsiT1112
 *          - ASCII character representation of IMSI_T_11_12 value
 *              - 0xFFFF - Not Available
 *
 *  \param  imsiTS1
 *          - ASCII character representation of IMSI_T_S1 value
 *
 *  \param  imsiTS2
 *          - ASCII character representation of IMSI_T_S2 value
 *
 *  \param  imsiTaddrNum
 *          - Value of IMSI_T_ADDR_NUM
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE mccT[PLMN_LENGTH];
    WORD imsiT1112;
    BYTE imsiTS1[IMSI_M_S1_LENGTH];
    BYTE imsiTS2[IMSI_M_S2_LENGTH];
    BYTE imsiTaddrNum;
} trueIMSI;

/**
 * This structure contains the parameters for CDMA Channel Information
 *
 *  \param  priChA
 *          - A Channel number for the primary carrier.
 *              - 0xFFFF - Not Available
 *
 *  \param  priChB
 *          - B Channel number for the primary carrier.
 *              - 0xFFFF - Not Available
 *
 *  \param  secChA
 *          - A Channel number for the secondary carrier.
 *              - 0xFFFF - Not Available
 *
 *  \param  secChB
 *          - B Channel number for the secondary carrier.
 *              - 0xFFFF - Not Available
 *
 */
typedef struct
{
    WORD priChA;
    WORD priChB;
    WORD secChA;
    WORD secChB;
} CDMAChannel;

/**
 *  This structure contains the SLQSNasGet3GPP2Subscription response
 *  parameters.
 *
 *  \param  pNAMNameInfo [Optional]
 *          - See \ref namName for more information
 *
 *  \param  pDirNum [Optional]
 *          - See \ref dirNum for more information
 *
 *  \param  pHomeSIDNID [Optional]
 *          - See \ref homeSIDNID for more information
 *
 *  \param  pMinBasedIMSI [Optional]
 *          - See \ref minBasedIMSI for more information
 *
 *  \param  pTrueIMSI [Optional]
 *          - See \ref trueIMSI for more information
 *
 *  \param  pCDMAChannel [Optional]
 *          - See \ref CDMAChannel for more information
 *
 */
typedef struct
{
    namName      *pNAMNameInfo;
    dirNum       *pDirNum;
    homeSIDNID   *pHomeSIDNID;
    minBasedIMSI *pMinBasedIMSI;
    trueIMSI     *pTrueIMSI;
    CDMAChannel  *pCDMAChannel;
} nasGet3GPP2SubscriptionInfoResp;

/**
 *  This API retrieves 3GPP2 subscription-related information.
 *
 *  \param  pGet3GPP2SubsInfoReq[IN]
 *          - See \ref nasGet3GPP2SubscriptionInfoReq for more information
 *
 *  \param  pGet3GPP2SubsInfoResp[OUT]
 *          - See \ref nasGet3GPP2SubscriptionInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds\n
 *          This command retrieves 3GPP2 subscription-related information.
 *          The QMI_ERR_INTERNAL error is returned when no information can
 *          be retrieved from the modem.
 *
 */
ULONG SLQSNasGet3GPP2Subscription (
    nasGet3GPP2SubscriptionInfoReq  *pGet3GPP2SubsInfoReq,
    nasGet3GPP2SubscriptionInfoResp *pGet3GPP2SubsInfoResp);

/**
 *  This structure contains information about the Network Measurement Report
 *  (NMR) Cell Information.
 *
 *  \param  nmrCellID
 *          - Cell ID.
 *          - 0xFFFFFFFF indicates cell ID information is not present.
 *
 *  \param  nmrPlmn[PLMN_LENGTH]
 *          - MCC/MNC information coded as octet 3, 4, and 5.
 *          - This field is ignored when nmrCellID is not present.
 *
 *  \param  nmrLac
 *          - Location area code.
 *          - This field is ignored when nmrCellID is not present.
 *              - 0xFFFF - Not Available
 *
 *  \param  nmrArfcn
 *          - Absolute RF channel number.
 *              - 0xFFFF - Not Available
 *
 *  \param  nmrBsic
 *          - Base station identity code.
 *              - 0xFF - Not Available
 *
 *  \param  nmrRxLev
 *          - Cell Rx measurement.
 *          - Values range between 0 and 63.
 *          - Mapped to a measured signal level:
 *              - Rxlev 0 is a signal strength less than -110 dBm
 *              - Rxlev 1 is -110 dBm to -109 dBm
 *              - Rxlev 2 is -109 dBm to -108 dBm
 *              - ...
 *              - Rxlev 62 is -49 dBm to -48 dBm
 *              - Rxlev 63 is greater than -48 dBm
 *              - 0xFFFF - Not Available
 *
 */
typedef struct
{
    ULONG nmrCellID;
    BYTE  nmrPlmn[PLMN_LENGTH];
    WORD  nmrLac;
    WORD  nmrArfcn;
    BYTE  nmrBsic;
    WORD  nmrRxLev;
} nmrCellInfo;

/**
 *  This structure contains information about the GERAN Network.
 *
 *  \param  cellID
 *          - Cell ID.
 *          - 0xFFFFFFFF indicates cell ID information is not present.
 *
 *  \param  plmn[PLMN_LENGTH]
 *          - MCC/MNC information coded as octet 3, 4, and 5.
 *          - This field is ignored when nmrCellID is not present.
 *
 *  \param  lac
 *          - Location area code.
 *          - This field is ignored when nmrCellID is not present.
 *              - 0xFFFF - Not Available
 *
 *  \param  arfcn
 *          - Absolute RF channel number.
 *              - 0xFFFF - Not Available
 *
 *  \param  bsic
 *          - Base station identity code.
 *              - 0xFF - Not Available
 *
 *  \param  timingAdvance
 *          - Measured delay (in bit periods; 1 bit period = 48/13 microsecond)
 *            of access burst transmission on RACH or PRACH to the expected
 *            signal from an MS at zero distance under static channel
 *            conditions.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  rxLev
 *          - Serving Cell Rx measurement.
 *          - Values range between 0 and 63.
 *          - Mapped to a measured signal level:
 *              - Rxlev 0 is a signal strength less than -110 dBm
 *              - Rxlev 1 is -110 dBm to -109 dBm
 *              - Rxlev 2 is -109 dBm to -108 dBm
 *              - ...
 *              - Rxlev 62 is -49 dBm to -48 dBm
 *              - Rxlev 63 is greater than -48 dBm
 *              - 0xFFFF - Not Available
 *
 *  \param  nmrInst
 *          - Provides the number of set of instances which follow.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  insNmrCellInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref nmrCellInfo for more information.
 *
 */
typedef struct
{
    ULONG       cellID;
    BYTE        plmn[PLMN_LENGTH];
    WORD        lac;
    WORD        arfcn;
    BYTE        bsic;
    ULONG       timingAdvance;
    WORD        rxLev;
    BYTE        nmrInst;
    nmrCellInfo insNmrCellInfo[MAX_DESCRIPTION_LENGTH];
} GERANInfo;

/**
 *  This structure contains information about the GERAN Instances in UMTS
 *  Network.
 *
 *  \param  geranArfcn
 *          - Absolute RF channel number.
 *
 *  \param  geranBsicNcc
 *          - Base station identity code network color code.
 *          - 0xFF indicates information is not present.
 *
 *  \param  geranBsicBcc
 *          - Base station identity code base station color code.
 *          - 0xFF indicates information is not present.
 *
 *  \param  geranRssi
 *          - Received signal strength indicator.
 *
 */
typedef struct
{
    WORD geranArfcn;
    BYTE geranBsicNcc;
    BYTE geranBsicBcc;
    SHORT geranRssi;
} geranInstInfo;

/**
 *  This structure contains information about the UMTS Instances in UMTS
 *  Network.
 *
 *  \param  umtsUarfcn
 *          - UTRA absolute RF channel number.
 *
 *  \param  umtsPsc
 *          - Primary scrambling code.
 *
 *  \param  umtsRscp
 *          - Received signal code power.
 *
 *  \param  umtsEcio
 *          - ECIO(Signal-to-Interference-ratio).
 *
 */
typedef struct
{
    WORD umtsUarfcn;
    WORD umtsPsc;
    SHORT umtsRscp;
    SHORT umtsEcio;
} UMTSinstInfo;

/**
 *  This structure contains information about the UMTS Network.
 *
 *  \param  cellID
 *          - Cell ID.
 *          - 0xFFFFFFFF indicates cell ID information is not present.
 *
 *  \param  plmn[PLMN_LENGTH]
 *          - MCC/MNC information coded as octet 3, 4, and 5.
 *          - This field is ignored when nmrCellID is not present.
 *
 *  \param  lac
 *          - Location area code.
 *          - This field is ignored when nmrCellID is not present.
 *              - 0xFFFF - Not Available
 *
 *  \param  uarfcn
 *          - UTRA absolute RF channel number.
 *              - 0xFFFF - Not Available
 *
 *  \param  psc
 *          - Primary scrambling code.
 *              - 0xFFFF - Not Available
 *
 *  \param  rscp
 *          - Received signal code power.
 *              - 0xFFFF - Not Available
 *
 *  \param  ecio
 *          - ECIO(Signal-to-Interference-ratio).
 *              - 0xFFFF - Not Available
 *
 *  \param  umtsInst
 *          - Provides the number of set of UMTS info instances.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  UMTSInstInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref UMTSinstInfo for more information.
 *
 *  \param  geranInst
 *          - Provides the number of set of GERAN info instances.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  GeranInstInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref geranInstInfo for more information.
 *
 */
typedef struct
{
    WORD          cellID;
    BYTE          plmn[PLMN_LENGTH];
    WORD          lac;
    WORD          uarfcn;
    WORD          psc;
    SHORT         rscp;
    SHORT         ecio;
    BYTE          umtsInst;
    UMTSinstInfo  UMTSInstInfo[MAX_DESCRIPTION_LENGTH];
    BYTE          geranInst;
    geranInstInfo GeranInstInfo[MAX_DESCRIPTION_LENGTH];
} UMTSInfo;

/**
 *  This structure contains information about the CDMA Network.
 *
 *  \param  sid
 *          - System ID.
 *              - 0xFFFF - Not Available
 *
 *  \param  nid
 *          - Network ID.
 *              - 0xFFFF - Not Available
 *
 *  \param  baseId
 *          - Base station ID.
 *              - 0xFFFF - Not Available
 *
 *  \param  refpn
 *          - Reference PN.
 *              - 0xFFFF - Not Available
 *
 *  \param  baseLat
 *          - Latitude of the current base station in units of 0.25 sec.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  baseLong
 *          - Longitude of the current base station in units of 0.25 sec.
 *              - 0xFFFFFFFF - Not Available
 *
 */
typedef struct
{
    WORD  sid;
    WORD  nid;
    WORD  baseId;
    WORD  refpn;
    ULONG baseLat;
    ULONG baseLong;
} CDMAInfo;

/**
 *  This structure contains information about the Cell parameters.
 *
 *  \param  pci
 *          - Physical cell ID.
 *          - Range: 0 to 503.
 *
 *  \param  rsrq
 *          - Current RSRQ in 1/10 dB as measured by L1.
 *          - Range: -20.0 dB to -3.0 dB.
 *
 *  \param  rsrp
 *          - Current RSRP in 1/10 dBm as measured by L1.
 *          - Range: -140.0 dBm to -44.0 dBm.
 *
 *  \param  rssi
 *          - Current RSSI in 1/10 dBm as measured by L1.
 *          - Range: -120.0 dBm to 0.
 *
 *  \param  srxlev
 *          - Cell selection Rx level (Srxlev) value.
 *          - Range: -128 to 128.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 */
typedef struct
{
    WORD pci;
    SHORT rsrq;
    SHORT rsrp;
    SHORT rssi;
    SHORT srxlev;
} cellParams;

/**
 *  This structure contains information about the LTE Intra-Frequency Network.
 *
 *  \param  ueInIdle
 *          - TRUE if the UE is in Idle mode, otherwise FALSE.
 *              - 0xFF - Not Available
 *
 *  \param  plmn[PLMN_LENGTH]
 *          - PLMN ID coded as octet 3, 4, and 5.
 *
 *  \param  tac
 *          - Tracking area code.
 *              - 0xFFFF - Not Available
 *
 *  \param  globalCellId
 *          - Global cell ID in the system information block.
 *              - 0xFFFFFFFF - Not Available
 *
 *  \param  earfcn
 *          - E-UTRA absolute radio frequency channel number of the serving
 *            cell.
 *          - Range: 0 to 65535.
 *              - 0xFFFF - Not Available
 *
 *  \param  servingCellId
 *          - LTE serving cell ID.
 *          - Range: 0 to 503.
 *          - This is the cell ID of the serving cell and can be found in the
 *            cell list.
 *              - 0xFFFF - Not Available
 *
 *  \param  cellReselPriority
 *          - Priority for serving frequency.
 *          - Range: 0 to 7.
 *          - This field is only valid when ue_in_idle is TRUE.
 *              - 0xFF - Not Available
 *
 *  \param  sNonIntraSearch
 *          - S non-intra search threshold to control non-intrafrequency
 *            searches.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *              - 0xFF - Not Available
 *
 *  \param  threshServingLow
 *          - Serving cell low threshold.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *              - 0xFF - Not Available
 *
 *  \param  sIntraSearch
 *          - S Intra search threshold.
 *          - Range: 0 to 31.
 *          - The current cell measurement must fall below this threshold to
 *            consider intrafrequency for reselection.
 *          - This field is only valid when ue_in_idle is TRUE.
 *              - 0xFF - Not Available
 *
 *  \param  cellsLen
 *          - Provides the number of set of cell params.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  CellParams[MAX_DESCRIPTION_LENGTH]
 *          - See \ref cellParams for more information.
 *
 */
typedef struct
{
    BYTE       ueInIdle;
    BYTE       plmn[PLMN_LENGTH];
    WORD       tac;
    ULONG      globalCellId;
    WORD       earfcn;
    WORD       servingCellId;
    BYTE       cellReselPriority;
    BYTE       sNonIntraSearch;
    BYTE       threshServingLow;
    BYTE       sIntraSearch;
    BYTE       cellsLen;
    cellParams CellParams[MAX_DESCRIPTION_LENGTH];
} LTEInfoIntrafreq;

/**
 *  This structure contains information about the inter-frequency.
 *
 *  \param  earfcn
 *          - E-UTRA absolute radio frequency channel number of the serving
 *            cell.
 *          - Range: 0 to 65535.
 *
 *  \param  threshXLow
 *          - Cell Srxlev low threshold.
 *          - Range: 0 to 31.
 *          - When the serving cell does not exceed thresh_serving_low, the
 *            value of an evaluated cell must be smaller than this value to be
 *            considered for re-selection.
 *
 *  \param  threshXHigh
 *          - Cell Srxlev high threshold.
 *          - Range: 0 to 31.
 *          - When the serving cell exceeds thresh_serving_low, the value of an
 *            evaluated cell must be greater than this value to be considered
 *            for re-selection.
 *
 *  \param  cell_resel_priority
 *          - Cell re-selection priority
 *          - Range: 0 to 7.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  cells_len
 *          - Provides the number of set of cell params.
 *
 *  \param  cellInterFreqParams[MAX_DESCRIPTION_LENGTH]
 *          - See \ref cellParams for more information.
 *
 */
typedef struct
{
    WORD       earfcn;
    BYTE       threshXLow;
    BYTE       threshXHigh;
    BYTE       cell_resel_priority;
    BYTE       cells_len;
    cellParams cellInterFreqParams[MAX_DESCRIPTION_LENGTH];
} infoInterFreq;

/**
 *  This structure contains information about the LTE Inter-Frequency Network.
 *
 *  \param  ueInIdle
 *          - TRUE if the UE is in Idle mode, otherwise FALSE.
 *              - 0xFF - Not Available
 *
 *  \param  freqsLen
 *          - Provides the number of set of inter frequency information.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  InfoInterfreq[MAX_DESCRIPTION_LENGTH]
 *          - See \ref infoInterFreq for more information.
 *
 */
typedef struct
{
    BYTE          ueInIdle;
    BYTE          freqsLen;
    infoInterFreq InfoInterfreq[MAX_DESCRIPTION_LENGTH];
} LTEInfoInterfreq;

/**
 *  This structure contains information about the GSM Cell.
 *
 *  \param  arfcn
 *          - GSM frequency being reported.
 *          - Range: 0 to 1023.
 *
 *  \param  band1900
 *          - Band indicator for the GSM ARFCN
 *          - This field is only valid if arfcn is in the overlapping region.
 *          - If TRUE and the cell is in the overlapping region, the ARFCN is
 *            on the 1900 band.
 *          - If FALSE, it is on the 1800 band.
 *
 *  \param  cellIdValid
 *          - Flag indicating whether the base station identity code ID is
 *            valid.
 *
 *  \param  bsicId
 *          - Base station identity code ID, including base station color code
 *            and network color code.
 *          - The lower 6 bits can be set to any value.
 *
 *  \param  rssi
 *          - Measured RSSI value in 1/10 dB.
 *          - Range: -200.0 dB to 0
 *
 *  \param  srxlev
 *          - Cell selection Rx level (Srxlev) value.
 *          - Range: -128 to 128.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 */
typedef struct
{
     WORD arfcn;
     BYTE band1900;
     BYTE cellIdValid;
     BYTE bsicId;
     SHORT rssi;
     SHORT  srxlev;
} gsmCellInfo;

/**
 *  This structure contains information about the LTE GSM Cell.
 *
 *  \param  cellReselPriority
 *          - Priority of this frequency group.
 *          - Range: 0 to 7.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  threshGsmHigh
 *          - Reselection threshold for high priority layers.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  threshGsmLow
 *          - Reselection threshold for low priority layers.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  nccPermitted
 *          - Bitmask specifying whether a neighbor with a specific network
 *            color code is to be reported.
 *          - Range: 0 to 255.
 *          - Bit n set to 1 means a neighbor with NCC n must be included in
 *            the report. This flag is synonymous with a blacklist in other
 *            RATs.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  cells_len
 *          - Provides the number of set of gsm cells.
 *
 *  \param  GsmCellInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref gsmCellInfo for more information.
 *
 */
typedef struct
{
     BYTE        cellReselPriority;
     BYTE        threshGsmHigh;
     BYTE        threshGsmLow;
     BYTE        nccPermitted;
     BYTE        cells_len;
     gsmCellInfo GsmCellInfo[MAX_DESCRIPTION_LENGTH];
} lteGsmCellInfo;

/**
 *  This structure contains information about the LTE Neighboring GSM Network.
 *
 *  \param  ueInIdle
 *          - TRUE if the UE is in Idle mode, otherwise FALSE.
 *              - 0xFF - Not Available
 *
 *  \param  freqsLen
 *          - Provides the number of set of LTE GSM cell information.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  LteGsmCellInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref lteGsmCellInfo for more information.
 *
 */
typedef struct
{
    BYTE           ueInIdle;
    BYTE           freqsLen;
    lteGsmCellInfo LteGsmCellInfo[MAX_DESCRIPTION_LENGTH];
} LTEInfoNeighboringGSM;

/**
 *  This structure contains information about the WCDMA Cell.
 *
 *  \param  psc
 *          - Primary scrambling code.
 *          - Range: 0 to 511.
 *
 *  \param  cpich_rscp
 *          - Absolute power level (in 1/10 dBm) of the common pilot channel
 *            as received by the UE.
 *          - Range: -120.0 dBm to -25.0 dBm
 *
 *  \param  cpich_ecno
 *          - CPICH Ec/No; ratio (in 1/10 dB) of the received energy per PN
 *            chip for the CPICH to the total received power spectral density
 *            at the UE antenna connector.
 *          - Range: -50.0 dB to 0.
 *
 *  \param  srxlev
 *          - Cell selection Rx level (Srxlev) value.
 *          - Range: -128 to 128.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 */
typedef struct
{
    WORD psc;
    SHORT cpich_rscp;
    SHORT cpich_ecno;
    SHORT srxlev;
} wcdmaCellInfo;

/**
 *  This structure contains information about the LTE WCDMA Cell.
 *
 *  \param  uarfcn
 *          - WCDMA layer frequency.
 *          - Range: 0 to 16383.
 *
 *  \param  cellReselPriority
 *          - Cell re-selection priority.
 *          - Range: 0 to 7.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  threshXhigh
 *          - Re-selection low threshold.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  threshXlow
 *          - Re-selection high threshold.
 *          - Range: 0 to 31.
 *          - This field is only valid when ue_in_idle is TRUE.
 *
 *  \param  cellsLen
 *          - Provides the number of set of WCDMA cells.
 *
 *  \param  WCDMACellInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref wcdmaCellInfo for more information.
 *
 */
typedef struct
{
    WORD          uarfcn;
    BYTE          cellReselPriority;
    WORD          threshXhigh;
    WORD          threshXlow;
    BYTE          cellsLen;
    wcdmaCellInfo WCDMACellInfo[MAX_DESCRIPTION_LENGTH];
} lteWcdmaCellInfo;

/**
 *  This structure contains information about the LTE Neighboring WCDMA Network.
 *
 *  \param  ueInIdle
 *          - TRUE if the UE is in Idle mode, otherwise FALSE.
 *              - 0xFF - Not Available
 *
 *  \param  freqsLen
 *          - Provides the number of set of LTE WCDMA cell information.
 *          - If 0(zero), then no information follows it.
 *
 *  \param  LTEWCDMACellInfo[MAX_DESCRIPTION_LENGTH]
 *          - See \ref lteWcdmaCellInfo for more information.
 *
 */
typedef struct
{
    BYTE             ueInIdle;
    BYTE             freqsLen;
    lteWcdmaCellInfo LTEWCDMACellInfo[MAX_DESCRIPTION_LENGTH];
} LTEInfoNeighboringWCDMA;

/**
 *  This structure contains information about the UMTS LTE neighbour Cell.
 *
 *  \param  earfcn
 *          - E-UTRA absolute RF channel number of the detected cell.
 *
 *  \param  pci
 *          - Physical cell ID of the detected cell.
 *          - Range is defined in 3GPP TS 36.211
 *
 *  \param  rsrp
 *          - Current received signal strength indication (in dBm)
 *            of the detected cell.
 *
 *  \param  rsrq
 *          - Current reference signal received quality (in dB) of
 *            the detected cell.
 *
 *  \param  srxlev
 *          - Cell selection Rx level (Srxlev) value of the detected
 *            cell in linear scale.
 *          - This field is only valid when wcdma_rrc_state is not
 *            NAS_WCDMA_RRC_STATE_CEL_FACH or NAS_WCDMA_RRC_STATE_CELL_DCH.
 *
 *  \param  cellIsTDD
 *          - TRUE if the cell is TDD; FALSE if the cell is FDD.
 *
 */
typedef struct
{
    WORD  earfcn;
    WORD  pci;
    ULONG rsrp;
    ULONG rsrq;
    SHORT srxlev;
    BYTE  cellIsTDD;
} umtsLTENbrCell;

/**
 *  This structure contains information about the WCDMA - LTE Neighboring
 *  Cell Info Set.
 *
 *  \param  wcdmaRRCState
 *          - WCDMA RRC states.
 *          - Defined in 3GPP TS 25.331
 *          - Values:
 *              - 0x00 - NAS_WCDMA_RRC_STATE_DISCONNECTED
 *                         - WCDMA RRC State is IDLE
 *              - 0x01 - NAS_WCDMA_RRC_STATE_CELL_PCH
 *                         - WCDMA RRC state is CELL_PCH
 *              - 0x02 - NAS_WCDMA_RRC_STATE_URA_PCH
 *                         - WCDMA RRC state is URA_PCH
 *              - 0x03 - NAS_WCDMA_RRC_STATE_CELL_FACH
 *                         - WCDMA RRC state is CELL_FACH
 *              - 0x04 - NAS_WCDMA_RRC_STATE_CELL_DCH
 *                         - WCDMA RRC state is CELL_DCH
 *
 *  \param  umtsLTENbrCellLen
 *          - Number of sets of UMTS LTE Neighbors.
 *
 *  \param  UMTSLTENbrCell
 *          - See \ref umtsLTENbrCell for more information.
 *
 */
typedef struct
{
    ULONG          wcdmaRRCState;
    BYTE           umtsLTENbrCellLen;
    umtsLTENbrCell UMTSLTENbrCell[MAX_DESCRIPTION_LENGTH];
} WCDMAInfoLTENeighborCell;

/**
 *  This structure contains information about the Get Cell Location response
 *  parameters.
 *
 *  \param  pGERANInfo
 *          - See \ref GERANInfo for more information.
 *
 *  \param  pUMTSInfo
 *          - See \ref UMTSInfo for more information.
 *
 *  \param  pCDMAInfo
 *          - See \ref CDMAInfo for more information.
 *
 *  \param  pLTEInfoIntrafreq
 *          - See \ref LTEInfoIntrafreq for more information.
 *
 *  \param  pLTEInfoInterfreq
 *          - See \ref LTEInfoInterfreq for more information.
 *
 *  \param  pLTEInfoNeighboringGSM
 *          - See \ref LTEInfoNeighboringGSM for more information.
 *
 *  \param  pLTEInfoNeighboringWCDMA
 *          - See \ref LTEInfoNeighboringWCDMA for more information.
 *
 *  \param  pUMTSCellID
 *          - Cell ID.
 *          - 0xFFFFFFFF indicates cell ID information is not present.
 *
 *  \param  pWCDMAInfoLTENeighborCell
 *          - See \ref WCDMAInfoLTENeighborCell for more information.
 */
typedef struct
{
    GERANInfo                *pGERANInfo;
    UMTSInfo                 *pUMTSInfo;
    CDMAInfo                 *pCDMAInfo;
    LTEInfoIntrafreq         *pLTEInfoIntrafreq;
    LTEInfoInterfreq         *pLTEInfoInterfreq;
    LTEInfoNeighboringGSM    *pLTEInfoNeighboringGSM;
    LTEInfoNeighboringWCDMA  *pLTEInfoNeighboringWCDMA;
    ULONG                    *pUMTSCellID;
    WCDMAInfoLTENeighborCell *pWCDMAInfoLTENeighborCell;
} nasCellLocationInfoResp;

/**
 *  This API retrieves cell location-related information
 *
 *  \param  pNasCellLocationInfoResp[OUT]
 *          - See \ref nasCellLocationInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *          This API retrieves cell location-related information, depending on
 *          current serving system.
 *
 */
ULONG SLQSNasGetCellLocationInfo(
    nasCellLocationInfoResp *pNasCellLocationInfoResp );

/**
 *  Structure contains Manual Network Register Information parameters
 *
 *  \param  mcc
 *          - A 16-bit integer representation of Mobile Country Code.
 *            Range - 0 to 999.
 *
 *  \param  mnc
 *          - A 16-bit integer representation of Mobile Network Code.
 *            Range - 0 to 999.
 *
 *  \param  rat
 *          - Radio access technology for which to register.
 *              - 0x04 - RADIO_IF_GSM
 *              - 0x05 - RADIO_IF_UMTS
 *              - 0x08 - RADIO_IF_LTE
 */
typedef struct
{
    WORD  mcc;
    WORD  mnc;
    ULONG rat;
} MNRInfo;

/**
 *  This structure contains Initiate Network Registration request parameters
 *
 *  \param  regAction
 *          - Specifies one of the following register actions :
 *            - AUTO_REGISTER - Device registers according to its provisioning
 *                  and optional parameters supplied with the command are ignored.
 *            - MANUAL_REGISTER - Device registers to a specified network and
 *                  the optional Manual Network Register Information parameter
 *                  pMNRInfo must also be included for the command to process
 *                  successfully and supported only for 3GPP.
 *
 *  \param  pMNRInfo [Optional]
 *          - Pointer to structure MNRInfo
 *              - See \ref MNRInfo for more information
 *
 *  \param  pChangeDuration [Optional]
 *          - Duration of the change.
 *              - 0x00 - Power cycle - Remains active until the next device power
 *                       cycle
 *              - 0x01 - Permanent - Remains active through power cycles until
 *                       changed by the client
 *
 * \param  pMncPcsDigitStatus [Optional]
 *         - MNC PCS Digit Include Status
 *             - True - MNC is a 3-digit value.
 *             - False - MNC is a 2-digit value.
 */
typedef struct
{
    ULONG   regAction;
    MNRInfo *pMNRInfo;
    ULONG   *pChangeDuration;
    BYTE    *pMncPcsDigitStatus;
} nasInitNetworkReg;

/**
 *  Initiates the network registration process.
 *
 *  \param  pNasInitNetRegistrationReq[IN]
 *          - Pointer to structure nasInitNetworkReq
 *              - See \ref nasInitNetworkReg for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Timeout: 30 seconds\n
 *
 */
ULONG SLQSInitiateNetworkRegistration(
    nasInitNetworkReg *pNasInitNetRegistrationReg);

/**
 *  This structure contains Protocol Subtype Elements for Protocol Subtype List
 *
 *  \param  PhysicalLayer
 *          - Specifies Physical Layer Protocol subtype
 *
 *  \param  ControlMac
 *          - Specifies Control Channel MAC Protocol subtype
 *
 *  \param  AccessMac
 *          - Specifies Access Channel MAC Protocol subtype
 *
 *  \param  ForwardMac
 *          - Specifies Forward Traffic Channel MAC Protocol subtype
 *
 *  \param  ReverseMac
 *          - Specifies Reverse Traffic Channel MAC Protocol subtype
 *
 *  \param  KeyExchange
 *          - Specifies Key exchange Protocol subtype
 *
 *  \param  AuthProt
 *          - Specifies Authentication Protocol subtype
 *
 *  \param  EncryptProt
 *          - Specifies Encryption Protocol subtype
 *
 *  \param  SecProt
 *          - Specifies Security Protocol subtype
 *
 *  \param  IdleState
 *          - Specifies Idle state Protocol subtype
 *
 *  \param  MultDisc
 *          - Specifies Generic multimode capability discovery Protocol subtype
 *
 *  \param  VirtStream
 *          - Specifies Generic Virtual Stream Protocol subtype
 *
 */
typedef struct
{
    WORD PhysicalLayer;
    WORD ControlMac;
    WORD AccessMac;
    WORD ForwardMac;
    WORD ReverseMac;
    WORD KeyExchange;
    WORD AuthProt;
    WORD EncryptProt;
    WORD SecProt;
    WORD IdleState;
    WORD MultDisc;
    WORD VirtStream;
} protocolSubtypeElement;

/**
 *  This structure contains information about the SLQSSwiGetHDRPersonality response
 *  parameters.
 *
 *  \param  pCurrentPersonality[Out]
 *          - Current active personality index.
 *
 *  \param  pPersonalityListLength[In/Out]
 *          - Number of Personality Protocol Subtype contains in this response.
 *          - maximum input value is 3
 *
 *  \param  pProtocolSubtypeElement[Out]
 *          - See \ref protocolSubtypeElement for more information.
 *
 */
typedef struct
{
    WORD                   *pCurrentPersonality;
    BYTE                   *pPersonalityListLength;
    protocolSubtypeElement *pProtocolSubtypeElement;
} HDRPersonalityResp;

/**
 *  This API retrieves HDR Personality related information
 *
 *  \param  pHDRPersonalityResp[OUT]
 *          - See \ref HDRPersonalityResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 */
ULONG SLQSSwiGetHDRPersonality ( HDRPersonalityResp *pHDRPersonalityResp );

/**
 *  This structure contains information about the SLQSSwiGetHDRProtSubtype response
 *  parameters.
 *
 *  \param  pCurrentPersonality[Out]
 *          - Current active personality index.
 *
 *  \param  pPersonalityListLength[In/Out]
 *          - Number of Personality Protocol Subtype contains in this response.
 *          - maximum input value is 4
 *
 *  \param  pProtocolSubtypeElement[Out]
 *          - See \ref protocolSubtypeElement for more information.
 *
 *  \param  pAppSubType[Out]
 *          - Stream application subtype
 *          - Application subtype for each stream,
 *
 */
typedef struct
{
    WORD                   *pCurrentPrsnlty;
    BYTE                   *pPersonalityListLength;
    protocolSubtypeElement *pProtoSubTypElmnt;
    ULONGLONG              *pAppSubType;
}HDRProtSubtypResp;

/**
 *  This API retrieves HDR Prototype Subtype related information
 *
 *  \param  pHDRProtSubtypResp[OUT]
 *          - See \ref HDRProtSubtypResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 */
ULONG SLQSSwiGetHDRProtSubtype ( HDRProtSubtypResp *pHDRProtSubtypResp );

/**
 *  This structure contains information about the SLQSSwiPSDetach request
 *  parameters.
 *
 *  \param  pDetachAction[IN]
 *          - Values
 *              - 2- Initiates an immediate packet domain detach.
 *
 */
typedef struct
{
    BYTE *pDetachAction;
}PSDetachReq;

/**
 *  This API detaches PS connection.
 *
 *  \param  pPSDetachReq[IN]
 *          - See \ref PSDetachReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 5 seconds\n
 */
ULONG SLQSSwiPSDetach ( PSDetachReq *pPSDetachReq );

/**
 *  This structure contains information about the SLQSGetErrorRate response
 *  parameters.
 *
 *  \param  pCDMAFrameErrRate[Out]
 *          - CDMA Frame Error Rate
 *          - Valid error rate values between 1 and 10000 are returned to
 *            indicate the percentage, e.g., a value of 300 means the error
 *            rate is 3%.
 *          - A value of 0xFFFF indicates that the error rate is
 *            unknown/unavailable.
 *
 *  \param  pHDRPackErrRate[Out]
 *          - HDR Packet Error Rate
 *          - Valid error rate values between 1 and 10000 are returned to
 *            indicate the percentage, e.g., a value of 300 means the error
 *            rate is 3%.
 *          - A value of 0xFFFF indicates that the error rate is
 *            unknown/unavailable.
 *
 *  \param  pGSMBER[Out]
 *          - GSM Bit Error Rate
 *          - Valid error rate values between 1 and 100 are returned to
 *            indicate the percentage value.
 *          - A 0% block error rate (BLER) indicates No Data.
 *
 *  \param  pWCDMABER[Out]
 *          - WCDMA Block Error Rate
 *          - Valid error rate values between 1 and 100 are returned to
 *            indicate the percentage value.
 *          - A value of 0xFF indicates that the error rate is
 *            unknown/unavailable.
 *
 */
typedef struct
{
    WORD *pCDMAFrameErrRate;
    WORD *pHDRPackErrRate;
    BYTE *pGSMBER;
    BYTE *pWCDMABER;
}GetErrRateResp;

/**
 *  This API retrieves current error rate information
 *
 *  \param  pGetErrRateResp[OUT]
 *          - See \ref GetErrRateResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSGetErrorRate ( GetErrRateResp *pGetErrRateResp );

/**
 *  This structure contains Data Rate Channel parameters
 *
 *  \param  DRCValue
 *          - Current Data Rate Channel
 *
 *  \param  DRCCover
 *          - Current Data Rate Channel cover
 *
 */
typedef struct
{
    BYTE DRCValue;
    BYTE DRCCover;
} DRCParams;

/**
 *  This structure contains Pilot Set parameters
 *
 *  \param  PilotType
 *          - 0x00 - NAS_HRPD_PILOT_CURR_ACT_PLT
 *                   Current Active Pilot
 *          - 0x01 - NAS_HRPD_PILOT_NEIGHBOR_PLT
 *                   Neighbor pilot information
 *
 *  \param  PilotPN
 *          - Pilot PN sequence offset index
 *
 *  \param  PilotStrength
 *          - Strength of the pilot (in dB)
 *
 */
typedef struct
{
    ULONG PilotType;
    WORD  PilotPN;
    WORD  PilotStrength;
} PilotSetParams;

/**
 *  This structure contains Pilot Set Data
 *
 *  \param  NumPilots(IN/OUT)
 *          - Number of Pilot Sets
 *          - As input specifies number of sets of parameter pPilotSetInfo
 *            for which memory has been assigned
 *          - As output specifies the actual number of sets of parameter
 *            pPilotSetInfo returned by device
 *
 *  \param  pPilotSetInfo
 *          - Pilot Set Parameters
 *          - See \ref PilotSetParams for more information.
 *
 *  note   A buffer under sized error is returned if the number of sets of
 *         pPilotSetInfo returned by the device is greater than the value in
 *         NumPilots input parameter.
 */
typedef struct
{
    BYTE           NumPilots;
    PilotSetParams *pPilotSetInfo;
} PilotSetData;

/**
 *  This structure contains information about the SLQSSwiGetHRPDStats response
 *  parameters.
 *
 *  \param  pDRCParams[Out]
 *          - See \ref DRCParams for more information.
 *
 *  \param  pUATI[Out]
 *          - A 128-bit address that includes the access terminal
 *            identifier and subnet ID
 *          - Size must be 16 bytes
 *
 *  \param  pPilotSetData[Out]
 *          - See \ref PilotSetData for more information.
 *
 */
typedef struct
{
    DRCParams    *pDRCParams;
    BYTE         *pUATI;
    PilotSetData *pPilotSetData;
}GetHRPDStatsResp;

/**
 *  This API retrieves currently acquired HRPD system statistics
 *
 *  \param  pGetHRPDStatsResp[OUT]
 *          - See \ref GetHRPDStatsResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSSwiGetHRPDStats ( GetHRPDStatsResp *pGetHRPDStatsResp );

/**
 *  This structure describes Active Pilot PN elements
 *
 *  \param  ActSetPilotPN
 *          - Active Pilot PN
 *
 *  \param  ActSetPilotPNStrength
 *          - Active Pilot PN strength
 */
typedef struct
{
    WORD ActSetPilotPN;
    BYTE ActSetPilotPNStrength;
} ActPilotPNElement;

/**
 *  This structure contains 1x network status details
 *
 *  \param  State
 *          - CDMA current access state
 *              - 0x01 - Searching
 *              - 0x02 - Idle
 *              - 0x03 - Traffic
 *              - Others - NA
 *
 *  \param  SO
 *          - CDMA service option
 *              - 0xFFFF - Not in a call
 *              - 0x0001 - Basic Variable Rate Voice Service(8kbps)
 *              - 0x0002 - Mobile Station Loopback(8kbps)
 *              - 0x0003 - Enhanced Variable Rate Codec(EVRC) Voice
 *                         Service(8kbps)
 *              - 0x0006 - Short message Services(Rate Set 1)
 *              - 0x0009 - Mobile Station Loopback(13kbps)
 *              - 0x000E - Short Message Service (Rate Set 2)
 *              - 0x0011 - High Rate Voice Service(13kbps)
 *              - 0x0020 - Test Data Service Option(TDSO)
 *              - 0x0021 - cdma2000 High Speed Packet Data Service,
 *                         Internet or ISO Protocol Stack
 *              - 0x0044 - EVRC-B Voice Service(8 kbps)
 *              - 0x0046 - EVRC-WB Voice Service(8 kbps)
 *              - 0x0049 - Voice Echo mode supports smart blanking(EVRC-NW)
 *              - 0x004B - Enhanced loopback
 *              - 0x8000 - Proprietary Service Option (Qualcomm Inc.)
 *
 *  \param  RX_PWR
 *          - RX Pwr(dBm)
 *              - 0xABCD00EF - -ABCD.EF dBm
 *              - ABCD00EF should be transferred to decimal while displaying
 *              - Example: 0x12340056 - -4660.86dBm\n
 *                 0x1234 = 4660, 0x0056 = 86
 *              - 0xFFFFFFFF - NA
 *
 *  \param  RX_EC_IO
 *          - RX EC/IO(dB)
 *              - 0xABCD - -AB.CD dB
 *              - ABCD should be transferred to decimal while displaying
 *              - Example: 0x1234 - -18.52dB \n
 *                 0x12 = 18, 0x34 = 52
 *              - 0xFFFF - NA
 *
 *  \param  TX_PWR
 *          - TX PWR(dBm)
 *              - 0xFFFFFFFF - NA
 *              - Others - display actual value in decimal
 *              - Example: 0x1234 - -4660dBm \n
 *                 0x1234 = 4660
 *
 *  \param  ActSetCnt( IN/OUT )
 *          - Count of active pilot PN elements
 *          - As input specifies number of sets of parameter pActPilotElements
 *            for which memory has been assigned
 *          - As output specifies the actual number of sets of parameter
 *            pActPilotElements returned by device
 *
 *  \param  pActPilotPNElements
 *          - See \ref ActPilotPNElement for more information
 *
 *  \param  NeighborSetCnt( IN/OUT )
 *          - Count of neighbor pilot PN elements
 *          - As input specifies number of sets of parameter pNeighborSetPilotPN
 *            for which memory has been assigned
 *          - As output specifies the actual number of sets of parameter
 *            pNeighborSetPilotPN returned by device
 *
 *  \param  pNeighborSetPilotPN
 *          - Neighbor pilot PN
 */
typedef struct
{
    BYTE              State;
    WORD              SO;
    ULONG             RX_PWR;
    WORD              RX_EC_IO;
    ULONG             TX_PWR;
    BYTE              ActSetCnt;
    ActPilotPNElement *pActPilotPNElements;
    BYTE              NeighborSetCnt;
    WORD              *pNeighborSetPilotPN;
} NetworkStat1x;

/**
 *  This structure contains EVDO network status details
 *
 *  \param  State
 *          - EVDO network access state
 *              - 0x00 - Sleep
 *              - 0x01 - Searching
 *              - 0x02 - Idle
 *              - 0x03 - Active
 *              - 0xFF - NA
 *
 *  \param  MACIndex
 *          - HDR Mac index
 *              - 0xFF - NA
 *              - Others - Display the actual value in decimal
 *              - Example: 0x12 - 18 \n
 *                 0x12 = 18
 *
 *  \param  SectorIDLen (IN/OUT)
 *          - Sector ID length
 *          - As input specifies length of parameter pSectorID
 *            for which memory has been assigned
 *          - As output specifies the actual length of parameter
 *            pSectorID returned by device
 *
 *  \param  pSectorID
 *          - Sector ID
 *
 *  \param  RX_PWR
 *          - TX PWR(dBm)
 *              - 0xABCD - -ABCD dBm
 *              - ABCD should be transferred to decimal while displaying
 *              - Example: 0x1234 - -4660dBm \n
 *                 0x1234 = 4660
 *              - 0xFFFF - NA
 *
 *  \param  PER
 *          - HDR Packet Error Rate
 *              - 0xFFFF - Unknown
 *              - Others - display the actual value in decimal
 *              - Example: 0x1234 - -4660dBm \n
 *                 0x1234 = 4660
 *
 *  \param  PilotEnergy
 *          - Pilt Energy (dB)
 *              - 0xFFFF - NA
 *              - 0xABCD should be transferred to decimal while displaying
 *              - Example: 0x1234 - -4660dBm \n
 *                 0x1234 = 4660
 *
 *  \param  SNR
 *          - Signal to Noise ratio (dB)
 */
typedef struct
{
    BYTE State;
    BYTE MACIndex;
    BYTE SectorIDLen;
    WORD *pSectorID;
    WORD RX_PWR;
    WORD PER;
    WORD PilotEnergy;
    BYTE SNR;
} NetworkStatEVDO;

/**
 *  This structure contains Device Configuration Details
 *
 *  \param  Technology
 *          - Current technology in use
 *              - 0x00 - 1x RTT
 *              - 0x01 - EVDO Rev 0
 *              - 0x02 - EVDO Rev A
 *              - 0xFF - NA
 *
 *  \param  QLIC
 *          - Quasi Linear Interference Cancellation
 *              - 0x00 - Not supported
 *              - 0x01 - Supported
 *
 *  \param  Chipset
 *          - Qualcomm platform
 *              - 0x4E - MDM6200
 *              - 0x4F - MDM6600
 *              - 0xFF - NA
 *
 *  \param  HWVersion
 *          - Hardware version
 *              - 0x00 - BSHWREV0
 *              - 0x01 - BSHWREV1
 *              - 0x02 - BSHWREV2
 *              - 0x03 - BSHWREV3
 *              - 0x04 - BSHWREVMAX
 *              - 0xFF - BSHWREVUNKNOWN
 *
 */
typedef struct
{
    BYTE Technology;
    BYTE QLIC;
    BYTE Chipset;
    BYTE HWVersion;
} DeviceConfigDetail;

/**
 *  This structure contains Data Status Details
 *
 *  \param  IPAddress
 *          - IP Address
 *          - 0xABCDEFGH - AB.CD.EF.GH
 *          - Example:
 *              - 0x12345678 - 18.52.86.120 \n
 *              0x12=18 0x34=52 0x56=86 0x78=120
 *          - 0xFFFFFFFF - NA
 *
 *  \param  LastErrCode
 *          - MIP Error code
 *              - 0x00 - MIP_RRP_CODE_SUCCESS
 *              - 0x01 - MIP_RRP_CODE_SUCCESS_NO_SIM_BINDINGS
 *              - 0x40 - MIP_RRP_CODE_FAILURE_FA_REASON_UNSPECIFIED
 *              - 0x41 - MIP_RRP_CODE_FAILURE_FA_ADMIN_PROHIBITED
 *              - 0x42 - MIP_RRP_CODE_FAILURE_FA_INSUFFICIENT_RESOURCES
 *              - 0x43 - MIP_RRP_CODE_FAILURE_FA_MOBILE_NODE_FAILED_AUTH
 *              - 0x44 - MIP_RRP_CODE_FAILURE_FA_HA_FAILED_AUTH
 *              - 0x45 - MIP_RRP_CODE_FAILURE_FA_REQUESTED_LIFETIME_TOO_LONG
 *              - 0x46 - MIP_RRP_CODE_FAILURE_FA_MALFORMED_REQUEST
 *              - 0x47 - MIP_RRP_CODE_FAILURE_FA_MALFORMED_REPLY
 *              - 0x48 - MIP_RRP_CODE_FAILURE_FA_ENCAPSULATION_UNAVAILABLE
 *              - 0x49 - MIP_RRP_CODE_FAILURE_FA_VJHC_UNAVAILABLE
 *              - 0x4A - MIP_RRP_CODE_FAILURE_FA_CANT_REV_TUN
 *              - 0x4B - MIP_RRP_CODE_FAILURE_FA_MUST_REV_TUN
 *              - 0x4C - MIP_RRP_CODE_FAILURE_FA_BAD_TTL
 *              - 0x4D - MIP_RRP_CODE_FAILURE_INVALID_COA
 *              - 0x4F - MIP_RRP_CODE_FAILURE_FA_DELIVERY_STYLE_NOT_SUPPORTED
 *              - 0x59 - MIP_RRP_CODE_FAILURE_FA_VS_REASON
 *              - 0x61 - MIP_RRP_CODE_FAILURE_MISSING_NAI
 *              - 0x62 - MIP_RRP_CODE_FAILURE_MISSING_HA_ADDR
 *              - 0x63 - MIP_RRP_CODE_FAILURE_MISSING_HOMEADDR
 *              - 0x68 - MIP_RRP_CODE_FAILURE_UNKNOWN_CHALLENGE
 *              - 0x69 - MIP_RRP_CODE_FAILURE_MISSING_CHALLENGE
 *              - 0x6A - MIP_RRP_CODE_FAILURE_STALE_CHALLENGE
 *              - 0x6B - MIP_RRP_CODE_FAILURE_MISSING_MN_FA
 *              - 0x80 - MIP_RRP_CODE_FAILURE_HA_REASON_UNSPECIFIED
 *              - 0x81 - MIP_RRP_CODE_FAILURE_HA_ADMIN_PROHIBITED
 *              - 0x82 - MIP_RRP_CODE_FAILURE_HA_INSUFFICIENT_RESOURCES
 *              - 0x83 - MIP_RRP_CODE_FAILURE_HA_MOBILE_NODE_FAILED_AUTH
 *              - 0x84 - MIP_RRP_CODE_FAILURE_HA_FA_FAILED_AUTH
 *              - 0x85 - MIP_RRP_CODE_FAILURE_HA_REG_ID_MISMATCH
 *              - 0x86 - MIP_RRP_CODE_FAILURE_HA_MALFORMED_REQUEST
 *              - 0x88 - MIP_RRP_CODE_FAILURE_UNKNOWN_HA
 *              - 0x89 - MIP_RRP_CODE_FAILURE_HA_CANT_REV_TUN
 *              - 0x8A - MIP_RRP_CODE_FAILURE_HA_MUST_REV_TUN
 *              - 0x8B - MIP_RRP_CODE_FAILURE_HA_ENCAPSULATION_UNAVAILABLE
 *              - 0x8F - MIP_RRP_CODE_FAILURE_REDIRECTED_HA
 *              - 0x90 - MIP_RRP_CODE_FAILURE_HA_BAD_AAA_AUTH
 *              - 0xFF - NA
 */
typedef struct
{
    ULONG IPAddress;
    BYTE LastErrCode;
} DataStatusDetail;

/**
 *  This structure contains information about the SLQSSwiNetworkDebug response
 *  parameters.
 *
 *  \param  pObjectVer
 *          - Object's version number for the host to handle
 *              - 0xFF - NA
 *              - Others - shows in decimal
 *
 *  \param  pNetworkStat1x
 *          - See \ref NetworkStat1x for more information
 *
 *  \param  pNetworkStatEVDO
 *          - See \ref NetworkStatEVDO for more information.
 *
 *  \param  pDeviceConfigDetail
 *          - See \ref DeviceConfigDetail for more information.
 *
 *  \param  pDataStatusDetail
 *          - See \ref DataStatusDetail for more information.
 */
typedef struct
{
    BYTE               *pObjectVer;
    NetworkStat1x      *pNetworkStat1x;
    NetworkStatEVDO    *pNetworkStatEVDO;
    DeviceConfigDetail *pDeviceConfigDetail;
    DataStatusDetail   *pDataStatusDetail;
}NetworkDebugResp;

/**
 *  This API retrieves device and network status details
 *
 *  \param  pNetworkDebugResp[OUT]
 *          - See \ref NetworkDebugResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSSwiNetworkDebug ( NetworkDebugResp *pNetworkDebugResp );

/**
 *  This structure contains information about the SLQSSwiGetLteCQI response
 *  parameters.
 *
 *  \param  pValidityCW0[OUT]
 *          - Values
 *              - 0- Invalid.
 *              - 1- Valid.
 *
 *  \param  pCQIValueCW0[OUT]
 *          - Values
 *              - Range 0~15
 *
 *  \param  pValidityCW1[OUT]
 *          - Values
 *              - 0- Invalid.
 *              - 1- Valid.
 *
 *  \param  pCQIValueCW1[OUT]
 *          - Values
 *              - Range 0~15
 */
typedef struct
{
    BYTE ValidityCW0;
    BYTE CQIValueCW0;
    BYTE ValidityCW1;
    BYTE CQIValueCW1;
}LteCQIParm;

/**
 *  This API Fetch CQI parameters for LTE data session
 *
 *  \param  pLteCQIParm[OUT]
 *          - See \ref LteCQIParm for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSSwiGetLteCQI ( LteCQIParm *pLteCQIResp );

/**
 *  This structure contains RSSI threshold related parameters.
 *
 *  \param  RSSIThresListLen
 *          - Length of the RSSI threshold list parameter to follow
 *
 *  \param  pRSSIThresList
 *          - RSSI in dBm( signed bytes )
 *          - A value of -125 dBm or lower is used to indicate No Signal
 *          - RSSI values have the following ranges (in dBm)
 *              - CDMA is -105 to -21
 *              - HDR is -118 to -13
 *              - GSM is -111 to -48
 *              - WCDMA is -121 to 0
 *              - LTE is -120 to 0
 *          - Threshold values specified above are used for all RATs
 *          - The maximum number of threshold values is 16, each a signed byte value.
 *
 */
typedef struct
{
    BYTE RSSIThresListLen;
    INT8 *pRSSIThresList;
}RSSIThresh;

/**
 *  This structure contains ECIO threshold related parameters.
 *
 *  \param  ECIOThresListLen
 *          - Length of the ECIO threshold list parameter to follow
 *
 *  \param  pECIOThresList
 *          - Sequence of thresholds delimiting ECIO event reporting bands
 *          - Every time a new ECIO value crosses a threshold value, an
 *            event report indication message with the new ECIO value is sent
 *            to the requesting control point. For this field
 *              - Each ECIO threshold value is a signed 2 byte value
 *              - Each ECIO threshold value increments in negative
 *                0.5 dB, e.g., an ECIO threshold value of 2 means -1dB
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *              - Threshold values specified above are used for all RATs
 *
 */
typedef struct
{
    BYTE ECIOThresListLen;
    SHORT *pECIOThresList;
}ECIOThresh;

/**
 *  This structure contains HDR SINR threshold related parameters.
 *
 *  \param  HDRSINRThresListLen
 *          - Length of the HDR SINR threshold list parameter to follow
 *
 *  \param  pHDRSINRThresList
 *          - Sequence of thresholds delimiting SINR event reporting bands
 *          - Every time a new SINR value crosses a threshold value, an
 *            event report indication message with the new SINR value is sent
 *            to the requesting control point. For this field
 *              - SINR is reported only for HDR
 *              - Each SINR threshold value is an unsigned 1 byte value
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *
 */
typedef struct
{
    BYTE HDRSINRThresListLen;
    BYTE *pHDRSINRThresList;
}HDRSINRThresh;

/**
 *  This structure contains LTE SNR threshold related parameters.
 *
 *  \param  LTESNRThresListLen
 *          - Length of the LTE SNR threshold list parameter to follow
 *
 *  \param  pLTESNRThresList
 *          - Sequence of thresholds delimiting SNR event reporting bands
 *          - Every time a SNR value crosses a threshold value, an
 *            event report indication message with the new SNR value is sent
 *            to the requesting control point. For this field
 *              - For LTE, each SNR threshold value is a signed 2 Byte value
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *              - SNR level as a scaled integer in units of 0.1 dB; e.g., -16 dB
 *                has a value of -160 and 24.6 dB has a value of 246
 *
 */
typedef struct
{
    BYTE LTESNRThresListLen;
    SHORT *pLTESNRThresList;
}LTESNRThresh;

/**
 *  This structure contains IO threshold related parameters.
 *
 *  \param  IOThresListLen
 *          - Length of the LTE SNR threshold list parameter to follow
 *
 *  \param  pIOThresList
 *          - Sequence of thresholds delimiting IO event reporting bands
 *          - Every time a new IO value crosses a threshold value, an
 *            event report indication message with the new IO value is sent
 *            to the requesting control point. For this field
 *              - IO is applicable only for HDR
 *              - Each IO threshold value is a signed 4 byte value
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *
 */
typedef struct
{
    BYTE IOThresListLen;
    INT32 *pIOThresList;
}IOThresh;

/**
 *  This structure contains RSRQ threshold related parameters.
 *
 *  \param  RSRQThresListLen
 *          - Length of the LTE RSRQ threshold list parameter to follow
 *
 *  \param  pRSRQThresList
 *          - Sequence of thresholds delimiting current RSRQ event reporting
 *            bands
 *          - Every time a new RSRQ value crosses a threshold value, an
 *            event report indication message with the new RSRQ value is sent
 *            to the requesting control point. For this field
 *              - RSRQ values are applicable only for LTE
 *              - RSRQ values are measured in dBm, with a range of -20
 *                dBm to -3 dBm
 *              - Each RSRQ threshold value is a signed byte value
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *
 */
typedef struct
{
    BYTE RSRQThresListLen;
    INT8 *pRSRQThresList;
}RSRQThresh;

/**
 *  This structure contains RSRP threshold related parameters.
 *
 *  \param  RSRPThresListLen
 *          - Length of the LTE RSRP threshold list parameter to follow
 *
 *  \param  pRSRPThresList
 *          - Sequence of thresholds delimiting current RSRP event reporting
 *            bands
 *          - Every time a new RSRP value crosses a specified threshold value,
 *            an event report indication message with the new RSRQ value is
 *            sent to the requesting control point. For this field
 *              - RSRP values are applicable only for LTE
 *              - RSRP values are measured in dBm, with a range of -44
 *                dBm to -140 dBm
 *              - Each RSRP threshold value is a signed byte value
 *              - Maximum number of threshold values is 16
 *              - At least one value must be specified
 *
 */
typedef struct
{
    BYTE  RSRPThresListLen;
    SHORT *pRSRPThresList;
}RSRPThresh;

/**
 *  This structure contains LTE Signal Report Config parameters.
 *
 *  \param  rptRate
 *          - Rate on how often the LTE signal must be checked for reporting
 *          - Values:
 *              - 0 - Report using the default configuration
 *              - 1 - Report every 1 sec
 *              - 2 - Report every 2 sec
 *              - 3 - Report every 3 sec
 *              - 4 - Report every 4 sec
 *              - 5 - Report every 5 sec
 *
 *  \param  avgPeriod
 *          - Averaging period to be used for the LTE signal
 *          - Values:
 *              - 0 - Average using the default configuration
 *              - 1 - Average over 1 sec
 *              - 2 - Average over 2 sec
 *              - 3 - Average over 3 sec
 *              - 4 - Average over 4 sec
 *              - 5 - Average over 5 sec
 *              - 6 - Average over 6 sec
 *              - 7 - Average over 7 sec
 *              - 8 - Average over 8 sec
 *              - 9 - Average over 9 sec
 *              - 10 - Average over 10 sec
 *
 */
typedef struct
{
    BYTE rptRate;
    BYTE avgPeriod;
}LTESigRptCfg;

/**
 *  This structure contains TDSCDMA SINR threshold related parameters.
 *
 *  \param  TDSCDMASINRCONFThreshListLen
 *          - Length of the TDSCDMA SINR threshold list parameter to follow
 *
 *  \param  pTDSCDMASINRCONFThreshList
 *          - Array of SINR thresholds (in dB) used by TD-SCDMA
 *          - Maximum of 32 values
 */
typedef struct
{
    BYTE   TDSCDMASINRCONFThreshListLen;
    FLOAT  *pTDSCDMASINRCONFThreshList;
}TDSCDMASINRCONFThresh;


/**
 *  This structure contains the 3gpp Configuration Item information.
 *
 *  \param  pRSSIThresh
 *          - RSSI threshold List
 *          - See \ref RSSIThresh for more details
 *
 *  \param  pECIOThresh
 *          - ECIO Threshold List
 *          - See \ref ECIOThresh for more details
 *
 *  \param  pHDRSINRThresh
 *          - HDR SINR Threshold List
 *          - See \ref HDRSINRThresh for more details
 *
 *  \param  pLTESNRThresh
 *          - LTE SNR Threshold List
 *          - See \ref LTESNRThresh for more details
 *
 *  \param  pIOThresh
 *          - IO Threshold List
 *          - See \ref IOThresh for more details
 *
 *  \param  pRSRQThresh
 *          - RSRQ Threshold List
 *          - See \ref RSRQThresh for more details
 *
 *  \param  pRSRPThresh
 *          - RSRP Threshold List
 *          - See \ref RSRPThresh for more details
 *
 *  \param  pLTESigRptCfg
 *          - LTE signal report config
 *          - See \ref LTESigRptCfg for more details
 *
 *  \param  pTDSCDMASINRCONFThresh
 *          - TD-SCDMA SINR Threshold List
 *          - See \ref TDSCDMASINRCONFThresh for more details

 *
 */
typedef struct
{
    RSSIThresh    *pRSSIThresh;
    ECIOThresh    *pECIOThresh;
    HDRSINRThresh *pHDRSINRThresh;
    LTESNRThresh  *pLTESNRThresh;
    IOThresh      *pIOThresh;
    RSRQThresh    *pRSRQThresh;
    RSRPThresh    *pRSRPThresh;
    LTESigRptCfg  *pLTESigRptCfg;
    TDSCDMASINRCONFThresh *pTDSCDMASINRCONFThresh;
} sigInfo;

/**
 *  Sets the signal reporting thresholds. This API is deprecated on
 *  MC73xx/EM73xx modules since firmware version SWI9X15C_05_xx_xx_xx
 *  and all EM74xx firmware versions. Please use SLQSNasConfigSigInfo2()
 *  instead for new firmware versions and new modules.
 *
 *  \param  pSigInfo[IN]
 *          - See \ref sigInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG SLQSConfigSigInfo( sigInfo *pSigInfo );

/**
 *  This structure contains the OTA message indication.
 *
 *  \param  lteEsmUl
 *          - 0 - do not report
 *          - 1 - report LTE ESM uplink messages
 *
 *  \param  lteEsmDl
 *          - 0 - do not report
 *          - 1 - report LTE ESM downlink messages
 *
 *  \param  lteEmmUl
 *          - 0 - do not report
 *          - 1 - report LTE EMM uplink messages
 *
 *  \param  lteEmmDl
 *          - 0 - do not report
 *          - 1 - report GSM/UMTS uplink messages
 *
 *  \param  gsmUmtsUl
 *          - 0 - do not report
 *          - 1 - report GSM/UMTS uplink messages
 *
 *  \param  gsmUmtsDl
 *          - 0 - do not report
 *          - 1 - report GSM/UMTS downlink messages
 *
 *  \param  pRankIndicatorInd
 *          - 0 - do not report
 *          - 1 - report Rank Indicator messages
 *
 *  \param  pTimer
 *          - 0 - do not report
 *          - 1 - report Timer Indicator messages
 */
typedef struct
{
    BYTE lteEsmUl;
    BYTE lteEsmDl;
    BYTE lteEmmUl;
    BYTE lteEmmDl;
    BYTE gsmUmtsUl;
    BYTE gsmUmtsDl;
    BYTE *pRankIndicatorInd;
    BYTE *pTimer;
}NasSwiIndReg;

/**
 *  sets the registration state for different QMI_NAS  SWI indications
 *
 *  \param  pIndRegReq[IN]
 *          - See \ref NasSwiIndReg for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSNasSwiIndicationRegister( NasSwiIndReg *pIndRegReq );
/**
 *  Retrieves information about the home network of the device.
 *  It will extract 3GPP2 Network Information also.
 *
 *  \param  pMCC[OUT]
 *          - Mobile country code (UMTS only).
 *
 *  \param  pMNC[OUT]
 *          - Mobile network code (UMTS only).
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator) that
 8            network name array can contain (UMTS only).
 *
 *  \param  pName[OUT]
 *          - Network name or description represented as a NULL terminated
 *            string (empty string returned when unknown) (UMTS only).
 *
 *  \param  pSID[OUT]
 *          - Home network system ID
 *              - 0xFFFF - Unknown.
 *              - Only applies to cdma2000
 *
 *  \param  pNID[OUT]
 *          - Home network ID\n
 *              - 0xFFFF - Unknown.
 *              - Only applies to cdma2000
 *
 *  \param  pNw2MCC[OUT]
 *          - Mobile country code (3GPP2 only).
 *          - Range : 0 to 999
 *
 *  \param  pNw2MNC[OUT]
 *          - Mobile network code (3GPP2 only).
 *          - Range : 0 to 999
 *
 *  \param  pNw2DescDisp[OUT]
 *          - Network Name Display (3GPP2 only).
 *          -Valid Value
 *             - 0x00 - Do not display
 *             - 0x01 - Display
 *             - 0xFF - Unknown
 *
 *  \param  pNw2DescDisp[OUT]
 *          - Encoding of the network description (3GPP2 only).
 *          - Valid Value
 *              - 0x00 - Octet, unspecified
 *              - 0x02 - 7-bit ASCII
 *              - 0x04 - Unicode
 *              - 0x09 - GSM 7-bit default
 *
 *  \param  nw2DescLen[OUT]
 *          - Network Description Length (3GPP2 only).
 *
 *  \param  pNw2Name[OUT]
 *          - Network Name (3GPP2 only).
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG GetHomeNetwork3GPP2(
    WORD *pMCC,
    WORD *pMNC,
    BYTE nameSize,
    CHAR *pName,
    WORD *pSID,
    WORD *pNID,
    WORD *pNw2MCC,
    WORD *pNw2MNC,
    BYTE *pNw2DescDisp,
    BYTE *pNw2DescEnc,
    BYTE *pNw2DescLen,
    BYTE *pNw2Name );

/**
 *  This structure contains CDMA RSSI threshold related parameters.
 *
 *  \param  CDMARSSIThreshListLen
 *          - Length of the CDMARSSI threshold list parameter to follow
 *
 *  \param  pCDMARSSIThreshList
 *          - Array of RSSI thresholds (in units of 0.1 dBm)
 *          - maximum of 32 values.
 *          - Range for RSSI values:-105 to -21 (in dBm).
 *
 */
typedef struct
{
    BYTE  CDMARSSIThreshListLen;
    WORD  *pCDMARSSIThreshList;
}CDMARSSIThresh;

/**
 *  This structure contains CDMA ECIO threshold related parameters.
 *
 *  \param  CDMAECIOThreshListLen
 *          - Length of the CDMA ECIO threshold list parameter to follow
 *
 *  \param  pCDMAECIOThreshList
 *          - Array of ECIO thresholds (in units of 0.1 dB)
 *          - Maximum of 32 values.
 *          Range for ECIO values: -31.5 to 0 (in dB)
 *
 */
typedef struct
{
    BYTE  CDMAECIOThreshListLen;
    WORD  *pCDMAECIOThreshList;
}CDMAECIOThresh;

/**
 *  This structure contains HDR RSSI threshold related parameters.
 *
 *  \param  HDRRSSIThreshListLen
 *          - Length of the HDR RSSI threshold list parameter to follow
 *
 *  \param  pHDRRSSIThreshList
 *          - Array of RSSI thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values.
 *          - Range for RSSI values: -118 to -13 (in dBm).
 *
 */
typedef struct
{
    BYTE  HDRRSSIThreshListLen;
    WORD *pHDRRSSIThreshList;
}HDRRSSIThresh;

/**
 *  This structure contains HDR ECIO threshold related parameters.
 *
 *  \param  HDRECIOThreshListLen
 *          - Length of the HDR ECIO threshold list parameter to follow
 *
 *  \param  pHDRECIOThreshList
 *          - Array of ECIO thresholds (in units of 0.1 dB)
 *          - Maximum of 32 values
 *          - Range for ECIO values: -31.5 to 0 (in dB).
 */
typedef struct
{
    BYTE  HDRECIOThreshListLen;
    WORD  *pHDRECIOThreshList;
}HDRECIOThresh;

/**
 *  This structure contains HDR SINR threshold related parameters.
 *
 *  \param  HDRSINRThreshListLen
 *          - Length of the HDR ECIO threshold list parameter to follow
 *
 *  \param  pHDRSINRThreshList
 *          - Array of SINR level thresholds (in units of 1)
 *          - maximum of 32 values.
 *          - Valid levels are 0 to 8
 *            - 0x00 - SINR_LEVEL_0 is -9 dB
 *            - 0x01 - SINR_LEVEL_1 is -6 dB
 *            - 0x02 - SINR_LEVEL_2 is -4.5 dB
 *            - 0x03 - SINR_LEVEL_3 is -3 dB
 *            - 0x04 - SINR_LEVEL_4 is -2 dB
 *            - 0x05 - SINR_LEVEL_5 is +1 dB
 *            - 0x06 - SINR_LEVEL_6 is +3 dB
 *            - 0x07 - SINR_LEVEL_7 is +6 dB
 *            - 0x08 - SINR_LEVEL_8 is +9 dB
*/
typedef struct
{
    BYTE  HDRSINRThreshListLen;
    WORD  *pHDRSINRThreshList;
}HDRSINRThreshold;

/**
 *  This structure contains HDR IO threshold related parameters.
 *
 *  \param  HDRIOThreshListLen
 *          - Length of the HDR IO threshold list parameter to follow
 *
 *  \param  pHDRIOThreshList
 *          - Array of IO thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values
 *          - Range for IO values: -128 to -13 (in dBm).
 */

typedef struct
{
    BYTE  HDRIOThreshListLen;
    WORD  *pHDRIOThreshList;
}HDRIOThresh;

/**
 *  This structure contains GSM RSSI threshold related parameters.
 *
 *  \param  GSMRSSIThreshListLen
 *          - Length of the GSM RSSI threshold list parameter to follow
 *
 *  \param  pGSMRSSIThreshList
 *          - Array of RSSI thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values
 *          - Range for RSSI values: -111 to -48 (in dBm)
 */

typedef struct
{
    BYTE  GSMRSSIThreshListLen;
    WORD  *pGSMRSSIThreshList;
}GSMRSSIThresh;

/**
 *  This structure contains WCDMA RSSI threshold related parameters.
 *
 *  \param  WCDMARSSIThreshListLen
 *          - Length of the WCDMA RSSI threshold list parameter to follow
 *
 *  \param  pWCDMARSSIThreshList
 *          - Array of RSSI thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values.
 *          - Range for RSSI values: -121 to 0 (in dBm)
 */
typedef struct
{
    BYTE  WCDMARSSIThreshListLen;
    WORD  *pWCDMARSSIThreshList;
}WCDMARSSIThresh;

/**
 *  This structure contains WCDMA ECIO threshold related parameters.
 *
 *  \param  WCDMAECIOThreshListLen
 *          - Length of the WCDMA ECIO threshold list parameter to follow
 *
 *  \param  pWCDMAECIOThreshList
 *          - Array of ECIO thresholds (in units of 0.1 dB)
 *          - Maximum of 32 values
 *          - Range for ECIO values: -31.5 to 0 (in dB)
 */
typedef struct
{
    BYTE  WCDMAECIOThreshListLen;
    WORD  *pWCDMAECIOThreshList;
}WCDMAECIOThresh;

/**
 *  This structure contains LTE RSSI threshold related parameters.
 *
 *  \param  LTERSSIThreshListLen
 *          - Length of the LTE RSSI threshold list parameter to follow
 *
 *  \param  pLTERSSIThreshList
 *          - Array of RSSI thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values.
 *          - Range for RSSI values: -120 to 0 (in dBm)
 */
typedef struct
{
    BYTE  LTERSSIThreshListLen;
    WORD  *pLTERSSIThreshList;
}LTERSSIThresh;

/**
 *  This structure contains LTE SNR threshold related parameters.
 *
 *  \param  LTESNRThreshListLen
 *          - Length of the LTE SNR threshold list parameter to follow
 *
 *  \param  pLTESNRThreshList
 *          - Array of SNR thresholds (in units of 0.1 dB)
 *          - Maximum of 32 values
 *          - Range for SNR values: -20 to 30 (in dB).
 */
typedef struct
{
    BYTE  LTESNRThreshListLen;
    WORD  *pLTESNRThreshList;
}LTESNRThreshold;

/**
 *  This structure contains LTE RSRQ threshold related parameters.
 *
 *  \param  LTERSRQThreshListLen
 *          - Length of the LTE RSRQ threshold list parameter to follow
 *
 *  \param  pLTERSRQThreshList
 *          - Array of RSRQ thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values.
 *          - Range for RSRQ values: -20 to -3 (in dBm)
 */
typedef struct
{
    BYTE  LTERSRQThreshListLen;
    WORD  *pLTERSRQThreshList;
}LTERSRQThresh;

/**
 *  This structure contains LTE RSRP threshold related parameters.
 *
 *  \param  LTERSRPThreshListLen
 *          - Length of the LTE RSRP threshold list parameter to follow
 *
 *  \param  pLTERSRPThreshList
 *          - Array of RSRP thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values
 *          - Range for RSRP values: -140 to -44 (in dBm).
 */
typedef struct
{
    BYTE  LTERSRPThreshListLen;
    WORD  *pLTERSRPThreshList;
}LTERSRPThresh;

/**
 *  This structure contains LTE RSRP threshold related parameters.
 *
 *  \param  rptRate
 *          - Rate on how often the LTE signal must be checked for reporting
 *          Values
 *          - 0 - Report using the default configuration
 *          - 1 - Report every 1 sec
 *          - 2 - Report every 2 sec
 *          - 3 - Report every 3 sec
 *          - 4 - Report every 4 sec
 *          - 5 - Report every 5 sec
 *
 *  \param  avgPeriod
 *          - Averaging period to be used for the LTE signal.
 *          - Values
 *           - 0 - Average using the default configuration
 *           - 1 - Average over 1 sec
 *           - 2 - Average over 2 sec
 *           - 3 - Average over 3 sec
 *           - 4 - Average over 4 sec
 *           - 5 - Average over 5 sec
 *           - 6 - Average over 6 sec
 *           - 7 - Average over 7 sec
 *           - 8 - Average over 8 sec
 *           - 9 - Average over 9 sec
 *           - 10 - Average over 10 sec
 */
typedef struct
{
    BYTE  rptRate;
    BYTE  avgPeriod;
}LTESigRptConfig;

/**
 *  This structure contains TDSCDMA RSCP threshold related parameters.
 *
 *  \param  TDSCDMARSCPThreshListLen
 *          - Length of the TDSCDMA RSCP threshold list parameter to follow
 *
 *  \param  pTDSCDMARSCPThreshList
 *          - Array of RSCP thresholds (in units of 0.1 dBm)
 *          - Maximum of 32 values
 *          - Range for RSCP values: -120 to -25 (in dBm).
 */
typedef struct
{
    BYTE  TDSCDMARSCPThreshListLen;
    WORD  *pTDSCDMARSCPThreshList;
}TDSCDMARSCPThresh;

/**
 *  This structure contains TDSCDMA RSSI threshold related parameters.
 *
 *  \param  TDSCDMARSSIThreshListLen
 *          - Length of the TDSCDMA RSSI threshold list parameter to follow
 *
 *  \param  pTDSCDMARSSIThreshList
 *          - Array of RSSI thresholds (in dBm) used by TD-SCDMA
 *          - Maximum of 32 values.
 */
typedef struct
{
    BYTE   TDSCDMARSSIThreshListLen;
    ULONG  *pTDSCDMARSSIThreshList;
}TDSCDMARSSIThresh;

/**
 *  This structure contains TDSCDMA ECIO threshold related parameters.
 *
 *  \param  TDSCDMAECIOThreshListLen
 *          - Length of the TDSCDMA ECIO threshold list parameter to follow
 *
 *  \param  pTDSCDMAECIOThreshList
 *          - Array of ECIO thresholds (in dB) used by TD-SCDMA
 *          - Maximum of 32 values.
 */
typedef struct
{
    BYTE   TDSCDMAECIOThreshListLen;
    ULONG  *pTDSCDMAECIOThreshList;
}TDSCDMAECIOThresh;

/**
 *  This structure contains TDSCDMA SINR threshold related parameters.
 *
 *  \param  TDSCDMASINRThreshListLen
 *          - Length of the TDSCDMA SINR threshold list parameter to follow
 *
 *  \param  pTDSCDMASINRThreshList
 *          - Array of SINR thresholds (in dB) used by TD-SCDMA
 *          - Maximum of 32 values
 */
typedef struct
{
    BYTE   TDSCDMASINRThreshListLen;
    ULONG  *pTDSCDMASINRThreshList;
}TDSCDMASINRThresh;
/**
 *  This structure contains the Signal Strength reporting thresholds Item information.
 *
 *  \param  pCDMARSSIThresh
 *          - CDMA RSSI threshold List
 *          - See \ref CDMARSSIThresh for more details
 *
 *  \param  pCDMARSSIDelta
 *          - RSSI delta (in units of 0.1 dBm).
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pCDMAECIOThresh
 *          - CDMA ECIO Threshold List
 *          - See \ref CDMAECIOThresh for more details
 *
 *  \param  pCDMAECIODelta
 *          - ECIO delta (in units of 0.1 dB).
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pHDRRSSIThresh
 *          - HDR RSSI Threshold List
 *          - See \ref HDRRSSIThresh for more details
 *
 *  \param  pHDRRSSIDelta
 *          - RSSI delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pHDRECIOThresh
 *          - HDR ECIO Threshold List
 *          - See \ref HDRECIOThresh for more details
 *
 *  \param  pHDRECIODelta
 *          - ECIO delta (in units of 0.1 dB)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pHDRSINRThresh
 *          - HDR SINR Threshold List
 *          - See \ref HDRSINRThreshold for more details
 *
 *  \param  pHDRSINRDelta
 *          - SINR delta (in units of 1 SINR level)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pHDRIOThresh
 *          - HDR IO Threshold List
 *          - See \ref HDRIOThresh for more details
 *
 *  \param  pHDRIODelta
 *          - IO delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pGSMRSSIThresh
 *          - GSM RSSI Threshold List
 *          - See \ref GSMRSSIThresh for more details
 *
 *  \param  pGSMRSSIDelta
 *          - RSSI delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pWCDMARSSIThresh
 *          - WCDMA RSSI Threshold List
 *          - See \ref WCDMARSSIThresh for more details
 *
 *  \param  pWCDMARSSIDelta
 *          - RSSI delta (in units of 0.1 dBm).
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pWCDMAECIOThresh
 *          - WCDMA ECIO Threshold List
 *          - See \ref WCDMAECIOThresh for more details
 *
 *  \param  pWCDMAECIODelta
 *          - ECIO delta (in units of 0.1 dB)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pLTERSSIThresh
 *          - LTE RSSI Threshold List
 *          - See \ref LTERSSIThresh for more details
 *
 *  \param  pLTERSSIDelta
 *          - RSSI delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pLTESNRThresh
 *          - LTE SNR Threshold List
 *          - See \ref LTESNRThreshold for more details
 *
 *  \param  pLTESNRDelta
 *          - SNR delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pLTERSRQThresh
 *          - LTE RSRQ Threshold List
 *          - See \ref LTERSRQThresh for more details
 *
 *  \param  pLTERSRQDelta
 *          - RSRQ delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pLTERSRPThresh
 *          - LTE RSRP Threshold List
 *          - See \ref LTERSRPThresh for more details
 *
 *  \param  pLTERSRPDelta
 *          - RSRP delta (in units of 0.1 dBm).
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pLTESigRptConfig
 *          - LTE Signal Report Config
 *          - See \ref LTESigRptConfig for more details
 *
 *  \param  pTDSCDMARSCPThresh
 *          - TDSCDMA RSCP Threshold List
 *          - See \ref TDSCDMARSCPThresh for more details
 *
 *  \param  pTDSCDMARSCPDelta
 *          - RSCP delta (in units of 0.1 dBm)
 *          - A value of 0 is rejected with a QMI_ERR_INVALID_ARG error.
 *
 *  \param  pTDSCDMARSSIThresh
 *          - TDSCDMA RSSI Threshold List
 *          - See \ref TDSCDMARSSIThresh for more details
 *
 *  \param  pTDSCDMARSSIDelta
 *          - RSSI delta (in dBm) used by TD-SCDMA.
 *
 *  \param  pTDSCDMAECIOThresh
 *          - TDSCDMA ECIO Threshold List
 *          - See \ref TDSCDMAECIOThresh for more details
 *
 *  \param  pTDSCDMAECIODelta
 *          - ECIO delta (in dB) used by TD-SCDMA
 *
 *  \param  pTDSCDMASINRThresh
 *          - TDSCDMA SINR Threshold List
 *          - See \ref TDSCDMASINRThresh for more details
 *
 *  \param  pTDSCDMASINRDelta
 *          - SINR delta (in dB) used by TD-SCDMA.
 *
 */
typedef struct
{
    CDMARSSIThresh    *pCDMARSSIThresh;
    WORD              *pCDMARSSIDelta;
    CDMAECIOThresh    *pCDMAECIOThresh;
    WORD              *pCDMAECIODelta;
    HDRRSSIThresh     *pHDRRSSIThresh;
    WORD              *pHDRRSSIDelta;
    HDRECIOThresh     *pHDRECIOThresh;
    WORD              *pHDRECIODelta;
    HDRSINRThreshold  *pHDRSINRThresh;
    WORD              *pHDRSINRDelta;
    HDRIOThresh       *pHDRIOThresh;
    WORD              *pHDRIODelta;
    GSMRSSIThresh     *pGSMRSSIThresh;
    WORD              *pGSMRSSIDelta;
    WCDMARSSIThresh   *pWCDMARSSIThresh;
    WORD              *pWCDMARSSIDelta;
    WCDMAECIOThresh   *pWCDMAECIOThresh;
    WORD              *pWCDMAECIODelta;
    LTERSSIThresh     *pLTERSSIThresh;
    WORD              *pLTERSSIDelta;
    LTESNRThreshold   *pLTESNRThresh;
    WORD              *pLTESNRDelta;
    LTERSRQThresh     *pLTERSRQThresh;
    WORD              *pLTERSRQDelta;
    LTERSRPThresh     *pLTERSRPThresh;
    WORD              *pLTERSRPDelta;
    LTESigRptConfig   *pLTESigRptConfig;
    TDSCDMARSCPThresh *pTDSCDMARSCPThresh;
    WORD              *pTDSCDMARSCPDelta;
    TDSCDMARSSIThresh *pTDSCDMARSSIThresh;
    ULONG             *pTDSCDMARSSIDelta;
    TDSCDMAECIOThresh *pTDSCDMAECIOThresh;
    ULONG             *pTDSCDMAECIODelta;
    TDSCDMASINRThresh *pTDSCDMASINRThresh;
    ULONG             *pTDSCDMASINRDelta;
} setSignalStrengthInfo;
/**
 *  Sets the signal strength reporting thresholds
 *
 *  \param  pSetSignalStrengthInfo[IN]
 *          - See \ref setSignalStrengthInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 *  \note   Mixture of threshold and delta values can be provided in the request.
 *          But for each type and RAT,only one of threshold list or delta value is
 *          to be provided.
 *
 */
ULONG SLQSNasConfigSigInfo2( setSignalStrengthInfo *pSetSignalStrengthInfo );

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_SET_SYS_SELECT_PREF Radio access technology
 *
 */

enum _NAMS_RADIO_IF_TECHNOLOGY_{
    eNAS_RADIO_IF_GSM       = 0x04,
    eNAS_RADIO_IF_UMTS      = 0x05,
    eNAS_RADIO_IF_LTE       = 0x08,
    eNAS_RADIO_IF_TDSCDMA   = 0x09
};

/*
 * NAS LTE CPHY Scell States.
 */
typedef enum {
    eNAS_LTE_CPHY_SCELL_STATE_DECONFIGURED=0x00,
    eNAS_LTE_CPHY_SCELL_STATE_CONFIGURED_DEACTIVATED=0x01,
    eNAS_LTE_CPHY_SCELL_STATE_CONFIGURED_ACTIVATED=0x02,
}NAS_LTE_CPHY_SCELL_STATE;

/*
 * NAS LTE CPHY Downlink Values.
 */
typedef enum {
    eNAS_LTE_CPHY_CA_BW_NRB_6=0x00,
    eNAS_LTE_CPHY_CA_BW_NRB_15=0x01,
    eNAS_LTE_CPHY_CA_BW_NRB_25=0x02,
    eNAS_LTE_CPHY_CA_BW_NRB_50=0x03,
    eNAS_LTE_CPHY_CA_BW_NRB_75=0x04,
    eNAS_LTE_CPHY_CA_BW_NRB_100=0x05,
}NAS_LTE_CPHY_CA_BW_NRB;

/**
 * This structure contains the parameters for Physical Carrier
 * aggregation of Scell Indeicator Type.
 *
 *  \param  pci
 *          - Physical cell ID of the SCell Range.
 *          - Range for ID values: 0 to 503.
 *
 *  \param  freq
 *          - Frequency of the absolute cell Range.
 *          - Range for ID values: 0 to 65535.
 *
 *  \param  scell_state
 *          - Scell state Values.
 *          - See \ref NAS_LTE_CPHY_SCELL_STATE for more information.
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 */

typedef struct{
    int pci;
    int freq;
    NAS_LTE_CPHY_SCELL_STATE scell_state;
    BYTE TlvPresent;
}PhyCaAggScellIndType;

/**
 * This structure contains the parameters for Physical Carrier
 * aggregation Downlink Bandwidth of Scell.
 *
 *  \param  dl_bw_value
 *          - Downlink Bandwidth Values.
 *          - See \ref NAS_LTE_CPHY_CA_BW_NRB for more information.
 *
 */
typedef struct{
    NAS_LTE_CPHY_CA_BW_NRB dl_bw_value;
    BYTE TlvPresent;
}PhyCaAggScellDlBw;

/**
 * This structure contains the parameters for Physical Carrier
 * aggregation of Scell Information.
 *
 *  \param  pci
 *          - Physical cell ID of the SCell Range.
 *          - Range for ID values: 0 to 503.
 *
 *  \param  freq
 *          - Frequency of the absolute cell Range.
 *          - Range for ID values: 0 to 65535.
 *
 *  \param  dl_bw_value
 *          - Downlink Bandwidth Values.
 *          - See \ref NAS_LTE_CPHY_CA_BW_NRB for more information.
 *
 *  \param  iLTEbandValue
 *          - Band value.
 *          - Range for LTE Band class 120 to 160.
 *              - 120 - LTE E-UTRA Operating Band 1
 *              - 121 - LTE E-UTRA Operating Band 2
 *              - 122 - LTE E-UTRA Operating Band 3
 *              - 123 - LTE E-UTRA Operating Band 4
 *              - 124 - LTE E-UTRA Operating Band 5
 *              - 125 - LTE E-UTRA Operating Band 6
 *              - 126 - LTE E-UTRA Operating Band 7
 *              - 127 - LTE E-UTRA Operating Band 8
 *              - 128 - LTE E-UTRA Operating Band 9
 *              - 129 - LTE E-UTRA Operating Band 10
 *              - 130 - LTE E-UTRA Operating Band 11
 *              - 131 - LTE E-UTRA Operating Band 12
 *              - 132 - LTE E-UTRA Operating Band 13
 *              - 133 - LTE E-UTRA Operating Band 14
 *              - 134 - LTE E-UTRA Operating Band 17
 *              - 135 - LTE E-UTRA Operating Band 33
 *              - 136 - LTE E-UTRA Operating Band 34
 *              - 137 - LTE E-UTRA Operating Band 35
 *              - 138 - LTE E-UTRA Operating Band 36
 *              - 139 - LTE E-UTRA Operating Band 37
 *              - 140 - LTE E-UTRA Operating Band 38
 *              - 141 - LTE E-UTRA Operating Band 39
 *              - 142 - LTE E-UTRA Operating Band 40
 *              - 143 - LTE E-UTRA Operating Band 18
 *              - 144 - LTE E-UTRA Operating Band 19
 *              - 145 - LTE E-UTRA Operating Band 20
 *              - 146 - LTE E-UTRA Operating Band 21
 *              - 147 - LTE E-UTRA Operating Band 24
 *              - 148 - LTE E-UTRA Operating Band 25
 *              - 149 - LTE E-UTRA Operating Band 41
 *              - 150 - LTE E-UTRA Operating Band 42
 *              - 151 - LTE E-UTRA Operating Band 43
 *              - 152 - LTE E-UTRA Operating Band 23
 *              - 153 - LTE E-UTRA Operating Band 26
 *              - 154 - LTE E-UTRA Operating Band 32
 *              - 155 - LTE E-UTRA Operating Band 125
 *              - 156 - LTE E-UTRA Operating Band 126
 *              - 157 - LTE E-UTRA Operating Band 127
 *              - 158 - LTE E-UTRA Operating Band 28
 *              - 159 - LTE E-UTRA Operating Band 29
 *              - 160 - LTE E-UTRA Operating Band 30
 *
 *
 *  \param  scell_state
 *          - Scell state Values.
 *          - See \ref NAS_LTE_CPHY_SCELL_STATE for more information.
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 */
typedef struct{
    int pci;
    int freq;
    NAS_LTE_CPHY_CA_BW_NRB dl_bw_value;
    int iLTEbandValue;
    NAS_LTE_CPHY_SCELL_STATE scell_state;
    BYTE TlvPresent;
}PhyCaAggScellInfo;

/**
 * This structure contains the parameters for Physical Carrier
 * aggregation of Pcell Information.
 *
 *  \param  pci
 *          - Physical cell ID of the SCell Range.
 *          - Range for ID values: 0 to 503.
 *
 *  \param  freq
 *          - Frequency of the absolute cell Range.
 *          - Range for ID values: 0 to 65535.
 *
 *  \param  dl_bw_value
 *          - Downlink Bandwidth Values.
 *          - See \ref NAS_LTE_CPHY_CA_BW_NRB for more information.
 *
 *  \param  scell_state
 *          - Scell state Values.
 *          - See \ref NAS_LTE_CPHY_SCELL_STATE for more information.
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 */
typedef struct{
    int pci;
    int freq;
    NAS_LTE_CPHY_CA_BW_NRB dl_bw_value;
    int iLTEbandValue;
    BYTE TlvPresent;
}PhyCaAggPcellInfo;

/**
 * This structure contains the parameters for Physical Carrier
 * aggregation of Scell Index.
 *
 *  \param  scell_idx
 *          - Physical cell ID of the SCell Range.
 *          - Range for ID values: 0 to 503.
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 */
typedef struct{
    BYTE scell_idx;
    BYTE TlvPresent;
}PhyCaAggScellIndex;


typedef struct
{
    PhyCaAggScellIndType    *pPhyCaAggScellIndType;
    PhyCaAggScellDlBw       *pPhyCaAggScellDlBw;
    PhyCaAggScellInfo       *pPhyCaAggScellInfo;
    PhyCaAggPcellInfo       *pPhyCaAggPcellInfo;
    PhyCaAggScellIndex      *pPhyCaAggScellIndex;
} nasGetLTECphyCaResp;

typedef struct
{
    PhyCaAggScellIndType    sPhyCaAggScellIndType;
    PhyCaAggScellDlBw       sPhyCaAggScellDlBw;
    PhyCaAggScellInfo       sPhyCaAggScellInfo;
    PhyCaAggPcellInfo       sPhyCaAggPcellInfo;
    PhyCaAggScellIndex      sPhyCaAggScellIndex;
} nasGetLTECphyCa;

/**
 *  This API Get LTE CPHY Carrier Info
 *
 *  \param  pLTECPhyCa[IN]
 *          - See \ref nasGetLTECphyCa for more information.
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 */

ULONG SLQSNASGetLTECPHYCaInfo(nasGetLTECphyCa *pLTECPhyCa);

/**
 *  This API Registers/De-registers for NAS CPHY Carrier Info.
 *
 *  \param  bStatus[IN]
 *          - Values
 *           - 0 - De-register.
 *           - 1 - Register. 
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 */
ULONG SLQSNasIndicationRegisterLTECphyCa(
    BYTE *bStatus );

/**
 * This structure contains the parameters for WCDMA UARFCN.
 *
 *  \param  status
 *          - 0 - Disable
 *          - 1 - Enable
 *
 *  \param  uarfcn
 *          - UARFCN to which UE is locked
 */
typedef struct{
    BYTE status;
    ULONG uarfcn;
}wcdmaUARFCN;

/**
 * This structure contains the parameters for WCDMA UARCFN.
 *
 *  \param  status
 *          - 0 - Disable
 *          - 1 - Enable
 *
 *  \param  earfcn0
 *          - Primary DL EARFCN to which the UE is locked
 *
 *  \param  earfcn1
 *          - Secondary DL EARFCN to which the UE is locked
 *          - Note : Make earfcn1 value equal to earfcn0
 *                   if only one EARFCN is desired.
 */
typedef struct{
    BYTE status;
    ULONG earfcn0;
    ULONG earfcn1;
}lteEARFCN;

/**
 * This structure contains the parameters for WCDMA UARCFN.
 *
 *  \param  status
 *          - 0 - Disable
 *          - 1 - Enable
 *
 *  \param  earfcn
 *          - UARFCN to which UE is locked
 *
 *  \param  pci
 *          - PCI to which the UE is locked
 */
typedef struct{
    BYTE status;
    ULONG earfcn;
    ULONG pci;
}ltePCI;

/**
 *  This structure contains the SLQSNASSwiGetChannelLock response parameters.
 *
 *  \param  pWcdmaUARFCN [Optional]
 *          - See \ref wcdmaUARFCN for more information
 *
 *  \param  pLteEARFCN [Optional]
 *          - See \ref lteEARFCN for more information
 *
 *  \param  pLtePCI [Optional]
 *          - See \ref ltePCI for more information
 */
typedef struct
{
    wcdmaUARFCN    *pWcdmaUARFCN;
    lteEARFCN      *pLteEARFCN;
    ltePCI         *pLtePCI;
} nasSwiGetChannelLockResp;

/**
 *  This API queries the channel or cell which the UE is locked into.
 *
 *  \param  pNasSwiGetChannelLock[OUT]
 *          - See \ref nasSwiGetChannelLockResp for more information.
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 */

ULONG SLQSNASSwiGetChannelLock(nasSwiGetChannelLockResp *pNasSwiGetChannelLockResp);

/**
 *  This structure contains the SLQSNASSwiSetChannelLock response parameters.
 *
 *  \param  pWcdmaUARFCN [Optional]
 *          - See \ref wcdmaUARCFN for more information
 *
 *  \param  pLteEARFCN [Optional]
 *          - See \ref lteEARCFN for more information
 *
 *  \param  pLtePCI [Optional]
 *          - See \ref ltePCI for more information
 */
typedef struct
{
    wcdmaUARFCN    *pWcdmaUARFCN;
    lteEARFCN      *pLteEARFCN;
    ltePCI         *pLtePCI;
} nasSwiSetChannelLockReq;

/**
 *  This API allows the host to lock the UE to a specific channel or cell.
 *
 *  \param  pNasSwiSetChannelLockReq[IN]
 *          - See \ref nasSwiSetChannelLockReq for more information.
 *
 *  \return eQCWWAN_ERR_sNONE on success, eQCWWAN_xxx error value otherwise.
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   The settings are persistent across reboots.\n
 *
 */

ULONG SLQSNASSwiSetChannelLock(nasSwiSetChannelLockReq *pNasSwiSetChannelLockReq);

/* enum declarations */
/*
 *
 * An enumeration of eSYS_SRV_DOMAIN Type of service Domain.
 *
 */
enum eSYS_SRV_DOMAIN{
    eSYS_SRV_DOMAIN_NO_SRV  = 0x00,//No Service
    eSYS_SRV_DOMAIN_CS_ONLY = 0x01, //Circuit-switched only
    eSYS_SRV_DOMAIN_PS_ONLY = 0x02, //Pack-switched only
    eSYS_SRV_DOMAIN_CS_PS   = 0x03, //Pack-switched and Circuit-switched
    eSYS_SRV_DOMAIN_CAMPED  = 0x04, //Camped
    eSYS_SRV_DOMAIN_UNKNOWN
};

/**
 * This structure contains the parameters for Network Time.
 *
 *  \param  year
 *          - Year
 *
 *  \param  month
 *          - Month
 *          - 1 is January and 12 is December
 *
 *  \param  day
 *          - Day
 *          - Range - 1 to 31
 *
 *  \param  hour
 *          - Hour
 *          - Range - 0 to 59
 *
 *  \param  minute
 *          - Minute
 *          - Range - 0 to 59
 *
 *  \param  second
 *          - Second
 *          - Range - 0 to 59
 *
 *  \param  dayOfWeek
 *          - Day of the week
 *          - 0 is Monday and 6 is Sunday
 *
 *  \param  timeZone
 *          - Offset from Universal time
 *          - The difference between local time and Universal time,
 *            in increments of 15 min
 *          - Signed Value
 *
 *  \param  dayLtSavingAdj
 *          - Daylight saving adjustment in hours
 *          - Possible values - 0, 1, and 2.
 *          - This field is ignored if radio_if is NAS_RADIO_IF_CDMA_1XEVDO
 *
 *  \param  radioInterface
 *          - Radio interface from which the information comes
 *          - Values
 *            - 0x01 - NAS_RADIO_IF_CDMA_1X - cdma2000 1X
 *            - 0x02 - NAS_RADIO_IF_CDMA_1XEVDO - cdma2000 HRPD (1xEV-DO)
 *            - 0x04 - NAS_RADIO_IF_GSM - GSM
 *            - 0x05 - NAS_RADIO_IF_UMTS - UMTS
 *            - 0x08 - NAS_RADIO_IF_LTE - LTE
 *            - 0x09 - NAS_RADIO_IF_TDSCDMA -TD-SCDMA
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 *
 */
typedef struct{
    WORD    year;
    BYTE    month;
    BYTE    day;
    BYTE    hour;
    BYTE    minute;
    BYTE    second;
    BYTE    dayOfWeek;
    INT8    timeZone;
    BYTE    dayLtSavingAdj;
    BYTE    radioInterface;
    BYTE    TlvPresent;
}timeInfo;

/**
 *  This structure contains information about the GetNetworkTime response
 *  parameters.
 *
 *  \param  p3GPP2TimeInfo [Optional]
 *          - See \ref timeInfo for more information
 *
 *  \param  p3GPPTimeInfo [Optional]
 *          - See \ref timeInfo for more information
 *
 */
typedef struct
{
    timeInfo  *p3GPP2TimeInfo;
    timeInfo  *p3GPPTimeInfo;
}GetNetworkTimeResp;

/**
 *  This API retrieves the last known network time information from the UE.
 *
 *  \param  pGetNetworkTimeResp[OUT]
 *          - See \ref GetNetworkTimeResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSGetNetworkTime ( GetNetworkTimeResp *pGetNetworkTimeResp );

/**
 * This structure contains the parameters for Rx Signal Info.
 *
 *  \param  rxChainIndex
 *          - Rx antenna path
 *          - Valid Values
 *            - 0 - Primary Rx
 *            - 1 - Diversity Rx
 *
 *  \param  isRadioTuned
 *          - Rx path is tuned to a channel or Not
 *          - Values
 *            - 0x00 - Not tuned
 *            - 0x01 - Tuned
 *            \note   If the radio is tuned, the instantaneous values are set for the fields below.
 *                    If the radio is not tuned, the values set below may be invalid.
 *
 *  \param  rxPower
 *          - Rx power value in 1/10 dBm resolution
 *
 *  \param  rsrp
 *          - Current reference signal received power in 1/10 dBm resolution
 *
 */
typedef struct{
    BYTE      rxChainIndex;
    BYTE      isRadioTuned;
    INT32     rxPower;
    INT32     rsrp;
}RxSigInfo;

/**
 *  This structure contains information about the SccRxInfo parameters.
 *
 *  \param  rsrq
 *          - Current reference signal
 *          - Receive quality in 1/10 dB resolution
 *
 *  \param  snr
 *          - Reference signal signal-to-noise ratio in dB.
 *          - Range -10 to 30
 *
 *  \param  numInstances
 *          - Number of sets of the following
 *            - rxChainIndex
 *            - isRadioTuned
 *            - rxPower
 *            - rsrp
 *
 *  \param  sigInfo
 *          - See \ref RxSigInfo for more information
 *
 *  \param  TlvPresent
 *          - Tlv Present.
 *
 */
typedef struct
{
    INT32     rsrq;
    SHORT     snr;
    BYTE      numInstances;
    RxSigInfo sigInfo[NAS_MAX_SCC_RX_INFO_INSTANCES];
    BYTE      TlvPresent;
}SccRxInfo;

/**
 *  This structure contains information about the SLQSSwiGetLteSccRxInfo response
 *  parameters.
 *
 *  \param  pSccRxInfo [Optional]
 *          - See \ref SccRxInfo for more information
 *
 */
typedef struct
{
    SccRxInfo     *pSccRxInfo;
}LteSccRxInfoResp;

/**
 *  This API retrieves the LTE Secondary carrier Rx signal level information.
 *
 *  \param  pGetLteSccRxInforesp[OUT]
 *          - See \ref GetLteSccRxInforesp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 */
ULONG SLQSSwiGetLteSccRxInfo ( LteSccRxInfoResp *pLteSccRxInfoResp);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_NAS_H__ */
