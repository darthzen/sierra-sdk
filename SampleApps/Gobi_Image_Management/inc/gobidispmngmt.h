/*************
 *
 * Filename:    gobidispmngmt.h
 *
 * Purpose:     Contains display funtion for Gobi Image Management SAP
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include <stdbool.h>

/****************************************************************
 * *                       DEFINES
 * ****************************************************************/
/*#define DBG */
#define SUCCESS                 0
#define ENTER_KEY               0x0A
#define OPTION_LEN              4
#define MAX_FIELD_SIZE          20
#define DEV_NODE_SZ             256
#define DEV_KEY_SZ              16
#define RESET_MODEM             5

/* Image type constants */
#define IMG_TYPE_MODEM                       0
#define IMG_TYPE_PRI                         1
#define UNKNOWN_EXECUTING_IMG_IDX            0xFF
#define FAIL_SAFE_IMG_IDX                    0xFF
#define IMG_BUF_SIZE                         100
#define MAX_IMAGE_PATH                       514
#define VALID_FILE                             1
#define INVALID_FILE                           0
#define MAX_IMAGES                             10
#define GPS_MAX_FIELD_SIZE                    26
#define GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET 10
#define GOBI33_BUILD_ID_VER_INITIALS_AMSS     14
#define GOBI33_BUILD_ID_VER_INITIALS_UQCN     13
#define GOBI33_CARRIER_INIT_OFFSET_FROM_END    4
#define MAX_IMAGES_LEN                        17
#define MAX_VER_STR_LEN                       15
#define VERSION_ID_OFFSET                     8
#define SL9090_BUILD_ID_CARRIER_INITIALS_OFFSET 21

/* macros*/
#define rcprint(s, u) syslog(LOG_USER, "%s: rc = 0x%X", s, u)

/****************************************************************
 * *                       DATA STRUCTURE
 * ****************************************************************/
/* Device information structure */
typedef struct device_info_param{
  CHAR deviceNode[DEV_NODE_SZ];
  CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

/* Image information structure */
struct imageInfo{
    ULONG firmwareId;
    ULONG technology;
    ULONG carrier;
    ULONG region;
    ULONG gps;
};

/* Image information in verbose mode - structure */
struct imgInfoVb{
    CHAR carrierName[MAX_FIELD_SIZE];
    CHAR technologyName[MAX_FIELD_SIZE];
    CHAR regionName[MAX_FIELD_SIZE];
    CHAR gpsName[GPS_MAX_FIELD_SIZE];
};

/* Image information table */
struct imgInfoTable{
    CHAR  *pTechnology;
    CHAR  carrierIntials;
    ULONG carrier;
    ULONG region;
    ULONG technology;
};

/* Final Image list which has to be displayed to the user */
struct ImageFinalList
{
    struct qmifwinfo_s imgInfo;
    BOOL   IsLatest;
};


/* User options enumeration */
enum eUserOptions{
    eHOST_IMAGE_INFO = 0x31,
    eDEV_IMAGE_INFO,
    eHOST_DETAIL_IMG_INFO,
    eDWL_IMG,
    eDEL_IMG,
    eACTIVATE_SEL_IMG,
    eDEV_RAW_IMG_DISP,
    eACTIVATE_RAW_SEL_IMG,
    eGET_BOOT_LOADER_VER,
    eEXIT_APP = ENTER_KEY,
};

/* Display Active Image field enumeration */
enum eDisplayActiveImageField{
    eACTIVE_IMG_NO_DIS,
    eACTIVE_IMG_DIS,
    eACTIVE_IMG = 3,
};

/* Enumeration to hold carrier Image Status */
enum carrierImgStatus
{
    FW_SAME_CARRIER_NEW,
    FW_SAME_CARRIER_OLD,
    FW_DIFFERENT_CARRIER,
    FW_NO_IMAGE_INFO
};

/* prototypes */

void DisplayImageInfo(
    BYTE               printActiveStatus,
    struct qmifwinfo_s *pImgInfo );

char *GetCarrierString (ULONG carrier);
void GetImagePath( CHAR *pImagePath );
void FlushStdinStream( );
int  GetUserImageId( BYTE userAction, CHAR *pImgType, BYTE *length );
bool GetPRIImageIdx( BYTE *pImgListIdx );
void spkgFwDisplay( struct qmifwinfo_s *pin );
void GetSpkgImagePath( CHAR *pImagePath );

