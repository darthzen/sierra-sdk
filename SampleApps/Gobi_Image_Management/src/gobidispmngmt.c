/*************
 *
 * Filename:    gobidispmngmt.c
 *
 * Purpose:     display related funtion of Gobi Image Management SAP
 *
 * Copyright:   © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "SWIWWANCMAPI.h"
#include"gobidispmngmt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>


/* GLOBAL DATA */

static struct imgInfoVb sImgInfoVb;
BYTE imgFolderIdx[MAX_IMAGES+1];
BYTE folderIdx = 0;
const CHAR *pImgFolderName[MAX_IMAGES] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
struct ImageList devImgList;

/****************************************************************************
 * *                       Functions
 * *****************************************************************************/
/*
 * Name:    GetCarrierString
 *
 * Purpose: Get the carrier string format present in the carrierImageOverride file
 *
 * param    [in] carrier
 *          The carrier of the Modem image
 *
 * return
 *          The carrier substring as present in the carrierImageOverride file
 *
 * note
 *          None
 */
char *GetCarrierString (ULONG carrier)
{
    switch (carrier)
    {
        case eGOBI_IMG_CAR_SPRINT:
            return "SP=";
        case eGOBI_IMG_CAR_VERIZON:
            return "VE=";
        case eGOBI_IMG_CAR_VODAFONE:
            return "VO=";
        case eGOBI_IMG_CAR_ATT:
            return "AT=";
        case eGOBI_IMG_CAR_TMOBILE:
            return "TM=";
        case eGOBI_IMG_CAR_GENERIC:
            return "GE=";
        case eGOBI_IMG_CAR_TELEFONICA:
            return "TE=";
        case eGOBI_IMG_CAR_TELCOM_ITALIA:
            return "TI=";
        case eGOBI_IMG_CAR_ORANGE:
            return "OR=";
        case eGOBI_IMG_CAR_CHINA_TELECOM:
            return "CT=";
        default:
            return NULL;
    }
    return NULL;
}

/*
 * Name:     DisplayImageInfo
 *
 * Purpose:  Print the information(passed as input) of the firmware image.
 *
 * Return:   None
 *
 * Notes:    none
 */
void DisplayImageInfo(
    BYTE               printActiveStatus,
    struct qmifwinfo_s *pImgInfo )
{

    /* Intialize the Image info buffer */
    memset( (void *)&sImgInfoVb, 0, sizeof( sImgInfoVb ) );

    /* Get the Carrier Name */
   switch( pImgInfo->dev.g.Carrier )
    {
        case eGOBI_IMG_CAR_GENERIC:
            strcpy( sImgInfoVb.carrierName, "Generic" );
            break;

        case eGOBI_IMG_CAR_FACTORY:
            strcpy( sImgInfoVb.carrierName, "Factory" );
            break;

        case eGOBI_IMG_CAR_VERIZON:
            strcpy( sImgInfoVb.carrierName, "Verizon" );
            break;

        case eGOBI_IMG_CAR_SPRINT:
            strcpy( sImgInfoVb.carrierName, "Sprint" );
            break;

        case eGOBI_IMG_CAR_ATT:
            strcpy( sImgInfoVb.carrierName, "AT&T" );
            break;

        case eGOBI_IMG_CAR_TMOBILE:
            strcpy( sImgInfoVb.carrierName, "T-Mobile" );
            break;

        case eGOBI_IMG_CAR_TELEFONICA:
            strcpy( sImgInfoVb.carrierName, "Telefonica" );
            break;

        case eGOBI_IMG_CAR_TELCOM_ITALIA:
            strcpy( sImgInfoVb.carrierName, "Telecom-Italia" );
            break;

        case eGOBI_IMG_CAR_ORANGE:
            strcpy( sImgInfoVb.carrierName, "Orange" );
            break;

        case eGOBI_IMG_CAR_VODAFONE:
            strcpy( sImgInfoVb.carrierName, "Vodafone" );
            break;

        case eGOBI_IMG_CAR_CHINA_TELECOM:
            strcpy( sImgInfoVb.carrierName, "China Telecom" );
            break;

        case eGOBI_IMG_CAR_GENERIC_CDMA:
            strcpy( sImgInfoVb.carrierName, "Generic CDMA" );
            break;

        case eGOBI_IMG_CAR_AERIS:
            strcpy( sImgInfoVb.carrierName, "Aeris" );
            break;

        case eGOBI_IMG_CAR_ROGERS:
            strcpy( sImgInfoVb.carrierName, "Rogers" );
            break;

        case eGOBI_IMG_CAR_BELL:
            strcpy( sImgInfoVb.carrierName, "Bell" );
            break;

        case eGOBI_IMG_CAR_TELUS:
            strcpy( sImgInfoVb.carrierName, "Telus" );
            break;

        default:
            strcpy( sImgInfoVb.carrierName, "Unknown" );
            break;
    }

    /* Get the technology Name */
    switch( pImgInfo->dev.g.Technology )
    {
        case eGOBI_IMG_TECH_CDMA:
            strcpy( sImgInfoVb.technologyName, "CDMA" );
            break;

        case eGOBI_IMG_TECH_UMTS:
            strcpy( sImgInfoVb.technologyName, "UMTS" );
            break;

        default:
            strcpy( sImgInfoVb.technologyName, "Unknown" );
            break;
    }

    /* Get the region */
    switch( pImgInfo->dev.g.Region )
    {
        case eGOBI_IMG_REG_NA:
            strcpy( sImgInfoVb.regionName, "NA" );
            break;

        case eGOBI_IMG_REG_LA:
            strcpy( sImgInfoVb.regionName, "LA" );
            break;

        case eGOBI_IMG_REG_EU:
            strcpy( sImgInfoVb.regionName, "EU" );
            break;

        case eGOBI_IMG_REG_ASIA:
            strcpy( sImgInfoVb.regionName, "AS" );
            break;

        case eGOBI_IMG_REG_AUS:
            strcpy( sImgInfoVb.regionName, "AU" );
            break;

        case eGOBI_IMG_REG_GLOBAL:
            strcpy( sImgInfoVb.regionName, "Global" );
            break;

        default:
            strcpy( sImgInfoVb.regionName, "Unknown");
            break;
    }

    /* Get the information about GPSfeature */
    switch( pImgInfo->dev.g.GPSCapability )
    {
        case eGOBI_IMG_GPS_NONE:
            strcpy( sImgInfoVb.gpsName, "NONE" );
            break;

        case eGOBI_IMG_GPS_STAND_ALONE:
            strcpy( sImgInfoVb.gpsName, "STAND-ALONE" );
            break;

        case eGOBI_IMG_GPS_ASSISTED:
            strcpy( sImgInfoVb.gpsName, "STAND-ALONE + AGPS + XTRA" );
            break;

        case eGOBI_IMG_GPS_NO_XTRA:
            strcpy( sImgInfoVb.gpsName, "STAND-ALONE + AGPS" );
            break;

        default:
            strcpy( sImgInfoVb.gpsName, "UNKNOWN" );
            break;
    }

    /* If the image <active> status needs to be displayed */
    int w10 = -10, w20 = -20;
    if( eACTIVE_IMG_DIS & printActiveStatus )
    {
        /* If the selected image is active image */
        fprintf( stderr,
                 "%2$*1$s%3$*1$s%4$*1$s",
                 w20,
                 sImgInfoVb.carrierName,
                 sImgInfoVb.technologyName,
                 sImgInfoVb.regionName );

        if( eACTIVE_IMG == printActiveStatus )
        {
            fprintf( stderr, "ACTIVE\n" );
        }
        else
        {
            fprintf( stderr, "INACTIVE\n" );
        }
    }
    else
    {
        fprintf( stderr, "%3$*2$s%4$*2$s%5$*1$s%6$*1$x%7$*1$s\n",
                 w10, w20,
                 sImgInfoVb.carrierName,
                 sImgInfoVb.technologyName,
                 sImgInfoVb.regionName,
                 (unsigned int)pImgInfo->dev.g.FirmwareID,
                 sImgInfoVb.gpsName );
    }
}

/*
 * Name:     FlushStdinStream
 *
 * Purpose:  Flush the stdin stream
 *
 * Return:   None
 *
 * Notes:    fflush does not work for input stream.
 */
void FlushStdinStream( )
{
    int inputChar;

    /* keep on reading until an <New Line> or end of file is received */
    do
    {
        inputChar = getchar();

#ifdef DBG
    fprintf( stderr,  "inputChar: 0x%x\n", inputChar );
#endif
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != EOF ) );
}

/*
 * Name:     SortArrToAscendOrder
 *
 * Purpose:  funtion sorts image folders present at host in ascending order
 *
 * Return:   none
 *
 * Notes:    none
 */
void SortArrToAscendOrder( BYTE *imgFolderIdx )
{
    BYTE i = 0, n = strlen((CHAR*)imgFolderIdx);
    BYTE j = 0;
    if ( n > MAX_IMAGES )
    {
        n = MAX_IMAGES;
        imgFolderIdx[MAX_IMAGES] = '\0';
    }
    for( i=0; i<n; i++ )
    {
        for( j=i+1; j<n; j++ )
        {
            if( imgFolderIdx[i]>imgFolderIdx[j] )
            {
                imgFolderIdx[i] = imgFolderIdx[i]^imgFolderIdx[j];
                imgFolderIdx[j] = imgFolderIdx[i]^imgFolderIdx[j];
                imgFolderIdx[i] = imgFolderIdx[i]^imgFolderIdx[j];
            }
        }
    }
}

/*
 * Name:     FileNameFilter
 *
 * Purpose:  Filter function used as an input parameter in scandir function call
 *
 * Return:   VALID_FILE   - In case the required file is found
 *           INVALID_FILE - In case the required file is not found
 *
 * Notes:    none
 */
int FileNameFilter( const struct dirent *entry)
{
    BYTE idx = 0;

    /* Check if the folder with the required name exist */
    while( idx < MAX_IMAGES )
    {
        if( SUCCESS == strcmp( entry->d_name,
                               pImgFolderName[idx] ) )
        {
            imgFolderIdx[folderIdx++] = *pImgFolderName[idx];
            return VALID_FILE;
        }
        idx++;
    }
    return INVALID_FILE;
}

/*
 * Name:     GetImagePath
 *
 * Purpose:  Receives the image path located at host from the user,validates it
 *           and retrieve the information from the image.
 *
 * Return:   None
 *
 * Notes:    none
 */
void GetImagePath( CHAR *pImagePath )
{
    struct dirent **nameList;
    WORD          len = 0, totalEntries = 0, idx = 0;

    while(1)
    {
        CHAR          *pEndOfLine = NULL;
        /* clear the imagePath buffer */
        memset( pImagePath, 0, MAX_IMAGE_PATH);
        memset( imgFolderIdx, 0, MAX_IMAGES+1 );

       /* Print the sub menu */
        fprintf( stderr,
                 "\nPlease specify the path (upto 510 Characters) or press <Enter> to return to the main menu: " );

        fgets( pImagePath, ( MAX_IMAGE_PATH - 3), stdin);

#ifdef DBG
    fprintf( stderr,  "Image Path: %s\n", pImagePath );
#endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == pImagePath[0] )
            return;

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream and prompt the user to enter the
         * valid path.
         */
        pEndOfLine = strchr( pImagePath, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
            continue;
        }

        /* fgets() is used to get user input, hence buffer overflow will never
         * occur. New line character is also scanned and put at the end of
         * buffer by fgets(). Remove the new line character from the end of the
         * buffer and NULL terminate it. Check if the path has a terminating
         * '/' character, else append a terminating '/' character.
         */
        len = strlen( pImagePath );
        if( pImagePath[len - 2] != '/' )
        {
            pImagePath[len - 1] = '/';
            pImagePath[len] = '\0';
        }
        else
        {
            pImagePath[len - 1] = '\0';
        }

        /* Get the valid folders from the specified path */
        totalEntries = scandir( pImagePath, &nameList, FileNameFilter, NULL );
        SortArrToAscendOrder(imgFolderIdx);

        /* If no folder exist with the required name */
        if ( ( 0 >= totalEntries ) ||
             ( MAX_IMAGES < totalEntries) )
        {
            fprintf( stderr, "Failed to read Directory or No Image folder exist\n");
            continue;
        }

        /* Valid image folders are found, exit the loop */
        break;
    }

    /* Free the memory allocated to the name list */
    while ( idx < totalEntries )
    {
#ifdef DBG
        fprintf( stderr, "Directory Name : %s\n", nameList[idx]->d_name );
#endif
        free( nameList[idx] );
        idx++;
    }
    free(nameList);
}

/*
 * Name:     GetSpkgImagePath
 *
 * Purpose:  Receives the image path located at host from the user
 *
 * Return:   None
 *
 * Notes:    none
 */
void GetSpkgImagePath( CHAR *pImagePath )
{
    WORD         len = 0;

    while(1)
    {
        CHAR          *pEndOfLine = NULL;
        /* clear the imagePath buffer */
        memset( pImagePath, 0, MAX_IMAGE_PATH);

       /* Print the sub menu */
        fprintf( stderr,
                 "\nPlease specify the path (upto 510 Characters) or press <Enter> to return to the main menu: " );

        fgets( pImagePath, ( MAX_IMAGE_PATH - 3), stdin);

#ifdef DBG
    fprintf( stderr,  "Image Path: %s\n", pImagePath );
#endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( ENTER_KEY == pImagePath[0] )
            return;

        /* If '/n' character is not read, there are more characters in input
         * stream. Clear the input stream and prompt the user to enter the
         * valid path.
         */
        pEndOfLine = strchr( pImagePath, ENTER_KEY );
        if( NULL == pEndOfLine )
        {
            FlushStdinStream();
            continue;
        }

        /* fgets() is used to get user input, hence buffer overflow will never
         * occur. New line character is also scanned and put at the end of
         * buffer by fgets(). Remove the new line character from the end of the
         * buffer and NULL terminate it. Check if the path has a terminating
         * '/' character, else append a terminating '/' character.
         */
        len = strlen( pImagePath );
        if( pImagePath[len - 2] != '/' )
        {
            pImagePath[len - 1] = '/';
            pImagePath[len] = '\0';
        }
        else
        {
            pImagePath[len - 1] = '\0';
        }
        break;
    }

}

/*
 * Name:     GetPRIImageIdx
 *
 * Purpose:  Gets the PRI image index from the list.
 *
 * Return:   PRI image index from the image list or false if the PRI image is
 *           not found.
 *
 * Notes:    none
 */
bool GetPRIImageIdx( BYTE *pImgListIdx)
{
    bool retVal = false;
    /* Figure out the index of PRI image type in the received image list */
    if( IMG_TYPE_PRI == devImgList.imageIDEntries[0].imageType )
    {
        *pImgListIdx = 0;
        retVal = true;
    }
    else if( 2 == devImgList.listSize )
    {
        /* If the PRI image is not located at the first index in the list and
         * sizeof the retrieved list is 2, then next image index is the PRI
         * image index.
         */
        *pImgListIdx = 1;
        retVal = true;
    }
    return retVal;
}

/*
 * Name:     GetUserImageId
 *
 * Purpose:  Prompt the user to select a valid imgage id from the list.
 *
 * Return:   index of the image selected by the user from the displayed list,
 *           or hex value of enter key if pressed by the user when the length
 *           is 1 else false.
 *
 * Notes:    none
 */
int GetUserImageId( BYTE userAction, CHAR *pImgType, BYTE *length )
{
    CHAR selOption[OPTION_LEN];
    CHAR *pEndOfLine = NULL;
    unsigned int len = 0;
    BYTE imageIdx = 0;
    BYTE imgListIdx = 0;
    BYTE maxImageIdx = 0;

    while(1)
    {
        if ( pImgType )
        {
            /* Print the menu */
            fprintf( stderr, "\nPlease select a %s image index,or press <Enter> to exit:\n"\
                             "Option : ",pImgType );
        }
        else
        {
            /* Print the menu */
            fprintf( stderr, "\nPlease select a image index,or press <Enter> to exit:\n"\
                             "Option : ");

        }

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

        len = strlen( selOption );
        *length = len;
#ifdef DBG
    fprintf( stderr, "Selected Option : %s length = %d\n", selOption, len );
#endif

        /* If only <ENTER> is pressed by the user, return to main menu */
        if( strchr(selOption, ENTER_KEY) && (len == 1) )
        {
            return ENTER_KEY;
        }

        /* Convert the option added by user into integer */
        selOption[ len - 1 ] = '\0';
        imageIdx = atoi( selOption );

        /* If image id needs to be selected from the list of images resident
         * on the device.
         */
        if( ( eDEL_IMG == userAction )              ||
            ( eHOST_DETAIL_IMG_INFO == userAction ) ||
            ( eACTIVATE_SEL_IMG == userAction )     ||
            ( eACTIVATE_RAW_SEL_IMG == userAction  ) )
        {
            /* Figure out index of PRI image type in the received image list */
            if( !GetPRIImageIdx( &imgListIdx ) )
            {
                fprintf(stderr,"FAIL SAFE Image Active!" );
                return false;
            }
            maxImageIdx = devImgList.imageIDEntries[imgListIdx].imageIDSize;
        }
        else
        {
            maxImageIdx = MAX_IMAGES;
        }

        /* if user has entered an invalid image id */
        if( imageIdx > maxImageIdx )
        {
            fprintf( stderr, "Invalid Selection, Please Select Valid Id \n");
            continue;
        }
        return imageIdx;
    }
}

/*
 * Name:     spkgFwDisplay
 *
 * Purpose:  SPKG CWE image info. display
 *
 * Return:   None
 *
 * Notes:    none
 */
void spkgFwDisplay( struct qmifwinfo_s *pin )
{
    fprintf(    stderr,
                "\nSPKG CWE Image Fields\n"\
                "---------------------\n"\
                "Model ID: %s\n"\
                "BOOT Version: %s\n"\
                "AMSS Version: %s\n"\
                "SKU ID: %s\n"\
                "Package ID: %s\n"
                "Carrier: %s\n"\
                "PRI version: %s\n",
                pin->dev.s.modelid_str,  pin->dev.s.bootversion_str,
                pin->dev.s.appversion_str, pin->dev.s.sku_str,
                pin->dev.s.packageid_str , pin->dev.s.carrier_str,
                pin->dev.s.priversion_str );
}

