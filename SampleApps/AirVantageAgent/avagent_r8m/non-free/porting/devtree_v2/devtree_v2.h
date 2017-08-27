/**
 * @file devtree_v2.h  Device Management Tree Specification - Version 2
 *
 * This file defines the standard function APIs that must be exported by shared object
 * libraries that implement the standard Device Management Tree API, version 2.
 * Any such library MUST be called libdevtree.2.so and MUST implement the functions
 * specified in this file EXACTLY as they are specified.
 *  
 * Copyright (C) Sierra Wireless, Inc., 2012.
 */

#ifndef __DEVTREE_V2_H
#define __DEVTREE_V2_H

/* ==================================================================
 *  DATA TYPE DEFINITIONS
 * ================================================================== */

/**
 * Result codes.
 */
typedef enum
{
    DT2_RESULT_OK,                  /**< Success! */
    DT2_RESULT_TRUNCATED,           /**< The value could not fit in the buffer provided. */
    DT2_RESULT_DEVICE_NOT_FOUND,    /**< The device is not connected or is not working. */
    DT2_RESULT_WRONG_MODE,          /**< The device is in a mode that does not allow this operation. */
    DT2_RESULT_ERROR,               /**< The device reported an error. */
}
dt2_ResultCode_t;


/* ==================================================================
 *  ACCESSOR FUNCTION PROTOTYPES
 * ================================================================== */

/** String "Get" function prototype.
 * 
 * All "get" accessor functions for string type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the value buffer contents are undefined. 
 */
typedef dt2_ResultCode_t (* dt2_StringGetFunc_t)
(
    char*   valuePtr,       /**< Pointer to the buffer into which the value will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Integer "Get" function prototype.
 * 
 * All "get" accessor functions for integer type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the value buffer contents are undefined. 
 */
typedef dt2_ResultCode_t (* dt2_IntegerGetFunc_t)
(
    int*    valuePtr        /**< Pointer to the buffer into which the value will be copied. */
);


/** Double-precision floating-point "Get" function prototype.
 * 
 * All "Get" accessor functions for "double" type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the value buffer contents are undefined. 
 */
typedef dt2_ResultCode_t (* dt2_DoubleGetFunc_t)
(
    double* valuePtr          /**< Pointer to the buffer into which the value will be copied. */
);


/** String "Set" function prototype.
 * 
 * All "set" accessor functions for string type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure. 
 */
typedef dt2_ResultCode_t (* dt2_StringSetFunc_t)
(
    const char* stringPtr       /**< Pointer to the new value. */
);


/** Integer "Set" function prototype.
 * 
 * All "set" accessor functions for integer type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure. 
 */
typedef dt2_ResultCode_t (* dt2_IntegerSetFunc_t)
(
    int    value        /**< The new value. */
);


/** Double-precision floating-point "Set" function prototype.
 * 
 * All "Set" accessor functions for "double" type variables must conform to this prototype.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure. 
 */
typedef dt2_ResultCode_t (* dt2_DoubleSetFunc_t)
(
    double  value       /**< The new value. */
);


/* ==================================================================
 *  VARIABLE ACCESSOR FUNCTION DECLARATIONS
 * ================================================================== */

/** Get Device ID.
 * 
 * This function is used to fetch the device ID that will be used to identify this
 * device to the Air Vantage server.
 * 
 * The device ID is a string of ANSI characters, usually (but not necessarily) the IMEI.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined. 
 */
dt2_ResultCode_t dt2_GetDeviceId
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */ 
);


/* ==================================================================
 *  system.cellular.*
 * ================================================================== */

/** Get APN Override.
 * 
 * This function is used to fetch the current APN override setting.  This is a writeable
 * string that can be used to override the default APN to be used by the device.
 * 
 * The APN is a string of ANSI characters.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined. 
 */
dt2_ResultCode_t dt2_GetApnOverride
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */ 
);


/** Set APN Override.
 * 
 * This function is used to set the current APN override setting.  This is a writeable
 * string that can be used to override the default APN to be used by the device.
 * 
 * The APN is a string of ANSI characters.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure. 
 */
dt2_ResultCode_t dt2_SetApnOverride
(
    const char* stringPtr       /**< Pointer to the new value. */
);


/** Get APN.
 * 
 * This function is used to fetch the current APN being used by the device.  This is a
 * read-only variable.
 * 
 * The APN is a string of ANSI characters.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetApn
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */ 
);


/** Get CDMA Link ECIO.
 * 
 * This function is used to fetch the name of the current CDMA link ecio, if any.
 * This is a read-only variable.
 * 
 * The ecio value is numeric. If device is currently not in cdma, the value is set to NULL
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetCdmaEcio
(
    double*   valuePtr      /**< Pointer to the buffer into which the ecio will be copied. */
);


/** Get CDMA Link Operator.
 *
 * This function is used to fetch the name of the current CDMA link operator, if any.
 * This is a read-only variable.
 *
 * The operator name is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetCdmaOperator
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get CDMA Link PN offset.
 *
 * This function is used to fetch the name of the current CDMA link PN offset, if any.
 * This is a read-only variable.
 *
 * The PN offset is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetCdmaPnOff
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get CDMA Link SID.
 *
 * This function is used to fetch the name of the current CDMA link SID, if any.
 * This is a read-only variable.
 *
 * The SID is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetCdmaSid
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get CDMA Link NID.
 *
 * This function is used to fetch the name of the current CDMA link NID, if any.
 * This is a read-only variable.
 *
 * The NID is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetCdmaNid
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get GSM Link Operator.
 * 
 * This function is used to fetch the name of the current GSM link operator, if any.
 * This is a read-only variable.
 * 
 * The operator name is a string of ANSI characters.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetGsmLinkOperator
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get GSM Cell ID.
 *
 * This function is used to fetch the name of the current GSM Cell ID, if any.
 * This is a read-only variable.
 *
 * The operator name is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetGsmCellId
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get IMEI.
 * 
 * This function is used to fetch the device's IMEI.  This is a read-only variable.
 * 
 * The IMEI is a string of ANSI characters.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetImei
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */ 
);


/** Get ICCID.
 *
 * This function is used to fetch the SIM's ICCID.  This is a read-only variable.
 *
 * The ICCID is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetIccid
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);

/** Get IMSI.
 *
 * This function is used to fetch the SIM's IMSI.  This is a read-only variable.
 *
 * The IMSI is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetImsi
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get Phone Number.
 *
 * This function is used to fetch the Phone Number.  This is a read-only variable.
 *
 * The Phone Number is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetPhoneNum
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Get Pin Status.
 *
 * This function is used to fetch the Pin Status.  This is a read-only variable.
 *
 * The Pin Status is a string of ANSI characters.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetPinStatus
(
    char*   stringPtr,      /**< Pointer to the buffer into which the string will be copied. */
    size_t  buffSize        /**< Size of the buffer, in bytes. */
);


/** Roaming Status Codes.
 * 
 * These are the possible values of the "Roaming Status" variable.
 */
typedef enum
{
    DT2_ROAM_STATUS_NOT_ROAMING = 0,
    DT2_ROAM_STATUS_ROAMING = 1,
    DT2_ROAM_STATUS_INTERNATIONAL = 2,
}
dt2_RoamingStatus_t;


/** Get Roaming Status.
 * 
 * Fetches the current roaming status of the device.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the value buffer contents are undefined. 
 */
dt2_ResultCode_t dt2_GetRoamStatus
(
    dt2_RoamingStatus_t* valuePtr  /**< Pointer to the buffer into which the value will be copied. */
);


/** Get RSSI.
 * 
 * Fetches the current cellular signal strength (RSSI).
 * 
 * The RSSI is a signed integer value from -150 to 0 dBm.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the value buffer contents are undefined. 
 */
dt2_ResultCode_t dt2_GetRssi
(
    int*    valuePtr        /**< Pointer to the buffer into which the value will be copied. */
);

    
/* ==================================================================
 *  system.gps.*
 * ================================================================== */

/** Get Altitude.
 * 
 * This function is used to fetch the device's current altitude, if equipped with
 * a sensor that provides this data (such as GPS).  This is a read-only variable.
 * 
 * The altitude value is numeric.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetAltitude
(
    double*   valuePtr      /**< Pointer to the buffer into which the altitude will be copied. */
);


/** Get Latitude.
 * 
 * This function is used to fetch the device's current latitude, if equipped with
 * a sensor that provides this data (such as GPS).  This is a read-only variable.
 * 
 * The latitude value is numeric.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetLatitude
(
    double*   valuePtr      /**< Pointer to the buffer into which the latitude will be copied. */
);


/** Get Longitude.
 * 
 * This function is used to fetch the device's current longitude, if equipped with
 * a sensor that provides this data (such as GPS).  This is a read-only variable.
 * 
 * The longitude value is numeric.
 * 
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the string buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetLongitude
(
    double*   valuePtr      /**< Pointer to the buffer into which the longitude will be copied. */
);

/** Get Date Time.
 *
 * This function is used to fetch the current time stamp of the device.
 * This is a read-only variable.
 *
 * The date time value is numeric.
 *
 * @return Result code.  Anything other than DT2_RESULT_OK indicates a failure, in which
 *         case the buffer contents are undefined.
 */
dt2_ResultCode_t dt2_GetDateTime
(
    double*   valuePtr      /**< Pointer to the buffer into which the time stamp will be copied. */
);

#endif /* __DEVTREE_V2_H */
