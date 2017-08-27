/*******************************************************************************
 * Copyright (c) 2012 Sierra Wireless and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * 
 *     
 *******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include "swi_log.h"
#include "swi_airvantage.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "testutils.h"

#define ASSET_ID_SLQS "SLQS_FW_UPDATER"
#define SLQS_MC77XX_UPDATE_REQ  91
#define SLQS_GOBI_UPDATE_REQ    81
#define SLQS_MC73XX_UPDATE_REQ  71

static volatile char waiting_notification = 1;
static char result = 1;

extern bool ra_FirmwareDownloader_Gobi(char *InputImagePath);
extern bool ra_FirmwareDownloader_MC77XX(char *pImagePath);
extern bool ra_FirmwareDownloader_MC73XX(char *pImagePath);
extern void ra_init_main(void);
char fw_path[500];
char g_component[100];

static void updateNotificationCb_slqs(swi_av_Asset_t *asset, const char* componentNamePtr, const char* versionPtr,
    const char *updateFilePathPtr, swi_dset_Iterator_t* customParams, void *userDataPtr)
{
    SWI_LOG("AV", DEBUG, "updateNotificationCb slqs : %s \n",componentNamePtr);

    if ((!strcmp("MC7710_pkg", componentNamePtr))||(!strcmp("MC7700_pkg", componentNamePtr))||(!strcmp("MC7750_pkg", componentNamePtr)))
    {
        //MC77XX
        result = SLQS_MC77XX_UPDATE_REQ;
        waiting_notification = 1;
        memset(g_component,0,sizeof(g_component));
        memset(fw_path,0,sizeof(fw_path));
        strcpy(g_component,componentNamePtr);
        strcpy(fw_path, updateFilePathPtr);
        strcat(fw_path,"/");
        SWI_LOG("AV", DEBUG, "updateNotificationCb slqs path = %s\n", fw_path);

        return;
    }

    else if ((!strcmp("MC835_pkg", componentNamePtr))||(!strcmp("SL9090_pkg", componentNamePtr)))
    {
        //Gobi
        result = SLQS_GOBI_UPDATE_REQ;
        waiting_notification = 1;
        memset(g_component,0,sizeof(g_component));
        memset(fw_path,0,sizeof(fw_path));
   
        strcpy(g_component,componentNamePtr);
        strcpy(fw_path, updateFilePathPtr);
        strcat(fw_path,"/");
        SWI_LOG("AV", DEBUG, "updateNotificationCb path = %s\n", fw_path);

        return;
    }

    else if ((strstr(componentNamePtr, "MC73") && strstr(componentNamePtr, "_pkg")) ||
             (strstr(componentNamePtr, "EM73") && strstr(componentNamePtr, "_pkg")))
    {
        //Gobi
        result = SLQS_MC73XX_UPDATE_REQ;
        waiting_notification = 1;
        memset(g_component,0,sizeof(g_component));
        memset(fw_path,0,sizeof(fw_path));
   
        strcpy(g_component,componentNamePtr);
        strcpy(fw_path, updateFilePathPtr);
        strcat(fw_path,"/");
        SWI_LOG("AV", DEBUG, "updateNotificationCb path = %s\n", fw_path);

        return;
    }

    result = 0;
    waiting_notification = 0;
}

static int UpdateNotification(void)
{
    swi_status_t res;

    swi_av_Asset_t* asset_slqs = NULL;
    waiting_notification = 1;

    res = swi_av_Init();
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Asset init fail\n");
    
    #ifdef AUTO_CONNECT_TO_SERVER
    SWI_LOG("AV", DEBUG, "start connecting to server\n");    
    res = swi_av_ConnectToServer(10);
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Connect to server fail\n");
    #endif
    
    res = swi_av_asset_Create(&asset_slqs, ASSET_ID_SLQS);
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Asset create fail\n");

    res = swi_av_asset_Start(asset_slqs);
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Asset start fail\n");

    res = swi_av_RegisterUpdateNotification(asset_slqs, (swi_av_updateNotificationCB) updateNotificationCb_slqs, "userData_slqs");
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Asset callback registration fail\n");

  do
  {
    //Gobi
    if(result==SLQS_GOBI_UPDATE_REQ)
    {
        result = 0;
        SWI_LOG("AV", DEBUG, "start SLQS Gobi fw update\n");  
        //build.default/runtime/update/tmp/updatepackage/fw/                             for local update 
        //build.default/runtime/update/tmp/package_2ef891f7e42c494ca2cef18b00540edb/fw/  for remote update 
        if(true == ra_FirmwareDownloader_Gobi(fw_path))
        {
            /* report success to server */
            swi_av_SendUpdateResult(asset_slqs,g_component,200);
            
            SWI_LOG("AV", DEBUG, "start SLQS Gobi fw update done\n");
        }
        else
        {
            swi_av_SendUpdateResult(asset_slqs,g_component,199);    
            SWI_LOG("AV", DEBUG, "start SLQS Gobi fw update fail\n");
        }
        continue;
    }    
    
    //MC77xx
    if(result==SLQS_MC77XX_UPDATE_REQ)
    {
        result = 0;
        SWI_LOG("AV", DEBUG, "start SLQS MC77xx fw update\n");  
        
        if(true == ra_FirmwareDownloader_MC77XX(fw_path))
        {    
            /* report success to server */
            swi_av_SendUpdateResult(asset_slqs,g_component,200);
            SWI_LOG("AV", DEBUG, "start SLQS MC7710 fw update done\n");
        }
        else
        {    
            swi_av_SendUpdateResult(asset_slqs,g_component,199);
             SWI_LOG("AV", DEBUG, "start SLQS MC7710 fw update fail\n");
        }
        continue;
    }        
    
    //MC73xx
    if(result==SLQS_MC73XX_UPDATE_REQ)
    {
        result = 0;
        SWI_LOG("AV", DEBUG, "start SLQS MC73xx fw update\n");  
        
        if(true == ra_FirmwareDownloader_MC73XX(fw_path))
        {    
            /* report success to server */
            swi_av_SendUpdateResult(asset_slqs,g_component,200);
            SWI_LOG("AV", DEBUG, "start SLQS MC73XX fw update done\n");
        }
        else
        {    
            swi_av_SendUpdateResult(asset_slqs,g_component,199);
             SWI_LOG("AV", DEBUG, "start SLQS MC73XX fw update fail\n");
        }
        continue;
    }

    sleep(60);
     #ifdef AUTO_CONNECT_TO_SERVER
    res = swi_av_ConnectToServer(10);
    if (res != SWI_STATUS_OK)
        SWI_LOG("AV", DEBUG, "Connect to server fail\n");
    #endif

  }while (waiting_notification==1);

  swi_av_asset_Destroy(asset_slqs);
  if (res != SWI_STATUS_OK)
    return res;
  return result;
}

int main(int argc, char *argv[])
{
    //INIT_TEST("AV_TEST");
    swi_log_setlevel(DEBUG, "AV", NULL );
    //defined in slqs lib to start SDK process 
    ra_init_main();
    
    sleep(5);
    UpdateNotification();
    //preventive clean if one previous test failed
    swi_av_Destroy();
    return 0;
}
