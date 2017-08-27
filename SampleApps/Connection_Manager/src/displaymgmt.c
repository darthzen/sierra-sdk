/*************
 *
 * Filename:    displaymgmt.c
 *
 * Purpose:     Contains function for managing the capturing of logs for
 *              Connection manager application
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include "displaymgmt.h"

/****************************************************************
*                       DEFINES
****************************************************************/
#define MAX_LABEL_LEN                     30
#define MAX_BUF_LEN                       100
#define DEVICE_INFO_TOTAL_FIELDS          13
#define HOME_NETWORK_TOTAL_FIELDS         7
#define RF_INFO_TOTAL_FIELDS              8
#define ACTIVE_PROFILE_PARAM_TOTAL_FIELDS 8
#define CALL_STATISTICS_TOTAL_FIELDS      7
#define HOME_NETWORK_MENU_SIZE            14
#define RF_INFO_MENU_SIZE                 16
#define ACTIVE_PROFILE_PARAM_MENU_SIZE    27
#define CALL_STATISTICS_MENU_SIZE         17
#define NEXT_VALUE_INDEX                  91
#define DEVICE_INFO_VALUE_INDEX           41
#define HOME_NETWORK_VALUE_INDEX          DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * DEVICE_INFO_TOTAL_FIELDS ) + HOME_NETWORK_MENU_SIZE // 465
#define RF_INFO_VALUE_INDEX               HOME_NETWORK_VALUE_INDEX +  ( NEXT_VALUE_INDEX * HOME_NETWORK_TOTAL_FIELDS ) + RF_INFO_MENU_SIZE // 768
#define ACTIVE_PROFILE_PARAM_VALUE_INDEX  RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * RF_INFO_TOTAL_FIELDS ) + ACTIVE_PROFILE_PARAM_MENU_SIZE // 1123
#define CALL_STATISTICS_VALUE_INDEX       ACTIVE_PROFILE_PARAM_VALUE_INDEX +  ( NEXT_VALUE_INDEX * ACTIVE_PROFILE_PARAM_TOTAL_FIELDS ) + CALL_STATISTICS_MENU_SIZE //1468

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/
struct displayLayoutTable
{
    char *pFieldName;
    int  valueDisplayPosition;
};

/****************************************************************
*                       GLOBALS
****************************************************************/
static int  windowDisplayState = FALSE;
static int  fileCreated = FALSE;
static FILE *pFile = NULL;
static int  wspace90 = -90, wspace69 = -69;

/* Display layout table */
static struct displayLayoutTable sLayoutTable[eMAX_DISPLAY_FIELDS] =
{
                         /* Field Name             Value Display position */
/* eMANUFACTURE_ID */    { "Manufacture        :", DEVICE_INFO_VALUE_INDEX },
/* eMODEL_ID */          { "Model Id           :", DEVICE_INFO_VALUE_INDEX + NEXT_VALUE_INDEX },
/* eFIRMWARE_REVISION */ { "Firmware Revisions :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 2 ) },
/* eBOOT_VERSION */      { "Boot Revisions     :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 3 ) },
/* ePRI_VERSION */       { "PRI Revisions      :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 4 ) },
/* ePRL_VERSION */       { "PRL Version        :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 5 ) },
/* ePRL_PREFERENCE*/     { "PRL Preference     :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 6 ) },
/* eIMSI */              { "IMSI               :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 7 ) },
/* eESN_NUMBER */        { "ESN Number         :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 8 ) },
/* eIMEI_NUMBER */       { "IMEI Number        :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 9 ) },
/* eMEID_NUMBER */       { "MEID Number        :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 10 ) },
/* eHARDWARE_REVISION */ { "Hardware Revision  :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 11 ) },
/* eDEVICE_STATE */      { "Device State       :", DEVICE_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 12 ) },
/* eHOME_NETWORK */      { "Home Network       :", HOME_NETWORK_VALUE_INDEX },
/* eROAMING_STATUS */    { "Roaming Status     :", HOME_NETWORK_VALUE_INDEX + NEXT_VALUE_INDEX },
/* eDATA_CAPABILITIES */ { "Data Capabilities  :", HOME_NETWORK_VALUE_INDEX + ( NEXT_VALUE_INDEX * 2 ) },
/* eSESSION_STATE */     { "Session State      :", HOME_NETWORK_VALUE_INDEX + ( NEXT_VALUE_INDEX * 3 ) },
/* eDATA_BEARER */       { "Data Bearer        :", HOME_NETWORK_VALUE_INDEX + ( NEXT_VALUE_INDEX * 4 ) },
/* eDORMANCY_STATUS */   { "Dormancy Status    :", HOME_NETWORK_VALUE_INDEX + ( NEXT_VALUE_INDEX * 5 ) },
/* eLU_REJECT */         { "LU Reject Cause    :", HOME_NETWORK_VALUE_INDEX + ( NEXT_VALUE_INDEX * 6 ) },
/* eRADIO_INTERFACE */   { "Radio Interface    :", RF_INFO_VALUE_INDEX },
/* eACTIVE_BAND_CLASS */ { "Active Band Class  :", RF_INFO_VALUE_INDEX + NEXT_VALUE_INDEX },
/* eACTIVE_CHANNEL */    { "Active Channel     :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 2 ) },
/* eSIGNAL_STRENGTH */   { "Signal Strength    :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 3 ) },
/* eECIO */              { "ECIO               :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 4 ) },
/* eIO */                { "IO                 :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 5 ) },
/* eSINR */              { "SINR               :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 6 ) },
/* eRSRQ */              { "RSRQ               :", RF_INFO_VALUE_INDEX + ( NEXT_VALUE_INDEX * 7 ) },
/* ePDP_TYPE */          { "PDP Type           :", ACTIVE_PROFILE_PARAM_VALUE_INDEX },
/* eAUTHENTICATION */    { "Authentication     :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + NEXT_VALUE_INDEX },
/* ePROFILE_NAME */      { "Profile Name       :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 2 ) },
/* eAPN_NAME */          { "APN Name           :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 3 ) },
/* eUSER_NAME */         { "User Name          :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 4 ) },
/* eIP_ADDRESS */        { "IP Address         :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 5 ) },
/* ePRIMARY_DNS */       { "Primary DNS        :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 6 ) },
/* eSECONDARY_DNS */     { "Secondary DNS      :", ACTIVE_PROFILE_PARAM_VALUE_INDEX + ( NEXT_VALUE_INDEX * 7 ) },
/*  eCALL_STATUS */      { "Call Status        :", CALL_STATISTICS_VALUE_INDEX },
/* eTX_BYTES */          { "Bytes Transferred  :", CALL_STATISTICS_VALUE_INDEX + NEXT_VALUE_INDEX },
/* eRX_BYTES */          { "Bytes Received     :", CALL_STATISTICS_VALUE_INDEX + ( NEXT_VALUE_INDEX * 2 ) },
/* eCURRENT_TX_RATE */   { "Current TX Rate    :", CALL_STATISTICS_VALUE_INDEX + ( NEXT_VALUE_INDEX * 3 ) },
/* eCURRENT_RX_RATE */   { "Current RX Rate    :", CALL_STATISTICS_VALUE_INDEX + ( NEXT_VALUE_INDEX * 4 ) },
/* eMAX_TX_RATE */       { "MAX TX Rate        :", CALL_STATISTICS_VALUE_INDEX + ( NEXT_VALUE_INDEX * 5 ) },
/* eMAX_RX_RATE */       { "MAX RX Rate        :", CALL_STATISTICS_VALUE_INDEX + ( NEXT_VALUE_INDEX * 6 ) },
};

/****************************************************************
*                       FUNCTIONS
****************************************************************/

/*
 * Name:     closeLogFile
 *
 * Purpose:  Close the log file created at the beginning and terminates the
 *           logs creating thread.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void closeLogFile()
{
    /* If log file is not created, nothing to close */
    if( fileCreated == TRUE )
    {
        fclose( pFile );
    }
}

/*
 * Name:     SetWindowDisplayState
 *
 * Purpose:  Set the value of the windowDisplayState variable.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void SetWindowDisplayState( int state )
{
    windowDisplayState = state;
}

/*
 * Name:     GetWindowDisplayState
 *
 * Purpose:  Get the value of the windowDisplayState variable.
 *
 * Params:   None
 *
 * Return:   TRUE  : If the log file is initialized.
 *           FALSE : If the log file is not initialized
 *
 * Notes:    None
 */
int GetWindowDisplayState()
{
    return windowDisplayState;
}

/*
 * Name:     UpdateUserDisplay
 *
 * Purpose:  This function updates the field specified by the user in log file
 *           with the passed value. If file creation fails then values are
 *           written to /var/log/user.log file.
 *
 * Params:   windowFieldIdx - Index of the parameter whose  value needs to be
 *                            updated in logs.
 *           pData          - Buffer containing the value to be updated.
 *
 * Return:   None
 *
 * Notes:    None
 */
void UpdateUserDisplay(
    int  windowFieldIdx,
    char *pData )
{
    #ifdef DBG
    fprintf( stderr, "windowFieldIdx = %d,Data = %s\n", windowFieldIdx, pData );
    #endif

    if( FALSE == fileCreated )
    {
        char buf[MAX_BUF_LEN];

        /* File is not created, Write the updated values to sys log */
        memset( buf, 0, MAX_BUF_LEN);
        strcpy( buf, sLayoutTable[windowFieldIdx].pFieldName );
        strcat( buf, pData );
        syslog( LOG_INFO, "%s", buf );
        return;
    }

    /* Update the data in the specified field in the user log file */
    fseek( pFile, sLayoutTable[windowFieldIdx].valueDisplayPosition, SEEK_SET );
    fprintf( pFile, "%2$*1$s", wspace69, " " );
    fseek( pFile, sLayoutTable[windowFieldIdx].valueDisplayPosition, SEEK_SET );
    fprintf( pFile, "%s", pData );
    fflush( pFile );
}

/*
 * Name:     ResetDisplay
 *
 * Purpose:  This function reset all the fields in the log file.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void ResetDisplay()
{
    int idx = 0;

    /* If log file is not created, nothing to reset */
    if( fileCreated == FALSE )
    {
        return;
    }

    /* Reset all the fields in the log file */
    for( idx = 0; idx < eMAX_DISPLAY_FIELDS; idx++ )
    {
        fseek( pFile,
               sLayoutTable[idx].valueDisplayPosition,
               SEEK_SET );

        fprintf( pFile, "%2$*1$s", wspace69, " " );
    }
}

/*
 * Name:     MakeInfoPair
 *
 * Purpose:  This function creates a label with the name passed by the user
 *           in the log file whose pointer is also passed by the user.
 *
 * Params:   pFile      - File pointer in which the value needs to be written.
 *           pLabelName - Pointer to the buffer containing value to be written.
 *
 * Return:   None
 *
 * Notes:    None
 */
void MakeInfoPair(
    FILE *pFile,
    char *pLabelName )
{
    fprintf( pFile, "%2$*1$s\n", wspace90, pLabelName );
}

/*
 * Name:     InitializeDisplay
 *
 * Purpose:  This function creates the log file and initializes with required
 *           labels.
 *
 * Params:   pParam - Pointer to the buffer containing information passed to
 *                    this thread. It is NULL in our case.
 *
 * Return:   None
 *
 * Notes:    None
 */
void InitializeDisplay( void )
{
#if 0
    /* Open the file for writing logs */
    pFile = fopen( "ConnectionManager.txt", "w" );
    if( NULL == pFile )
    {
        #ifdef DBG
        printf("\n Failed to open the file \n");
        #endif
        fileCreated = FALSE;
    }
    else
    {
        /* Display Device information menu */
        fprintf( pFile, "\nDEVICE INFORMATION\n" );
        MakeInfoPair( pFile, "Manufacture        :" );
        MakeInfoPair( pFile, "Model Id           :" );
        MakeInfoPair( pFile, "Firmware Revisions :" );
        MakeInfoPair( pFile, "Boot Revisions     :" );
        MakeInfoPair( pFile, "PRI Revisions      :" );
        MakeInfoPair( pFile, "PRL Version        :" );
        MakeInfoPair( pFile, "PRL Preference     :" );
        MakeInfoPair( pFile, "IMSI               :" );
        MakeInfoPair( pFile, "ESN Number         :" );
        MakeInfoPair( pFile, "IMEI Number        :" );
        MakeInfoPair( pFile, "MEID Number        :" );
        MakeInfoPair( pFile, "Hardware Revision  :" );
        MakeInfoPair( pFile, "Device State       :" );

        /* Display Home network menu */
        fprintf( pFile, "\nHOME NETWORK\n" );
        MakeInfoPair( pFile, "Home Network       :" );
        MakeInfoPair( pFile, "Roaming Status     :" );
        MakeInfoPair( pFile, "Data Capabilities  :" );
        MakeInfoPair( pFile, "Session State      :" );
        MakeInfoPair( pFile, "Data Bearer        :" );
        MakeInfoPair( pFile, "Dormancy Status    :" );
        MakeInfoPair( pFile, "LU Reject Cause    :" );

        /* Display the RF information menu */
        fprintf( pFile, "\nRF INFORMATION\n" );
        MakeInfoPair( pFile, "Radio Interface    :" );
        MakeInfoPair( pFile, "Active Band Class  :" );
        MakeInfoPair( pFile, "Active Channel     :" );
        MakeInfoPair( pFile, "Signal Strength    :" );
        MakeInfoPair( pFile, "ECIO               :" );
        MakeInfoPair( pFile, "IO                 :" );
        MakeInfoPair( pFile, "SINR               :" );
        MakeInfoPair( pFile, "RSRQ               :" );

        /* Display the Active profile parameters menu */
        fprintf( pFile, "\nACTIVE PROFILE PARAMETERS\n" );
        MakeInfoPair( pFile, "PDP Type           :" );
        MakeInfoPair( pFile, "Authentication     :" );
        MakeInfoPair( pFile, "Profile Name       :" );
        MakeInfoPair( pFile, "APN Name           :" );
        MakeInfoPair( pFile, "User Name          :" );
        MakeInfoPair( pFile, "IP Address         :" );
        MakeInfoPair( pFile, "Primary DNS        :" );
        MakeInfoPair( pFile, "Secondary DNS      :" );

        /* Display the Call statistics */
        fprintf( pFile, "\nCALL STATISTICS\n" );
        MakeInfoPair( pFile, "Call Status        :" );
        MakeInfoPair( pFile, "Bytes Transferred  :" );
        MakeInfoPair( pFile, "Bytes Received     :" );
        MakeInfoPair( pFile, "Current TX Rate    :" );
        MakeInfoPair( pFile, "Current RX Rate    :" );
        MakeInfoPair( pFile, "MAX TX Rate        :" );
        MakeInfoPair( pFile, "MAX RX Rate        :" );

        /* File is created successfully, all logs will be updated in file */
        fileCreated = TRUE;
    }
#endif

    /* Display menu is completely initialized, set the flag for it */
    SetWindowDisplayState( TRUE );
}

