/**
 * \ingroup wds
 *
 * \file    qaGobiApiWds.h
 *
 * \brief   Wireless Data Service API function prototypes
 */

/*
 * Copyright: © 2011-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_WDS_H__
#define __GOBI_API_WDS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define IPV6_ADDRESS_ARRAY_SIZE 8
/**
 *  Sets the current mobile IP setting.
 *
 *  \param  mode[IN]
 *          - Mobile IP setting
 *              - 0 - Mobile IP off (simple IP only)
 *              - 1 - Mobile IP preferred
 *              - 2 - Mobile IP only
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetMobileIP( ULONG mode );

/**
 *  Returns the current mobile IP setting.
 *
 *  \param  mode[OUT]
 *          - Mobile IP setting
 *              - 0 - Mobile IP off (simple IP only)
 *              - 1 - Mobile IP preferred
 *              - 2 - Mobile IP only
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetMobileIP( ULONG *pMode );

/**
 *  Sets the specified mobile IP parameters.
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing six digit service programming
 *            code.
 *
 *  \param  pMode[IN]
 *          - Mode to be set (optional)
 *              - 0 - Mobile IP off (simple IP only)
 *              - 1 - Mobile IP preferred
 *              - 2 - Mobile IP only
 *
 *  \param  pRetryLimit[IN]
 *          - Registration retry attempt limit (optional)
 *
 *  \param  pRetryInterval[IN]
 *          - Registration retry attempt interval used to determine the time
 *            between registration attempts (optional)
 *
 *  \param  pReRegPeriod[IN]
 *          - Period (in minutes) to attempt re-registration before current
 *            registration expires (optional)
 *
 *  \param  pReRegTraffic[IN]
 *          - Re-registration only if traffic since last attempt (optional)
 *              - Zero    - Disabled
 *              - NonZero - Enabled
 *
 *  \param  pHAAuthenticator[IN]
 *          - MH-HA authenticator calculator (optional)
 *              - Zero    - Disabled
 *              - NonZero - Enabled
 *
 *  \param  pHA2002bis[IN]
 *          - MH-HA RFC 2002bis authentication instead of RFC2002 (optional)
 *              - Zero    - Disabled
 *              - NonZero - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: None\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetMobileIPParameters(
    CHAR  *pSPC,
    ULONG *pMode,
    BYTE  *pRetryLimit,
    BYTE  *pRetryInterval,
    BYTE  *pReRegPeriod,
    BYTE  *pReRegTraffic,
    BYTE  *pHAAuthenticator,
    BYTE  *pHA2002bis );

/**
 *  Sets the auto connect data session setting.
 *
 *  \param  setting[IN]
 *          - NDIS autoconnect setting
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   When enabling, timeout is 5 minutes,\n
 *          When disabling, timeout is 5 seconds
 *
 */
ULONG SetAutoconnect( ULONG setting );

/**
 *  Returns the current auto connect data session setting.
 *
 *  \param  pSetting[OUT]
 *          - NDIS auto connect setting
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetAutoconnect( ULONG *pSetting );

/**
 *  Writes the default profile settings to the device. The default profile is
 *  used to establish an autoconnect data session.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  pPDPType[IN]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile (optional)
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddress[IN]
 *          - Preferred IPv4 addr to be assigned to device (optional)
 *
 *  \param  pPrimaryDNS[IN]
 *          - Primary DNS Ipv4 address preference (optional)
 *
 *  \param  pSecondaryDNS[IN]
 *          - Secondary DNS Ipv4 address preference (optional)
 *
 *  \param  pAuthentication[IN]
 *          - Bitmap that indicates authentication algorithm preference (optional)
 *              - 0x00000001 - PAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g. the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  pName[IN]
 *          - profile Name (optional)
 *
 *  \param  pAPNName[IN]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network (optional)
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested.
 *
 *  \param  pUsername[IN]
 *          - Username used during network authentication (optional)
 *
 *  \param  pPassword[IN]
 *          - Password used during network authentication (optional)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout is 2 seconds.
 *
 */
ULONG  SetDefaultProfile(
    ULONG   profileType,
    ULONG   *pPDPType,
    ULONG   *pIPAddress,
    ULONG   *pPrimaryDNS,
    ULONG   *pSecondaryDNS,
    ULONG   *pAuthentication,
    CHAR    *pName,
    CHAR    *pAPNName,
    CHAR    *pUsername,
    CHAR    *pPassword );
/**
 *  Writes the default profile settings to the device. The default profile is
 *  used to establish an auto connect data session.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  pPDPType[IN]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile (optional)
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddressv4[IN]
 *          - Preferred IPv4 address to be assigned to device (optional)
 *
 *  \param  pPrimaryDNSv4[IN]
 *          - Primary DNS Ipv4 address preference (optional)
 *
 *  \param  pSecondaryDNSv4[IN]
 *          - Secondary DNS Ipv4 address preference (optional)
 *
 *  \param  pIPAddressv6[IN]
 *          - Preferred IPv6 address to be assigned to device (optional)
 *
 *  \param  pPrimaryDNSv6[IN]
 *          - Primary DNS Ipv6 address preference (optional)
 *
 *  \param  pSecondaryDNSv6[IN]
 *          - Secondary DNS Ipv6 address preference (optional)
 *
 *  \param  pAuthentication[IN]
 *          - Bitmap that indicates authentication algorithm preference (optional)
 *              - 0x00000001 - PAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g.the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  pName[IN]
 *          - profile Name (optional)
 *
 *  \param  pAPNName[IN]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network (optional)
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested
 *
 *  \param  pUsername[IN]
 *          - Username used during network authentication (optional)
 *
 *  \param  pPassword[IN]
 *          - Password used during network authentication (optional)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Deprecated, please use SetDefaultProfileLTEV2 instead\n
 *          Technology Supported: LTE\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetDefaultProfileLTE(
    ULONG   profileType,
    ULONG   *pPDPType,
    ULONG   *pIPAddressv4,
    ULONG   *pPrimaryDNSv4,
    ULONG   *pSecondaryDNSv4,
    USHORT  *pIPAddressv6,
    USHORT  *PrimaryDNSv6,
    USHORT  *pSecondaryDNSv6,
    ULONG   *pAuthentication,
    CHAR    *pName,
    CHAR    *pAPNName,
    CHAR    *pUsername,
    CHAR    *pPassword);

/**
 *  Writes the default profile settings to the device. The default profile is
 *  used to establish an auto connect data session.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  pPDPType[IN]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile (optional)
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddressv4[IN]
 *          - Preferred IPv4 address to be assigned to device (optional)
 *
 *  \param  pPrimaryDNSv4[IN]
 *          - Primary DNS Ipv4 address preference (optional)
 *
 *  \param  pSecondaryDNSv4[IN]
 *          - Secondary DNS Ipv4 address preference (optional)
 *
 *  \param  pIPAddressv6[IN]
 *          - Preferred IPv6 addr to be assigned to device (optional)
 *
 *  \param  pPrimaryDNSv6[IN]
 *          - Primary DNS Ipv6 address preference (optional)
 *
 *  \param  pSecondaryDNSv6[IN]
 *          - Secondary DNS Ipv6 address preference (optional)
 *
 *  \param  pAuthentication[IN]
 *          - Bitmap that indicates authentication algorithm preference (optional)
 *              - 0x00000001 - PAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g.the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  pName[IN]
 *          - profile Name (optional)
 *
 *  \param  pAPNName[IN]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network (optional)
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested
 *
 *  \param  pUsername[IN]
 *          - Username used during network authentication (optional)
 *
 *  \param  pPassword[IN]
 *          - Password used during network authentication (optional)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: LTE\n
 *          Timeout: 2 seconds\n
 *          Replaces deprecated Function SetDefaultProfileLTE
 *
 */
ULONG SetDefaultProfileLTEV2(
    ULONG   profileType,
    ULONG   *pPDPType,
    ULONG   *pIPAddressv4,
    ULONG   *pPrimaryDNSv4,
    ULONG   *pSecondaryDNSv4,
    USHORT  *pIPAddressv6,
    USHORT  *PrimaryDNSv6,
    USHORT  *pSecondaryDNSv6,
    ULONG   *pAuthentication,
    CHAR    *pName,
    CHAR    *pAPNName,
    CHAR    *pUsername,
    CHAR    *pPassword);
/**
 *  Reads the default profile settings from the device. The default profile is
 *  used to establish an auto connect data session.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  pPDPType[OUT]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddress[OUT]
 *          - Preferred IPv4 address to be assigned to device
 *
 *  \param  pPrimaryDNS[OUT]
 *          - Primary DNS Ipv4 address preference
 *
 *  \param  pSecondaryDNS[OUT]
 *          - Secondary DNS Ipv4 address preference
 *
 *  \param  pAuthentication[OUT]
 *          - Bitmap that indicates authentication algorithm preference
 *              - 0x00000001 - PAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g. the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator) that
 *            profile name array can contain.
 *
 *  \param  pName[OUT]
 *          - Profile name
 *
 *  \param  apnSize
 *          - Maximum number of characters (including NULL terminator) that APN
 *            name array can contain
 *
 *  \param  pAPNName[OUT]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network.
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested.
 *
 *  \param  userSize
 *          - Maximum number of characters (including NULL terminator) that
 *            username array can contain.
 *
 *  \param  pUsername[OUT]
 *          - Username used during network authentication
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetDefaultProfile(
    ULONG profileType,
    ULONG *pPDPType,
    ULONG *pIPAddress,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pAuthentication,
    BYTE  nameSize,
    CHAR  *pName,
    BYTE  apnSize,
    CHAR  *pAPNName,
    BYTE  userSize,
    CHAR  *pUsername );

/**
 *  Reads the default profile settings from the device. The default profile is
 *  used to establish an auto connect data session.
 *
 *  \param  profileType
 *          - Type of profile\n
 *             - 0 - UMTS
 *
 *  \param  pPDPType[OUT]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddressv4[OUT]
 *          - Preferred IPv4 addr to be assigned to device
 *
 *  \param  pPrimaryDNSv4[OUT]
 *          - Primary DNS Ipv4 address preference
 *
 *  \param  pSecondaryDNSv4[OUT]
 *          - Secondary DNS Ipv4 address preference
 *
 *  \param  pIPAddressv6[OUT]
 *          - Preferred IPv6 addr to be assigned to device
 *            Space for storing 8 element array for the IPv6 addresses
 *            is allocated by the application. The IP Address will be
 *            retrieved in the big endian format. For example
 *            User buffer contents:
 *            [<U0>..<U7>]\n
 *
 *            IPv6 address:\n
 *            1234:2A01:....:5678\n
 *            U0 corresponds to 1234\n
 *            U1 corresponds to 2A01\n
 *            -- ----------- -- ----\n
 *            -- ----------- -- ----\n
 *            U7 corresponds to 5678\n
 *
 *  \param  pPrimaryDNSv6[OUT]
 *          - Primary DNS Ipv6 address preference
 *
 *  \param  pSecondaryDNSv6[OUT]
 *          - Secondary DNS Ipv6 address preference
 *
 *  \param  pAuthentication[OUT]
 *          - Bitmap that indicates authentication algorithm preference\n
 *              - 0x00000001 - PAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed\n
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g. the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator) that
 *            Profile name array can contain
 *
 *  \param  pName[OUT]
 *          - Profile name
 *
 *  \param  apnSize
 *          - Maximum number of characters (including NULL terminator) that APN
 *            name array can contain
 *
 *  \param  pAPNName[IN]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network.
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested.
 *
 *  \param  userSize
 *          - Maximum number of characters including NULL terminator) that
 *            username array can contain.
 *
 *  \param  pUsername[OUT]
 *          - Username used during network authentication
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: LTE\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetDefaultProfileLTE(
    ULONG  profileType,
    ULONG  *pPDPType,
    ULONG  *pIPAddressv4,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    USHORT *pIPAddressv6,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    ULONG  *pAuthentication,
    BYTE   nameSize,
    CHAR   *pName,
    BYTE   apnSize,
    CHAR   *pAPNName,
    BYTE   userSize,
    CHAR   *pUsername );

/**
 *  Returns the state of the current packet data session.
 *
 *  \param  pState[OUT]
 *          - Current link status
 *              - 1 - DISCONNECTED
 *              - 2 - CONNECTED
 *              - 3 - SUSPENDED (not supported)
 *              - 4 - AUTHENTICATING
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetSessionState(
    ULONG    *pState,
    BYTE     instance );

/**
 *  Returns the packet data transfer statistics since the start of the current
 *  packet data.
 *
 *  \param  pTXPacketSuccesses[OUT]
 *          - Number of packets transmitted without error
 *
 *  \param  pRXPacketSuccesses[OUT]
 *          - Number of packets received without error
 *
 *  \param  pTXPacketErrors[OUT]
 *          - Number of outgoing packets with framing errors
 *
 *  \param  pRXPacketErrors[OUT]
 *          - Number of incoming packets with framing errors
 *
 *  \param  pTXPacketOverflows[OUT]
 *          - Number of packets dropped  because Tx buffer overflowed
 *
 *  \param  pRXPacketOverflows[OUT]
 *          - Number of packets dropped because Rx buffer overflowed
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetPacketStatus(
    ULONG    *pTXPacketSuccesses,
    ULONG    *pRXPacketSuccesses,
    ULONG    *pTXPacketErrors,
    ULONG    *pRXPacketErrors,
    ULONG    *pTXPacketOverflows,
    ULONG    *pRXPacketOverflows,
    BYTE     instance );

/**
 *  Returns the Rx/Tx byte counts since the start of the last packet data
 *  session for IPV4 session only.
 *
 *  \param  pTXTotalBytes[OUT]
 *          - Bytes transmitted without error
 *
 *  \param  pRXTotalBytes[OUT]
 *          - Bytes received without error
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds, Rx/Tx byte counts for IPV4 only
 *
 */
ULONG GetByteTotals(
    ULONGLONG *pTXTotalBytes,
    ULONGLONG *pRXTotalBytes,
    BYTE      instance );

/**
 *  Returns the dormancy state of the current packet data session when
 *  connected.
 *
 *  \param  pDormancyState[OUT]
 *          - Dormancy state of current packet data session
 *              - 1 - Traffic channel dormant
 *              - 2 - Traffic channel active
 *
 *  \param  instance
 *          - PDP instance
 *
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetDormancyState(
    ULONG    *pDormancyState,
    BYTE     instance );

/**
 *  Retrieves the current data bearer technology (only valid when connected). 
 *  This API is deprecated on MC73xx/EM73xx modules since firmware version
 *  SWI9X15C_05_xx_xx_xx and all EM74xx firmware versions. Please use API
 *  SLQSGetDataBearerTechnologyExt() for new firmware versions and new modules.
 *
 *  \param  pDataBearer[OUT]
 *          - Data bearer technology\n
 *              - 0x01 - CDMA2000 1x
 *              - 0x02 - CDMA 1xEV-DO Rev 0
 *              - 0x03 - GSM
 *              - 0x04 - UMTS
 *              - 0x05 - CDMA2000 HRPD (1xEV-DO Rev A)
 *              - 0x06 - EDGE
 *              - 0x07 - HSDPA AND WCDMA
 *              - 0x08 - WCDMA AND HSUPA
 *              - 0x09 - HSDPA AND HSUPA
 *              - 0x0A - LTE
 *              - 0x0B - CDMA2000 EHRPD
 *              - 0x0C - HSDPA+ and WCDMA
 *              - 0x0D - HSDPA+ and HSUPA
 *              - 0x0E - DC_HSDPA+ and WCDMA
 *              - 0x0F - DC_HSDPA+ and HSUPA
 *              - 0x10 - HSDPA+ and 64QAM
 *              - 0x11 - HSDPA+, 64QAM and HSUPA
 *              - 0x12 - TDSCDMA
 *              - 0x13 - TDSCDMA and HSDPA
 *              - 0xFF - Unknown
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   The QMI command of this API is deprecated. Use SLQSGetDataBearerTechnologyExt()
 *          for new modules (chipset 9x15, 9x30).
 *          Version Introduced: Major-1, Minor-12
 *          Version Deprecated: Major-1, Minor-40
 *          Timeout: 2 seconds
 *
 */
ULONG GetDataBearerTechnology(
    ULONG    *pDataBearer,
    BYTE     instance );

/**
 *  Structure to hold the current data bearer technology values
 *
 *  \param  pCurrentNetwork[OUT]
 *          - current selected network\n
 *              - 0 - UNKNOWN
 *              - 1 - 3GPP2
 *              - 2 - 3GPP
 *
 *  \param  pRatMask[OUT]
 *          - Radio Access Technology (RAT) mask to indicate the type of
 *            technology (RAT mask value of zero indicates that this field
 *             is ignored)
 *              - 0x8000 - NULL Bearer
 *              - 0x0000 - DO_NOT_CARE
 *            CDMA RAT mask
 *              - 0x01 - CDMA_1X
 *              - 0x02 - EVDO_REV0
 *              - 0x04 - EVDO_REVA
 *            UMTS RAT mask
 *              - 0x01 - WCDMA
 *              - 0x02 - GPRS
 *              - 0x04 - HSDPA
 *              - 0x08 - HSUPA
 *              - 0x10 - EDGE
 *              - 0x20 - LTE
 *              - 0x40 - HSDPA+
 *              - 0x80 - DC_HSDPA+
 *
 *  \param  pSoMask[OUT]
 *          - Service Option (SO) mask to indicate the SO or type of application
 *            (SO mask value of zero indicates that this field is ignored)
 *              -  0x00 - DO_NOT_CARE
 *            CDMA 1X SO mask
 *              - 0x01 - CDMA_1X_IS95
 *              - 0x02 - CDMA_1X_IS2000
 *              - 0x04 - CDMA_1X_IS2000_REL_A
 *            CDMA EV-DO Rev A SO mask
 *              - 0x01 - EVDO_REVA_DPA
 *              - 0x02 - EVDO_REVA_MFPA
 *              - 0x04 - EVDO_REVA_EMPA
 *              - 0x08 - EVDO_REVA_EMPA_EHRPD
 */
typedef struct dataBearerTechnology
{
    BYTE  currentNetwork;
    ULONG ratMask;
    ULONG soMask;
} QmiWDSDataBearerTechnology;

/**
 *  Bit mask values to indicate the presence of data bearer information for the
 *  current and last data calls
 */
enum qmiDataBearerMasks
{
    QMI_WDS_CURRENT_CALL_DB_MASK = 0x01,
    QMI_WDS_LAST_CALL_DB_MASK    = 0x02
};

/**
 *  Structure to hold the data bearer technology values
 *
 *  \param  dataBearerMask[OUT]
 *          - This bit mask indicates if data bearer information for the current
 *            and/or last call has been received from the device. If a bit is
 *            set, then the information is available in the corresponding
 *            structure i.e. the one provided by the caller.
 *            Refer to \ref qmiDataBearerMasks for bit-mask positions.
 *
 *  \param  pCurDataBearerTechnology[OUT]
 *          - current data bearer technology value.
 *              - NULL if the parameter is not required
 *
 *  \param  pLastCallDataBearerTechnology[OUT]
 *          - last call data bearer technology value.
 *              - NULL if the parameter is not required
 */
typedef struct dataBearers
{
    BYTE dataBearerMask;
    QmiWDSDataBearerTechnology *pCurDataBearerTechnology;
    QmiWDSDataBearerTechnology *pLastCallDataBearerTechnology;
} QmiWDSDataBearers;

/**
 *  Retrieves the data bearer technology values for current and/or last data
 *  calls. The device must be in a data call for this function to execute
 *  successfully.
 *
 *  \param  qmiWDSDataBearers[OUT]
 *          - Indicates the current and the last call data bearer technology.
 *            Should not be NULL, on input.
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750,GOBI,MC7700\n
 *          Timeout: 2 seconds
 */
ULONG SLQSGetDataBearerTechnology(
    QmiWDSDataBearers *pDataBearers,
    BYTE              instance );

/**
 *  Returns the duration of the current packet data session.
 *
 *  \param  pDuration[OUT]
 *          - Duration of the current packet session in milliseconds
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetSessionDuration(
    ULONGLONG    *pDuration,
    BYTE         instance );

/**
 *  \param  pIPAddressV4[OUT]
 *          - Current IPv4 address
 *          - default value of 0 if not reported by the device.
 *
 *  \param  pIPAddressV6[OUT]
 *          - Current IPv6 address\n
 *            Space for storing the 8 element array of type USHORT for the IPv6
 *            address is allocated by the application.\n
 *            The IP Address is stored in the user supplied buffer as follows:\n
 *            User buffer:\n
 *            [<U0>..<U7>]\n
 *            IPv6 address from the network:\n
 *            1234:2A01:....:5678\n
 *            User buffer contents:\n
 *            U0 corresponds to 1234\n
 *            U1 corresponds to 2A01\n
 *            -- ----------- -- ----\n
 *            -- ----------- -- ----\n
 *            U7 corresponds to 5678
 *
 *  \param  pIPv6prefixlen[OUT]
 *          - IPv6 prefix length in number of bits
 */
typedef struct{
    ULONG *pIPAddressV4;
    USHORT *pIPAddressV6;
    BYTE  *pIPv6prefixlen;
}QmiWdsIpAddressInfo;


/**
 *  \param  pv4sessionId[OUT]
 *          - IPv4 session id
 *
 *  \param  pv6sessionId[OUT]
 *          - IPv6 session id
 *
 *  \param  ip[OUT]
 *          - IP address information see \ref QmiWdsIpAddressInfo for more details
 */
typedef struct{
    ULONG *pv4sessionId;
    ULONG *pv6sessionId;
    QmiWdsIpAddressInfo ip;
}WdsIpAddressInfoReq;

/**
 *  Returns the current packet data session IP address(es)
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: LTE\n
 *          Timeout: 2 seconds.
 *
 */
ULONG GetIPAddressLTE( WdsIpAddressInfoReq *);

/**
 *  Returns connection rate information for the packet data connection. This API
 *  is not applicable when multiple data session is up. For multiple PDN, please use
 *  API SLQSGetConnectionRate()
 *
 *  \param  pCurrentChannelTXRate[OUT]
 *          - Current channel Tx rate (in bps)
 *
 *  \param  pCurrentChannelRXRate[OUT]
 *          - Current channel Rx rate (in bps)
 *
 *  \param  pMaxChannelTXRate[OUT]
 *          - Maximum Tx rate (bps) that may be assigned  to device by serving
 *            system.
 *
 *  \param  pMaxChannelRXRate[OUT]
 *          - Maximum Rx rate (bps) that may be assigned to device by serving
 *            system.
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG GetConnectionRate(
    ULONG *pCurrentChannelTXRate,
    ULONG *pCurrentChannelRXRate,
    ULONG *pMaxChannelTXRate,
    ULONG *pMaxChannelRXRate,
    BYTE  instance );

/**
 *  Returns the specified mobile IP profile settings.
 *
 *  \param  index
 *          - Mobile IP profile ID
 *
 *  \param  pEnabled[OUT]
 *          - Profile enabled:
 *              - 0 - Disabled
 *              - 1 - Enabled
 *              - 0xFF - Unknown
 *
 *  \param  pAddress[OUT]
 *          - Home IPv4 address:
 *             - 0xFFFFFFFF - Unknown
 *
 *  \param  pPrimaryHA[OUT]
 *          - Primary home agent IPv4 address
 *             - 0xFFFFFFFF - Unknown
 *
 *  \param  pSecondaryHA[OUT]
 *          - Secondary home agent IPv4 address
 *             - 0xFFFFFFFF - Unknown
 *
 *  \param  pRevTunneling[OUT]
 *          - Reverse tunneling enabled
 *              - 0 - Disabled
 *              - 1 - Enabled
 *              - 0xFF - Unknown
 *
 *  \param  naiSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the NAI array can contain.
 *
 *  \param  pNAI[OUT]
 *          - Network access identifier string
 *
 *  \param  pHASPI[OUT]
 *          - Home agent security parameter index
 *
 *  \param  pAAASPI[OUT]
 *          - AAA server security parameter index
 *              - 0xFFFFFFFF -  Unknown
 *
 *  \param  pHAState[OUT]
 *          - Home agent key state
 *              - 0 - Unset
 *              - 1 - Set, default value
 *              - 2 - Set, modified from default
 *              - 3 - 0xFFFFFFFF - Unknown
 *
 *  \param  pAAAState[OUT]
 *          - AAA key state
 *              - 0 - Unset
 *              - 1 - Set, default value
 *              - 2 - Set, modified from default
 *              - 3 - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetMobileIPProfile(
   BYTE  index,
   BYTE  *pEnabled,
   ULONG *pAddress,
   ULONG *pPrimaryHA,
   ULONG *pSecondaryHA,
   BYTE  *pRevTunneling,
   BYTE  naiSize,
   CHAR  *pNAI,
   ULONG *pHASPI,
   ULONG *pAAASPI,
   ULONG *pHAState,
   ULONG *pAAAState );

/**
 *  Returns the last mobile IP error.
 *
 *  \param  pError[OUT]
 *          - Status of last MIP call (or attempt)
 *              - Zero - Success
 *              - NonZero - Error code \n
 *              See \ref qaGobiApiTableCallEndReasons.h for Mobile IP
 *              Error codes
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetLastMobileIPError( ULONG *pError );

/*
 *  This API used internally for Setting IP Family Preference
 *
 *  \param  IPFamilyPreference[IN]
 *          - IP Family preference
 *
 *  \param  instance[IN]
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 */
ULONG iSLQSMISetIPFamilyPreference(
    BYTE IPFamilyPreference,
    BYTE instance );

/*
 *  This API used internally for checking if a device is a Gobi device
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 */
BOOL WDS_IsGobiDevice( );

/**
 *  Sets active mobile IP profile.
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing six digit service
 *          programming code
 *
 *  \param  index[IN]
 *          - Index of the profile to be set as the active profile
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetActiveMobileIPProfile(
    CHAR *pSPC,
    BYTE index );

/**
 *  Sets the mobile IP parameters.
 *
 *  \param  pSPC[IN]
 *          - Six digit service programming code string
 *
 *  \param  index[IN]
 *          - Index of the profile to modify
 *
 *  \param  pEnabled[IN]
 *          - (Optional) Enable profile?
 *           0       - Disabled
 *           Nonzero - Enabled
 *
 *  \param  pAddress[IN]
 *          - (Optional) Home IPv4 address
 *
 *  \param  pPrimaryHA[IN]
 *          - (Optional) Primary home agent IPv4 address
 *
 *  \param  pSecondaryHA[IN]
 *          - (Optional) Secondary home agent IPv4 address
 *
 *  \param  pRevTunneling[IN]
 *          - (Optional) Enable reverse tunneling?
 *           0       - Disabled
 *           Nonzero - Enabled
 *
 *  \param  pNAI[IN]
 *          - (Optional) Network access identifier string
 *
 *  \param  pHASPI[IN]
 *          - (Optional) Home agent security parameter index
 *
 *  \param  pAAASPI[IN]
 *          - (Optional) AAA server security parameter index
 *
 *  \param  pMNHA[IN]
 *          - (Optional) MN-HA key string
 *
 *  \param  pMNAAA[IN]
 *          - (Optional) MN-AAA key string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetMobileIPProfile(
    CHAR  *pSPC,
    BYTE  index,
    BYTE  *pEnabled,
    ULONG *pAddress,
    ULONG *pPrimaryHA,
    ULONG *pSecondaryHA,
    BYTE  *pRevTunneling,
    CHAR  *pNAI,
    ULONG *pHASPI,
    ULONG *pAAASPI,
    CHAR  *pMNHA,
    CHAR  *pMNAAA );

/**
 * This structure contains the UMTS Quality Of Service Information
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  trafficClass
 *          - 0x00 - Subscribed
 *          - 0x01 - Conversational
 *          - 0x02 - Streaming
 *          - 0x03 - Interactive
 *          - 0x04 - Background
 *
 *  \param  maxUplinkBitrate
 *          - Maximum uplink bit rate in bits/sec
 *
 *  \param  maxDownlinkBitrate
 *          - Maximum downlink bit rate in bits/sec
 *
 *  \param  grntUplinkBitrate
 *          - Guaranteed uplink bit rate in bits/sec
 *
 *  \param  grntDownlinkBitrate
 *          - Guranteed downlink bit rate in bits/sec
 *
 *  \param  qosDeliveryOrder - Qos delivery order
 *          - 0x00 - Subscribe
 *          - 0x01 - delivery order on
 *          - 0x02 - delivery order off
 *
 *  \param  maxSDUSize
 *          - Maximum SDU size
 *
 *  \param  sduErrorRatio - SDU error ratio
 *          - Target value for fraction of SDUs lost or
 *            detected as erroneous.
 *          - 0x00 - Subscribe
 *          - 0x01 - 1*10^(-2)
 *          - 0x02 - 7*10^(-3)
 *          - 0x03 - 1*10^(-3)
 *          - 0x04 - 1*10^(-4)
 *          - 0x05 - 1*10^(-5)
 *          - 0x06 - 1*10^(-6)
 *          - 0x07 - 1*10^(-1)
 *
 *  \param  resBerRatio - Residual bit error ratio
 *          - Target value for undetected bit error ratio in
 *            in the delivered SDUs.
 *          - 0x00 - Subscribe
 *          - 0x01 - 5*10^(-2)
 *          - 0x02 - 1*10^(-2)
 *          - 0x03 - 5*10^(-3)
 *          - 0x04 - 4*10^(-3)
 *          - 0x05 - 1*10^(-3)
 *          - 0x06 - 1*10^(-4)
 *          - 0x07 - 1*10^(-5)
 *          - 0x08 - 1*10^(-6)
 *          - 0x09 - 1*10^(-8)
 *
 *  \param  deliveryErrSDU - Delivery of erroneous SDUs
 *          - Indicates whether SDUs detected as erroneous shall be
 *            delivered or not.
 *          - 0x00 - Subscribe
 *          - 0x01 - 5*10^(-2)
 *          - 0x02 - 1*10^(-2)
 *          - 0x03 - 5*10^(-3)
 *          - 0x04 - 4*10^(-3)
 *          - 0x05 - 1*10^(-3)
 *          - 0x06 - 1*10^(-4)
 *          - 0x07 - 1*10^(-5)
 *          - 0x08 - 1*10^(-6)
 *          - 0x09 - 1*10^(-8)
 *
 *  \param  transferDelay - Transfer delay (ms)
 *          - Indicates the targeted time between a request to transfer an
 *            SDU at one SAP to its delivery at the other SAP in milliseconds.
 *
 *  \param  trafficPriority - Transfer handling priority
 *          - Specifies the relative importance for handling of SDUs that
 *            belong to the UMTS bearer, compared to the SDUs of other bearers.
 *
 */
struct UMTSQoS
{
    BYTE  trafficClass;
    ULONG maxUplinkBitrate;
    ULONG maxDownlinkBitrate;
    ULONG grntUplinkBitrate;
    ULONG grntDownlinkBitrate;
    BYTE  qosDeliveryOrder;
    ULONG maxSDUSize;
    BYTE  sduErrorRatio;
    BYTE  resBerRatio;
    BYTE  deliveryErrSDU;
    ULONG transferDelay;
    ULONG trafficPriority;
};

/**
 * This structure contains the Profile Identifier Information
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  profileType
 *          - Identifies the type of profile
 *            0x00 = 3GPP
 *
 *  \param  profileIndex
 *          - Index of profile whose settings were loaded prior to
 *            session parameter negotiation for the current call.
 *            If this TLV is not present, data call parameters are
 *            based on device default settings for each parameter
 *
 */
struct ProfileIdentifier
{
    BYTE profileType;
    BYTE profileIndex;
};

/**
 * This structure contains the GPRS Quality Of Service Information
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  precedenceClass
 *          - Precedence class
 *
 *  \param  delayClass
 *          - Delay class
 *
 *  \param  reliabilityClass
 *          - Reliability class
 *
 *  \param  peakThroughputClass
 *          - Peak throughput class
 *
 *  \param  meanThroughputClass
 *          - Mean throughput class
 *
 */
struct GPRSQoS
{
    ULONG precedenceClass;
    ULONG delayClass;
    ULONG reliabilityClass;
    ULONG peakThroughputClass;
    ULONG meanThroughputClass;
};

/**
 * This structure contains the PCSCFIPv4ServerAddressList Information
 *
 *  \param  numInstances
 *          - number of address following
 *
 *  \param  pscsfIPv4Addr
 *          - P-CSCF IPv4 server addresses(Max 16 address, 4 bytes each)
 */
struct PCSCFIPv4ServerAddressList
{
    BYTE  numInstances;
    ULONG pscsfIPv4Addr[64];
};

/**
 * This structure contains the PCSCFFQDNAddress Information
 *
 *  \param  fqdnLen
 *          - length of the received FQDN address
 *
 *  \param  fqdnAddr
 *          - FQDN address(Max 256 characters)
 */
struct PCSCFFQDNAddress
{
    WORD fqdnLen;
    CHAR fqdnAddr[256];
};

/**
 * This structure contains the PCSCFFQDNAddressList Information
 *
 *  \param  numInstances
 *          - Number of FQDN addresses received
 *
 *  \param  pcsfFQDNAddress
 *          - FQDN address information(Max 10 addresses)
 */
struct PCSCFFQDNAddressList
{
    BYTE                    numInstances;
    struct PCSCFFQDNAddress pcsfFQDNAddress[10];
};

/**
 * This structure contains the DomainName Information
 *
 *  \param  domainLen
 *          - length of the received Domain name
 *
 *  \param  domainName
 *          - Domain name(Max 256 characters)
 */
struct Domain
{
    WORD domainLen;
    CHAR domainName[256];
};

/**
 * This structure contains the DomainNameList Information
 *
 *  \param  numInstances
 *          - Number of Domain name received
 *
 *  \param  domain
 *          - Domain name information(Max 10 Domain names)
 */
struct DomainNameList
{
    BYTE          numInstances;
    struct Domain domain[10];
};

/**
 * This structure contains the IPV6 Address Information
 *
 *  \param  IPV6PrefixLen
 *          - Length of the received IPv6 address in no. of bits;
 *            can take value between 0 and 128
 *              - 0xFF - Not Available
 *
 *  \param  IPAddressV6
 *          - IPv6 address(in network byte order);
 *            This is an 8-element array of 16 bit numbers,
 *            each of which is in big endian format.
 */
struct IPV6AddressInfo
{
    BYTE   IPV6PrefixLen;
    USHORT IPAddressV6[IPV6_ADDRESS_ARRAY_SIZE];
};

/**
 * This structure contains the IPV6 Gateway Address Information
 *
 *  \param  gwV6PrefixLen
 *          - Length of the received IPv6 Gateway address in no. of bits;
 *            can take value between 0 and 128
 *
 *  \param  IPAddressV6
 *          - IPv6 Gateway address(in network byte order);
 *            This is an 8-element array of 16 bit numbers,
 *            each of which is in big endian format.
 */
struct IPV6GWAddressInfo
{
    BYTE   gwV6PrefixLen;
    USHORT gwAddressV6[8];
};

/**
 * This structure contains the WdsRunTimeSettings Information
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  pProfileName
 *          - Profile name\n
 *          One or more bytes describing the profile.
 *          Description may be a user-defined name for the profile.\n
 *          QMI_ERR_ARG_TOO_LONG is returned if profile_name is too long.
 *
 *  \param  pPDPType
 *          - PDP type\n
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *              - 0xffffffff - invalid
 *
 *  \param  pAPNName
 *          - Access point name\n
 *          String parameter that is a logical name used to select the GGSN and
 *          external packet data network.\n
 *          If value is NULL or omitted, then the subscription default value is requested.\n
 *          QMI_ERR_ARG_TOO_LONG is returned if the APN name is too long.
 *
 *  \param  pPrimaryDNSV4
 *          - Primary DNS IPv4 Address
 *
 *  \param  pSecondaryDNSV4
 *          - Secondary DNS IPv4 Address
 *
 *  \param  pUMTSGrantedQoS
 *          - UMTS Granted QoS
 *
 *  \param  pGPRSGrantedQoS
 *          - GPRS Granted QoS
 *
 *  \param  pUsername
 *          - User name used during data network authentication
 *
 *  \param  pAuthentication
 *          - Authentication preference
 *              - Bit 0 - PAP preference
 *                  - 0 - PAP is never performed
 *                  - 1 - PAP may be performed
 *              - Bit 1 - CHAP preference
 *                  - 0 - CHAP is never performed
 *                  - 1 - CHAP may be performed
 *
 *  \param  pIPAddressV4
 *          - IPV4 Address assigned to the TE
 *
 *  \param  pProfileID
 *          - Profile Identifier
 *
 *  \param  pGWAddressV4
 *          - IPV4 Gateway Address
 *
 *  \param  pSubnetMaskV4
 *          - IPV4 Subnet Mask
 *
 *  \param  pPCSCFAddrPCO
 *          - PCSCF address using PCO values
 *              - 1 - (TRUE)  implies request PCSCF address using PCO
 *              - 0 - (FALSE) implies do not request. This is the default value.
 *
 *  \param  pServerAddrList
 *          - P-CSCF IPv4 Server Address List
 *
 *  \param  pPCSCFFQDNAddrList
 *          - P-CSCF FQDN Address List
 *
 *  \param  pPrimaryDNSV6
 *          - Primary DNS IPv6 Address
 *
 *  \param  pSecondaryDNSV6
 *          - Secondary DNS IPv6 Address
 *
 *  \param  pMtu
 *          - MTU
 *
 *  \param  pDomainList
 *          - Domain-Name List
 *
 *  \param  pIPFamilyPreference
 *          - IP family
 *              - 0x04 - IPV4 ADDR
 *              - 0x06 - IPV6 ADDR
 *
 *  \param  pIMCNflag
 *          - IM CN Flag
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 *  \param  pTechnology
 *          - Technology
 *              - CDMA - 0x8001
 *              - UMTS - 0x8004
 *
 *  \param  pIPV6AddressInfo
 *          - IPV6 Address Information
 *
 *  \param  pIPV6GWAddressInfo
 *          - IPV6 Gateway Address Information
 *
 */
struct qmiWdsRunTimeSettings
{
    CHAR                              *pProfileName;
    ULONG                             *pPDPType;
    CHAR                              *pAPNName;
    ULONG                             *pPrimaryDNSV4;
    ULONG                             *pSecondaryDNSV4;
    struct UMTSQoS                    *pUMTSGrantedQoS;
    struct GPRSQoS                    *pGPRSGrantedQoS;
    CHAR                              *pUsername;
    ULONG                             *pAuthentication;
    ULONG                             *pIPAddressV4;
    struct ProfileIdentifier          *pProfileID;
    ULONG                             *pGWAddressV4;
    ULONG                             *pSubnetMaskV4;
    BYTE                              *pPCSCFAddrPCO;
    struct PCSCFIPv4ServerAddressList *pServerAddrList;
    struct PCSCFFQDNAddressList       *pPCSCFFQDNAddrList;
    USHORT                            *pPrimaryDNSV6;
    USHORT                            *pSecondaryDNSV6;
    ULONG                             *pMtu;
    struct DomainNameList             *pDomainList;
    BYTE                              *pIPFamilyPreference;
    BYTE                              *pIMCNflag;
    WORD                              *pTechnology;
    struct IPV6AddressInfo            *pIPV6AddrInfo;
    struct IPV6GWAddressInfo          *pIPV6GWAddrInfo;
};

/**
 * WDS runtime settings request data structure
 *
 *  \param  v4sessionId
 *          - The v4 session ID for which the runtime settings are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param  v6sessionId
 *          - The v6 session ID for which the runtime settings are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param   qmiWdsRunTimeSettings
 *          - data structure to be populated with the runtime settings
 *
 *  \note   At least one of v4sessionId and v6sessionId must point to a valid
 *          session ID.
 *
 */
struct WdsRunTimeSettings
{
    ULONG  *v4sessionId;
    ULONG  *v6sessionId;
    struct qmiWdsRunTimeSettings rts;
};

/**
 *  Returns the packet data session settings currently in use.
 *
 *  \param  pRunTimeSettings[OUT]
 *          - SLQS Runtime Settings Information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSGetRuntimeSettings (
    struct WdsRunTimeSettings *pRunTimeSettings );

/**
 *  Writes the profile settings for the specified profile Id.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  profileId
 *          - Profile number to be modified
 *               - Value between 1 - 16 (EM/MC73xx or earlier)
 *               - Value between 1 - 24 (EM/MC74xx onwards)
 *
 *  \param  pPDPType[IN]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile (optional)
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddress[IN]
 *          - Preferred IPv4 address to be assigned to device (optional)
 *
 *  \param  pPrimaryDNS[IN]
 *          - Primary DNS Ipv4 address preference (optional)
 *
 *  \param  pSecondaryDNS[IN]
 *          - Secondary DNS Ipv4 address preference (optional)
 *
 *  \param  pAuthentication[IN]
 *          - Bitmap that indicates authentication algorithm preference (optional)
 *              - 0x00000001 - PAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference\n
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g. the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  pName[IN]
 *          - profile Name (optional)
 *
 *  \param  pAPNName[IN]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network (optional)
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested.
 *
 *  \param  pUsername[IN]
 *          - Username used during network authentication (optional)
 *
 *  \param  pPassword[IN]
 *          - Password used during network authentication (optional)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout is 2 seconds.
 *
 */
ULONG SLQSSetProfile(
    ULONG   profileType,
    BYTE    profileId,
    ULONG   *pPDPType,
    ULONG   *pIPAddress,
    ULONG   *pPrimaryDNS,
    ULONG   *pSecondaryDNS,
    ULONG   *pAuthentication,
    CHAR    *pName,
    CHAR    *pAPNName,
    CHAR    *pUsername,
    CHAR    *pPassword );

/**
 *  Reads the profile settings from the device for the specified profile id.
 *
 *  \param  profileType
 *          - Type of profile
 *              - 0 - UMTS
 *
 *  \param  profileId
 *          - Index of the configured profile for which settings are read
 *               - Value between 1 - 16 (EM/MC73xx or earlier)
 *               - Value between 1 - 24 (EM/MC74xx onwards)
 *
 *  \param  pPDPType[OUT]
 *          - Packet Data Protocol (PDP) type specifies the type of data payload
 *            exchanged over the air link when the packet data session is
 *            established with this profile
 *              - 0 - PDP-IP (IPv4)
 *              - 1 - PDP-PPP
 *              - 2 - PDP-IPv6
 *              - 3 - PDP-IPv4v6
 *
 *  \param  pIPAddress[OUT]
 *          - Preferred IPv4 address to be assigned to device
 *
 *  \param  pPrimaryDNS[OUT]
 *          - Primary DNS Ipv4 address preference
 *
 *  \param  pSecondaryDNS[OUT]
 *          - Secondary DNS Ipv4 address preference
 *
 *  \param  pAuthentication[OUT]
 *          - Bitmap that indicates authentication algorithm preference
 *              - 0x00000001 - PAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - 0x00000002 - CHAP preference
 *                  - 0 - Never performed
 *                  - 1 - May be performed
 *              - All other bits are reserved and must be set to 0
 *              - If more than 1 bit is set, then device decides which
 *                authentication procedure is performed while setting up data
 *                session e.g. the device may have a policy to select the most
 *                secure authentication mechanism.
 *
 *  \param  nameSize
 *          - Maximum number of characters (including NULL terminator) that
 *            profile name array can contain.
 *
 *  \param  pName[OUT]
 *          - Profile name
 *
 *  \param  apnSize
 *          - Maximum number of characters (including NULL terminator) that APN
 *            name array can contain
 *
 *  \param  pAPNName[OUT]
 *          - Access point name. NULL-terminated string parameter that is a
 *            logical name used to select GGSN and external packet data
 *            network.
 *          - If value is NULL or omitted, then subscription default value will
 *            be requested.
 *
 *  \param  userSize
 *          - Maximum number of characters (including NULL terminator) that
 *            username array can contain.
 *
 *  \param  pUsername[OUT]
 *          - Username used during network authentication
 *
 *  \param  pExtendedErrorCode
 *          - The extended error code received from DS Profile subsystem of type
 *            eWDS_ERR_PROFILE_REG_xxx.
 *          - Error code will only will be present if error code
 *            eQCWWAN_ERR_QMI_EXTENDED_INTERNAL is returned by device.
 *          - See \ref qm_wds_ds_profile_extended_err_codes enum in qmerrno.h
 *            for received error description.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSGetProfile(
    ULONG profileType,
    BYTE  profileId,
    ULONG *pPDPType,
    ULONG *pIPAddress,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pAuthentication,
    BYTE  nameSize,
    CHAR  *pName,
    BYTE  apnSize,
    CHAR  *pAPNName,
    BYTE  userSize,
    CHAR  *pUsername,
    WORD  *pExtendedErrorCode );

/**
 * This structure contains the start/stop data session params Information
 *
 *  \param  action
 *          - 1 - Start Session
 *          - 0 - Stop Session
 *
 *  \param  instanceId
 *          - PDP Instance.
 *          - Instance ID corresponding to the session ID
 *
 *  \param  pTechnology
 *          - Indicates the technology preference (optional)
 *              - 1 - UMTS
 *              - 2 - CDMA
 *              - 3 - eMBMS
 *              - 4 - Modem Link Label.
 *                    Modem Link is an interface for transferring data between
 *                    entities on AP and modem.
 *
 *  \param  pProfileId3GPP
 *          - configured 3GPP profile identifier
 *
 *  \param  pProfileId3GPP2
 *          - configured 3GPP2 profile identifier
 *
 *  \param  sessionId[IN\OUT]
 *           - [IN]  - Passed session ID when stopping the data session
 *           - [OUT] - Assigned session ID when starting a data session
 *
 *  \param  failureReason
 *          - Reason data session failed to be established
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason
 *
 *  \param  failureReasonv4
 *          - Reason v4 data session failed to be established
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason
 *
 *  \param  failureReasonv6
 *          - Reason v6 data session failed to be established
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason
 *
 *  \param  rc4
 *          - v4 result code
 *          - See qmerrno.h
 *
 *  \param  rc6
 *          - v6 result code
 *          - See qmerrno.h
 *
 *  \param  v4sessionId
 *          - Do not modify - used for internal management of data sessions
 *          - Non zero value indicates that a session is active
 *
 *  \param  v6sessionId
 *          - Do not modify - used for internal management of data sessions
 *          - Non zero value indicates that a session is active
 *
 *  \param  ipfamily
 *          - 4 for an IPv4 data session
 *          - 6 for an IPv6 data session
 *          - 7 for an IPv4v6 data session
 *
 *  \param  pAuthentication
 *          - Authentication type, it can be PAP or CHAP
 *
 *  \param  pUsername
 *          - username for authentication process
 *
 *  \param  pPassword
 *          - password for authentication process
 *
 *  \param  verbFailReasonType
 *          - Parameter describing type of verbose failure reason
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason Type
 *
 *  \param  verbFailReason
 *          - Verbose reason explaining why call failed. Depends on
 *            verbFailReasonType parameter
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason
 */
struct ssdatasession_params{
    BOOL  action;
    BYTE  instanceId;
    ULONG *pTechnology;
    ULONG *pProfileId3GPP;
    ULONG *pProfileId3GPP2;
    ULONG sessionId;
    ULONG failureReason;
    ULONG failureReasonv4;
    ULONG failureReasonv6;
    ULONG rcv4;
    ULONG rcv6;
    ULONG v4sessionId;
    ULONG v6sessionId;
    BYTE  ipfamily;
    ULONG *pAuthentication;
    CHAR  *pUsername;
    CHAR  *pPassword;
    ULONG verbFailReasonType;
    ULONG verbFailReason;
};

/**
 *  Starts or stops a 3GPP/3GPP2 data session on a preconfigured profile.
 *  To set the IP family for the data session, execute SLQSSetIPFamilyPreference
 *  prior to calling this API.
 *
 *  \param  pin[IN]
 *          - ssdatasession_params structure
 *          - See \ref ssdatasession_params for more details
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Minutes\n
 *          Use \ref SLQSSetProfile to configure 3GPP profiles
 *
 */
ULONG SLQSStartStopDataSession(
    struct ssdatasession_params *pin );

/**
 * This structure contains the information about the profile to be deleted.
 *
 *  \param  profileType
 *          - Identifies the type of profile
 *            - 0x00 - 3GPP
 *          - Note: Deletion of 3GPP2 profiles is not supported.
 *
 *  \param  profileIndex
 *          - Index of the configured profile to be deleted
 *               - Value between 1 - 16 (EM/MC73xx or earlier)
 *               - Value between 1 - 24 (EM/MC74xx onwards)
 *
 */
struct SLQSDeleteProfileParams{
    BYTE profileType;
    BYTE profileIndex;
};

/**
 *  Deletes a configured profile stored on the device. The deletion of a
 *  profile does not affect profile index assignments.
 *
 *  \param  pProfileToDelete[IN]
 *          - Information about the profile to be deleted.
 *          - See \ref SLQSDeleteProfileParams  for more details.
 *
 *  \param  pExtendedErrorCode[OUT]
 *          - The extended error code received from DS Profile subsystem of type
 *            eWDS_ERR_PROFILE_REG_xxx.
 *          - Error code will only will be present if error code
 *            eQCWWAN_ERR_QMI_EXTENDED_INTERNAL is returned by device.
 *          - See \ref qm_wds_ds_profile_extended_err_codes enum in qmerrno.h
 *            for received error description.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values.
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSDeleteProfile(
    struct SLQSDeleteProfileParams *pProfileToDelete,
    WORD                           *pExtendedErrorCode );

/**
 * This structure contains Input parameters of SLQSCreateProfile
 * and SLQSModifyProfile and output parameters of SLQSGetProfileSettings
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  pProfileName
 *          - One or more bytes describing the profile
 *
 *  \param  pProfilenameSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pProfileName field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pPDPType
 *          - Packet Data Protocol (PDP) type specifies the
 *            type of data payload exchanged over the air link
 *            when the packet data session is established with
 *            this profile
 *            - 0x00 - PDP-IP (IPv4)
 *            - 0x01 - PDP-PPP
 *            - 0x02 - PDP-IPV6
 *            - 0x03 - PDP-IPV4V6
 *
 *  \param  pPdpHdrCompType
 *          - PDP header compression type
 *            - 0 - PDP header compression is OFF
 *            - 1 - Manufacturer preferred compression
 *            - 2 - PDP header compression based on RFC 1144
 *            - 3 - PDP header compression based on RFC 25074
 *                  PDP header compression based on RFC 3095
 *
 *  \param  pPdpDataCompType
 *          - PDP data compression type
 *            - 0 - PDP data compression is OFF
 *            - 1 - Manufacturer preferred compression
 *            - 2 - V.42BIS data compression
 *            - 3 - V.44 data compression
 *
 *  \param  pAPNName
 *          - Access point name
 *
 *  \param  pAPNnameSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pAPNName field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pPriDNSIPv4AddPref
 *          - Primary DNS IPv4 Address Preference
 *
 *  \param  pSecDNSIPv4AddPref
 *          - Secondary DNS IPv4 Address Preference
 *
 *  \param  pUMTSReqQoS
 *          - UMTS Requested QoS
 *
 *  \param  pUMTSMinQoS
 *          - UMTS Minimum QoS
 *
 *  \param  pGPRSRequestedQoS
 *          - GPRS Minimum QoS
 *
 *  \param  pUsername
 *          - User name
 *
 *  \param  pUsernameSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pUsername field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pPassword
 *          - Password
 *
 *  \param  pPasswordSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pPassword field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pAuthenticationPref
 *          - Authentication Preference
 *             - Bit map that indicates the authentication
 *               algorithm preference
 *               - Bit 0 - PAP preference
 *                 - 0 - PAP is never performed
 *                 - 1 - PAP may be performed
 *               - Bit 1 - CHAP preference
 *                 - 0 - CHAP is never performed
 *                 - 1 - CHAP may be performed
 *               - If more than one bit is set, then the device decides
 *                 which authentication procedure is performed while setting
 *                 up the data session. For example, the device may have a
 *                 policy to select the most secure authentication mechanism.
 *
 *  \param  pIPv4AddrPref
 *          - IPv4 Address Preference
 *
 *  \param  pPcscfAddrUsingPCO
 *          - P-CSCF Address using PCO Flag
 *            - 1 - (TRUE) implies request PCSCF address using PCO
 *            - 0 - (FALSE) implies do not request
 *                  By default, this value is 0
 *
 *  \param  pPdpAccessConFlag
 *          - PDP access control flag
 *            - 0 - PDP access control none
 *            - 1 - PDP access control reject
 *            - 2 - PDP access control permission
 *
 *  \param  pPcscfAddrUsingDhcp
 *          - P-CSCF address using DHCP
 *            - 1 - (TRUE) implies Request PCSCF address using DHCP
 *            - 0 - (FALSE) implies do not request
 *                  By default, value is 0
 *
 *  \param  pImCnFlag
 *          - IM CN flag
 *            - 1 - (TRUE) implies request IM CN flag for
 *                   this profile
 *            - 0 - (FALSE) implies do not request IM CN
 *                  flag for this profile
 *
 *  \param  pTFTID1Params
 *          - Traffic Flow Template
 *
 *  \param  pTFTID2Params
 *          - Traffic Flow Template
 *
 *  \param  pPdpContext
 *          - PDP context number
 *
 *  \param  pSecondaryFlag
 *          - PDP context secondary flag
 *            - 1 - (TRUE) implies this is secondary profile
 *            - 0 - (FALSE) implies this is not secondary profile
 *
 *  \param  pPrimaryID
 *          - PDP context primary ID
 *          - function SLQSGetProfileSettings() returns a default value
 *            0xFF if this parameter is not returned by the device
 *
 *  \param  pIPv6AddPref
 *          - IPv6 address preference
 *            Preferred IPv6 address to be assigned to the TE; actual
 *            assigned address is negotiated with the network and may
 *            differ from this value; if not specified, the IPv6
 *            address is obtained automatically from the network
 *
 *  \param  pUMTSReqQoSSigInd
 *          - UMTS requested QoS with Signalling Indication flag
 *
 *  \param  pUMTSMinQoSSigInd
 *          - UMTS minimum QoS with Signalling Indication flag
 *
 *  \param  pPrimaryDNSIPv6addpref
 *          - Primary DNS IPv6 address preference
 *            - The value may be used as a preference during
 *              negotiation with the network; if not specified, the
 *              wireless device will attempt to obtain the DNS
 *              address automatically from the network; the
 *              negotiated value is provided to the host via DHCP
 *
 *  \param  pSecondaryDNSIPv6addpref
 *          - Secondary DNS IPv6 address preference
 *
 *  \param  paddrAllocationPref
 *          - DHCP/NAS preference
 *            - This enumerated value may be used to indicate
 *              the address allocation preference
 *               - 0 - NAS signaling is used for address allocation
 *               - 1 - DHCP is used for address allocation
 *
 *  \param  pQosClassID
 *          - 3GPP LTE QoS parameters
 *
 *  \param  pAPNDisabledFlag
 *          - Optional 1 Byte Flag indicating if the APN is disabled/enabled
 *          - If set, the profile can not be used for making data calls
 *          - Any data call is failed locally
 *          - Values:
 *            - 0 - FALSE(default)
 *            - 1 - True
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 *  \param  pPDNInactivTimeout
 *          - Optional 4 Bytes indicating the duration of inactivity timer
 *            in seconds
 *          - If the PDP context/PDN connection is inactive for this duration
 *            i.e. No data Tx/Rx occurs, the PDP context/PDN connection is
 *            disconnected
 *          - Default value of zero indicates infinite value
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 *  \param  pAPNClass
 *          - Optional 1 Byte numeric identifier representing the APN in profile
 *          - Can be set and queried but is not used by the modem
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 */
struct Profile3GPP
{
    CHAR                    *pProfilename;
    WORD                    *pProfilenameSize;
    BYTE                    *pPDPtype;
    BYTE                    *pPdpHdrCompType;
    BYTE                    *pPdpDataCompType;
    CHAR                    *pAPNName;
    WORD                    *pAPNnameSize;
    ULONG                   *pPriDNSIPv4AddPref;
    ULONG                   *pSecDNSIPv4AddPref;
    struct UMTSQoS          *pUMTSReqQoS;
    struct UMTSQoS          *pUMTSMinQoS;
    struct GPRSRequestedQoS *pGPRSRequestedQos;
    struct GPRSRequestedQoS *pGPRSMinimumQoS;
    CHAR                    *pUsername;
    WORD                    *pUsernameSize;
    CHAR                    *pPassword;
    WORD                    *pPasswordSize;
    BYTE                    *pAuthenticationPref;
    ULONG                   *pIPv4AddrPref;
    BYTE                    *pPcscfAddrUsingPCO;
    BYTE                    *pPdpAccessConFlag;
    BYTE                    *pPcscfAddrUsingDhcp;
    BYTE                    *pImCnFlag;
    struct TFTIDParams      *pTFTID1Params;
    struct TFTIDParams      *pTFTID2Params;
    BYTE                    *pPdpContext;
    BYTE                    *pSecondaryFlag;
    BYTE                    *pPrimaryID;
    USHORT                  *pIPv6AddPref;
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd;
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd;
    USHORT                  *pPriDNSIPv6addpref;
    USHORT                  *pSecDNSIPv6addpref;
    BYTE                    *pAddrAllocPref;
    struct QosClassID       *pQosClassID;
    BYTE                    *pAPNDisabledFlag;
    ULONG                   *pPDNInactivTimeout;
    BYTE                    *pAPNClass;
};
/**
 * This structure contains the 3GPP2 profile parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  pNegoDnsSrvrPref
 *          - Negotiate DNS Server Preference
 *            - 1 - (TRUE)implies request DNS addresses
 *             from the PDSN
 *            - 0 - (FALSE)implies do not request DNS
 *             addresses from the PDSN
 *            - Default value is 1 (TRUE)
 *
 *  \param  pPppSessCloseTimerDO
 *          - PPP Session Close Timer for DO
 *            - Timer value (in seconds) on DO indicating how
 *              long the PPP Session should linger before
 *              closing down
 *
 *  \param  pPppSessCloseTimer1x
 *          - PPP Session Close Timer for 1X
 *            - Timer value (in seconds) on 1X indicating how
 *              long the PPP session should linger before closing down
 *
 *  \param  pAllowLinger
 *          - Allow/disallow lingering of interface
 *            - 1 -(TRUE) implies allow lingering
 *            - 0 -(FALSE) implies do not allow lingering
 *
 *  \param  pLcpAckTimeout
 *          - LCP ACK Timeout
 *            - Value of LCP ACK Timeout in milliseconds
 *
 *  \param  pIpcpAckTimeout
 *          - IPCP ACK Timeout
 *            - Value of IPCP ACK Timeout in milliseconds
 *
 *  \param  pAuthTimeout
 *          - AUTH Timeout
 *            - Value of Authentication Timeout in milliseconds
 *
 *  \param  pLcpCreqRetryCount
 *          - LCP Configuration Request Retry Count
 *
 *  \param  pIpcpCreqRetryCount
 *          - IPCP Configuration Request Retry Count
 *
 *  \param  pAuthRetryCount
 *          - Authentication Retry Count value
 *
 *  \param  pAuthProtocol
 *          - Authentication Protocol
 *            - 1 - PAP
 *            - 2 - CHAP
 *            - 3 - PAP or CHAP
 *
 *  \param  pUserId
 *          - User ID to be used during data network authentication
 *          - maximum length allowed is 127 bytes;
 *          - QMI_ERR_ARG_TOO_LONG will be returned if the storage on the
 *            wireless device is insufficient in size to hold the value.
 *
 *  \param  pUserIdSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pUserId field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pAuthPassword
 *          - Password to be used during data network authentication;
 *          - maximum length allowed is 127 bytes
 *          - QMI_ERR_ARG_TOO_LONG will be returned if the storage on the
 *            wireless device is insufficient in size to hold the value.
 *
 *  \param  pAuthPasswordSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pAuthPassword field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pDataRate
 *          - Data Rate Requested
 *            - 0 - Low (Low speed Service Options (SO15) only)
 *            - 1 - Medium (SO33 + low R-SCH)
 *            - 2 - High (SO33 + high R-SCH)
 *            - Default is 2
 *
 *  \param  pAppType
 *          - Application Type:
 *            - 0x00000001 - Default Application Type
 *            - 0x00000020 - LBS Application Type
 *            - 0x00000040 - Tethered Application Type
 *            - This parameter is not used while creating/modifying a profile
 *
 *  \param  pDataMode
 *          - Data Mode to use:
 *            - 0 - CDMA or HDR (Hybrid 1X/1xEV-DO)
 *            - 1 - CDMA Only (1X only)
 *            - 2 - HDR Only (1xEV-DO only)
 *            - Default is 0
 *
 *  \param  pAppPriority
 *          - Application Priority
 *            - Numerical 1 byte value defining the application
 *              priority; higher value implies higher priority
 *            - This parameter is not used while creating/modifying a profile
 *
 *  \param  pApnString
 *          - String representing the Access Point Name
 *          - maximum length allowed is 100 bytes
 *          - QMI_ERR_ARG_TOO_LONG will be returned if the APN
 *            name is too long.
 *
 *  \param  pApnStringSize;
 *          - This parameter is an input parameter and should be initialised
 *            to the size of pApnString field. Size of this parameter is 2
 *            bytes.
 *
 *  \param  pPdnType
 *          - Packed Data Network Type Requested:
 *            - 0 - IPv4 PDN Type
 *            - 1 - IPv6 PDN Type
 *            - 2 - IPv4 or IPv6 PDN Type
 *            - 3 - Unspecified PDN Type (implying no preference)
 *
 *  \param  pIsPcscfAddressNedded
 *          - This boolean value is used to control if PCSCF
 *            address is requested from PDSN
 *            - 1 -(TRUE) implies request for PCSCF value from the PDSN
 *            - 0 -(FALSE) implies do not request for PCSCF value from the PDSN
 *
 *  \param  pPrimaryV4DnsAddress
 *          - IPv4 Primary DNS address
 *            - The Primary IPv4 DNS address that can be statically assigned
 *              to the UE
 *
 *  \param  pSecondaryV4DnsAddress
 *          - IPv4 Secondary DNS address
 *            - The Secondary IPv4 DNS address that can be statically assigned
 *              to the UE
 *
 *  \param  pPriV6DnsAddress
 *          - Primary IPv6 DNS address
 *            - The Primary IPv6 DNS address that can be statically assigned
 *              to the UE
 *
 *  \param  pSecV6DnsAddress
 *          - Secondary IPv6 DNS address
 *            - The Secondary IPv6 DNS address that can be statically assigned
 *              to the UE
 *
 *  \param  pRATType
 *          - Optional 1 Byte Flag indicating RAT Type
 *          - Values:
 *            - 1 - HRPD
 *            - 2 - EHRPD
 *            - 3 - HRPD_EHRPD
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 *  \param  pAPNEnabled3GPP2
 *          - Optional 1 Byte Flag indicating if the APN is disabled/enabled
 *          - If disabled, the profile can not be used for making data calls
 *          - Values:
 *            - 0 - Disabled
 *            - 1 - Enabled(default value)
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 *  \param  pPDNInactivTimeout3GPP2
 *          - Optional 4 Bytes indicating the duration of inactivity timer
 *            in seconds
 *          - If the PDP context/PDN connection is inactive for this duration
 *            i.e. No data Tx/Rx occurs, the PDP context/PDN connection is
 *            disconnected
 *          - Default value of zero indicates infinite value
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 *  \param  pAPNClass3GPP2
 *          - Optional 1 Byte numeric identifier representing the APN in profile
 *          - Can be set and queried but is not used by the modem
 *          - This parameter is currently read only and can be read by using
 *            the function SLQSGetProfileSettings().
 *
 */
struct Profile3GPP2
{
    BYTE   *pNegoDnsSrvrPref;
    ULONG  *pPppSessCloseTimerDO;
    ULONG  *pPppSessCloseTimer1x;
    BYTE   *pAllowLinger;
    USHORT *pLcpAckTimeout;
    USHORT *pIpcpAckTimeout;
    USHORT *pAuthTimeout;
    BYTE   *pLcpCreqRetryCount;
    BYTE   *pIpcpCreqRetryCount;
    BYTE   *pAuthRetryCount;
    BYTE   *pAuthProtocol;
    CHAR   *pUserId;
    WORD   *pUserIdSize;
    CHAR   *pAuthPassword;
    WORD   *pAuthPasswordSize;
    BYTE   *pDataRate;
    ULONG  *pAppType;
    BYTE   *pDataMode;
    BYTE   *pAppPriority;
    CHAR   *pApnString;
    WORD   *pApnStringSize;
    BYTE   *pPdnType;
    BYTE   *pIsPcscfAddressNedded;
    ULONG  *pPrimaryV4DnsAddress;
    ULONG  *pSecondaryV4DnsAddress;
    USHORT *pPriV6DnsAddress;
    USHORT *pSecV6DnsAddress;
    BYTE   *pRATType;
    BYTE   *pAPNEnabled3GPP2;
    ULONG  *pPDNInactivTimeout3GPP2;
    BYTE   *pAPNClass3GPP2;
};

/**
 * This union WdsProfileParam consist of Profile3GPP and Profile3GPP2
 * out of which one will be used to create profile.
 *
 *  \param  SlqsProfile3GPP
 *          - 3GPP profile
 *
 *  \param  SlqsProfile3GPP2
 *          - 3GPP2 profile
 */
typedef union WdsProfileParam
{
    struct Profile3GPP  SlqsProfile3GPP;
    struct Profile3GPP2 SlqsProfile3GPP2;
}QmiProfileInfo;

/**
 * This structure contains the input parameters for SLQSCreateProfile
 *
 *  \param  ProfileID
 *          - 1 to 16 for 3GPP profile (EM/MC73xx or earlier)
 *          - 1 to 24 for 3GPP profile (EM/MC74xx onwards)
 *          - 101 to 106 for 3GPP2 profile
 *
 *  \param  ProfileType
 *          - Identifies the technology type of the profile
 *            - 0x00 - 3GPP
 *            - 0x01 - 3GPP2
 *            - NULL is not allowed
 *
 *  \param  curProfile
 *          - union of Profile3GPP and Profile3GPP2
 *
 *  \note   - If profileID is NULL, 3GPP profile will be created
 *            and index will be assigned based on availability in device.
 *  \note   - If profileID is not NULL depending on pProfileType 3GPP/3GPP2
 *            relevant profile will be created
 *
 */
struct CreateProfileIn
{
    BYTE           *pProfileID;
    BYTE           *pProfileType;
    QmiProfileInfo curProfile;
};

/**
 * This structure contains the UMTS Quality Of Service Information
 *
 *  \param  trafficClass
 *          - 0x00 - Subscribed
 *          - 0x01 - Conversational
 *          - 0x02 - Streaming
 *          - 0x03 - Interactive
 *          - 0x04 - Background
 *
 *  \param  maxUplinkBitrate
 *          - Maximum uplink bit rate in bits/sec
 *
 *  \param  maxDownlinkBitrate
 *          - Maximum downlink bit rate in bits/sec
 *
 *  \param  grntUplinkBitrate
 *          - Guaranteed uplink bit rate in bits/sec
 *
 *  \param  grntDownlinkBitrate
 *          - Guaranteed downlink bit rate in bits/sec
 *
 *  \param  qosDeliveryOrder - Qos delivery order
 *          - 0x00 - Subscribe
 *          - 0x01 - Delivery order on
 *          - 0x02 - Delivery order off
 *
 *  \param  maxSDUSize
 *          - Maximum SDU size
 *
 *  \param  sduErrorRatio - SDU error ratio
 *          - Target value for fraction of SDUs lost or
 *            detected as erroneous.
 *          - 0x00 - Subscribe
 *          - 0x01 - 1*10^(-2)
 *          - 0x02 - 7*10^(-3)
 *          - 0x03 - 1*10^(-3)
 *          - 0x04 - 1*10^(-4)
 *          - 0x05 - 1*10^(-5)
 *          - 0x06 - 1*10^(-6)
 *          - 0x07 - 1*10^(-1)
 *
 *  \param  resBerRatio - Residual bit error ratio
 *          - Target value for undetected bit error ratio in
 *            in the delivered SDUs.
 *          - 0x00 - Subscribe
 *          - 0x01 - 5*10^(-2)
 *          - 0x02 - 1*10^(-2)
 *          - 0x03 - 5*10^(-3)
 *          - 0x04 - 4*10^(-3)
 *          - 0x05 - 1*10^(-3)
 *          - 0x06 - 1*10^(-4)
 *          - 0x07 - 1*10^(-5)
 *          - 0x08 - 1*10^(-6)
 *          - 0x09 - 1*10^(-8)
 *
 *  \param  deliveryErrSDU - delivery of erroneous SDUs
 *          - Indicates whether SDUs detected as erroneous shall be
 *            delivered or not.
 *          - 0x00 - Subscribe
 *          - 0x01 - 5*10^(-2)
 *          - 0x02 - 1*10^(-2)
 *          - 0x03 - 5*10^(-3)
 *          - 0x04 - 4*10^(-3)
 *          - 0x05 - 1*10^(-3)
 *          - 0x06 - 1*10^(-4)
 *          - 0x07 - 1*10^(-5)
 *          - 0x08 - 1*10^(-6)
 *          - 0x09 - 1*10^(-8)
 *
 *  \param  transferDelay - Transfer delay (ms)
 *          - Indicates the targeted time between a request to transfer an
 *            SDU at one SAP to its delivery at the other SAP in milliseconds.
 *
 *  \param  trafficPriority - Transfer handling priority
 *          - Specifies the relative importance for handling of SDUs that
 *            belong to the UMTS bearer, compared to the SDUs of other bearers.
 */
struct UMTSMinQoS
{
    BYTE  trafficClass;
    ULONG maxUplinkBitrate;
    ULONG maxDownlinkBitrate;
    ULONG grntUplinkBitrate;
    ULONG grntDownlinkBitrate;
    BYTE  qosDeliveryOrder;
    ULONG maxSDUSize;
    BYTE  sduErrorRatio;
    BYTE  resBerRatio;
    BYTE  deliveryErrSDU;
    ULONG transferDelay;
    ULONG trafficPriority;
};

/**
 * This structure contains the GPRS Quality Of Service Information
 *
 *  \param  precedenceClass
 *          - Precedence class
 *
 *  \param  delayClass
 *          - Delay class
 *
 *  \param  reliabilityClass
 *          - Reliability class
 *
 *  \param  peakThroughputClass
 *          - Peak throughput class
 *
 *  \param  meanThroughputClass
 *          - Mean throughput class
 *
 */
struct GPRSRequestedQoS
{
    ULONG precedenceClass;
    ULONG delayClass;
    ULONG reliabilityClass;
    ULONG peakThroughputClass;
    ULONG meanThroughputClass;
};

/**
 * structure contains traffic flow template parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  filterId
 *          - Filter identifier
 *
 *  \param  eValid
 *          - Evaluation precedence index
 *
 *  \param  pVersion
 *          - IP version number
 *            - 4 - IPv4
 *            - 6 - IPv6
 *
 *  \param  sourceIP
 *          - Source IP address
 *           - IPv4 - Fill the first 4 bytes
 *           - IPv6 - Fill all the 16 bytes
 *
 *  \param  sourceIPMask
 *          - Mask value for the source address
 *
 *  \param  nextHeader
 *          - Next header/protocol value
 *
 *  \param  destPortRangeStart
 *          - Start value of the destination port range
 *
 *  \param  destPortRangeEnd
 *          - End value of the destination port range
 *
 *  \param  srcPortRangeStart
 *          - Start value of the source port range
 *
 *  \param  srcPortRangeEnd
 *          - End value of the source port range
 *
 *  \param  IPSECSPI
 *          - IPSEC security parameter index
 *
 *  \param  tosMask
 *          - TOS mask (Traffic class for IPv6)
 *
 *  \param  flowLabel
 *          - Flow label
 *
 */
struct TFTIDParams
{
    BYTE  filterId;
    BYTE  eValid;
    BYTE  ipVersion;
    WORD  *pSourceIP;
    BYTE  sourceIPMask;
    BYTE  nextHeader;
    WORD  destPortRangeStart;
    WORD  destPortRangeEnd;
    WORD  srcPortRangeStart;
    WORD  srcPortRangeEnd;
    ULONG IPSECSPI;
    WORD  tosMask;
    ULONG flowLabel;
};

/**
 * structure contains UMTS requested QoS with Signaling Indication flag
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  UMTSReqQoS
 *          - Contains the UMTS Quality Of Service Information
 *
 *  \param  SigInd - Signaling Indication flag
 *          - TRUE  - Signaling indication ON
 *          - FALSE - Signaling indication OFF
 *
 */
struct  UMTSReqQoSSigInd
{
    struct UMTSQoS UMTSReqQoS;
    BYTE           SigInd;
};

/**
 * structure contains 3GPP LTE QoS parameters
 *  - Parameter values default to their data type's maximum unsigned value
 *    unless explicitly stated otherwise.
 *
 *  \param  QCI
 *          - QOS specified using the QOS Class Identifier (QOS) values
 *            QCI value 0    - Requests the network to assign the
 *                             appropriate QCI value
 *            QCI values 1-4 - Associated with guaranteed bit rates
 *            QCI values 5-9 - Associated with non-guaranteed bit rates
 *
 *  \param  gDlBitRate
 *          - Guaranteed DL bit rate
 *
 *  \param  maxDlBitRate
 *          - maxDlBitRate
 *
 *  \param  gUlBitRate
 *          - Guaranteed UL bit rate
 *
 *  \param  maxUlBitRate
 *          - Maximum UL bit rate
 *
 */
struct QosClassID
{
    BYTE QCI;
    ULONG gDlBitRate;
    ULONG maxDlBitRate;
    ULONG gUlBitRate;
    ULONG maxUlBitRate;
};

/**
 * structure contains out parameter Information
 *
 *  \param  profileType
 *          - Identifies the type of profile
 *            0x00 = 3GPP
 *            0x01 = 3GPP2
 *
 *  \param  profileIndex
 *          - Index identifying the profile that was created
 *
 *  \param  pExtErrorCode
 *          - The extended error code received from DS Profile
 *           subsystem
 */
struct CreateProfileOut
{
    BYTE   *pProfileType;
    BYTE   *pProfileIndex;
    USHORT *pExtErrorCode;
};

/**
 * Create a new profile with the specified parameters.
 * Note that some firmware versions do not support the optional
 * Profile ID parameter. In this case an error will be returned
 * and the caller can subsequently create a profile by specifying
 * a NULL pointer for the Profile ID parameter. The Profile ID
 * pertaining to the newly created profile is returned in the
 * response structure (pResp).
 *
 *  \param  pReq[IN]
 *          - SLQS Create profile Information
 *
 *  \param  pResp[OUT]
 *          - SLQS profile identifier information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG SLQSCreateProfile(
    struct CreateProfileIn  *pReq,
    struct CreateProfileOut *pResp );

/**
 * structure contains autoconnect settings parameters
 *
 *  \param  action
 *          - 0 - get autoconnect settings
 *          - 1 - set autoconnect settings
 *
 *  \param  acsetting
 *          - Current autoconnect setting:
 *            - 0x00 - Autoconnect disabled
 *            - 0x01 - Autoconnect enabled
 *            - 0x02 - Autoconnect paused (resume on powercycle)
 *
 *  \param  acroamsetting
 *          - Current autoconnect roaming status
 *            - 0x00 - Autoconnect always allowed
 *            - 0x01 - Autoconnect while in home service area only
 */
struct slqsautoconnect{
    BOOL action;
    BYTE acsetting;
    BYTE acroamsetting;
};

/**
 *  Returns auto connect settings
 *
 *  \param  slqsautoconnect[IN]
 *          - SLQS auto connect settings
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSAutoConnect( struct slqsautoconnect *pacreq );

/**
 * This structure contains input parameters for SLQSModifyProfile
 *
 *  \param  ProfileID
 *          - 1 to 16 for 3GPP profile (EM/MC73xx or earlier)
 *          - 1 to 24 for 3GPP profile (EM/MC74xx onwards)
 *          - 101 to 106 for 3GPP2 profile
 *
 *  \param  ProfileType
 *          - Identifies the technology type of the profile
 *            - 0x00 - 3GPP
 *            - 0x01 - 3GPP2
 *            - NULL is not allowed
 *
 *  \param  curProfile
 *          - Contains Union of profile(3GPP/3GPP2) structures
 *
 */
struct ModifyProfileIn
{
    BYTE           *pProfileID;
    BYTE           *pProfileType;
    QmiProfileInfo curProfile;
};

/**
 * This structure contains out parameters for SLQSModifyProfile
 *
 *  \param  pExtErrorCode
 *          - The extended error code received from DS Profile subsystem of type
 *            eWDS_ERR_PROFILE_REG_xxx.
 *          - Error code will only will be present if error code
 *            eQCWWAN_ERR_QMI_EXTENDED_INTERNAL is returned by device.
 *          - See \ref qm_wds_ds_profile_extended_err_codes enum in qmerrno.h
 *            for received error description.
 */
struct ModifyProfileOut
{
    USHORT *pExtErrorCode;
};

/**
 * Modify a profile(3GPP/3GPP2) with the specified parameters.
 *
 *  \param  pReq[IN]
 *          - Contains parameters which can be modified
 *
 *  \param  pResp[OUT]
 *          - Contains parameters which indicates modification success
 *            or failure
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG SLQSModifyProfile(
    struct ModifyProfileIn  *pReq,
    struct ModifyProfileOut *pResp );

/**
 *  This structure contains the 3gpp Configuration Item information.
 *
 *  \param  pLTEAttachProfile
 *          - LTE Attach Profile
 *              - points to a single WORD Value indicating the
 *                attached LTE Profile
 *              - Optional parameter with possible values 1-16 (EM/MC73xx or earlier)
 *              - function SLQSGet3GPPConfigItem() returns a default value 255
 *                if no LTE Attach Profile is configured
 *          - This setting is deprecated on MC/EM74xx
 *
 *  \param  pProfileList
 *          - Profile List
 *              - an array of 4 profile configurations
 *              - Each element points to a single WORD value indicating profile
 *              - Optional parameter with possible values
 *                  - 1 - 16 (MC/EM73xx and before)
 *                  - 1 - 24 (MC/EM74xx and onwards)
 *              - function SLQSGet3GPPConfigItem() returns a default value 255
 *                if no 3gpp configuration is present
 *
 *  \param  pDefaultPDNEnabled
 *          - Always Connect Default PDN
 *              - A single BYTE value indicating the status of
 *                Always connect default PDN
 *                  - 0 - disabled
 *                  - 1 - enabled
 *              - Optional parameter
 *              - function SLQSGet3GPPConfigItem() returns a default value 255
 *                if no 3gpp configuration is present
 *
 *  \param  p3gppRelease
 *          - 3gpp release
 *              - A single BYTE value indicating the 3gpp release
 *                  - 0 - Release 99
 *                  - 1 - Release 5
 *                  - 2 - Release 6
 *                  - 3 - Release 7
 *                  - 4 - Release 8
 *              - In 9x30 and onwards
 *                  - 5 - Release 9
 *                  - 6 - Release 10
 *                  - 7 - Release 11
 *              - Optional parameter
 *              - function SLQSGet3GPPConfigItem() returns a default value 255
 *                if no 3gpp configuration is present
 *
 *  \param pLTEAttachProfileList
 *          - pointer to WORD array indicating LTE Attach Profile List
 *              - Optional parameter
 *              - possible values: 1-24
 *              - This setting is only supported for MC/EM74xx onwards
 *              - The new equivalent option for "pLTEAttachProfile" on 74xx 
 *                modems is "pLTEAttachProfileList". Please provide attach profiles 
 *                in order of decreasing priority in this list. 
 *
 *  \param LTEAttachProfileListLen
 *          - Number of element in pLTEAttachProfileList
 *             - valid range: 1-24
 *              - This setting is only supported for MC/EM74xx onwards
 *
 */
typedef struct _slqs3GPPConfigItem
{
    WORD *pLTEAttachProfile;
    WORD *pProfileList;
    BYTE *pDefaultPDNEnabled;
    BYTE *p3gppRelease;
    WORD LTEAttachProfileListLen;
    WORD *pLTEAttachProfileList;
} slqs3GPPConfigItem;

/**
 *  Sets the 3gpp configuration item.
 *
 *  \param  pSLQS3GPPConfigItem[IN]
 *          - See \ref slqs3GPPConfigItem for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/LTE\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSSet3GPPConfigItem( slqs3GPPConfigItem *pSLQS3GPPConfigItem );

/**
 *  Reads the 3gpp configuration item.
 *
 *  \param  pSLQS3GPPConfigItem[OUT]
 *          - See \ref slqs3GPPConfigItem for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/LTE\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSGet3GPPConfigItem( slqs3GPPConfigItem *pSLQS3GPPConfigItem );

/**
 * This structure contains the input parameters for SLQSGetProfileSettings
 *
 *  \param  ProfileType
 *          - Identifies the technology type of the profile
 *            - 0x00 - 3GPP
 *            - 0x01 - 3GPP2
 *
 *  \param  ProfileID
 *          - index identifying the profile
 *
 */
typedef struct _GetProfileSettingIn
{
    BYTE ProfileType;
    BYTE ProfileID;
}GetProfileSettingIn;

/**
 * This structure contains the profile settings retrieved by
 * the API SLQSGetProfileSettings
 *
 *  \param  curProfile
 *          - Structure containing details of the profile
 *          - See \ref QmiProfileInfo for more details
 *
 *  \param  pExtErrCode
 *          - pointer to a 2 byte extended error code
 *          - Error code will only will be present if error code
 *            eQCWWAN_ERR_QMI_EXTENDED_INTERNAL is returned by device.
 *          - See \ref qm_wds_ds_profile_extended_err_codes enum in qmerrno.h
 *            for received error description.
 *
 */
typedef struct _GetProfileSettingOut
{
    QmiProfileInfo curProfile;
    WORD           *pExtErrCode;
}GetProfileSettingOut;

/**
 * Retrieves a profile(3GPP/3GPP2) with the specified parameters.
 *
 *  \param  pReq[IN]
 *          - details of the profile to be fetched
 *          - See \ref GetProfileSettingIn for more information
 *
 *  \param  pResp[OUT]
 *          - The profile settings and/or extended error code returned by the
 *            device based on input parameters.
 *          - See \ref GetProfileSettingOut for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG SLQSGetProfileSettings(
    GetProfileSettingIn  *pReq,
    GetProfileSettingOut *pResp );

/**
 * This structure contains the information about the Transfer Statistics
 * Indicator parameters.
 *
 *  \param  statsPeriod
 *          - Period between transfer statistics reports.
 *              - 0 - Do not report
 *              - Other - Period between reports (seconds)
 *
 *  \param  statsMask
 *          - Requested statistic bit mask.
 *              - 0x00000001 - Tx packets OK
 *              - 0x00000002 - Rx packets OK
 *              - 0x00000004 - Tx packet errors
 *              - 0x00000008 - Rx packet errors
 *              - 0x00000010 - Tx overflows
 *              - 0x00000020 - Rx overflows
 *              - 0x00000040 - Tx bytes OK
 *              - 0x00000080 - Rx bytes OK
 *          - Each bit set causes the corresponding optional information to be
 *            sent in SLQSWdsEventReportCallBack.
 *          - All unlisted bits are reserved for future use and must be set to
 *            zero.
 *
 */
typedef struct
{
    BYTE  statsPeriod;
    ULONG statsMask;
} TrStatInd;

/**
 * This structure contains the information about the Set Event Report Request
 * parameters.
 *
 *  \param  pCurrChannelRateInd (optional)
 *          - Current Channel Rate Indicator.
 *              - 0 - Do not report
 *              - 1 - Report channel rate when it changes
 *
 *  \param  pTransferStatInd (optional)
 *          - See \ref TrStatInd for more information.
 *
 *  \param  pDataBearerTechInd (optional)
 *          - Data Bearer Technology Indicator.
 *              - 0 - Do not report
 *              - 1 - Report radio interface used for data transfer when it
 *                    changes
 *
 *  \param  pDormancyStatusInd (optional)
 *          - Dormancy Status indicator.
 *              - 0 - Do not report
 *              - 1 - Report traffic channel state of interface used for data
 *                    connection
 *
 *  \param  pMIPStatusInd (optional)
 *          - MIP Status Indicator.
 *              - 0 - Do not report
 *              - 1 - Report MIP status
 *
 *  \param  pCurrDataBearerTechInd (optional)
 *          - Current Data Bearer Technology Indicator.
 *              - 0 - Do not report
 *              - 1 - Report current data bearer technology when it changes
 *
 *  \param  pDataCallStatusChangeInd (optional)
 *          - Data Call Status Change Indicator.
 *              - 0 - Do not report
 *              - 1 - Report data call status change when it changes
 *
 *  \param  pCurrPrefDataSysInd (optional)
 *          - Current Preferred Data System Indicator.
 *              - 0 - Do not report
 *              - 1 - Report preferred data system when it changes
 *
 *  \param  pEVDOPageMonPerChangeInd (optional)
 *          - EV-DO Page Monitor Period Change Indicator.
 *              - 0 - Do not report
 *              - 1 - Report EV-DO page monitor period change event
 *
 *  \param  pDataSystemStatusChangeInd (optional)
 *          - Data System Status Change Indicator.
 *              - 0 - Do not report
 *              - 1 - Report data system status change event
 *
 *  \note At least one parameter should be present.
 *
 */
typedef struct
{
    BYTE      *pCurrChannelRateInd;
    TrStatInd *pTransferStatInd;
    BYTE      *pDataBearerTechInd;
    BYTE      *pDormancyStatusInd;
    BYTE      *pMIPStatusInd;
    BYTE      *pCurrDataBearerTechInd;
    BYTE      *pDataCallStatusChangeInd;
    BYTE      *pCurrPrefDataSysInd;
    BYTE      *pEVDOPageMonPerChangeInd;
    BYTE      *pDataSystemStatusChangeInd;
} wdsSetEventReportReq;

/**
 *  This API sets the wireless data connection state reporting conditions for
 *  the requesting control point.
 *
 *  \param  pSetEventReportReq[IN]
 *          - See \ref wdsSetEventReportReq for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds\n
 *          The control point event reporting state variables are modified to
 *          reflect the settings indicated in the request message. The service
 *          maintains a set of state variables for each control point.
 *          Relevant wireless data connection state changes are communicated to
 *          the registered WDS control point via the
 *          SLQSWdsSetEventReportCallBack.
 *          The AT command equivalents to this command are AT+CMER, AT+CIND,
 *          and AT+CIEV
 *
 */
ULONG SLQSWdsSetEventReport( wdsSetEventReportReq *pSetEventReportReq );

/**
 * This structure contains the PDP Runtime Settings Request parameters.
 *
 *  \param  contextId
 *          - Context Identifier
 *
 *  \param  contextType
 *          - Identifies technology type
 *             - 0 - 3GPP
 *             - 1 - 3GPP2
 *
 */
typedef struct
{
    BYTE  contextId;
    BYTE  contextType;
} swiPDPRuntimeSettingsReq;

/**
 * This structure contains the response parameters retrieved by the API
 * SLQSWdsSwiPDPRuntimeSettings
 *
 *  \param  pContextId (optional)
 *          - Context Identifier
 *              - 0xFF - Not Available
 *
 *  \param  pBearerId (optional)
 *          - Bearer Identity
 *          - An EPS bearer identity uniquely identifies an EPS bearer for one
 *            UE accessing via E-UTRAN. The EPS Bearer Identity is allocated by
 *            the MME.
 *              - 0xFF - Not Available
 *
 *  \param  pAPNName (optional)
 *          - APN name associated with the context id
 *              - NULL terminated by default.
 *
 *  \param  pIPv4Address (optional)
 *          - IPv4 Address
 *              - 0xFFFF - Not Available
 *
 *  \param  pIPv4GWAddress (optional)
 *          - IPv4 Gateway Address
 *              - 0xFFFF - Not Available
 *
 *  \param  pPrDNSIPv4Address (optional)
 *          - Primary DNS IPv4 Address
 *              - 0xFFFF - Not Available
 *
 *  \param  pSeDNSIPv4Address (optional)
 *          - Secondary DNS IPv4 Address
 *              - 0xFFFF - Not Available
 *
 *  \param  pIPv6Address (optional)
 *          - IPv6 Address
 *          - See \ref IPV6AddressInfo for more information
 *
 *  \param  pIPv6GWAddress (optional)
 *          - IPv6 Gateway Address
 *          - See \ref IPV6AddressInfo for more information
 *
 *  \param  pPrDNSIPv6Address (optional)
 *          - Primary IPv6 DNS Address(in network byte order)
 *          - This is an 8-element array of 16-bit numbers, each of which is
 *            in big-endian format
 *
 *  \param  pSeDNSIPv6Address (optional)
 *          - Secondary IPv6 DNS Address(in network byte order)
 *          - This is an 8-element array of 16-bit numbers, each of which is
 *            in big-endian format
 *
 *  \param  pPrPCSCFIPv4Address (optional)
 *          - Primary PCSCF IPv4 Address
 *
 *  \param  pSePCSCFIPv4Address (optional)
 *          - Secondary PCSCF IPv4 Address
 *
 *  \param  pPrPCSCFIPv6Address (optional)
 *          - Primary PCSCF IPv6 Address
 *          - This is an 8-element array of 16-bit numbers, each of which is
 *            in big-endian format
 *
 *  \param  pSePCSCFIPv6Address (optional)
 *          - Secondary PCSCF IPv6 Address
 *          - This is an 8-element array of 16-bit numbers, each of which is
 *            in big-endian format
 *
 *  \note  Parameters which are mentioned as NULL will be ignored.
 *
 */
typedef struct
{
    BYTE                   *pContextId;
    BYTE                   *pBearerId;
    CHAR                   *pAPNName;
    ULONG                  *pIPv4Address;
    ULONG                  *pIPv4GWAddress;
    ULONG                  *pPrDNSIPv4Address;
    ULONG                  *pSeDNSIPv4Address;
    struct IPV6AddressInfo *pIPv6Address;
    struct IPV6AddressInfo *pIPv6GWAddress;
    WORD                   *pPrDNSIPv6Address;
    WORD                   *pSeDNSIPv6Address;
    ULONG                  *pPrPCSCFIPv4Address;
    ULONG                  *pSePCSCFIPv4Address;
    WORD                   *pPrPCSCFIPv6Address;
    WORD                   *pSePCSCFIPv6Address;
} swiPDPRuntimeSettingsResp;

/**
 * This API requests the device to return the active PDP context associated
 * with a context id.
 *
 *  \param  pPDPRuntimeSettingsReq[IN]
 *          - See \ref swiPDPRuntimeSettingsReq for more information
 *
 *  \param  pPDPRuntimeSettingsResp[OUT]
 *          - See \ref swiPDPRuntimeSettingsResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 *          Timeout: 2 seconds\n
 *          The AT command equivalent to this command is AT+CGCONTRDP
 *
 */
ULONG SLQSWdsSwiPDPRuntimeSettings(
    swiPDPRuntimeSettingsReq  *pPDPRuntimeSettingsReq,
    swiPDPRuntimeSettingsResp *pPDPRuntimeSettingsResp );

/**
 * WDS Connection rates request data structure for individual session
 *
 *  \param  pCurrentChannelTXRate[OUT]
 *          - Instantaneous channel Tx rate in bits per second.
 *
 *  \param  pCurrentChannelRXRate[OUT]
 *          - Instantaneous channel Rx rate in bits per second.
 *
 *  \param   pMaxChannelTXRate[OUT]
 *          - Maximum Tx rate that can be assigned to the device by the
 *          - serving system in bits per second
 *
 *  \param   pMaxChannelRXRate[OUT]
 *          - Maximum Rx rate that can be assigned to the device by the
 *          - serving system in bits per second
 *
 */
struct WdsConnectionRateElmnts
{
    ULONG  *pCurrentChannelTXRate;
    ULONG  *pCurrentChannelRXRate;
    ULONG  *pMaxChannelTXRate;
    ULONG  *pMaxChannelRXRate;
};

/**
 * WDS ConnectionRate request data structure
 *
 *  \param  pV4sessionId
 *          - The v4 session ID for which the connection rate are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param  pV6sessionId
 *          - The v6 session ID for which the connection rate are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param   ConnRateElmntsV4
 *          - data structure to be populated with the connection rate for V4 session
 *
 *  \param   ConnRateElmntsV6
 *          - data structure to be populated with the connection rate for V6 session
 *
 *  \note   At least one of pV4sessionId and pV6sessionId must point to a valid
 *          session ID.
 *
 */
struct WdsConnectionRate
{
    ULONG  *pV4sessionId;
    ULONG  *pV6sessionId;
    struct WdsConnectionRateElmnts ConnRateElmntsV4;
    struct WdsConnectionRateElmnts ConnRateElmntsV6;
};

/*
 * This API is an internal Api.Used to switch the call for GetConnectionRate.
 *
 *  \param  pV4sessionId[IN]
 *          - use to store the current IPV4 Session Id. It can be NULL
 *
 *  \param  pV6sessionId[IN]
 *          - use to store the current IPV6 Session Id. It can be NULL
 *
 *  \param  pConnectionRateElmnt[OUT]
 *          - See \ref WdsConnectionRateElmnts for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG iGetConnectionRate (
    ULONG *pv4sessionId,
    ULONG *pv6sessionId,
    struct WdsConnectionRateElmnts *pConnectionRateElmnt );

/**
 * This API request the device to fetch ConnectionRate. It can be used for both
 * mono and multiple PDN use case.
 *
 *  \param  pConnectionRate[IN/OUT]
 *          - See \ref WdsConnectionRate for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG SLQSGetConnectionRate (
    struct WdsConnectionRate *pConnectionRate );

/**
 * WDS Bytes Totals request data structure for individual session
 *
 *  \param  pTXTotalBytes
 *          - No of transmitted bytes without error.
 *
 *  \param  pRXTotalBytes
 *          - No of received bytes without error.
 *
 */
struct WdsByteTotalsElmnts
{
    ULONGLONG  *pTXTotalBytes;
    ULONGLONG  *pRXTotalBytes;
};

/**
 * WDS ByteTotals request data structure
 *
 *  \param  pV4sessionId
 *          - The v4 session ID for which the byte totals are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param  pV6sessionId
 *          - The v6 session ID for which the byte totals are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param   ByteTotalsElmntsV4
 *          - data structure to be populated with the byte totals for V4 session
 *
 *  \param   ByteTotalsElmntsV6
 *          - data structure to be populated with the byte totals for V6 session
 *
 *  \note   At least one of pV4sessionId and pV6sessionId must point to a valid
 *          session ID.
 *
 */
struct WdsByteTotals
{
    ULONG                      *pV4sessionId;
    ULONG                      *pV6sessionId;
    struct WdsByteTotalsElmnts ByteTotalsElmntsV4;
    struct WdsByteTotalsElmnts ByteTotalsElmntsV6;
};

/*
 * This API is an internal Api.Used to switch the call for GetByteTotals.
 *
 *  \param  pV4sessionId[IN]
 *          - use to store the current IPV4 Session Id. It can be NULL
 *
 *  \param  pV6sessionId[IN]
 *          - use to store the current IPV6 Session Id. It can be NULL
 *
 *  \param  pByteTotalsElmnt[OUT]
 *          - See \ref WdsByteTotalsElmnts for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG iGetByteTotals (
    ULONG                      *pv4sessionId,
    ULONG                      *pv6sessionId,
    struct WdsByteTotalsElmnts *pByteTotalsElmnt );

/**
 * This API request the device to fetch ByteTotals for IPV4 and IPV6.
 *
 *  \param  pByteTotals[IN/OUT]
 *          - See \ref WdsByteTotals for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG SLQSGetByteTotals (
    struct WdsByteTotals *pByteTotals );

/**
 *  Forces the device to immediately drop the traffic channel on the serving
 *  radio interface
 *
 *  \param  None
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   This channel can be reactivated as soon as data is sent over the
 *          network interface. There is no assurance that the channel remains
 *          dormant for any guaranteed period.
 *           Timeout: 5 seconds
 *
 */
ULONG SLQSWdsGoDormant( void );

/**
 *  Forces the device to immediately reestablish the traffic channel on the
 *  serving radio interface
 *
 *  \param  None
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   This channel can go dormant any time after it has been reactivated.
 *          There is no assurance that the channel remains active for any
 *          guaranteed period.
 *          Timeout: 5 seconds
 *
 */
ULONG SLQSWdsGoActive( void );

/**
 *  Returns the state of the current packet data session.
 *
 *  \param  pStateV4[OUT]
 *          - Current link status for IPV4 Session
 *              - 1 - DISCONNECTED
 *              - 2 - CONNECTED
 *              - 3 - SUSPENDED (not supported)
 *              - 4 - AUTHENTICATING
 *
 *  \param  pStateV6[OUT]
 *          - Current link status for IPV6 Session
 *              - 1 - DISCONNECTED
 *              - 2 - CONNECTED
 *              - 3 - SUSPENDED (not supported)
 *              - 4 - AUTHENTICATING
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG  SLQSGetSessionState(
    ULONG    *pStateV4,
    ULONG    *pStateV6,
    BYTE     instance );

/*
 * This structure contains the response parameters retrieved by the API
 * iSLQSGetInstanceID. This is for SDK internal use only.
 *
 *  \param  pInstanceID
 *          - Instance ID corresponding to the session ID
 *
 *  \param  pIPFamily
 *          - IP Family for the session ID
 *            - 4 - IPv4
 *            - 6 - IPv6
 *
 *  \note  none
 *
 */
typedef struct
{
    BYTE *pInstanceID;
    BYTE *pIPFamily;
} GetInstIDResp;

/*
 * This structure contains the response parameters retrieved by the API
 * iSLQSGetSessionID. This is for SDK internal use only.
 *
 *  \param  pSessionIDv4
 *          - IPv4 session ID
 *
 *  \param  pSessionIDv6
 *          - IPv6 session ID
 *
 *  \note  none
 *
 */
typedef struct
{
    ULONG *pSessionIDv4;
    ULONG *pSessionIDv6;
} GetSessionIDResp;

/**
 *  This structure contains Transfer Statistcics Indicator
 *
 *  \param  StatsPeriod
 *          - Period between transfer statistics reports
 *              - 0 - Do not report
 *              - Other - Period between reports (seconds)
 *
 *  \param  StatsMask
 *          - Requested statistic bit mask. Each bit set
 *            causes the corresponding optional TLV to be sent in the
 *            indication. All unlisted bits are reserviced for future
 *            use and must be set to zero.
 *              - 0x00000040 - Tx bytes OK
 *              - 0x00000080 - Rx bytes OK
 */
typedef struct
{
    BYTE  StatsPeriod;
    ULONG StatsMask;
} TransferStatInd;

/**
 * This structure contains the DUN Call Info Request parameters.
 *
 *  \param  Mask
 *          - Mandatory parameter
 *          - Set the bits corresponding to the information requested to 1
 *          - All other bits must be set to 0.
 *          - If any values are not available or applicable, the corresponding
 *            TLVs are not returned in the response.
 *              - Bit 0 - Connection Status
 *              - Bit 1 - Last call end reason
 *              - Bit 2 - Tx/Rx bytes OK
 *              - Bit 3 - Dormancy status
 *              - Bit 4 - Data bearer
 *              - Bit 5 - Channel rate
 *              - Bit 6 - Call active duration
 *
 *  \param  pReportConnStatus
 *          - Connect Status Indicator
 *              - 0 - Do not report
 *              - 1 - Report connection status and call end reason
 *
 *  \param  pTransferStatInd
 *          - See \ref TransferStatInd for more information
 *
 *  \param  pReportDormStatus
 *          - Dormancy Status Indicator
 *              - 0 - Do not report
 *              - 1 - Report traffic channel state of interface used for
 *                    data connection
 *
 *  \param  pReportDataBearerTech
 *          - Current Data Bearer Technology Indicator
 *              - 0 - Do not report
 *              - 1 - Report radio interface used for data transfer when
 *                    it changes
 *
 *  \param  pReportChannelRate
 *          - Channel Rate Indicator
 *              - 0 - Do not report
 *              - 1 - Report channel rate
 */
typedef struct
{
    ULONG           Mask;
    BYTE            *pReportConnStatus;
    TransferStatInd *pTransferStatInd;
    BYTE            *pReportDormStatus;
    BYTE            *pReportDataBearerTech;
    BYTE            *pReportChannelRate;
} getDUNCallInfoReq;

/**
 *  This structure contains modem connection status
 *
 *  \param  MDMConnStatus
 *          - Current link status
 *              - 0x01 - DISCONNECTED
 *              - 0x02 - CONNECTED
 *
 *  \param  MDMCallDuration
 *          - Call duration in milliseconds.
 *          - If the modem connection status is connected,
 *            this represent the duration of the current DUN call.
 *          - If the modem connection status is disconnected, this
 *            represents the duration of the last DUN call since the
 *            device was powered up (zero, if no call has been made or
 *            if the last call was not DUN).
 */
typedef struct
{
    BYTE      MDMConnStatus;
    ULONGLONG MDMCallDuration;
} ConnectionStatus;

/**
 *  This structure contains Channel Rate
 *
 *  \param  CurrChanTxRate
 *          - Instantaneous channel Tx rate in bits per second
 *
 *  \param  CurrChanRxRate
 *          - Instantaneous channel Rx rate in bits per second
 *
 *  \param  MaxChanTxRate
 *          - maximum Tx rate that can be assigned to the device
 *            by the serving system in bits per second
 *
 *  \param  MaxChanRxRate
 *          - maximum Rx rate that can be assigned to the device
 *            by the serving system in bits per second
 */
typedef struct
{
    ULONG CurrChanTxRate;
    ULONG CurrChanRxRate;
    ULONG MaxChanTxRate;
    ULONG MaxChanRxRate;
} ChannelRate;

/**
 * This structure contains the DUN Call Info response parameters
 *
 *  \param  pConnectionStatus
 *          - See \ref ConnectionStatus for more information
 *
 *  \param  pCallEndReason
 *          - Last modem call end reason
 *          - See qaGobiApiTableCallEndReasons.h for Call End Reason
 *          - Only valid if the last call made was DUN, else zero is returned
 *
 *  \param  pTXOKBytesCount
 *          - Number of bytes transmitted without error
 *          - Returned only if a data call is up
 *
 *  \param  pRXOKBytesCount
 *          - Number of bytes received without error
 *          - Returned only if a data call is up
 *
 *  \param  pDormancyStatus
 *          - Current traffic channel status
 *          - Returned if a data call is up
 *              - 0x01 - Traffic channel dormant
 *              - 0x02 - Traffic channel active
 *
 *  \param  pDataBearerTech
 *          - Current data bearer technology
 *          - Returned only if a data call is up
 *              - 0x01 - cdma2000 1X
 *              - 0x02 - cdma2000 HRPD (1xEV-DO)
 *              - 0x03 - GSM
 *              - 0x04 - UMTS
 *              - 0x05 - cdma200 HRPD ( 1xEV-DO RevA)
 *              - 0x06 - EDGE
 *              - 0x07 - HSDPA and WCDMA
 *              - 0x08 - WCDMA and HSUPA
 *              - 0x09 - HSDPA and HSUPA
 *              - 0x0A - LTE
 *              - 0x0B - cdma2000 EHRPD
 *              - 0x0C - HSDPA+ and WCDMA
 *              - 0x0D - HSDPA+ and HSUPA
 *              - 0x0E - DC_HSDPA+ and WCDMA
 *              - 0x0F - DC_HSDPA+ and HSUPA
 *              - 0x10 - HSDPA+ and 64QAM
 *              - 0x11 - HSDPA+, 64QAM and HSUPA
 *              - 0x12 - TDSCDMA
 *              - 0x13 - TDSCDMA and HSDPA
 *              - 0xFF - Unknown
 *
 *  \param  pChannelRate
 *          - See \ref ChannelRate for more information
 *
 *  \param  pLastCallTXOKBytesCnt
 *          - Number of bytes transmitted without error during the last
 *            data call ( 0 if no call was made ).
 *          - Return only if not in a call and the previous call was made
 *            using DUN.
 *
 *  \param  pLastCallRXOKBytesCnt
 *          - Number of bytes transmitted without error during the last
 *            data call ( 0 if no call was made ).
 *          - Return only if not in a call and the previous call was made
 *            using DUN.
 *
 *  \param  pMdmCallDurationActive
 *          - Duration that the call is active in milliseconds
 *          - If the modem connection status is connected, this represents
 *            the active duration of the current DUN call
 *          - If the modem connection status is disconnected, this represents
 *            the duration of the last DUN call since the device was powered
 *            up (0 if no call has been made or if the last call was not DUN)
 *
 *  \param  pLastCallDataBearerTech
 *          - Last Call Data Bearer Technology
 *          - Returned only if not in a call and when the previous call was
 *            made using DUN
 *              - 0x01 - cdma2000 1X
 *              - 0x02 - cdma2000 HRPD (1xEV-DO)
 *              - 0x03 - GSM
 *              - 0x04 - UMTS
 *              - 0x05 - cdma200 HRPD (1xEV-DO Rev A)
 *              - 0x06 - EDGE
 *              - 0x07 - HSDPA and WCDMA
 *              - 0x08 - WCDMA and HSUPA
 *              - 0x09 - HSDPA and HSUPA
 *              - 0x0A - LTE
 *              - 0x0B - cdma2000 EHRPD
 *              - 0x0C - HSDPA+ and WCDMA
 *              - 0x0D - HSDPA+ and HSUPA
 *              - 0x0E - DC_HSDPA+ and WCDMA
 *              - 0x0F - DC_HSDPA+ and HSUPA
 *              - 0x10 - HSDPA+ and 64QAM
 *              - 0x11 - HSDPA+, 64QAM and HSUPA
 *              - 0x12 - TDSCDMA
 *              - 0x13 - TDSCDMA and HSDPA
 *              - 0xFF - Unknown
 *
 */
typedef struct
{
    ConnectionStatus *pConnectionStatus;
    WORD             *pCallEndReason;
    ULONGLONG        *pTXOKBytesCount;
    ULONGLONG        *pRXOKBytesCount;
    BYTE             *pDormancyStatus;
    BYTE             *pDataBearerTech;
    ChannelRate      *pChannelRate;
    ULONGLONG        *pLastCallTXOKBytesCnt;
    ULONGLONG        *pLastCallRXOKBytesCnt;
    ULONGLONG        *pMdmCallDurationActive;
    BYTE             *pLastCallDataBearerTech;
} getDUNCallInfoResp;

/**
 * This API queries the current modem connection status.
 *
 *  \param  pGetDUNCallInfoReq[IN]
 *          - See \ref getDUNCallInfoReq for more information
 *
 *  \param  pGetDUNCallInfoResp[OUT]
 *          - See \ref getDUNCallInfoResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds\n
 *
 */
ULONG SLQSGetDUNCallInfo(
    getDUNCallInfoReq  *pGetDUNCallInfoReq,
    getDUNCallInfoResp *pGetDUNCallInfoResp );

/**
 * WDS Pkt Statistics request data structure for individual session
 *
 *  \param  pTXPacketSuccesses
 *          - Number of transmitted Packets without error.
 *
 *  \param  pRXPacketSuccesses
 *          - Number of received Packets without error.
 *
 *  \param  pTXPacketErrors
 *          - Number of outgoing packets with framing errors.
 *
 *  \param  pRXPacketErrors
 *          - Number of incoming packets with framing errors.
 *
 *  \param  pTXPacketOverflows
 *          - Number of packets dropped because Tx buffer overflowed (out of memory).
 *
 *  \param  pRXPacketOverflows
 *          - Number of packets dropped because Rx buffer overflowed (out of memory).
 *
 *  \param  pTXOkBytesCount
 *          - Number of bytes transmitted without error.
 *
 *  \param  pRXOkBytesCount
 *          - Number of bytes received without error.
 *
 *  \param  pTXOKBytesLastCall
 *          - Number of bytes transmitted without error during the
 *            last data call (0 if no call was made earlier).
 *            Returned only if not in a call, and when the previous call was
 *            made using RmNet (for any devices that support
 *
 *  \param  pRXOKBytesLastCall
 *          - Number of bytes received without error
 *            during the last data call (0 if no call was
 *            made earlier). Returned only if not in a
 *            call, and when the previous call was
 *            made using RmNet (for any devices that
 *            support
 *
 *  \param  pTXDroppedCount
 *          - Number of outgoing packets dropped.
 *
 *  \param  pRXDroppedCount
 *          - Number of incoming packets dropped.
 *
 */
struct WdsPktStatisticsElmnts
{
    ULONG        *pTXPacketSuccesses;
    ULONG        *pRXPacketSuccesses;
    ULONG        *pTXPacketErrors;
    ULONG        *pRXPacketErrors;
    ULONG        *pTXPacketOverflows;
    ULONG        *pRXPacketOverflows;
    ULONGLONG    *pTXOkBytesCount;
    ULONGLONG    *pRXOkBytesCount;
    ULONGLONG    *pTXOKBytesLastCall;
    ULONGLONG    *pRXOKBytesLastCall;
    ULONG        *pTXDroppedCount;
    ULONG        *pRXDroppedCount;
};

/**
 * WDS PktStatistics response data structure
 *
 *  \param  pV4sessionId
 *          - The v4 session ID for which the byte totals are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param  pV6sessionId
 *          - The v6 session ID for which the byte totals are to be retrieved
 *          - provide a NULL pointer if not applicable
 *
 *  \param   PktStatElmntsV4
 *          - data structure to be populated with the Pkt Statistics for V4 session
 *
 *  \param   PktStatElmntsV6
 *          - data structure to be populated with the Pkt Statistics for V6 session
 *
 *  \note   At least one of pV4sessionId and pV6sessionId must point to a valid
 *          session ID.
 *
 */
struct WdsPktStatisticsResp
{
    ULONG                         *pV4sessionId;
    ULONG                         *pV6sessionId;
    struct WdsPktStatisticsElmnts PktStatElmntsV4;
    struct WdsPktStatisticsElmnts PktStatElmntsV6;
};

/**
 * WDS PktStatistics request data structure
 *
 *  \param  pStatMask
 *          - Packet Statistics Mask
 *             0x00000001 - Tx packets OK
 *             0x00000002 - Rx packets OK
 *             0x00000004 - Tx packet errors
 *             0x00000008 - Rx packet errors
 *             0x00000010 - Tx overflows
 *             0x00000020 - Rx overflows
 *             0x00000040 - Tx bytes OK
 *             0x00000080 - Rx bytes OK
 *
 */
struct WdsPktStatisticsReq
{
    ULONG *pStatMask;
};

/**
 *  Returns the current packet transfer counter values from the device.
 *   Since the start of the last packet data session.
 *
 * \param  pStatMask[IN]
 *          - See \ref WdsPktStatisticsReq for more information
 *
 *  \param  pPktStatisticsElmnt[OUT]
 *          - See \ref WdsPktStatisticsElmnts for more information
 *
 *  \param  instance
 *          - PDP instance
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG GetPacketStatistics(
    struct WdsPktStatisticsReq    *pStatMask,
    struct WdsPktStatisticsElmnts *pPktStatisticsElmnt,
    BYTE                          instance );

/*
 * This API is an internal Api.Used to switch the call for GetPacketStatistics.
 *
 *  \param  pV4sessionId[IN]
 *          - use to store the current IPV4 Session Id. It can be NULL
 *
 *  \param  pV6sessionId[IN]
 *          - use to store the current IPV6 Session Id. It can be NULL
 *
 *  \param  pStatMask[IN]
 *          - See \ref WdsPktStatisticsReq for more information
 *
 *  \param  pPktStatisticsElmnt[OUT]
 *          - See \ref WdsPktStatisticsElmnts for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 */
ULONG iGetPacketStatistics (
    ULONG                         *pV4sessionId,
    ULONG                         *pV6sessionId,
    struct WdsPktStatisticsReq    *pStatMask,
    struct WdsPktStatisticsElmnts *pPktStatisticsElmnt );

/**
 * This API request the device to fetch current packet transfer counter values
 * from the device
 *
 * *  \param  pStatMask[IN]
 *          - See \ref WdsPktStatisticsReq for more information
 *
 *  \param  pPktStatistics[OUT]
 *          - See \ref WdsPktStatisticsResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 5 seconds\n
 *
 */
ULONG SLQSGetPacketStatistics (
    struct WdsPktStatisticsReq  *pStatMask,
    struct WdsPktStatisticsResp *pPktStatistics );

/**
 * Network information structure
 *
 *  \param  NetworkType
 *          - Values:
 *              - 0 - 3GPP
 *              - 1 - 3GPP2
 *
 *  \param  RATMask
 *          - Radio Access Technology (RAT) mask to
 *          indicate the type of technology.
 *          - Values:
 *              - 0 - Don't Care
 *              - 0x8000 - NULL Bearer
 *          - CDMA RAT mask values:
 *              - 0x01 - CDMA_1x
 *              - 0x02 - EVDO_REV0
 *              - 0x04 - EVDO_REVA
 *              - 0x08 - EVDO_REVB
 *              - 0x10 - EHRPD
 *              - 0x20 - FMC
 *          - UMTS RAT mask values:
 *              - 0x01 - WCDMA
 *              - 0x02 - GPRS
 *              - 0x04 - HSDPA
 *              - 0x08 - HSUPA
 *              - 0x10 - EDGE
 *              - 0x20 - LTE
 *              - 0x40 - HSDPA+
 *              - 0x80 - DC_HSDPA+
 *              - 0x100 - 64_QAM
 *              - 0x200 - TDSCDMA
 *
 *  \param  SOMask
 *          - Service Option (SO) mask to indicate the
 *            service option or type of application.
 *          - Values:
 *              - 0 - Don't Care
 *          - CDMA 1x SO mask values:
 *              - 0x01 - CDMA_1X_IS95
 *              - 0x02 - CDMA_1X_IS2000
 *              - 0x04 - CDMA_1X_IS2000_REL_A
 *          - CDMA EV-DO Rev 0 SO mask values:
 *              - 0x01 - DPA
 *          - CDMA EV-DO Rev A SO mask values:
 *              - 0x01 - DPA
 *              - 0x02 - MFPA
 *              - 0x04 - EMPA
 *              - 0x08 - EMPA_EHRPD
 *          - CDMA EV-DO Rev B SO mask values:
 *              - 0x01 - DPA
 *              - 0x02 - MFPA
 *              - 0x04 - EMPA
 *              - 0x08 - EMPA_EHRPD
 *              - 0x10 - MMPA
 *              - 0x20 - MMPA_EHRPD
 */
typedef struct
{
    BYTE  NetworkType;
    ULONG RATMask;
    ULONG SOMask;
}CurrNetworkInfo;

/**
 * Data System Status
 *
 *  \param  pPrefNetwork [OUT]
 *          - Preferred Network
 *          - Values:
 *              - 0 - 3GPP
 *              - 1 - 3GPP2
 *
 *  \param  pNetworkInfoLen [IN/OUT]
 *          - As input parameter size assigned to next parameter i.e.
 *            network information
 *          - As output the actual number of network information elements
 *            returned by the device
 *
 *  \param   pCurrNetworkInfo [OUT]
 *          - Network information
 *          - See \ref CurrNetworkInfo for more details
 */
typedef struct
{
    BYTE            *pPrefNetwork;
    BYTE            *pNetworkInfoLen;
    CurrNetworkInfo *pCurrNetworkInfo;
}CurrDataSysStat;

/**
 * This API request the device to fetch current data system status.
 *
 *  \param  pCurrDataSysStat[IN/OUT]
 *          - See \ref CurrDataSysStat for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 5 seconds\n
 *
 */
ULONG SLQSGetCurrDataSystemStat ( CurrDataSysStat *pCurrDataSysStat );

/**
  * RM Transfer Satistics Structure
  *  \param  bResetStatistics
  *          - Reset Statistics
  *          - Values:
  *          - 0 - Not Reset
  *          - Other - Reset
  *
  *  \param  ulMask
  *         - Enable/Disable RM Transfer Satatistics Indiscation Mask
  *         - Bit 0: Tx Packet Ok
  *         - Bit 1: Rx Packet Ok
  *         - Bit 2: Tx Bytes Ok
  *         - Bit 3: Rx Bytes Ok
  *         - Bit 4: Tx Packets Dropped
  *         - Bit 5: Rx Packets Dropped
  *         - Value:
  *         -0 - Disable
  *         -1 - Enable
  */

typedef struct
{
    BYTE   bResetStatistics;
    ULONG  ulMask;
} swiRMTrasnferStaticsReq;

/**
 * This API request the device to fetch current data system transfer Statistics.
 *
 *  \param  pswiRMTrasnferStaticsReq[IN]
 *          - See \ref swiRMTrasnferStaticsReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */

ULONG RMSetTransferStatistics( swiRMTrasnferStaticsReq *pSwiRMTrasnferStaticsReq );

typedef struct
{
    BYTE    TlvPresent;
    ULONG   ulData;
}DataULongTlv;

typedef struct 
{
    BYTE    TlvPresent;
    ULONGLONG   ullData;
}DataULongLongTlv;

/**
 * WDS Pkt RM Transfer Statistics data structure for individual session
 *
 *  \param  TxOkConutTlv
 *          - Tx Ok Packet Tlv Value.
 *
 *  \param  RxOkConutTlv
 *          - Rx Ok Packet Tlv Value.
 *
 *  \param  TxOkByteCountTlv
 *          - Tx Ok Byte Count Packet Tlv Value.
 *
 *  \param  RxOkByteCountTlv
 *          - Rx Ok Byte Count Packet Tlv Value.
 *
 *  \param  TxDropConutTlv
 *          - Tx Drop Count Packet Tlv Value.
 *
 *  \param  RxDropConutTlv
 *          - Rx Drop Count Packet Tlv Value.
 */

typedef struct {
    DataULongTlv        TxOkConutTlv;
    DataULongTlv        RxOkConutTlv;
    DataULongLongTlv    TxOkByteCountTlv;
    DataULongLongTlv    RxOkByteCountTlv;
    DataULongTlv        TxDropConutTlv;
    DataULongTlv        RxDropConutTlv;
}QmiCbkWdsStatisticsIndState;


/**
 * This API request the device to reset packet data transfer statistics.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */

ULONG SLQSResetPacketStatics();

/**
 * Network information structure
 *
 *  \param  TechType
 *          - Technology type
 *          - Values:
 *              - 0 - WDS_BEARER_TECH_NETWORK_3GPP - 3GPP
 *              - 1 - WDS_BEARER_TECH_NETWORK_3GPP2 - 3GPP2
 *
 *  \param  ratValue
 *          - Radio Access Technology (RAT) value
 *          - Values:
 *              - 0x00 - WDS_BEARER_TECH_RAT_EX_NULL_BEARER - NULL bearer
 *              - 0x01 - WDS_BEARER_TECH_RAT_EX_3GPP_WCDMA - 3GPP WCDMA
 *              - 0x02 - WDS_BEARER_TECH_RAT_EX_3GPP_GERAN - 3GPP GERAN
 *              - 0x03 - WDS_BEARER_TECH_RAT_EX_3GPP_LTE - 3GPP LTE
 *              - 0x04 - WDS_BEARER_TECH_RAT_EX_3GPP_TDSCDMA - 3GPP TDSCDMA
 *              - 0x05 - WDS_BEARER_TECH_RAT_EX_3GPP_WLAN - 3GPP WLAN
 *              - 0x64 - WDS_BEARER_TECH_RAT_EX_3GPP_MAX - 3GPP maximum
 *              - 0x65 - WDS_BEARER_TECH_RAT_EX_3GPP2_1X - 3GPP2 1X
 *              - 0x66 - WDS_BEARER_TECH_RAT_EX_3GPP2_HRPD - 3GPP2 HRPD
 *              - 0x67 - WDS_BEARER_TECH_RAT_EX_3GPP2_EHRPD - 3GPP2 EHRPD
 *              - 0x68 - WDS_BEARER_TECH_RAT_EX_3GPP2_WLAN - 3GPP2 WLAN
 *              - 0xC8 - WDS_BEARER_TECH_RAT_EX_3GPP2_MAX - 3GPP2 maximum
 *
 *  \param  SOMask
 *          - Service Option (SO) mask to indicate the
 *            service option or type of application.
 *            An SO mask value of zero indicates that
 *            this field is ignored.
 *          - Values:
 *              - 0x00 - SO mask unspecified
 *          - 3GPP SO mask:
 *              - 0x01 - WCDMA
 *              - 0x02 - HSDPA
 *              - 0x04 - HSUPA
 *              - 0x08 - HSDPAPLUS
 *              - 0x10 - DC HSDPAPLUS
 *              - 0x20 - 64 QAM
 *              - 0x40 - HSPA
 *              - 0x80 - GPRS
 *              - 0x100 - EDGE
 *              - 0x200 - GSM
 *              - 0x400 - S2B
 *              - 0x800 - LTE limited service
 *              - 0x1000 - LTE FDD
 *              - 0x2000 - LTE TDD
 *          
 *          - 3GPP2 SO mask:
 *              - 0x01000000 - 1X IS95
 *              - 0x02000000 - 1X IS2000
 *              - 0x04000000 - 1X IS2000 REL A
 *              - 0x08000000 - HDR REV0 DPA
 *              - 0x10000000 - HDR REVA DPA
 *              - 0x20000000 - HDR REVB DPA
 *              - 0x40000000 - HDR REVA MPA
 *              - 0x80000000 - HDR REVB MPA
 *              - 0x100000000 - HDR REVA EMPA
 *              - 0x200000000 - HDR REVB EMPA
 *              - 0x400000000 - HDR REVB MMPA
 *              - 0x800000000 - HDR EVDO FMC
 */
typedef struct
{
    ULONG techType;
    ULONG ratValue;
    ULONGLONG soMask;
}DataBearerTech;

/**
 * Data Bearer Technology Ext
 *
 *  \param  pBearerTech [OUT]
 *          - See \ref DataBearerTech for more information
 *
 *  \param  pLastBearerTech [OUT]
 *          - See \ref DataBearerTech for more information
 */

typedef struct
{
    DataBearerTech *pBearerTech;
    DataBearerTech *pLastBearerTech;
}DataBearerTechExt;


/**
 * This API Get Data Bearer Technology. This is a new API to replace API
 * GetDataBearerTechnology(). see the description of GetDataBearerTechnology()
 * to get more information
 *
 *  \param  pDataBearerTech [IN]
  *         - See \ref DataBearerTechExt for more information
 *  \param  instance [IN]
 *          - PDP instance
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG  SLQSGetDataBearerTechnologyExt(
    DataBearerTechExt *pDataBearerTech,
    BYTE  instance );

/**
 * WDS SWI Current Channel Rates Structure of Packet Data Connection Information.
 *
 *  \param  current_channel_tx_rate
 *          - Current Channel Tx Rate.
 *
 *  \param  current_channel_rx_rate
 *          - Current Channel Rx Rate.
 *
 *  \param  max_channel_tx_rate
 *          - Max Channel Tx Rate.
 *
 *  \param  max_channel_rx_rate
 *          - Max Channel Rx Rate.
 */


typedef struct
{
    unsigned long current_channel_tx_rate;
    unsigned long current_channel_rx_rate;
    unsigned long max_channel_tx_rate;
    unsigned long max_channel_rx_rate;
}WDSSWICurrentChannelRates;

/**
 * This API Queries current bitrate of a packet data connection.
 *
 *  \param  pRates [IN]
 *         - See \ref WDSSWICurrentChannelRates for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   This feature depends on custom feature setting IPCHANNELRATEEN
 *          which can be set via SLQSSetCustFeatures
 *
 *          Timeout: 2 seconds\n
 *
 */

ULONG SLQSGetCurrentChannelRate(WDSSWICurrentChannelRates *pRates,BYTE instance);

/**
 * WDS SWI Set Loopback  Structure of Set Loopback Information.
 *
 *  \param  pLoopbackMode
 *          - Loopback Mode.
                - 0 - Disable
                - 1 - Enable
 *
 *  \param  pLoopbackMultiplier
 *          - Loopback multiplier. Number of downlink bytes to send for each uplink byte.
 */

typedef struct
{
    BYTE *pLoopbackMode;
    BYTE *pLoopbackMultiplier;
} WDSSetLoopbackData;

/**
 * This API to Enable/disable Data Loopback Mode and set the value of loopback multiplier.
 *
 *  \param  pReq [IN]
 *         - See \ref WDSSetLoopbackData for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */

ULONG SLQSSSetLoopback(WDSSetLoopbackData *pReq);

/**
 * This API to Queries Enable/disable Data Loopback Mode and set the value of loopback multiplier.
 *
 *  \param  pReq [IN]
 *         - See \ref WDSSetLoopbackData for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */

/**
 * WDS SWI Get Loopback  Structure of Packet Data Connection Information.
 *
 *  \param  ByteLoopbackMode
 *          - Loopback Mode.
                - 0 - Disable
                - 1 - Enable
 *
 *  \param  ByteLoopbackMultiplier
 *          - Loopback multiplier. Number of downlink bytes to send for each uplink byte.
 */

typedef struct
{
    BYTE ByteLoopbackMode;
    BYTE ByteLoopbackMultiplier;
} WDSGetLoopbackData;

/**
 * This API to Get the value of loopback mode and multiplier.
 *
 *  \param  pReq [IN]
 *         - See \ref WDSGetLoopbackData for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */

ULONG SLQSSGetLoopback(WDSGetLoopbackData *data);

/**
 * This API to Get default profile number
 *
 *  \param  profile_type [IN]
 *  - 0 - 3GPP
 *  - 1 - 3GPP2
 *
 *  \param  profile_family [IN]
 *  - 0 - Embedded
 *  - 1 - Tethered
 *
 *  \param  pProfile_no [OUT]
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG GetDefaultProfileNum(
        BYTE profile_type,
        BYTE profile_family,
        BYTE *pProfile_no
        );

/**
 * This API to Set default profile number
 *
 *  \param  profile_type [IN]
 *  - 0 - 3GPP
 *  - 1 - 3GPP2
 *
 *  \param  profile_family [IN]
 *  - 0 - Embedded
 *  - 1 - Tethered
 *
 *  \param  profile_index [IN]
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG SetDefaultProfileNum(
        BYTE profile_type,
        BYTE profile_family,
        BYTE profile_index
        );

/**
 * WDS SWI DHCPv4 Profile Identifier Structure
 *
 *  \param  profileType
 *          - 0 for 3GPP
 *  \param  profileId
 *          - 1 to 24 for 3GPP profile
 * 
 */
typedef struct
{
    BYTE profileType;
    BYTE profileId;
} WdsDHCPv4ProfileId;

/**
 * WDS SWI DHCPv4 HW Config Structure.
 *
 *  \param  hwType
 *           - HW Type
 *           1 - Ethernet
 *           20 - Serial
 *
 *  \param  chaddrlen
 *          - chaddrlen
 * 
 *  \param  chaddr
 *          - chaddr. Max size 16 bytes
 * 
 */
typedef struct
{
    BYTE hwType;
    BYTE chaddrLen;
    BYTE chaddr[16];
} WdsDHCPv4HWConfig;

/**
 * WDS SWI DHCPv4 Option Structure
 *
 *  \param  optCode
 *          - Option code
 *              - 0 - 255
 * 
 *  \param  optValLen
 *          - Option value length
 *              - 0 - 255
 * 
 *  \param  optVal
 *          - Option value
 * 
 */
typedef struct
{
    BYTE optCode;
    BYTE optValLen;
    BYTE optVal[255];
} WdsDHCPv4Option;

/**
 * WDS SWI DHCPv4 Option List Structure
 *
 *  \param  numOpt
 *          - number of options
 *              - 0 - 255
 * 
 *  \param  pOptList
 *          - pointer to list of DHCP Options
 * 
 */
typedef struct
{
    BYTE numOpt;
    WdsDHCPv4Option *pOptList;
} WdsDHCPv4OptionList;

/**
 * WDS SWI DHCPv4 Config Structure
 *
 *  \param  pProfileId [IN]
 *          - pointer to Profile Id structure
 * 
 *  \param  pHWConfig [IN/OUT]
 *          - pointer to HW Config structure
 * 
 *  \param  pRequestOptionList [IN/OUT]
 *          - pointer to Option List structure to be sent in
 *            DHCP request
 * 
 */
typedef struct
{
    WdsDHCPv4ProfileId   *pProfileId;
    WdsDHCPv4HWConfig    *pHwConfig;
    WdsDHCPv4OptionList  *pRequestOptionList;
} WdsDHCPv4Config;

/**
 * WDS SWI DHCPv4 Client Lease Change Structure
 *
 *  \param  pEnableNotification [IN]
 *          - Enable Notification or not
 */
typedef struct
{
    BYTE *pEnableNotification;
}WdsClientLeaseChange;

/**
 * This API sets the DHCP Client V4 Configuration.
 *
 *  \param  pReq [IN]
 *         - See \ref WdsDHCPv4Config for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG SLQSSSetDHCPv4ClientConfig(WdsDHCPv4Config *pReq);

/**
 * This API gets the DHCP Client V4 Configuration.
 *
 *  \param  pReq [IN]
 *         - See \ref WdsDHCPv4Config for more information
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *          Timeout: 2 seconds\n
 *
 */
ULONG SLQSSGetDHCPv4ClientConfig(WdsDHCPv4Config *pReqResp);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_WDS_H__ */
