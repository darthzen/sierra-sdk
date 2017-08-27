/**
 * \ingroup dcs
 *
 * \file    qaGobiApiDcs.h
 *
 * \brief   Device Connectivity Service API function prototypes
 */

/*
 * Copyright: © 2011-2105 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_DCS_H__
#define __GOBI_API_DCS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LEN 10
/**
 * This structure contains the SLQSGetUsbPortNames Information
 *
 *  \param  AtCmdPort [OUT]
 *          - Name of AT command port
 *
 *  \param  NmeaPort [OUT]
 *          - Name of NMEA port
 *
 *  \param  DmPort [OUT]
 *          - Name of DM port
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 seconds\n
 *          Port names are limited to 32 characters.
 */
#define PORTNAM_LEN 32
struct DcsUsbPortNames
{
    CHAR AtCmdPort[PORTNAM_LEN];
    CHAR NmeaPort [PORTNAM_LEN];
    CHAR DmPort   [PORTNAM_LEN];
};

/**
 * This structure contains the SLQSQosDumpMap Information
 *
 *  \param  dscp
 *          - Differential Service Code Point(DSCP) value
 *
 *  \param  qos_id
 *          - QoS identifier
 *
 *  \param  state
 *          - QoS Flow state
 *
 */
struct QosMap
{
    BYTE dscp;
    ULONG qos_id;
    BYTE state;
};

/**
 * This structure contains the SLQSGetNetStatistic Information
 *
 *  \param  rx_packets
 *          - Number of received Packets without error
 *
 *  \param  tx_packets
 *          - Number of transmitted Packets without error
 *
 *  \param  rx_bytes
 *          - Number of bytes recieved without error
 *
 *  \param  tx_bytes
 *          - NNumbero of bytes transmitted without error
 *
 *  \param  rx_error
 *          - Number of incoming packets with framing errors
 *
 *  \param  tx_error
 *          - Number of outgoing packets with framing errors
 *
 *  \param  rx_overflows
 *          - Number of packets dropped because Rx buffer overflowed
 *
 *  \param  tx_overflows
 *          - Number of packets dropped because Tx buffer overflowed
 *
 */
struct NetStats
{
    ULONG rx_packets;
    ULONG tx_packets;
    ULONGLONG rx_bytes;
    ULONGLONG tx_bytes;
    ULONG rx_errors;
    ULONG tx_errors;
    ULONG rx_overflows;
    ULONG tx_overflows;
};

/**
 *  Enumerates the QC WWAN devices currently attached to the host. This API
 *  MUST be called before any other API.
 *
 *  \param  pDeviceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the device array can
 *          contain.
 *          - Upon successful output, actual number of elements in the
 *          device array.
 *
 *  \param  pDevices[IN/OUT]
 *          - Device array; array elements are structures with the following elements:\n
 *          CHAR deviceID[256] - Device path (e.g. /dev/qcqmi0)\n
 *          CHAR deviceKey[16] - Device key stored in the device (e.g. A1000004B01051)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_NO_DEVICE on otherwise
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices );

/**
 *  Connects the Connection Manager API to the first detected QC WWAN device.
 *  This function MUST be called after QCWWAN2kEnumerateDevices has been called.
 *
 *  \param  pDeviceID[IN]
 *          - Device path pertaining to the device for which the API is being
 *          invoked e.g. /dev/qcqmi0.
 *
 *  \param  pDeviceKey[IN]
 *          - Device key pertaining to the device for which the API is being invoked
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kConnect(
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey );

/**
 *  Disconnects the Connection Manager API from a previously connected QC device.
 *  This function de-registers all the callback functions that have been registered.
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG QCWWANDisconnect();

/**
 *  Returns the device ID and device key of the currently connected QC WWAN
 *  device.
 *
 *  \param  deviceIDSize
 *          - Maximum number of characters (including NULL terminator) that the
 *          device ID array can contain.
 *
 *  \param  pDeviceID[OUT]
 *          - Device path string
 *
 *  \param  deviceKeySize
 *          - Maximum number of characters (including NULL terminator) that the
 *          device key array can contain.
 *
 *  \param  pDeviceKey[OUT]
 *          - Device key string
 *
 *  \return eQCWWAN_ERR_NONE if device found, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kGetConnectedDeviceID(
    ULONG    deviceIDSize,
    CHAR    *pDeviceID,
    ULONG    deviceKeySize,
    CHAR    *pDeviceKey );

/**
 *  Enumerates the QC WWAN devices currently attached to the host. This API is
 *  deprecated; use QCWWAN2kEnumerateDevices() instead.
 *
 *  \param  pDeviceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the device array can
 *          contain.
 *          - Upon successful output, actual number of elements in the device
 *          array.
 *
 *  \param  pDevices[IN/OUT]
 *          - Device array; array elements are structures with the following elements:\n
 *          CHAR deviceID[256] - Device path (e.g. /dev/qcqmi0)\n
 *          CHAR deviceKey[16] - Device key stored in the device
 *
 *  \return eQCWWAN_ERR_NONE
 *
 *  \note   Timeout: 2 seconds\n
 *          This API must be called prior to any other APIs.
 *
 */
ULONG QCWWANEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices );

/**
 *  Enumerates the QC WWAN devices currently attached to the host.
 *  This API MUST be called before any other API.
 *
 *  \param  pDeviceID[IN]
 *          - Device path pertaining to the device for which the API is being
 *          invoked e.g. /dev/qcqmi0.
 *  \param  pDeviceKey[IN]
 *          - Device key pertaining to the device for which the API is being invoked
 *
 *  \return eQCWWAN_ERR_NONE if device found, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Timeout: 2 seconds\n
 *          This API is deprecated; use QCWWAN2kConnect() instead
 *
 */
ULONG  QCWWANConnect(
    CHAR *pDeviceID,
    CHAR *pDeviceKey);

/**
 *  Set the location of the SLQS executable
 *
 *  \param   pPath[IN]  - Pointer to fully qualified path of SLQS executable
 *                        (includes the executable file’s name)
 *
 *  \return  eQCWWAN_ERR_NONE
 *
 *  \note   Timeout: None\n
 *
 */
ULONG SetSDKImagePath (
    LPCSTR pPath );

/**
 *  Returns the Usb Port Names currently in use.
 *
 *  \param  pUsbPortNames[OUT]
 *          - Pointer to SLQS USB Port Names Information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSGetUsbPortNames(
    struct DcsUsbPortNames *pUsbPortNames );


/**
 *  Create IPC sockets for AirVantage Agent and SDK processes to communicate over
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: N/A
 *
 */
ULONG SLQSStart_AVAgent(BYTE modem_index);

/**
 *  Create the SDK process and IPC sockets for the Application and SDK
 *  processes to communicate over.
 *
 *  \param[in]  modem_index
 *              - 0: first modem detected
 *              - 1: second modem detected
 *              - 2: third modem detected
 *              - ...
 *              - 7: seventh modem detected
 *
 *  \param[in]  sn
 *              This field is optional, it can be serial number or usb path 
 *              for multiple modem feature, it can be retrieved from sytem command "dmesg"
 *              when specified, the modem_index will be mapping to sn or usb path
 *              Please set to NULL when not used
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: N/A
 *
 */
ULONG SLQSStart(BYTE modem_index, CHAR* sn);

/**
 *  Kill the SDK process
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: N/A\n
 *          This API useful if the application was started with non-root
 *          privileges as subsequent attempt to start any application will fail
 *          because the SDK requires root permission to access /dev/qcqmi
 *          device special files.
 *
 */
ULONG SLQSKillSDKProcess();

/**
 *  Returns the Device Mode
 *
 *  \param  pDeviceMode[OUT]
 *          - Pointer to SLQS Device Mode of type \ref eDevState
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *
 */
ULONG SLQSGetDeviceMode(
    BYTE *pDeviceMode );

/**
 *  Registers/deregisters for service with unsolicited notifications
 *
 *  \param  action, 1 for register, 0 for deregister
 *
 *  \param  mask
 *          - Bit mask for unsolicited notifications
 *            - Bit0 - WDS
 *            - Bit1 - NAS
 *            - Bit2 - PDS
 *            - Bit3 - VOICE
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 *          API is useful to register for the services which supports
 *          unsolicited notifications. Registration/deregistration can be done
 *          by using parameter action if action is set then the mask (set bits)
 *          will be used for registering service and if action is "0"
 *          mask(set bits) will be used to deregister services. For example :
 *          bit mask 0x03 - Registers for services WDS and NAS if action is "1"
 *          and deregisters WDS and NAS if action is "0".
 */
ULONG SLQSStartSrv( BYTE action, BYTE mask );

/**
 *  Limit Syslog messages according to the Mask provided by user
 *
 *  \param  mask
 *          - Mask 0x01: disable all log
 *          - Mask 0xFF: enable all log
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 */
ULONG  SLQSSetLoggingMask(
    BYTE mask );

/**
 *  Returns the usbnet statistics for a particular PDN.
 *
 *  \param  pNetStatistic[OUT]
 *          - Pointer to the structure NetStats which the value of every member is to be retrieved
 *
 *  \param[in]  instance
 *              - PDP Instance id
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 seconds
 *
 */
ULONG  SLQSGetNetStatistic(
    struct NetStats *pNetStatistic, BYTE instance );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_DCS_H__ */
