/*************
 *
 * Filename:    gobiimgmgmt.c
 *
 * Purpose:     GOBI Image Management application
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include "gobidispmngmt.h"
#include "qmerrno.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <syslog.h>

#define SL9090         "SL9090"
#define MC9090         "MC9090"

/****************************************************************
 *                    GLOBAL DATA
 ****************************************************************/
/* path to sdk binary */
static char *sdkbinpath = NULL;

/* Folder info holding the image */
extern const CHAR *pImgFolderName[MAX_IMAGES];
extern BYTE imgFolderIdx[MAX_IMAGES+1];
extern BYTE folderIdx;
extern struct ImageList devImgList;

/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];

/* firmware download */
static BOOL fwdwfail = false;
static bool fwDwlComplete = false;
static bool dwlImage      = false;
static CHAR imagePath[MAX_IMAGE_PATH];
char   *configFilePath = NULL;
struct ImageFinalList devImgFinalList[50];
struct ImageFinalList ModemImgFinalList[50];

/* ImageID String for Fail Safe AMSS image */
const char imgIddefAMSSImage[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const ULONG imageIDLen = 0x10;

/* Image information table */
struct imgInfoTable imgDetailInfo[MAX_IMAGES_LEN] =
{
    { "D3600", 'S', eGOBI_IMG_CAR_SPRINT,        eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_CDMA },
    { "D3600", 'V', eGOBI_IMG_CAR_VERIZON,       eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_CDMA },
    { "D3600", 'C', eGOBI_IMG_CAR_CHINA_TELECOM, eGOBI_IMG_REG_ASIA,   eGOBI_IMG_TECH_CDMA },
    { "D3600", 'G', eGOBI_IMG_CAR_GENERIC_CDMA,  eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_CDMA }, /* item for Generic CDMA UQCN */
    { "D3600", 'H', eGOBI_IMG_CAR_GENERIC_CDMA,  eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_CDMA }, /* item for Generic CDMA AMSS */
    { "D3200", 'V', eGOBI_IMG_CAR_VODAFONE,      eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'A', eGOBI_IMG_CAR_ATT,           eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'L', eGOBI_IMG_CAR_TMOBILE,       eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'G', eGOBI_IMG_CAR_GENERIC,       eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_UMTS },
    { "D3200", 'H', eGOBI_IMG_CAR_TELEFONICA,    eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'I', eGOBI_IMG_CAR_TELCOM_ITALIA, eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'O', eGOBI_IMG_CAR_ORANGE,        eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'U', eGOBI_IMG_CAR_GENERIC,       eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_UMTS },
    { "D3200", 'R', eGOBI_IMG_CAR_ROGERS,        eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3600", 'A', eGOBI_IMG_CAR_AERIS,         eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_CDMA },
    { "D3200", 'C', eGOBI_IMG_CAR_BELL,          eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'T', eGOBI_IMG_CAR_TELUS,         eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
};

#define TRUE 1
#define FALSE 0
/****************************************************************
*                    COMMON FUNCTIONS
****************************************************************/
/*
 * Name:     ExtractPRIImgInfo
 *
 * Purpose:  Extract the Carrier, Technology, region and GPS information from
 *           the received build id.
 *
 * Return:   true  - If the image is successfully identified
 *           false - If the image is not identified
 *
 * Notes:    none
 */
bool ExtractPRIImgInfo(
    char               *pBuildId,
    struct qmifwinfo_s *pImgInfo,
    int                 BuildIdLen )
{
    BYTE idx = 0;

    /* Extract the information from the Image Info table */
    for( idx = 0; idx < MAX_IMAGES_LEN; idx++ )
    {
        /* If there ia a match for technology string */
        if( NULL != strstr( pBuildId, imgDetailInfo[idx].pTechnology ) )
        {
            if( pBuildId[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
                pBuildId[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0')
            {
                if ( pBuildId[BuildIdLen - GOBI33_CARRIER_INIT_OFFSET_FROM_END] ==
                      imgDetailInfo[idx].carrierIntials )
                {
                        pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                        pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                        pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                        return true;
                }
            }
            /* If there is a match for carrier names's initial */
            else if( pBuildId[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                         imgDetailInfo[idx].carrierIntials )
            {
                pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                return true;
            }
        }
    }
    return false;
}

/*
 * Name:     ExtractModemImgInfo
 *
 * Purpose:  Extract the Carrier, Technology, region and GPS information from
 *           the received build id.
 *
 * Return:   true  - If the image is successfully identified
 *           false - If the image is not identified
 *
 * Notes:    none
 */
bool ExtractModemImgInfo(
    char               *pBuildId,
    struct qmifwinfo_s *pImgInfo,
    int                 BuildIdLen )
{
    BYTE idx = 0;
    CHAR ModelId[128] = {0};
    BYTE stringsize = sizeof(ModelId);

    if (eQCWWAN_ERR_NONE != GetModelID( stringsize,ModelId ))
    {
        fprintf(stderr, "[%s][line:%d] Error: cannot get model id!\n", __func__, __LINE__);
        return false;
    }

    /* Extract the information from the Image Info table */
    for( idx = 0; idx < MAX_IMAGES_LEN; idx++ )
    {
        /* If there ia a match for technology string */
        if( NULL != strstr( pBuildId, imgDetailInfo[idx].pTechnology ) )
        {
            if ( (strstr (ModelId, SL9090) || strstr (ModelId, MC9090)) && 
                 (pBuildId[SL9090_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                  imgDetailInfo[idx].carrierIntials))
            {
                pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                return true;
            }
            else
            {
                if( pBuildId[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
                    pBuildId[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0')
                {
                    if ( pBuildId[BuildIdLen - GOBI33_CARRIER_INIT_OFFSET_FROM_END] ==
                      imgDetailInfo[idx].carrierIntials )
                    {
                        pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                        pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                        pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                        return true;
                     }
    
                }
                /* If there is a match for carrier names's initial */
                else if( pBuildId[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                         imgDetailInfo[idx].carrierIntials )
                {
                    pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                    pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                    pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 * Name:     IsFwNew
 *
 * Purpose:  Return the status of the PRI Image based on the rest of the images on the
 *           host
 *
 * param     [out] pPRIImgBuildID
 *            The build ID of the PRI image
 *
 * param     [out] pOldPRIImgBuildID
 *           The build ID used to compare whether the current Image is new
 *
 * return
 *      FW_DIFFERENT_CARRIER if the carrier image is not present
 *      FW_SAME_CARRIER_OLD if no new carrier of the same image is present
 *      FW_SAME_CARRIER_NEW if new carrier of the same image is present
 *
 * note
 *     none
 */
enum carrierImgStatus IsFwNew( CHAR *pPRIImgBuildID,
                               CHAR *pOldPRIImgBuildID,
                               ULONG PRIBuildIdLen,
                               ULONG OldPRIBuildLen )
{
    CHAR PRIImgStr[100], OldPRIImgStr[100];
    CHAR *pPRIBuildIDVer, *pOldPRIBuildIDVer;
    struct qmifwinfo_s ImgInfo, OldImgInfo;
    CHAR BuildIdVerNew[MAX_VER_STR_LEN] = {0};
    CHAR BuildIdVerOld[MAX_VER_STR_LEN] = {0};
    BYTE count = 0, idx = 0, DigitsInOldVer = 0 ,DigitsInNewVer = 0;

    CHAR DigitsAfterString[3] = {'\0','\0','\0'};

    strcpy( PRIImgStr, pPRIImgBuildID );
    strcpy( OldPRIImgStr, pOldPRIImgBuildID );

    /* Extract the image informations */
    if ( !ExtractPRIImgInfo(pPRIImgBuildID, &ImgInfo, PRIBuildIdLen ) ||
         !ExtractPRIImgInfo(pOldPRIImgBuildID, &OldImgInfo, OldPRIBuildLen ))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Make sure that the carrier, technology and region are the same */
    if ((ImgInfo.dev.g.Carrier    != OldImgInfo.dev.g.Carrier) ||
        (ImgInfo.dev.g.Region     != OldImgInfo.dev.g.Region) ||
        (ImgInfo.dev.g.Technology != OldImgInfo.dev.g.Technology))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Get the PRI Image version */
    pPRIBuildIDVer = strtok ( PRIImgStr, "-" );
    pPRIBuildIDVer = strtok ( NULL, "-" );
    pPRIBuildIDVer = strtok ( NULL, "-" );

    /* Get the old PRI Image version */
    pOldPRIBuildIDVer = strtok ( OldPRIImgStr, "-" );
    pOldPRIBuildIDVer = strtok ( NULL, "-" );
    pOldPRIBuildIDVer = strtok ( NULL, "-" );

    if ( (pPRIBuildIDVer == NULL) || (pOldPRIBuildIDVer == NULL))
    {
        return FW_NO_IMAGE_INFO;
    }

    /* Check if AMSS iamge is Gobi3.3 or higher
     * (to allow Gobi5.5 down the line)
     */
    if( pPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
        pPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0' )
    {
        if( pOldPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
            pOldPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0' )
        {
            int diff = 0;
            /* Read and store digits in NULL terminated Version string */
            for ( idx = 0; pOldPRIBuildIDVer[idx] != '\0'; idx++ )
            {
                if ( pOldPRIBuildIDVer[idx] >= '0' && pOldPRIBuildIDVer[idx] <= '9' )
                {
                    BuildIdVerOld[DigitsInOldVer] = pOldPRIBuildIDVer[idx];
                    DigitsInOldVer++;
                }
            }

            for ( idx = 0; pPRIBuildIDVer[idx] != '\0'; idx++ )
            {
                if ( pPRIBuildIDVer[idx] >= '0' && pPRIBuildIDVer[idx] <= '9')
                {
                    BuildIdVerNew[DigitsInNewVer] = pPRIBuildIDVer[idx];
                    DigitsInNewVer++;
                }
            }

            /* If digits in version string are not equal do Normalization */
            diff = DigitsInOldVer -DigitsInNewVer;
            if ( diff > 0 )
            {
                /* Save digits "zz" after YY string i.e. carrier string */
                DigitsAfterString[0] = BuildIdVerNew[DigitsInNewVer-2];
                DigitsAfterString[1] = BuildIdVerNew[DigitsInNewVer-1];

                for( count = 0; diff > count; count++)
                {
                    BuildIdVerNew[DigitsInNewVer -2 + count]= '0';
                }
                /* Terminate buffer with NULL */
                BuildIdVerNew[DigitsInNewVer-2 + count ]= '\0';
                /* Append digits "zz" in buffer after Normalization */
                strcat(BuildIdVerNew,DigitsAfterString);
            }

            if( diff < 0 )
            {
                /* To make diff a positive number */
                diff *= -1;
                DigitsAfterString[0] = BuildIdVerOld[DigitsInOldVer-2];
                DigitsAfterString[1] = BuildIdVerOld[DigitsInOldVer-1];
                for( count = 0; diff > count; count++)
                {
                    BuildIdVerOld[DigitsInOldVer -2 + count]= '0';
                }
                BuildIdVerOld[DigitsInOldVer-2 + count ]= '\0';
                strcat(BuildIdVerOld,DigitsAfterString);
            }

            /*
             * Check to see which of the FW is newer:
             * Only of AMSS Images are Gobi3.3 or higher
             */
            if (atol(BuildIdVerNew) > atol(BuildIdVerOld))
            {
                return FW_SAME_CARRIER_NEW;
            }
            else
            {
                return FW_SAME_CARRIER_OLD;
            }
        }
        else
        {
             /* Check to see which of the FW is newer */
             return FW_SAME_CARRIER_NEW;
        }
    }

    /* Check to see which of the FW is newer: Only if AMSS Images Gobi1.7 */
    if (atoi(pPRIBuildIDVer) > atoi(pOldPRIBuildIDVer))
    {
        return FW_SAME_CARRIER_NEW;
    }
    return FW_SAME_CARRIER_OLD;
}

/*
 * Name:     IsFwModemImageNew
 *
 * Purpose:  Return the status of the Modem Image based on the rest of the images on the
 *           host
 *
 * param     [out] pModemImgBuildID
 *           The build ID of the Modem image
 *
 * param     [out] pOldModemImgBuildID
 *           The build ID used to compare whether the current Image is new
 *
 * return
 *      FW_DIFFERENT_CARRIER if the carrier image is not present
 *      FW_SAME_CARRIER_OLD if no new carrier of the same image is present
 *      FW_SAME_CARRIER_NEW if new carrier of the same image is present
 *
 * note
 *     none
 */
enum carrierImgStatus IsFwModemImageNew( CHAR *pModemImgBuildID,
                                         CHAR *pOldModemImgBuildID,
                                         ULONG ModemBuildIdLen,
                                         ULONG OldModemBuildLen )
{
    CHAR ModemImgStr[100]={0}, OldModemImgStr[100]={0};
    CHAR *pModemBuildIDVer, *pOldModemBuildIDVer;
    CHAR newModemBuildIDVer[10]={0};
    CHAR oldModemBuildIDVer[10]={0};
    BYTE offset, newImageCount = 0, oldImageCount = 0;
    struct qmifwinfo_s ImgInfo, OldImgInfo;
    CHAR BuildIdVerNew[MAX_VER_STR_LEN]={0};
    CHAR BuildIdVerOld[MAX_VER_STR_LEN]={0};
    BYTE count = 0, idx = 0, DigitsInOldVer = 0 ,DigitsInNewVer = 0;
    CHAR DigitsAfterString[3] = {'\0','\0','\0'};
    CHAR ModelId[128] = {0};
    BYTE stringsize = sizeof(ModelId);

    strcpy( ModemImgStr, pModemImgBuildID );
    strcpy( OldModemImgStr, pOldModemImgBuildID );

    /* Extract the image informations */
    if ( !ExtractModemImgInfo(pModemImgBuildID, &ImgInfo,ModemBuildIdLen ) ||
         !ExtractModemImgInfo(pOldModemImgBuildID, &OldImgInfo, OldModemBuildLen))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Make sure that the carrier, technology and region are the same */
    if ((ImgInfo.dev.g.Carrier    != OldImgInfo.dev.g.Carrier) ||
        (ImgInfo.dev.g.Region     != OldImgInfo.dev.g.Region) ||
        (ImgInfo.dev.g.Technology != OldImgInfo.dev.g.Technology))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Get the Modem Image version */
    pModemBuildIDVer = strtok ( ModemImgStr, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );

    /* Get the old Modem Image version */
    pOldModemBuildIDVer = strtok ( OldModemImgStr, "-" );
    pOldModemBuildIDVer = strtok ( NULL, "-" );
    pOldModemBuildIDVer = strtok ( NULL, "-" );

    if ( (pModemBuildIDVer == NULL) || (pOldModemBuildIDVer == NULL))
    {
        return FW_NO_IMAGE_INFO;
    }

    if (eQCWWAN_ERR_NONE != GetModelID( stringsize,ModelId ))
    {
        fprintf(stderr, "[%s][line:%d] Error: cannot get model id!\n", __func__, __LINE__);
        return false;
    }

    if (strstr (ModelId, SL9090) || strstr (ModelId, MC9090))
    {
        for( offset = VERSION_ID_OFFSET; offset <= strlen(pModemBuildIDVer); offset++)
        {
            newModemBuildIDVer[newImageCount] = pModemBuildIDVer[offset];
            newImageCount++;
        }
        for( offset = VERSION_ID_OFFSET; offset<= strlen(pOldModemBuildIDVer); offset++)
        {
            oldModemBuildIDVer[oldImageCount] = pOldModemBuildIDVer[offset];
            oldImageCount++;
        }
        if(atol(newModemBuildIDVer) > atol(oldModemBuildIDVer))
        {
            return FW_SAME_CARRIER_NEW;
        }
        else
        {
            return FW_SAME_CARRIER_OLD;
        }
    }
    else
    {
        /* Check if AMSS iamge is Gobi3.3 or higher
         * (to allow Gobi5.5 down the line)
         */
         if( pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
             pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0' )
         {
             if( pOldModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
                 pOldModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0' )
             {
                 int diff = 0;
                 /* Read and store digits in NULL terminated Version string */
                 for ( idx = 0; pOldModemBuildIDVer[idx] != '\0'; idx++ )
                 {
                    if ( pOldModemBuildIDVer[idx] >= '0' && pOldModemBuildIDVer[idx] <= '9' )
                    {
                        BuildIdVerOld[DigitsInOldVer] = pOldModemBuildIDVer[idx];
                        DigitsInOldVer++;
                    }
                 }

                 for ( idx = 0; pModemBuildIDVer[idx] != '\0'; idx++ )
                 {
                     if ( pModemBuildIDVer[idx] >= '0' && pModemBuildIDVer[idx] <= '9')
                     {
                         BuildIdVerNew[DigitsInNewVer] = pModemBuildIDVer[idx];
                         DigitsInNewVer++;
                     }
                 }

                 /* If digits in version string are not equal do Normalization */
                 diff = DigitsInOldVer -DigitsInNewVer;
                 if ( diff > 0 )
                 {
                     /* Save digits "zz" after YY string i.e. carrier string */
                     DigitsAfterString[0] = BuildIdVerNew[DigitsInNewVer-2];
                     DigitsAfterString[1] = BuildIdVerNew[DigitsInNewVer-1];

                     for( count = 0; diff > count; count++)
                     {
                         BuildIdVerNew[DigitsInNewVer -2 + count]= '0';
                     }
                     /* Terminate buffer with NULL */
                     BuildIdVerNew[DigitsInNewVer-2 + count ]= '\0';
                     /* Append digits "zz" in buffer after Normalization */
                     strcat(BuildIdVerNew,DigitsAfterString);
                 }

                 if( diff < 0 )
                 {
                     /* To make diff a positive number */
                     diff *= -1;
                     DigitsAfterString[0] = BuildIdVerOld[DigitsInOldVer-2];
                     DigitsAfterString[1] = BuildIdVerOld[DigitsInOldVer-1];
                     for( count = 0; diff > count; count++)
                     {
                         BuildIdVerOld[DigitsInOldVer -2 + count]= '0';
                     }
                     BuildIdVerOld[DigitsInOldVer-2 + count ]= '\0';
                     strcat(BuildIdVerOld,DigitsAfterString);
                 }

                 /*
                  * Check to see which of the FW is newer:
                  * Only of AMSS Images are Gobi3.3 or higher
                  */
                 if (atol(BuildIdVerNew) > atol(BuildIdVerOld))
                 {
                     return FW_SAME_CARRIER_NEW;
                 }
                 else
                 {
                     return FW_SAME_CARRIER_OLD;
                 }
             }
             else
             {
                  /* Check to see which of the FW is newer */
                  return FW_SAME_CARRIER_NEW;
             }
         }
         /* Check to see which of the FW is newer: Only if AMSS Images Gobi1.7 */
         if (atoi(pModemBuildIDVer) > atoi(pOldModemBuildIDVer))
         {
             return FW_SAME_CARRIER_NEW;
         }
         return FW_SAME_CARRIER_OLD;
    }
}

/* Name:    GetFinalImageList
 *
 * Purpose: Gets the Final PRI image index from the list.
 *
 * param [out] pPriImgEntry
 *    The list of PRI images on the device
 *
 * return
 *     None
 *
 * note
 *      The list which needs to be displayed to the user is based on two
 *      conditions:
 *          - The Image is the latest Image (based on build ID version )
 *          - The Image is ACTIVE if not the latest
 */
void GetFinalImageList( struct ImageIDEntries *pPriImgEntry )
{
    BYTE idx = 0, idx_old = 0;

    /* Clear the Image list structure */
    memset( (void *)&devImgFinalList, 0, sizeof(devImgFinalList) );

    /* Extract the information from each PRI image */
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++ )
    {
        struct ImageIdElement *pImgElement = &pPriImgEntry->imageIDElement[idx];

        /* Set this by default to true */
        devImgFinalList[idx].IsLatest = true;

        if ( !ExtractPRIImgInfo( pImgElement->buildID,
                                        &devImgFinalList[idx].imgInfo,
                                        pImgElement->buildIDLength ) )
        {
            fprintf( stderr, "Unknown Image : %s\n",
                              pPriImgEntry->imageIDElement[idx].buildID );
            devImgFinalList[idx].IsLatest = false;
            continue;
        }

        /*
         * Cross check with the other PRI Images and proceed to display only
         * when the FW is new or if the FW is currently active one on the host
         */
        for( idx_old = 0; idx_old < idx; idx_old++ )
        {
            struct ImageIdElement *pOldImgElement =
                         &pPriImgEntry->imageIDElement[idx_old];

            /* Check to see if we found a new FW, of the same carrier */
            switch( IsFwNew( pImgElement->buildID,
                             pOldImgElement->buildID,
                             pImgElement->buildIDLength,
                             pOldImgElement->buildIDLength) )
            {
                case FW_SAME_CARRIER_NEW:
                     /* Change the status if this is not an executing image */
                    if( idx_old != pPriImgEntry->executingImage ) {
                        devImgFinalList[idx_old].IsLatest = false;
                    }
                    break;
                case FW_SAME_CARRIER_OLD:
                    /* Change the status if this is not an executing image */
                    if( idx != pPriImgEntry->executingImage ) {
                        devImgFinalList[idx].IsLatest = false;
                    }
                    break;
                case FW_DIFFERENT_CARRIER:
                default:
                    break;
            }

            /*
             * Break from the loop if the current Image is neither the latest
             * nor the active one on the host
             */
            if( !devImgFinalList[idx].IsLatest )
                break;
        }
    }
}

/*
 * Name:    GetFinalModemImageList
 *
 * Purpose: Gets the Final PRI image index from the list.
 *
 * param    [out] pPriImgEntry
 *          The list of PRI images on the device
 *
 * return
 *          None
 *
 * note
 *      The list which needs to be displayed to the user is based on two
 *      conditions:
 *          - The Image is the lastest Image (based on build ID version )
 *          - The Image is ACTIVE if not the latest
 */
void GetFinalModemImageList( struct ImageIDEntries *pModemImgEntry )
{
    BYTE idx = 0, idx_old = 0;

    /* Clear the Image list structure */
    memset( (void *)&ModemImgFinalList, 0, sizeof(ModemImgFinalList) );

    /* Extract the information from each PRI image */
    for( idx = 0; idx < pModemImgEntry->imageIDSize; idx++ )
    {
        struct ImageIdElement *pImgElement = &pModemImgEntry->imageIDElement[idx];

        /* Set this by default to true */
        ModemImgFinalList[idx].IsLatest = true;

        if ( !ExtractModemImgInfo( pImgElement->buildID,
                                   &ModemImgFinalList[idx].imgInfo,
                                   pImgElement->buildIDLength) )
        {
            /* Do not print unknown Image for fail safe image of SL9090 and Gobi3k v3.3*/
            if( 'X' != pImgElement->buildID[SL9090_BUILD_ID_CARRIER_INITIALS_OFFSET] &&
                'X' != pImgElement->buildID[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] )
            {
                fprintf( stderr, "Unknown Image : %s\n", pModemImgEntry->imageIDElement[idx].buildID );
            }
            ModemImgFinalList[idx].IsLatest = false;
            continue;
        }

        /* Check if the image ID is zero*/
        if( strncmp( (const char *)pImgElement->imageID,
                     imgIddefAMSSImage,
                     imageIDLen ) == 0 )
        {
            /* Avoid setting the fail safe AMSS image as latest image */
            ModemImgFinalList[idx].IsLatest = false;
            continue;
        }

        /*
         * Cross check with the other PRI Images and proceed to display only
         * when the FW is new or if the FW is currently active one on the host
         */
        for( idx_old = 0; idx_old < idx; idx_old++ )
        {
            struct ImageIdElement *pOldImgElement =
                         &pModemImgEntry->imageIDElement[idx_old];
            /* Check if the image ID is zero*/
            if( strncmp( (const char *)pOldImgElement->imageID,
                        imgIddefAMSSImage,
                        imageIDLen ) == 0 )
            {
                /* Ignore the fail safe AMSS image for cross checking for latest image */
                continue;
            }
            /* Check to see if we found a new FW, of the same carrier */
            switch( IsFwModemImageNew( pImgElement->buildID,
                                       pOldImgElement->buildID,
                                       pImgElement->buildIDLength,
                                       pOldImgElement->buildIDLength ) )
            {
                case FW_SAME_CARRIER_NEW:
                     /* Change the status if not latest image */
                        ModemImgFinalList[idx_old].IsLatest = false;
                    break;
                case FW_SAME_CARRIER_OLD:
                    /* Change the status if this is not latest image */
                        ModemImgFinalList[idx].IsLatest = false;
                    break;
                case FW_DIFFERENT_CARRIER:
                default:
                    break;
            }

            /*
             * Break from the loop if the current Image is neither the latest
             * nor the active one on the host
             */
            if( !ModemImgFinalList[idx].IsLatest )
                break;
        }
    }
}

/*
 * Name:    Scanfile
 *
 * Purpose: Scans carrierImageOverride file line by line
 *
 * param   [in] filename
 *          File Path
 *
 * param   [in] Buffer
 *          Buffer to store carrierImageOverride file content.
 *
 * return
 *         The carrier substring as present in the carrierImageOverride file.
 *
 * note
 *         None
 */
bool Scanfile( char *filename, char *buffer )
{
    FILE *file = fopen ( filename, "r" );
    if ( file != NULL )
    {
        /* Assume Maximum line length is not more than 100 characters */
        char line [ 100 ];

        /* Read a line */
        while ( fgets ( line, sizeof line, file ) != NULL )
        {
            char *pch;
            /* Discard the content if read line is comment */
            pch = strstr(line,"#");
            if( !pch )
            {
                strcat(buffer, line);
            }
        }
        /* Close file */
        fclose ( file );
        return true;
    }
    else
    {
        return false;
    }
}

/*
 * Name:    IsAptModemImagePresent
 *
 * Purpose: Check whether the modem image has a version according to the carrierImageOverride file
 *
 * param    [in] carrier
 *          The carrier of the Modem image
 *
 * param    [in] version
 *          The version of the Modem image
 *
 * return
 *          true if function succeed
 *          false if function failed
 *
 * note
 *          None
 */
bool IsAptModemImagePresent( BYTE modemImdIdx, ULONG carrier, ULONG version )
{
    char buffer[1000];
    bool nRet = false,fileScan = false;
    CHAR ModelId[128] = {0};
    BYTE stringsize = sizeof(ModelId);

    /* Initialize path for carrierImageOverride.txt file
     * to point at directory where we fire build for application */

    if (eQCWWAN_ERR_NONE != GetModelID( stringsize,ModelId ))
    {
        fprintf(stderr, "[%s][line:%d] Error: cannot get model id!\n", __func__, __LINE__);
        return false;
    }

    if (strstr (ModelId, SL9090) || strstr (ModelId, MC9090))
    {
        configFilePath = "./carrierImageOverrideSierraGobi.txt.";
    }
    else
    {
        configFilePath = "./carrierImageOverride.txt";
    }

    /* Scan file line by line */
    fileScan = Scanfile( configFilePath, buffer );

    /*
     * If file scan fails, Default to normal image switching.
     * This could be a case when file is not present in make directory
     */
    if( !fileScan )
    {
        /* If modem image at present index is latest? */
        if ( !ModemImgFinalList[modemImdIdx].IsLatest)
        {
            /* Check for other images present in modem */
            return false;
        }
        else
        {
            /* If modem Image latest activate this */
            return true;
        }

    }

    /* If the file is present, check for the configure file for compatibility */
    if ( fileScan )
    {
        char *pVersion, *pCarrier;
        pCarrier = GetCarrierString(carrier);

        /* If the carrier information is present in the file, extract version */
        if( pCarrier )
        {
            pVersion = strstr(buffer, pCarrier);

            if( pVersion )
            {
                strtok(pVersion, "=");
                pVersion = strtok(NULL, "\n");
                nRet = true;
            }
            else
            {
                /* If modem image at present index is latest? */
                if ( !ModemImgFinalList[modemImdIdx].IsLatest)
                {
                    /* Check for other images present in modem */
                    return false;
                }
                else
                {
                    /* If modem Image latest activate this */
                    return true;
                }
            }
        }

        /*
         * Once the version is extracted, compare with the version of the
         * image on the device
         */
        if ( nRet )
        {
            if ( pVersion != NULL)
            {
                if ( (ULONG)atol(pVersion) == version )
                {
                    nRet = true;
                }
                else
                {
                    nRet = false;
                }
            }
        }
    /* If the file is not present select the first image found, hence true */
    }
    else
    {
        nRet = true;
    }
    return nRet;
}

/* Name:    CheckModemImage
 *
 * Purpose: Check whether the corresponding (with respect to the PRI image in the
 *          mentioned imageIndex) modem image is present
 *
 * param    [in] imageIndex
 *          The image index of the PRI image$
 *
 * param    [in] modemImdIdx,
 *          The modem image index of the Modem image
 *
 * param    [in] pModemImgBuildID
 *          The build ID of the Modem image
 *
 * return
 *          true if function succeed
 *          false if function failed
 *
 * note
 *          None
 */
bool CheckModemImage( BYTE imageIndex, BYTE modemImdIdx, CHAR *pModemImgBuildID, ULONG BuildIDlen )
{
    struct qmifwinfo_s ModemImgInfo, *pPRIImgInfo;
    CHAR *pModemBuildIDVer;
    CHAR ModemImgStr[100];

    strcpy( ModemImgStr, pModemImgBuildID );
    pPRIImgInfo = &devImgFinalList[imageIndex].imgInfo;
    ExtractModemImgInfo( pModemImgBuildID, &ModemImgInfo, BuildIDlen );

    /* Get the modem build ID Image version */
    pModemBuildIDVer = strtok ( ModemImgStr, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );

    if ( pModemBuildIDVer == NULL)
    {
        return false;
    }

    /*
     * If the carrier, region and technology are the same; check whether modem
     * image has a similar or higher version present in the configuration file
     */
    if (((ModemImgInfo.dev.g.Technology == eGOBI_IMG_TECH_UMTS) &&
         (ModemImgInfo.dev.g.Carrier    == eGOBI_IMG_CAR_GENERIC) &&
         (ModemImgInfo.dev.g.Technology == pPRIImgInfo->dev.g.Technology)) ||
        ((ModemImgInfo.dev.g.Technology == eGOBI_IMG_TECH_CDMA) &&
         (ModemImgInfo.dev.g.Carrier    == pPRIImgInfo->dev.g.Carrier) &&
         (ModemImgInfo.dev.g.Region     == pPRIImgInfo->dev.g.Region) &&
         (ModemImgInfo.dev.g.Technology == pPRIImgInfo->dev.g.Technology)) ||
         (ModemImgInfo.dev.g.Carrier == eGOBI_IMG_CAR_GENERIC_CDMA)) /* if the modem image is generic CDMA, the PRI can be any */
    {
        CHAR ModelId[128] = {0};
        BYTE stringsize = sizeof(ModelId);
        
        if (eQCWWAN_ERR_NONE != GetModelID( stringsize,ModelId ))
        {
            fprintf(stderr, "[%s][line:%d] Error: cannot get model id!\n", __func__, __LINE__);
            return false;
        }

        if (strstr (ModelId, SL9090) || strstr (ModelId, MC9090))
        {
            CHAR *pNewModemBuildIDVer = NULL;
            pNewModemBuildIDVer = pModemBuildIDVer + VERSION_ID_OFFSET;
            return IsAptModemImagePresent ( modemImdIdx,
                     pPRIImgInfo->dev.g.Carrier, atol(pNewModemBuildIDVer));
        }
        else
        {
            if( pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
                pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS +1 ] > '0')
            {
                /* If modem image at present index is latest? */
                if ( !ModemImgFinalList[modemImdIdx].IsLatest)
                {
                    /* Check for other images present in modem */
                    return false;
                }
                else
                {
                    /* If modem Image latest activate this */
                    return true;
                }
             }
             else
             {
                 return IsAptModemImagePresent ( modemImdIdx,
                                                 pPRIImgInfo->dev.g.Carrier,
                                                 atoi(pModemBuildIDVer));
             }
       }
    }
    return false;
}

/* Name:    PrintDeviceModel
 *
 * Purpose: Prints the model of the device connected.
 *
 * param    None
 *
 * return
 *          None
 *
 * note
 *          None
 */
void PrintDeviceModel( void )
{
    BYTE stringsize = 128;
    CHAR ModelId[stringsize];

    GetModelID( stringsize,
                ModelId );
    fprintf( stderr, "Device Connected: %s\n", ModelId );
}

/*************************************************************************
 * Option 1 - Display information about host images.
 ************************************************************************/
/*
 * Name:     GetImgInformation
 *
 * Purpose:  Retrieve the Image information located at the specified path.
 *
 * Return:   None
 *
 * Notes:    none
 */
static ULONG GetImgInformation(
    const CHAR          *pImagePath,
    const CHAR          *pImgFolderName,
    struct qmifwinfo_s  *pImgInfo,
    bool                *cwe_format)
{
    CHAR  completeImagePath[MAX_IMAGE_PATH];

    /* Intialize the image path buffer */
    memset( completeImagePath, 0, MAX_IMAGE_PATH );
    strcpy( completeImagePath, pImagePath);

    /* special handling for SL9090/MC9090, if the boot loader version is equal or greater
       than SWI6600X_02.00.02.02, a SPKG file will be provided for download,
       otherwise, it uses amss.mbn+uqcn.mbn (G3K) firmware download method */
    if (SLQSIsSpkgFormatRequired())
    {
        *cwe_format = TRUE;
        return SLQSGetImageInfoMC77xx( completeImagePath, pImgInfo );
    }
    else
    {
        /* Copy the path, add the folder name index and add '/' end of the path*/
        strcat( completeImagePath, pImgFolderName );
        strcat( completeImagePath, "/" );
        *cwe_format = FALSE;
        return SLQSGetImageInfoMC83xx( completeImagePath, pImgInfo );
    }

    /* Copy the path, add the folder name index and add '/' end of the path*/
    strcat( completeImagePath, pImgFolderName );
    strcat( completeImagePath, "/" );

    /* Get the Image information */
    return SLQSGetImageInfo( completeImagePath,
                             pImgInfo);
}

/*************************************************************************
 * Option 2 - Display information about device images.
 ************************************************************************/
/*
 * Name:     ListDeviceImages
 *
 * Purpose:  Display the list of carrier images loaded in the device.
 *
 * Return:   true  - If the image listing is successful
 *           false - If the image listing fails.
 *
 * Notes:    none
 */
static bool ListDeviceImages( BOOL MDMImgMatch )
{
    struct ImageIDEntries *pPriImgEntry, *pModemImgEntry = NULL;
    ULONG            resultCode, size = 0;
    BYTE             imgListIdx = 0;
    BYTE             activeImageFound = eACTIVE_IMG_DIS, idx = 0;
    struct ImageIDEntries *pActModemImg = NULL;
    BYTE modemImdIdx = 0;
    int modemImageFound = false;;

    size = sizeof( devImgList );

    /* Clear the Image list structure */
    memset( (void *)&devImgList, 0, size );

    /* Retrieve the information about the images loaded in device */
    resultCode = GetStoredImages( &size,
                                  (BYTE *)&devImgList );

    /* Return if we receive an error */
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "ListDeviceImages failed\n"\
                         "Failure code : %u\n", resultCode );
        return false;
    }

    /* Figure out the index of PRI image type in the received image list */
    if( !GetPRIImageIdx( &imgListIdx ) )
    {
        fprintf(stderr,"FAIL SAFE Image Active!!\n\n" );
        return false;
    }

    /* Store the pointer to PRI and MODEM image list */
    pPriImgEntry   = &( devImgList.imageIDEntries[imgListIdx] );
    pModemImgEntry = &( devImgList.imageIDEntries[ ( !imgListIdx) ] );

    /* Get final image list of UQCN(PRI) Images */
    GetFinalImageList( pPriImgEntry );

    /* Get Final list of AMSS(MODEM) Images */
    GetFinalModemImageList( pModemImgEntry );

    int w10=-10, w20=-20;
    fprintf( stderr,
             "%3$*1$s%4$*2$s%5$*2$s%6$*2$s%7$*2$s\n",
             w10, w20, "Id", "Carrier", "Technology", "Region", "Status" );

    /* Extract the information from each PRI image */
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++ )
    {
        /* Consider image as inactive by default */
        activeImageFound = eACTIVE_IMG_DIS;

        /*  If fail safe image is active, do not consider PRI image */
        if( FAIL_SAFE_IMG_IDX != pModemImgEntry->executingImage )
        {
             /* If the current image is executing image */
            if( idx == pPriImgEntry->executingImage )
            {
                /* Set the value to display the image as active */
                activeImageFound = eACTIVE_IMG;
            }
            else
            {
                activeImageFound = eACTIVE_IMG_DIS;
            }
        }


        if ( devImgFinalList[idx].IsLatest )
        {
            /* Check if Modem Image match is requested before displaying */
            if( MDMImgMatch == TRUE )
            {
                /* Store the pointer to Modem image list */
                pActModemImg = &( devImgList.imageIDEntries[!imgListIdx] );

                /* Check if the corresponding MODEM image exist on device */
                for( modemImdIdx = 0;
                     modemImdIdx < pActModemImg->imageIDSize;
                     modemImdIdx++ )
                {
                     CHAR *pbuildID = pActModemImg->imageIDElement[modemImdIdx].buildID;

                     /* If the corresponding MODEM image is found */
                     if ( CheckModemImage( idx, modemImdIdx, pbuildID,
                                           pActModemImg->imageIDElement[modemImdIdx].buildIDLength ))
                     {
                         modemImageFound = true;
                         break;
                     }
                }

                /* If corresponding MODEM image does not exist on the device, return */
                if( !modemImageFound )
                {
                    continue;
                }

                /* Reset the flag to false */
                modemImageFound = false;
            }

            fprintf( stderr, "%2$*1$d", w10, idx + 1);
            /* Display the image information */
            DisplayImageInfo( activeImageFound, &devImgFinalList[idx].imgInfo );
        }
    }

    /* If no image is active, then fail safe image will be executing */
    if( ( UNKNOWN_EXECUTING_IMG_IDX == pPriImgEntry->executingImage ) ||
        ( FAIL_SAFE_IMG_IDX == pModemImgEntry->executingImage ) )
    {
        fprintf( stderr, "FAIL SAFE Image Active!!\n\n" );
    }
    return true;
}

/*
 * Name:     ListHostImages
 *
 * Purpose:  Display the list of carrier images available for download at the
 *           path specified by user.
 *
 * Return:   true      - If the image listing is success
 *           ENTER_KEY - If <ENTER> key is pressed.
 *
 * Notes:    none
 */
static bool ListHostImages()
{
    CHAR completeImagePath[MAX_IMAGE_PATH];
    while(1)
    {
        /* query user for path to directory containing carrier image folders */
        memset( (void *)imagePath, 0, MAX_IMAGE_PATH );
        GetImagePath( imagePath );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == imagePath[0] )
        {
            return false;
        }

        /* Display the headings */
        int w10=-10, w20=-20;
        fprintf( stderr,
                 "%3$*1$s%4$*2$s%5$*2$s%6$*1$s%7$*1$s%8$*2$s\n",
                 w10, w20, "Id", "Carrier", "Technology", "Region", "Version", "GPS Support" );

        memset( completeImagePath, 0, MAX_IMAGE_PATH );
        strcpy( completeImagePath, imagePath);
        int offset = strlen(completeImagePath);
        BYTE idx;
        if((MAX_IMAGE_PATH-1)<=offset)
        {
            fprintf(stderr,"Image Path too long\n");
            return false;
        }
        for ( idx = 0 ; idx < folderIdx; idx++ )
        {
            /* Copy the path and file name into the image path buffer */
            sprintf( &completeImagePath[offset], "%c",imgFolderIdx[idx] );

            /* get the image info */
            struct qmifwinfo_s imgInfo;
            ULONG rc = SLQSGetImageInfoMC83xx( completeImagePath, &imgInfo );
            if ( SUCCESS == rc )
            {
                /* Display image information in verbose mode */
                fprintf( stderr, "%2$*1$c", w10, imgFolderIdx[idx] );
                DisplayImageInfo( eACTIVE_IMG_NO_DIS, &imgInfo );
            }
        }
        folderIdx = 0;

        /* if image listing is displayed for downloading the image, exit */
        if( dwlImage )
        {
            dwlImage = false;
            return true;
        }
    }
}

/*
 * Name:     DisplayHostImageSpkg
 *
 * Purpose:  Display the host image with spkg format
 *
 * Return:   true      - If the image listing is success
 *           ENTER_KEY - If <ENTER> key is pressed.
 *
 * Notes:    none
 */
static bool DisplayHostImageSpkg()
{
    struct qmifwinfo_s imgInfo;
    ULONG resultCode;

    while(1)
    {
        /* query user for path to directory containing carrier image folders */
        memset( (void *)imagePath, 0, MAX_IMAGE_PATH );
        GetSpkgImagePath(imagePath);
        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == imagePath[0] )
        {
            return false;
        }
        resultCode = SLQSGetImageInfoMC77xx(imagePath, &imgInfo);
        if (resultCode == SUCCESS)
        {
            spkgFwDisplay(&imgInfo);
        }
        else
        {
            fprintf( stderr,  "SLQSGetImageInfoMC77xx error !\n" );
        }

        /* if image listing is displayed for downloading the image, exit */
        if( dwlImage )
        {
            dwlImage = false;
            return true;
        }
    }
}

/****************************************************************
*                       FUNCTIONS
****************************************************************/
/*
 * Name:     FirmwareDwlCbk
 *
 * Purpose:  Firmware download completion callback
 *
 * Return:   None
 *
 * Notes:    none
 */
void FirmwareDwlCbk(ULONG status)
{
    switch (status)
    {
        case eQCWWAN_ERR_NONE:
        {
            fprintf( stderr, "\nFirmware download complete; waiting for device...\n" );
            /* set firmware complete to true */
            fwDwlComplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED:
        {
            fprintf( stderr, "\nFirmware Not Downloaded\n" );
            fwdwfail = FALSE; // This is not an error as the modem did not need the FW to be downloaded
            /* set firmware complete to true */
            fwDwlComplete = TRUE;
            break;
        }
        case eQCWWAN_ERR_SWIIM_FW_ENTER_DOWNLOAD_MODE:
        {
            fprintf( stderr, "\nEnter Download Mode\n" );
            return;
        }
        case eQCWWAN_ERR_SWIIM_FW_FLASH_COMPLETE:
        {
            fprintf( stderr, "\nFlash Complete, Waiting for Modem to Reboot\n" );
            return;
        }
        default:
        {
            fprintf( stderr, "\nERROR: Firmware Download failed. Callback status %u\n", status);
            fwdwfail = TRUE;
            /* set firmware complete to true */
            fwDwlComplete = TRUE;
            break;
        }
    }

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);
}

/*************************************************************************
 * Option 7 - List Device Images Build ID
 ************************************************************************/
/*
 * Name:     ListRawDeviceImages
 *
 * Purpose:  Display the list of carrier images loaded in the device.
 *
 * Return:   true  - If the image listing is successful
 *           false - If the image listing fails.
 *
 * Notes:    none
 */
static bool ListRawDeviceImages()
{
    struct ImageIDEntries *pPriImgEntry, *pModemImgEntry = NULL;
    ULONG            resultCode, size = 0;
    BYTE             imgListIdx = 0;
    BYTE             idx = 0;

    size = sizeof( devImgList );

    /* Clear the Image list structure */
    memset( (void *)&devImgList, 0, size );

    /* Retrieve the information about the images loaded in device */
    resultCode = GetStoredImages( &size,
                                  (BYTE *)&devImgList );

    /* Return if we receive an error */
    if( SUCCESS != resultCode )
    {
        fprintf( stderr, "ListRawDeviceImages failed\n"\
                         "Failure code : %u\n", resultCode );
        return false;
    }

    /* Figure out the index of PRI image type in the received image list */
    if( !GetPRIImageIdx( &imgListIdx ) )
    {
        fprintf(stderr,"FAIL SAFE Image Active!!\n\n" );
        return false;
    }

    /* Store the pointer to PRI and MODEM image list */
    pPriImgEntry   = &( devImgList.imageIDEntries[imgListIdx] );
    pModemImgEntry = &( devImgList.imageIDEntries[ ( !imgListIdx) ] );

    fprintf( stderr,"UQCN Images:");
    fprintf(stderr,"\tExecuting Image ID:%d\n",pPriImgEntry->executingImage + 1);
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++)
    {
        fprintf( stderr, "%d\t\t%s\n",idx+1, pPriImgEntry->imageIDElement[idx].buildID);
    }

    fprintf( stderr,"\n");
    fprintf( stderr,"AMSS Images:");

    /* If AMSS Images is fail safe i.e. index "0" skip printing */
    if ( 0 != pModemImgEntry->executingImage )
    {
        fprintf(stderr,"\tExecuting Image ID:%d\n",pModemImgEntry->executingImage );
    }

    for( idx = 1; idx < pModemImgEntry->imageIDSize; idx++)
    {
        fprintf( stderr, "%d\t\t%s\n",
                 pModemImgEntry->imageIDElement[idx].storageIndex,
                 pModemImgEntry->imageIDElement[idx].buildID );
    }

    /* If no image is active, then fail safe image will be executing */
    if( ( UNKNOWN_EXECUTING_IMG_IDX == pPriImgEntry->executingImage ) ||
        ( FAIL_SAFE_IMG_IDX == pModemImgEntry->executingImage ) )
    {
        fprintf( stderr, "FAIL SAFE Image Active!!\n\n" );
    }
    return true;
}

/*************************************************************************
 * Option 8 - Select a carrier image resident on device as the new active image.
 ************************************************************************/
/*
 * Name:     ActivateRawSelectedImage
 *
 * Purpose:  List all the images present on the device and then activate the
 *           image on the device selected by the user.
 *
 * Return:   none
 *
 * Notes:    none
 */
bool ActivateRawSelectedImage()
{
    struct PrefImageList prefImageList;
    struct ImageIdElement *pActPRIImg = NULL;
    struct ImageIDEntries *pActModemImg = NULL;
    ULONG resultCode = 0, imageListSize = 0, imageTypeSize = 0;
    BYTE modemImdIdx = 0, imageIndex = 0, priImageIdx = 0, length = 0;
    BYTE imageType[IMG_BUF_SIZE];

    /* If we fail to list the device images, return */
    if( !ListRawDeviceImages() )
    {
        return false;
    }

    while(1)
    {
        /* Receive the user input */
        imageIndex = GetUserImageId( eDEL_IMG , "UQCN", &length);

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( (ENTER_KEY == imageIndex) && (length == 1) )
            return false;

        /* Decrement the image index selected by the user */
        imageIndex--;

        /* Get the PRI image index from the Image list */
        if( !GetPRIImageIdx( &priImageIdx ) )
        {
            fprintf(stderr,"FAIL SAFE Image Active!!\n\n" );
            return false;
        }

        /* Store the pointer to PRI image list */
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );

        /* Fill the PrefImageList structure with PRI image info */
        prefImageList.listSize = 1;
        prefImageList.listEntries[0].imageType = IMG_TYPE_PRI;
        memcpy( (void *)prefImageList.listEntries[0].imageId,
                (void *)pActPRIImg->imageID,
                GOBI_MBN_IMG_ID_STR_LEN );

        prefImageList.listEntries[0].buildIdLength = pActPRIImg->buildIDLength;
        if( 0 != pActPRIImg->buildIDLength )
        {
            strcpy( prefImageList.listEntries[0].buildId,
                    pActPRIImg->buildID );
        }

        /* Store the pointer to Modem image list */
        pActModemImg = &( devImgList.imageIDEntries[!priImageIdx] );
#ifdef DBG
             fprintf(stderr, "Modem image index number:%d\n ", modemImdIdx);
#endif
        modemImdIdx = GetUserImageId( eDEL_IMG, "AMSS", &length);
        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == modemImdIdx && (length == 1))
            return false;

        /* MODEM image exist, retrieve the information */
        prefImageList.listSize++;
        prefImageList.listEntries[1].imageType = IMG_TYPE_MODEM;
        memcpy( (void *)prefImageList.listEntries[1].imageId,
                (void *)pActModemImg->imageIDElement[modemImdIdx].imageID,
                GOBI_MBN_IMG_ID_STR_LEN );
        prefImageList.listEntries[1].buildIdLength = pActModemImg->imageIDElement[modemImdIdx].buildIDLength;
        if( 0 != pActModemImg->imageIDElement[modemImdIdx].buildIDLength )
        {
            strcpy( prefImageList.listEntries[1].buildId,
                    pActModemImg->imageIDElement[modemImdIdx].buildID );
        }

        imageListSize = sizeof( prefImageList );
        imageTypeSize = IMG_BUF_SIZE;
        memset( (void *)imageType, 0, imageTypeSize );

        /* Set the preference for selected image in device */
        resultCode = SetImagesPreference( imageListSize,
                                          (BYTE *)&prefImageList,
                                          0,
                                          0xFF,
                                          &imageTypeSize,
                                          imageType );

        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Failed to activate selected image\n"\
                             "Failure Code : %u\n", resultCode );
            continue;
        }

        /* Subscribe to Device State Change callback */
        resultCode = SetFwDldCompletionCbk(FirmwareDwlCbk);
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "FwDwldCompletionCallback failed.\n"\
                             "Failure Code : %u\n", resultCode );
            return false;
        }

        /* Reset the device to activate the image */
        resultCode = SetPower( RESET_MODEM );
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Failed to reset modem\n"\
                             "Failure Code : %u\n", resultCode );
            continue;
        }

        /* Keep displaying "." until selected image gets activated */
        fprintf( stderr, "Activating Selected Image. . .");
        fwDwlComplete = false;
        while( !fwDwlComplete )
        {
            fprintf( stderr, " .");
            sleep(1);
        }

        fprintf( stderr, "Retrieved Image list from the device\n");

        /* If we fail to list the device images, return */
        if( !ListRawDeviceImages() )
        {
            return false;
        }

        imageIndex = ( devImgList.imageIDEntries[priImageIdx].executingImage );
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );

        /* Check if the selected image gets activated */
        if( SUCCESS == strcmp( pActPRIImg->buildID,
                               prefImageList.listEntries[0].buildId ) )
        {
            fprintf( stderr, "\nSelected Image activated successfully\n");
        }
        else
        {
           fprintf( stderr, "\nFailed to activate selected image\n");

#ifdef DBG
           fprintf( stderr, "Selected build id = %s\n",prefImageList.listEntries[0].buildId);
           fprintf( stderr, "Activated build id = %s\n",pActPRIImg->buildID);
#endif
        }
    }
}

/*************************************************************************
 * Option 6 - Select a carrier image resident on device as the new active image.
 ************************************************************************/
/*
 * Name:     ActivateSelectedImage
 *
 * Purpose:  List all the images present on the device and then activate the
 *           image on the device selected by the user.
 *
 * Return:   none
 *
 * Notes:    none
 */
bool ActivateSelectedImage()
{
    struct PrefImageList prefImageList;
    struct ImageIdElement *pActPRIImg = NULL;
    struct ImageIDEntries *pActModemImg = NULL;
    ULONG resultCode = 0, imageListSize = 0, imageTypeSize = 0;
    int modemImageFound = false;
    BYTE modemImdIdx = 0, imageIndex = 0, priImageIdx = 0, length = 0;
    BYTE imageType[IMG_BUF_SIZE];

    /* If we fail to list the device images, return */
    if( !ListDeviceImages(TRUE) )
    {
        return false;
    }

    while(1)
    {
        /* Receive the user input */
        imageIndex = GetUserImageId( eDEL_IMG, NULL, &length );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( (ENTER_KEY == imageIndex) && (length == 1) )
            return false;

        /* Decrement the image index selected by the user */
        imageIndex--;

        /* Get the PRI image index from the Image list */
        if( !GetPRIImageIdx( &priImageIdx ) )
        {
            fprintf(stderr,"FAIL SAFE Image Active!\n" );
            return false;
        }

        /* Store the pointer to PRI image list */
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );

        /* Fill the PrefImageList structure with PRI image info */
        prefImageList.listSize = 1;
        prefImageList.listEntries[0].imageType = IMG_TYPE_PRI;
        memcpy( (void *)prefImageList.listEntries[0].imageId,
                (void *)pActPRIImg->imageID,
                GOBI_MBN_IMG_ID_STR_LEN );

        prefImageList.listEntries[0].buildIdLength = pActPRIImg->buildIDLength;
        if( 0 != pActPRIImg->buildIDLength )
        {
            strcpy( prefImageList.listEntries[0].buildId,
                    pActPRIImg->buildID );
        }

        /* Store the pointer to Modem image list */
        pActModemImg = &( devImgList.imageIDEntries[!priImageIdx] );

        /* Check if the corresponding MODEM image exist on device */
        for( modemImdIdx = 0;
             modemImdIdx < pActModemImg->imageIDSize;
             modemImdIdx++ )
        {
             CHAR *pbuildID = pActModemImg->imageIDElement[modemImdIdx].buildID;
#ifdef DBG
             fprintf(stderr, "Modem image index number:%d\n ", modemImdIdx);
#endif
             /* If the corresponding MODEM image is found */
             if ( CheckModemImage( imageIndex, modemImdIdx, pbuildID,
                                   pActModemImg->imageIDElement[modemImdIdx].buildIDLength ))
             {
                 modemImageFound = true;
                 break;
             }
        }

        /* If corresponding MODEM image does not exist on the device, return */
        if( !modemImageFound )
        {
            fprintf( stderr,"Failed to activate selected image\n"\
                            "Failure reason : Corresponding MODEM image does not exist on the device\n");
            continue;
        }

        /* Reset the flag to false */
        modemImageFound = false;

        /* MODEM image exist, retrieve the information */
        prefImageList.listSize++;
        prefImageList.listEntries[1].imageType = IMG_TYPE_MODEM;
        memcpy( (void *)prefImageList.listEntries[1].imageId,
                (void *)pActModemImg->imageIDElement[modemImdIdx].imageID,
                GOBI_MBN_IMG_ID_STR_LEN );
        prefImageList.listEntries[1].buildIdLength = pActModemImg->imageIDElement[modemImdIdx].buildIDLength;
        if( 0 != pActModemImg->imageIDElement[modemImdIdx].buildIDLength )
        {
            strcpy( prefImageList.listEntries[1].buildId,
                    pActModemImg->imageIDElement[modemImdIdx].buildID );
        }

        imageListSize = sizeof( prefImageList );
        imageTypeSize = IMG_BUF_SIZE;
        memset( (void *)imageType, 0, imageTypeSize );

        /* Set the preference for selected image in device */
        resultCode = SetImagesPreference( imageListSize,
                                          (BYTE *)&prefImageList,
                                          0,
                                          0xFF,
                                          &imageTypeSize,
                                          imageType );

        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Failed to activate selected image\n"\
                             "Failure Code : %u\n", resultCode );
            continue;
        }

        /* Subscribe to Device State Change callback */
        resultCode = SetFwDldCompletionCbk(FirmwareDwlCbk);
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "FwDwldCompletionCallback failed.\n"\
                             "Failure Code : %u\n", resultCode );
            return false;
        }

        /* Reset the device to activate the image */
        resultCode = SetPower( RESET_MODEM );
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Failed to reset modem\n"\
                             "Failure Code : %u\n", resultCode );
            continue;
        }

        /* Keep displaying "." until selected image gets activated */
        fprintf( stderr, "Activating Selected Image...");
        fwDwlComplete = false;
        while( !fwDwlComplete )
        {
            fprintf( stderr, ".");
            sleep(1);
        }

        fprintf( stderr, "\nRetrieved Image list from the device\n");

        /* If we fail to list the device images, return */
        if( !ListDeviceImages(TRUE) )
        {
            return false;
        }

        imageIndex = ( devImgList.imageIDEntries[priImageIdx].executingImage );
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );

        /* Check if the selected image gets activated */
        if( SUCCESS == strcmp( pActPRIImg->buildID,
                               prefImageList.listEntries[0].buildId ) )
        {
            fprintf( stderr, "Selected Image activated successfully\n");
        }
        else
        {
           fprintf( stderr, "Failed to activate selected image\n");

#ifdef DBG
           fprintf( stderr, "Selected build id = %s\n",prefImageList.listEntries[0].buildId);
           fprintf( stderr, "Activated build id = %s\n",pActPRIImg->buildID);
#endif
        }
    }
}

/*************************************************************************
 * Option 5 - Delete a carrier image from the device.
 ************************************************************************/
/*
 * Name:     DeleteImage
 *
 * Purpose:  List all the images present on the device and then delete the
 *           image selected by the user from the device.
 *
 * Return:   none
 *
 * Notes:    none
 */
bool DeleteImage()
{
    BYTE idx = 0, idx1 = 0, totalImgs = 0, imageIdEntries = 0;
    BYTE imgType = 0;
    int imageIndex = 0;
    ULONG nRet,imageListsize = 0;
    struct ImageElement   imageInfo;
    struct ImageIdElement *pImageIdElement = NULL;
    struct ImageList      imageList;
    char buf[3];

    imageListsize = sizeof( imageList );
    memset( (char *)&imageList, 0, imageListsize);

    /* Get the images stored on the device */
    nRet =  GetStoredImages( &imageListsize, (BYTE *)&imageList );

    /* If image retrieving failed, return */
    if ( nRet || ( 0 == imageList.listSize) )
    {
        fprintf(stderr,"IMAGE RETREIVING FAILED : %u\n",nRet );
        return false;
    }

    /* Display the retrieved Image List */
    fprintf(stderr, "Image Index\t  Image Type\t   Image ID\n");

    for( idx = 0; idx < imageList.listSize; idx++ )
    {
        imageIdEntries = imageList.imageIDEntries[idx].imageIDSize;

        for ( idx1 = 0; idx1 < imageIdEntries; idx1++ )
        {
            pImageIdElement = ( struct ImageIdElement *)
                                &imageList.imageIDEntries[idx].
                                imageIDElement[idx1];
            fprintf(stderr, "%-18d", ++totalImgs );
            fprintf(stderr, "%-17d", imageList.imageIDEntries[idx].imageType );
            fprintf(stderr, "%-30s", (CHAR*)pImageIdElement->buildID);
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");

    while(1) // For recursive menu dsplay
    {
        fprintf(stderr, "Enter 0 to EXIT or <Image Index> of Image to be deleted :  ");
        if( fgets(buf, 3, stdin) == 0)
            continue;

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == imageIndex)
            return false;

        imageIndex = atoi (buf);

        /* Validate the image index specified by user */
        if( 0 == imageIndex )
        {
             fprintf(stderr, "EXIT selected! Exiting..\n");
             return false;
        }
        else if( ( 0 > imageIndex ) || ( totalImgs < imageIndex) )
        {
            continue;
        }

        /* By default image type is MODEM */
        imgType = IMG_TYPE_MODEM;

        /* If the image index is not MODEM image index */
        if( imageIndex > imageList.imageIDEntries[imgType].imageIDSize )
        {
             /* Get the image index in PRI image list */
            imageIndex -= ( imageList.imageIDEntries[imgType].imageIDSize );
            imgType = IMG_TYPE_PRI;
        }
        imageIndex--;

        if (imageIndex >= 50)
            continue;
        if(imageIndex<0)
        {
            fprintf(stderr, "IMAGE Index FAILED :%d\n", eQCWWAN_ERR_QMI_INVALID_INDEX );
            return false;
        }
        /* Retrieve the information about the image to be deleted */
        imageInfo.imageType = imgType;
        pImageIdElement = ( struct ImageIdElement *)&imageList.\
                           imageIDEntries[imgType].imageIDElement[imageIndex];

        for ( idx = 0; idx < 16; idx++ )
        {
            imageInfo.imageId[idx] = pImageIdElement->imageID[idx];
        }

        imageInfo.buildIdLength = pImageIdElement->buildIDLength;
        strncpy( imageInfo.buildId,
                   pImageIdElement->buildID,
                   strlen( pImageIdElement->buildID ) + 1 );

        /* Delete the selected image , API call */
        nRet = DeleteStoredImage( sizeof( imageInfo ),
                                  (BYTE *)&imageInfo );

       /* If image deleting failed, return */
        if ( eQCWWAN_ERR_NONE != nRet )
        {
            fprintf(stderr, "IMAGE DELETING FAILED : %u\n", nRet );
            return false;
        }

        totalImgs = 0;
        memset( (char *)&imageList, 0, imageListsize);

        /* Get the images stored on the device */
        nRet =  GetStoredImages( &imageListsize, (BYTE *)&imageList );

        /* If image retrieving failed, return */
        if ( nRet || ( 0 == imageList.listSize) )
        {
            fprintf(stderr, "IMAGE RETREIVING FAILED : %u\n",nRet );
            return false;
        }

        /* Display the retrieved Image List */
        fprintf(stderr, "Image Index\t  Image Type\t   Image ID\n");

        for( idx = 0; idx < imageList.listSize; idx++ )
        {
            imageIdEntries = imageList.imageIDEntries[idx].imageIDSize;

            for ( idx1 = 0; idx1 < imageIdEntries; idx1++ )
            {
                pImageIdElement = ( struct ImageIdElement *)
                                    &imageList.imageIDEntries[idx].
                                    imageIDElement[idx1];
                fprintf(stderr,"%-18d", ++totalImgs );
                fprintf(stderr,"%-17d", imageList.imageIDEntries[idx].imageType );
                fprintf(stderr,"%-30s", (CHAR*)pImageIdElement->buildID);
                fprintf(stderr,"\n");
            }
        }
    }//while(1) end 
}

/*************************************************************************
 * Option 4 - Download carrier image to the device.
 ************************************************************************/
/*
 * Name:     DownloadImage
 *
 * Purpose:  List all the images present on the device, prompt the user for the
 *           image path on the host, display their information, prompt again for
 *           the index of the image to be downloaded and then download that
 *           image to the device.
 *
 * Return:   none
 *
 * Notes:    none
 */
bool DownloadImage()
{
    int              dwlImageId = 0;
    bool             returnValue = false;
    bool             cwe_format = false;
    ULONG            resultCode = 0;
    struct qmifwinfo_s imgInformation;
    BYTE             length = 0;
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    BYTE downloadMethod;

    fwDwlComplete = false;
    fwdwfail = FALSE;
    while( 1 )
    {
        /* get firmware download method, G3K download or SPKG download? */
        fprintf( stderr, "\nPlease select firmware download method\n");
        fprintf( stderr, "1 - G3K Firmware Download Method\n");
        fprintf( stderr, "2 - SPKG Firmware Download Method\n");
        fprintf( stderr, "or press <Enter> to exit. Option :\n");

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream.
         */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

        length = strlen( selOption );
        #ifdef DBG
        fprintf( stderr, "Selected Option : %s length = %d\n", selOption, length );
        #endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( strchr(selOption, ENTER_KEY) && (length == 1) )
        {
            return ENTER_KEY;
        }

        /* Convert the option added by user into integer */
        selOption[ length - 1 ] = '\0';
        downloadMethod = atoi( selOption );

        /* application sets the firmware download method so that SDK knows the preference later */
        SLQSSetSpkgFormatRequired(downloadMethod);

       /* Prompt the user for path of the imges located on host and display the
         * information
         */
        dwlImage    = true;
        if (downloadMethod == SPKG_FIRMWARE_DOWNLOAD)
        {
            returnValue = DisplayHostImageSpkg();
        }
        else if (downloadMethod == G3K_FIRMWARE_DOWNLOAD)
        {
            returnValue = ListHostImages();
        }
        else
        {
            /* invaild input, exit */
            return false;
        }

        /* if the user has pressed <enter> key, return to the main menu */
        if( !returnValue )
        {
            dwlImage = false;
            return false;
        }

        /* Prompt the user to select an index of the image to be downloaded to the
         * device from above displayed list
         */
        if (!SLQSIsSpkgFormatRequired())
        {
            dwlImageId = GetUserImageId( eDWL_IMG, NULL, &length );

            /* If only <ENTER> is pressed by the user, return to main menu */
            if( (ENTER_KEY == dwlImageId) && (length == 1) )
            {
                return false;
            }
            else if (MAX_IMAGES<=dwlImageId)
            {
                return false;
            }
        }

        /* Get the image information about downloading image */
        resultCode = GetImgInformation( imagePath,
                                        pImgFolderName[dwlImageId],
                                        &imgInformation,
                                        &cwe_format );
        if ( SUCCESS == resultCode )
        {
#ifdef DBG
            fprintf( stderr, "Image Listing SUCCESS for %s%s\n", imagePath, pImgFolderName[dwlImageId] );
#endif
            if (!cwe_format)
            {
                /* Copy the path and file name into the image path buffer */
                strcat( imagePath, pImgFolderName[dwlImageId] );
                strcat( imagePath, "/" );
            }
        }
        else
        {
            fprintf( stderr, "Image Listing FAILED for %s%s\n", imagePath, pImgFolderName[dwlImageId] );
            fprintf( stderr, "Failure Code : %u\n", resultCode );
            return false;
        }

#ifdef DBG
        fprintf( stderr, "Path for the image to download : %s\n", imagePath );
#endif

        /* Subscribe to Firmware Download completion callback */
        resultCode = SetFwDldCompletionCbk(FirmwareDwlCbk);
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "REGISTRATION FAILED - Firmware Download Completion Callback\n"\
                             "Failure Code : %u\n", resultCode );
            return false;
        }

        /* Start downloading the firmware */
        resultCode = UpgradeFirmware2k( imagePath );
        if( SUCCESS != resultCode )
        {
            fprintf( stderr, "Firmware Download Failed\n"\
                             "Failure Code : %u\n", resultCode );

            /* De register device state change and firmware download completion
             * callback.
             */
            SetFwDldCompletionCbk( NULL );
            SetDeviceStateChangeCbk( NULL );
            return false;
        }

        /* Keep displaying "." until fimrware downloads complete */
        fprintf( stderr, "\nDownloading Firmware...");
        while( !fwDwlComplete )
        {
            fprintf( stderr, ".");
            sleep(1);
        }

        if (fwdwfail)
        {
            fprintf( stderr, "Firmware Download Failed\n");
            return false;
        }

        fprintf(stderr, "Applying updates - please wait 20 seconds...\n\n");
        sleep(20);
        memset( (void *)&imgInformation, 0, sizeof( imgInformation ) );

        fprintf( stderr, "Firmware Upgrade Success\n" );

        fprintf( stderr, "\nList Of images present on the device:\n" );

        /* If we fail to list the device images, return */
        if( !ListDeviceImages(TRUE) )
        {
            return false;
        }
        return true;
    }
}

/*************************************************************************
 * Option 3 - Display detail information about selected device image.
 ************************************************************************/
/*
 * Name:     ListDetailImageInfo
 *
 * Purpose:  List all the images present on the device and then display the
 *           build id and image id for the image selected by the user.
 *
 * Return:   none
 *
 * Notes:    none
 */
bool ListDetailImageInfo()
{
    struct qmifwinfo_s imgInfo;
    BYTE idx = 0, activeImageFound = 0;
    BYTE priImageIdx = 0, length = 0;

    /* If we fail to list the device images, return */
    if( !ListDeviceImages(TRUE) )
    {
        return false;
    }

    while(1)
    {
        int imageIndex = 0;
        struct ImageIDEntries *pPriImgEntry = NULL, *pModemImgEntry = NULL;
        /* Receive the user input */
        imageIndex = GetUserImageId( eHOST_DETAIL_IMG_INFO, NULL, &length );

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( (ENTER_KEY == imageIndex) && (length == 1) )
            return false;

        int w10=-10, w20=-20;
        fprintf( stderr,
                 "%3$*1$s%4$*2$s%5$*2$s%6$*2$s%7$*2$s\n",
                 w10, w20, "Id", "Carrier", "Technology", "Region", "Status" );

        /* Display the image index selected by the user */
        fprintf( stderr, "%2$*1$d", w10, imageIndex);
        imageIndex--;

        /* Get the PRI image index from the Image list */
        if( !GetPRIImageIdx( &priImageIdx ) )
        {
            fprintf(stderr,"tFAIL SAFE Image Active!!\n\n" );
            return false;
        }

        /* Store the pointer to PRI and MODEM image list */
        pPriImgEntry   = &( devImgList.imageIDEntries[priImageIdx] );
        pModemImgEntry = &( devImgList.imageIDEntries[ ( !priImageIdx) ] );
        if((imageIndex<0)||(imageIndex>=MAX_IMAGE_IDE_ELEMENTS))
        {
            fprintf(stderr,"Image Index Error\n" );
            return false;
        }
        /* Extract information about the image */
        if( !ExtractPRIImgInfo( pPriImgEntry->imageIDElement[imageIndex].buildID,
                                     &imgInfo,
                                     pPriImgEntry->imageIDElement[imageIndex].buildIDLength ) )
        {
#ifdef DBG
            fprintf( stderr,
                     "Unkown Image : %s\n",
                     pPriImgEntry->imageIDElement[imageIndex].buildID );
#endif
            continue;
        }

        /* Consider image as inactive by default */
        activeImageFound = eACTIVE_IMG_DIS;

        /*  If fail safe image is active, do not consider PRI image */
        if( FAIL_SAFE_IMG_IDX != pModemImgEntry->executingImage )
        {
            /* If the current image is executing image */
            if( imageIndex == pPriImgEntry->executingImage )
            {
                /* Set the value to display the image as active */
                activeImageFound = eACTIVE_IMG;
            }
            else
            {
                activeImageFound = eACTIVE_IMG_DIS;
            }
        }

        /* Display the image information */
        DisplayImageInfo( activeImageFound, &imgInfo );

        /* Dispaly the build id and image id of the selected image */
        fprintf( stderr, "Build ID : %s\n", pPriImgEntry->imageIDElement[imageIndex].buildID );
        fprintf( stderr, "Image ID : ");
        for ( idx = 0; idx < 16; idx++ )
        {
            fprintf( stderr,"%.2x ", pPriImgEntry->imageIDElement[imageIndex].imageID[idx] );
        }
        fprintf( stderr,"\n\n");
    }
}

void GetBootLoaderVersion(void)
{
    ULONG bootversion;
    ULONG resultCode;

    /* Get the information about the currently running image on device */
    resultCode = SLQSGetBootVersionNumber(&bootversion);
    if (resultCode == eQCWWAN_ERR_NONE)
    {
        fprintf(stderr, "\nBoot Loader Version : 0x%08x\n", (unsigned int) bootversion);
    }
    else
    {
        fprintf(stderr, "\nSLQSGetFirmwareInfo failed!\n");
    }

}

/*************************************************************************
 * Application starting and SDK intialization functions.
 ************************************************************************/
/*
 * Name:     StartSDK
 *
 * Purpose:  It starts the SDK by setting the SDK path, enumerates the device
 *           and connects to the SDK.
 *
 * Return:   SUCCESS on successfully starting SDK, else error code
 *
 * Notes:    none
 */
ULONG StartSDK()
{
    ULONG resultCode = 0;
    BYTE  devicesSize = 1;

    /* Set SDK image path */
    if( eQCWWAN_ERR_NONE != (resultCode = SetSDKImagePath(sdkbinpath)) )
    {
        rcprint( __func__, resultCode );
        return resultCode;
    }

    /* Establish APP<->SDK IPC */
    if( eQCWWAN_ERR_NONE != (resultCode = SLQSStart(0, NULL)) )
    {
        /* first attempt failed, kill SDK process */
        if( eQCWWAN_ERR_NONE != SLQSKillSDKProcess() )
        {
            return resultCode;
        }
        else
        {
            /* start new SDK process */
            if( eQCWWAN_ERR_NONE != (resultCode = SLQSStart(0, NULL)) )
            {
                return resultCode;
            }
        }
    }

    /* Enumerate the device */
    while (QCWWAN2kEnumerateDevices(&devicesSize, (BYTE *)pdev) != 0)
    {
        printf ("\nUnable to find device..\n");
        sleep(1);
    }

#ifdef DBG
    fprintf( stderr,  "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
            devicesSize,
            pdev->deviceNode,
            pdev->deviceKey );
#endif

    /* Connect to the SDK */
    resultCode = QCWWANConnect( pdev->deviceNode,
                                pdev->deviceKey );
    return resultCode;
}

/*
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Return:   EXIT_SUCCESS, EXIT_FAILURE on unexpected error
 *
 * Notes:    none
 */
int main( int argc, const char *argv[])
{
    ULONG resultCode = 0;
    CHAR  selOption[OPTION_LEN];


    if( argc < 2 )
    {
        fprintf( stderr, "usage: %s <path to sdk binary>\n", argv[0] );
        exit( EXIT_SUCCESS );
    }

    if( NULL == (sdkbinpath = malloc(strlen(argv[1]) + 1)) )
    {
        perror(__func__);
        exit( EXIT_FAILURE );
    }

    strncpy( sdkbinpath, argv[1], strlen(argv[1]) + 1);

    /* Start the SDk */
    resultCode = StartSDK();
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr,
                 "\nRunning with device in boot and hold"\
                 " mode or disconnected\n");
    }
    else
    {
        fprintf( stderr,
                 "\nRunning with device in application mode\n");

        PrintDeviceModel();
    }

    while(1)
    {
        CHAR  *pEndOfLine = NULL;
        /* display menu */

        fprintf( stderr,
                 "\nPlease select one of the following options or press <Enter> to exit:\n"\
                 "1. Display the list of carrier images available for download\n" );

        if( eQCWWAN_ERR_NONE == resultCode )
        {
            fprintf( stderr,
                     "2. Display the list of carrier images resident on the device\n"\
                     "3. Display the information specific to a carrier image resident on the device\n"\
                     "4. Download carrier image to the device\n"\
                     "5. Delete a carrier image from the device\n"\
                     "6. Select a carrier image resident on the device as the new active image\n"\
                     "7. Display Raw Images present in device\n"\
                     "8. Activate from Raw Images avilable on device\n"\
                     "9. Display boot loader version number\n");

        }

        fprintf( stderr, "Option: " );

        /* Receive the input from the user */
        fgets( selOption, ( OPTION_LEN ), stdin );

        /* flush the input stream */
        pEndOfLine = strchr( selOption, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
        }

#ifdef DBG
    fprintf( stderr, "Selected Option : %s\n", selOption );
#endif

       /* If user has entered an invalid input, prompt again */
       if( 2 < strlen(selOption) )
       {
           continue;
       }

       /* if user requests anything but host image info or to exit while
        * operating in non-application device state, then re-prompt.
        */
       if( eQCWWAN_ERR_NONE != resultCode &&
           eHOST_IMAGE_INFO != selOption[0] &&
           eEXIT_APP != selOption[0] )
       {
           continue;
       }

        /* Process user input */
        switch( selOption[0] ){
            case eHOST_IMAGE_INFO:
                if (SLQSIsSpkgFormatRequired())
                {
                    DisplayHostImageSpkg();
                }
                else
                {
                    ListHostImages();
                }
                break;

            case eDEV_IMAGE_INFO:
                ListDeviceImages(TRUE);
                break;

            case eHOST_DETAIL_IMG_INFO:
                ListDetailImageInfo();
                break;

            case eDWL_IMG:
                DownloadImage();
                break;

            case eDEL_IMG:
                DeleteImage();
                break;

            case eACTIVATE_SEL_IMG:
                ActivateSelectedImage();
                break;

            case eDEV_RAW_IMG_DISP:
                ListRawDeviceImages();
                break;

            case eACTIVATE_RAW_SEL_IMG:
                ActivateRawSelectedImage();
                break;

            case eGET_BOOT_LOADER_VER:
                GetBootLoaderVersion();
                break;

            case eEXIT_APP:
                free(sdkbinpath);
                fprintf( stderr, "Exiting Application!!!\n");
                QCWWANDisconnect();
                exit( EXIT_SUCCESS );
                break;

            default:
                fprintf( stderr, "INVALID OPTION!!!\n" );
                break;
        }
    }
}
