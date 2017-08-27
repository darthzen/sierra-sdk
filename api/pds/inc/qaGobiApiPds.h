/**
 * \ingroup pds
 *
 * \file    qaGobiApiPds.h
 *
 * \brief   Position Determination Service API function prototypes
 */

/*
 * Copyright: © 2011-2015 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_PDS_H__
#define __GOBI_API_PDS_H__

#define DEFAULTBYTEVALUE 0xFF
#define DEFAULTWORDVALUE 0xFFFF
#define DEFAULTLONGVALUE 0xFFFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Returns the current PDS state.
 *
 *  \param  pEnabledStatus[OUT]
 *          - Current PDS state
 *              - 0 - disable
 *              - 1 - enable
 *
 *  \param  pTrackingStatus[OUT]
 *          - Current PDS tracking session state
 *          - Values:
 *              - 0x00 - Unknown
 *              - 0x01 - Inactive
 *              - 0x02 - Active
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 seconds
 */
ULONG GetPDSState(
    ULONG *pEnabledStatus,
    ULONG *pTrackingStatus );

/**
 *  Sets the PDS state.
 *
 *  \param  enable[IN]
 *          - Desired PDS state
 *              - Zero     - disable
 *              - Non-Zero - enable
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Seconds
 *
 */
ULONG SetPDSState(
    ULONG enable );

/**
 *  This function starts a PDS tracking session.
 *
 *  \param  sessionControl[IN]
 *          - Control method:
 *              - 0x0 - Manual
 *
 *  \param  sessionType[IN]
 *          - Type:
 *              - 0x0 - New
 *
 *  \param  sessionOperation[IN]
 *          - Operating mode:
 *              - 0x00 - Standalone
 *              - 0x01 - MS-based
 *
 *  \param  sessionServerOption[IN]
 *          - Location server option:
 *              - 0x0 - Default
 *
 *  \param  fixTimeout[IN]
 *          - Maximum time to work on each fix (in seconds, max 255)
 *
 *  \param  fixCount[IN]
 *          - Count of position fix requests for this session
 *            (must be at least 1)
 *
 *  \param  fixInterval[IN]
 *          - interval between position fix requests (in seconds)
 *
 *  \param  fixAccuracy[IN]
 *          - Preferred accuracy threshold(in meters)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG StartPDSTrackingSessionExt(
    BYTE  sessionControl,
    BYTE  sessionType,
    BYTE  sessionOperation,
    BYTE  sessionServerOption,
    BYTE  fixTimeout,
    ULONG fixInterval,
    ULONG fixCount,
    ULONG fixAccuracy );

/**
 *  This function stops a PDS tracking session.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG StopPDSTrackingSession();

/**
 *  Injects a system time into the PDS engine.
 *
 *  \param  systemTime
 *          - System time( milliseconds )
 *
 *  \param  systemDiscontinuities
 *          - Number of system time discontinuities
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG PDSInjectTimeReference(
    ULONGLONG systemTime,
    USHORT    systemDiscontinuities );

/**
 *  Returns the default tracking session configuration.
 *  The tracking session configuration is used when a tracking session is
 *  automatically started using SetServiceAutomaticTracking or due to the
 *  device detecting an application opening the NMEA port.
 *
 *  \param  pOperation[OUT]
 *          - Current session operating mode
 *              - 0 - Standalone
 *              - 1 - MS based
 *              - 2 - MS assisted
 *
 *  \param  pTimeout[OUT]
 *          - Maximum amount of time (seconds) to work on each fix, maximum is 255
 *
 *  \param  pInterval[OUT]
 *          - Interval (seconds) between fix requests
 *
 *  \param  pAccuracy[OUT]
 *          - Preferred accuracy threshold (meters)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetPDSDefaults(
    ULONG *pOperation,
    BYTE  *pTimeout,
    ULONG *pInterval,
    ULONG *pAccuracy );

/**
 *  Sets the default tracking session configuration.
 *  The tracking session configuration is used when a tracking session is
 *  automatically started using SetServiceAutomaticTracking or due to the
 *  device detecting an application opening the NMEA port.
 *
 *  \param  operation
 *          - Current session operating mode
 *              - 0 - Standalone
 *              - 1 - MS based
 *              - 2 - MS assisted
 *
 *  \param  timeout
 *          - Maximum amount of time (seconds) to work on each fix, maximum is 255
 *
 *  \param  interval
 *          - Interval (seconds) between fix requests
 *
 *  \param  accuracy
 *          - Preferred accuracy threshold (meters)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SetPDSDefaults(
    ULONG operation,
    BYTE  timeout,
    ULONG interval,
    ULONG accuracy );

/**
 *  Returns the XTRA automatic database download configuration.
 *
 *  \param  pbEnabled[OUT]
 *          - Automatic XTRA download status
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \param  pInterval[OUT]
 *          - Interval (hours) between XTRA downloads
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetXTRAAutomaticDownload(
    ULONG  *pbEnabled,
    USHORT *pInterval );

/**
 *  Sets the XTRA automatic database download configuration.
 *
 *  \param  bEnabled[IN]
 *          - Automatic XTRA download status
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \param  interval[IN]
 *          - Interval (hours) between XTRA downloads
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SetXTRAAutomaticDownload(
    ULONG  bEnabled,
    USHORT interval );

/**
 *  Returns the XTRA WWAN network preference.
 *  When automatic XTRA database downloading is enabled this preference
 *  determines which WWAN networks will be used to perform the XTRA
 *  database download.
 *
 *  \param  pPreference[OUT]
 *          - XTRA WWAN network preference
 *              - 0x00 - None (any available network)
 *              - 0x01 - Home-only, only when on home systems
 *              - 0x02 - Roam-only, only when on non-home systems
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetXTRANetwork(
    ULONG *pPreference );

/**
 *  Sets the XTRA WWAN network preference.
 *  When automatic XTRA database downloading is enabled this preference
 *  determines which WWAN networks will be used to perform the XTRA
 *  database download.
 *
 *  \param  preference[IN]
 *          - XTRA WWAN network preference
 *              - 0x00 - None (any available network)
 *              - 0x01 - Home-only, only when on home systems
 *              - 0x02 - Roam-only, only when on non-home systems
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SetXTRANetwork(
    ULONG preference );

/**
 *  Returns the XTRA database validity period.
 *  When automatic XTRA database downloading is enabled the validity
 *  period determines when the XTRA database will be updated through
 *  a new download.
 *
 *  \param  pGPSWeek[OUT]
 *          - Starting GPS week of validity period
 *
 *  \param  pGPSWeekOffset[OUT]
 *          - Starting GPS week offset (minutes) of validity period
 *
 *  \param  pDuration[OUT]
 *          - Length of validity period (hours)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetXTRAValidity(
    USHORT *pGPSWeek,
    USHORT *pGPSWeekOffset,
    USHORT *pDuration );

/**
 *  Forces the XTRA database to be downloaded to the device.
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values

 *  \note   Timeout: 2 Seconds
 *
 */
ULONG ForceXTRADownload();

/**
 *  Returns the automatic tracking state for the service.
 *
 *  \param  pbAuto[OUT]
 *          - Automatic tracking session started for service
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetServiceAutomaticTracking(
    ULONG *pbAuto );

enum {
   eSetServiceAutomaticTrackingDisable=0,
   eSetServiceAutomaticTrackingEnable=1
};
/**
 *  Sets the automatic tracking state for the service.
 *  Tracking session being started using the default session configuration.
 *  Auto-tracking continues to generate fixes indefinitely until requested to be disabled.
 *  In StartPDSTrackingSessionExt a tracking session get started using the specified session
 *  control method and input parameters. After completion of requested no. of position fixes or
 *  service times out to perform fix, tracking session ends and GPS service deactivates.
 *
 *  \param  bAuto[IN]
 *          - Automatic tracking session started for service
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */

ULONG SetServiceAutomaticTracking(
    ULONG bAuto );

/**
 *  Returns the automatic tracking configuration for the NMEA COM port.
 *
 *  \param  pbAuto[OUT]
 *          - Automatic tracking enabled for NMEA COM port
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG GetPortAutomaticTracking(
    ULONG *pbAuto );

/**
 *  Sets the automatic tracking configuration for the NMEA COM port.
 *
 *  \param  bAuto[IN]
 *          - Enable automatic tracking for NMEA COM port
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SetPortAutomaticTracking(
    ULONG bAuto );

/**
 *  Resets the specified PDS data.
 *
 *  \param  pGPSDataMask[IN]
 *          - Bitmask of GPS data to clear (optional)
 *              - 0x00000001 - EPH
 *              - 0x00000002 - ALM
 *              - 0x00000004 - POS
 *              - 0x00000008 - TIME
 *              - 0x00000010 - IONO
 *              - 0x00000020 - UTC
 *              - 0x00000040 - HEALTH
 *              - 0x00000080 - SVDIR
 *              - 0x00000100 - SVSTEER
 *              - 0x00000200 - SADATA
 *              - 0x00000400 - RTI
 *              - 0x00000800 - ALM_CORR
 *              - 0x00001000 - FREQ_BIAS_EST
 *
 *  \param  pCellDataMask[IN]
 *          - Bitmask of cell data to clear (optional)
 *              - 0x00000001 - POS
 *              - 0x00000002 - LATEST_GPS_POS
 *              - 0x00000004 - OTA_POS
 *              - 0x00000008 - EXT_REF_POS
 *              - 0x00000010 - TIMETAG
 *              - 0x00000020 - CELLID
 *              - 0x00000040 - CACHED_CELLID
 *              - 0x00000080 - LAST_SRV_CELL
 *              - 0x00000100 - CUR_SRV_CELL
 *              - 0x00000200 - NEIGHBOR_INFO
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG ResetPDSData(
    ULONG *pGPSDataMask,
    ULONG *pCellDataMask );

/**
 *  Sets the PDS AGPS (MS-based) configuration.
 *
 *  \param  pServerAddress[IN]
 *          - IPv4 address of AGPS server [optional]
 *
 *  \param  pServerPort[IN]
 *          - Port number of AGPS server [optional - should be present
 *            when pServerAddress is present]
 *
 *  \param  pServerURL[IN]
 *          - URL of the AGPS server [optional]
 *
 *  \param  pServerURLLength[IN]
 *          - URL length of AGPS server [optional - should be present
 *            when pServerURL is present]
 *
 *  \param  pNetworkMode[IN]
 *          - Network Mode of AGPS Server [optional - should be present
 *            in Multimode Systems]
 *              - 0x00 - UMTS
 *              - 0x01 - CDMA
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSSetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength,
    BYTE  *pNetworkMode );

/**
 *  Injects a absolute time reference into the PDS engine.
 *
 *  \param  timeMsec[IN]
 *          - Represents the number of milliseconds elapsed since either
 *            a GPS or UTC time base. If the time base is UTC, this value
 *            should NOT include leap seconds
 *
 *  \param  timeUncMsec[IN]
 *          - Time uncertainty in milliseconds
 *
 *  \param  timeBase[IN]
 *          - Time base
 *              - 0x00 - GPS (midnight, Jan 6, 1980)
 *              - 0x01 - UTC (midnight, Jan 1, 1970)
 *
 *  \param  forceFlag[IN]
 *          - Force acceptance of data
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSPDSInjectAbsoluteTimeReference(
    ULONGLONG timeMsec,
    ULONG     timeUncMsec,
    BYTE      timeBase,
    BYTE      forceFlag );

/**
 *  Gets the PDS AGPS (MS-based) configuration.
 *
 *  \param  pServerAddress[OUT]
 *          - IPv4 address of AGPS server. "0" if not set
 *
 *  \param  pServerPort[OUT]
 *          - Port number of AGPS server. "0" if not set
 *
 *  \param  pServerURL[OUT]
 *          - URL of the AGPS server. "0" if not set
 *
 *  \param  pServerURLLength[OUT]
 *          - URL length of AGPS server. "0" if not set
 *
 *  \param  pNetworkMode[IN]
 *          - Network Mode of AGPS Server [optional - should be present
 *            in Multimode Systems]
 *              - 0x00 - UMTS
 *              - 0x01 - CDMA
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSGetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength,
    BYTE  *pNetworkMode);

/**
 *  Position Data Parameters from the external source to be injected to PDS
 *  engine.
 *
 *  \param  pTimeStamp
 *          - Timestamp of the injected position in msec. The time can be of
 *            type UTC, GPS, or Age and is defined in the pTimeType parameter.
 *            If the pTimeType is not present, the timestamp shall be
 *            assumed to be UTC time
 *
 *  \param  pLatitude
 *          - Latitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles north of the equator and negative angles
 *            south of the equator. Value (in decimal degrees) in the range
 *            from -90 degrees to +90 degrees.Value in double float format
 *            (refer toIEEE Std 754-1985)
 *
 *  \param  pLongitude
 *          - Longitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles east of the Greenwich Meridian and
 *            negative angles west of Greenwich meridian. Value (in decimal
 *            degrees) in the range from -180 degrees to +180 degrees.
 *
 *  \param  pAltitudeWrtEllipsoid
 *          - Height above the WGS-84 reference ellipsoid. Value conveys
 *          height (in meters). When injecting altitude information, the
 *          control point should include either this parameter or the
 *          pAltitudeWrtSealevel parameter. Value in single float format
 *          (refer to IEEE Std 754-1985)
 *
 *  \param  pAltitudeWrtSealevel
 *          - Height of MS above the mean sea level in units (in meters).
 *          When injecting altitude information, the control point should
 *          include either this parameter or the pAltitudeWrtEllipsoid
 *          parameter. Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  pHorizontalUncCircular
 *          - Circular horizontal uncertainty (in meters). This parameter must
 *          be included if the latitude and longitude parameters are specified.
 *          Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  pVerticalUnc
 *          - Vertical uncertainty (in meters). This parameter must be included
 *          if one of the altitude parameter are specified.Value in single float
 *          format (refer to IEEE Std 754-1985)
 *
 *  \param  pHorizontalConfidence
 *          - Confidence value of the location horizontal uncertainty,
 *          specified as percentage, 1 to 100. This parameter must be included
 *          if the latitude and longitude parameters are specified.
 *
 *  \param  pVerticalConfidence
 *          - Confidence value of the location vertical uncertainty, specified
 *          as percentage, 1 to 100. This parameter must be included if one of
 *          the altitude paramters are specified.
 *
 *  \param  pPositionSource
 *          - Source of injected position:
 *              - 0x00 - Unknown
 *              - 0x01 - GPS
 *              - 0x02 - Cell ID
 *              - 0x03 - Enhanced cell ID
 *              - 0x04 - WiFi
 *              - 0x05 - Terrestrial
 *              - 0x06 - Terrestrial hybrid
 *              - 0x07 - Other
 *
 *  \param  pTimeType
 *          - Defines the time value set in the pTimeStamp parameter.
 *              - 0x00 - UTC Time: starting Jan 1, 1970
 *              - 0x01 - GPS Time: starting Jan 6, 1980
 *              - 0x02 - Age: Age of position information
 *
 */
struct PDSPositionData
{
    ULONGLONG *pTimeStamp;
    ULONGLONG *pLatitude;
    ULONGLONG *pLongitude;
    ULONG     *pAltitudeWrtEllipsoid;
    ULONG     *pAltitudeWrtSealevel;
    ULONG     *pHorizontalUncCircular;
    ULONG     *pVerticalUnc;
    BYTE      *pHorizontalConfidence;
    BYTE      *pVerticalConfidence;
    BYTE      *pPositionSource;
    BYTE      *pTimeType;
};

/**
 *  Injects position data into the PDS engine.
 *
 *  \param  pPositionData[IN]
 *          - contains the position data to be injected to the PDS engine
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 2 Seconds
 *
 */
ULONG SLQSPDSInjectPositionData( struct PDSPositionData *pPositionData );

/**
 *  Requests the MSM GPS service to obtain the current position for manually
 *  controlled tracking sessions.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Seconds
 *
 */
ULONG SLQSPDSDeterminePosition( );

/**
 *  GPS state Info.
 *
 *  \param  EngineState
 *          - Values:
 *              - 0 - OFF
 *              - 1 - ON
 *          - This field is always valid
 *
 *  \param  ValidMask
 *          - Mask of valid state information data.
 *          - Values:
 *              - 0x00000001 - Position(latitude/longitude/horizontal uncertainty)
 *              - 0x00000002 - Altitude and vertical uncertainty
 *              - 0x00000004 - Time ms
 *              - 0x00000008 - Time week number
 *              - 0x00000010 - Time uncertainty
 *              - 0x00000020 - Iono validity
 *              - 0x00000040 - GPS ephemeris
 *              - 0x00000080 - GPS almanac
 *              - 0x00000100 - GPS health
 *              - 0x00000200 - GPS visible SVs
 *              - 0x00000400 - GLONASS ephemeris
 *              - 0x00000800 - GLONASS almanac
 *              - 0x00001000 - GLONASS health
 *              - 0x00002000 - GLONASS visible SVs
 *              - 0x00004000 - SBAS ephemeris
 *              - 0x00008000 - SBAS almanac
 *              - 0x00010000 - SBAS health
 *              - 0x00020000 - SBAS visible SVs
 *              - 0x00040000 - XTRA information
 *
 *  \param  Latitude
 *          - Latitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles north of the equator and negative angles
 *            south of the equator.
 *          - Units: Decimal degrees
            - Range: -90 to +90 degrees.
            - Value is in double float format (refer to IEEE Std 754-1985)
 *
 *  \param  Longitude
 *          - Longitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles east of the Greenwich Meridian and
 *            negative angles west of Greenwich meridian.
 *          - Units: Decimal degrees
 *          - Range: -180 to +180 degrees
 *          - Value is in double float format (refer to IEEE Std 754-1985)
 *
 *  \param  HorizontalUncertainty
 *          - Circular horizontal uncertainty (in meters). The uncertainty is
 *            provided at 63 percent confidence.
 *          - Value is in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  Altitude
 *          - Height above the WGS-84 reference ellipsoid. Value conveys
 *            height (in meters) plus 500 m
 *          - Range -500 to 15883
 *          - Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  VerticalUncertainty
 *          - Vertical uncertainty (in meters). The uncertainty is
 *            provided at 68 percent confidence.
 *          - Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  TimeStmp_tow_ms
 *          - Time stamp in GPS time of week( in milliseconds)
 *
 *  \param  TimeStmp_gps_week
 *          - GPS week number
 *
 *  \param  Time_uncert_ms
 *          - Time uncertainty (in milliseconds). The uncertainty
 *            is provided at 99 percent confidence.
 *
 *  \param  Iono_valid
 *          - Iono validity.
 *          - Values:
 *              - 0 - Invalid
 *              - 1 - Valid
 *
 *  \param  gps_ephemeris_sv_msk
 *          - GPS SV mask for ephemeris; if the bit is set, ephemeris for that
 *            SV is available.
 *
 *  \param  gps_almanac_sv_msk
 *          - GPS SV mask for almanac; if the bit is set, almanac for that
 *            SV is available.
 *
 *  \param  gps_health_sv_msk
 *          - GPS SV mask for health; if the bit is set, health for that
 *            SV is available.
 *
 *  \param  gps_visible_sv_msk
 *          - GPS SV mask for visible Svs; if the bit is set, the SV is
 *            available.
 *
 *  \param  glo_ephemeris_sv_msk
 *          - GLONASS SV mask for ephemeris; if the bit is set, ephemeris
 *            for that SV is available.
 *
 *  \param  glo_almanac_sv_msk
 *          - GLONASS SV mask for almanac; if the bit is set, almanac for that
 *            SV is available.
 *
 *  \param  glo_health_sv_msk
 *          - GLONASS SV mask for health; if the bit is set, health for that
 *            SV is available.
 *
 *  \param  glo_visible_sv_msk
 *          - GLONASS SV mask for visible SVs; if the bit is set, the SV is
 *            available.
 *
 *  \param  sbas_ephemeris_sv_msk
 *          - SBAS SV mask for ephemeris; if the bit is set, ephemeris
 *            for that SV is available.
 *
 *  \param  sbas_almanac_sv_msk
 *          - SBAS SV mask for almanac; if the bit is set, almanac for that
 *            SV is available.
 *
 *  \param  sbas_health_sv_msk
 *          - SBAS SV mask for health; if the bit is set, health for that
 *            SV is available.
 *
 *  \param  sbas_visible_sv_msk
 *          - SBAS SV mask for visible SVs; if the bit is set, the SV is
 *            available.
 *
 *  \param  xtra_start_gps_week
 *          - Current XTRA information is valid starting from this GPS week
 *            number
 *
 *  \param  xtra_start_gps_minutes
 *          - Current XTRA information is valid starting from the GPS minutes
 *            with the GPS week
 *
 *  \param  xtra_valid_duration_hours
 *          - XTRA information is valid for this many hours starting from the
 *            specified GPS week/minutes
 */
typedef struct
{
    BYTE      EngineState;
    ULONG ValidMask;
    ULONGLONG Latitude;
    ULONGLONG Longitude;
    ULONG HorizontalUncertainty;
    ULONG Altitude;
    ULONG VerticalUncertainty;
    ULONG TimeStmp_tow_ms;
    WORD TimeStmp_gps_week;
    ULONG Time_uncert_ms;
    BYTE Iono_valid;
    ULONG gps_ephemeris_sv_msk;
    ULONG gps_almanac_sv_msk;
    ULONG gps_health_sv_msk;
    ULONG gps_visible_sv_msk;
    ULONG glo_ephemeris_sv_msk;
    ULONG glo_almanac_sv_msk;
    ULONG glo_health_sv_msk;
    ULONG glo_visible_sv_msk;
    ULONG sbas_ephemeris_sv_msk;
    ULONG sbas_almanac_sv_msk;
    ULONG sbas_health_sv_msk;
    ULONG sbas_visible_sv_msk;
    WORD xtra_start_gps_week;
    WORD xtra_start_gps_minutes;
    WORD xtra_valid_duration_hours;
}GPSStateInfo;

/**
 *  Queries the MSM GPS server for receiver state information
 *
 *  \param  pGPSStateInfo[OUT]
 *          - contains the GPS State Info
 *          - See \ref GPSStateInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Seconds
 *
 */
ULONG SLQSGetGPSStateInfo( GPSStateInfo *pGPSStateInfo );

/**
 *  Parameters to Set state of positioning method for a device.
 *
 *  \param  pXtraTimeState
 *          - XTRA Time Position Method State.
 *          - Values:
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pXtraDataState
 *          - XTRA Data Position Method State.
 *          - Values:
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  Latitude
 *          - WiFi Position Method State
 *          - Values:
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 */
typedef struct
{
    BYTE      *pXtraTimeState;
    BYTE      *pXtraDataState ;
    BYTE      *pWifiState;
}PDSPosMethodStateReq;

/**
 *  Sets the state of positioning methods for the device.
 *
 *  \param  pPDSPosMethodStateReq[IN]
 *          - See \ref PDSPosMethodStateReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Timeout: 5 Seconds
 *
 */
ULONG SLQSSetPositionMethodState( PDSPosMethodStateReq *pPDSPosMethodStateReq );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_PDS_H__ */
