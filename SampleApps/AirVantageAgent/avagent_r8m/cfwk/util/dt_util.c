/*******************************************************************************
 * Copyright (c) 2015 Sierra Wireless and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Romain Perier      for Sierra Wireless - initial API and implementation
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>

#include "swi_util.h"
#include "swi_dset.h"
#include "swi_log.h"
#include "testutils.h"

void PrintUsage(void)
{
    fprintf(stderr, "\nUsage: \n\
\t[--apn|-a] APN \n \
\t[--interface|-i] Network Inferfiace\n \
\t[--username|-u] Network Username\n \
\t[--password|-p] Password\n \
\t[--auth|--c] \n\
\t\t 0 for None\n\
\t\t 1 for PAP\n\
\t\t 2 for CHAP Authentication\n\
\t\t 3 for PAP/CHAP \n\
\t[--set|-s] Set config variable.\n\
\t\t Patten \"Config String:Type:Config Value\"\n\
\t\t String Type : \n\
\t\t\t b : boolean.\n\
\t\t\t s : String.\n\
\t\t\t n : Number.\n\
\t\t e.g. change APN:\n\
\t\t\t config.network.apn:s:internet\n\
\t\t e.g. Switch to CDMA:\n\
\t\t\t config.network.isCdma:b:true\n\
\t\t e.g. Change CHAP Authentication to 0:\n\
\t\t\t config.network.auth:n:0\n\
");
}

static swi_status_t notificationCb(swi_dset_Iterator_t *data)
{
  return SWI_STATUS_OK;
}


static swi_status_t dt_util_Register(swi_util_regId_t *regId)
{
  swi_status_t res;
  const char * regVarsPtr[] = {
    "config.apn",
    "config.interface",
    NULL
  };

  res = swi_util_Init();

  if (res != SWI_STATUS_OK)
    return res;

  res = swi_util_Register(2, regVarsPtr, (swi_util_NotifyCB_t)notificationCb, 0, NULL, regId);
  if (res != SWI_STATUS_OK)
    return res;
  return 0;
}

static swi_status_t dt_util_Unregister(swi_util_regId_t regId)
{
  swi_status_t res;

  res = swi_util_Init();

  if (res != SWI_STATUS_OK)
    return res;

  res = swi_util_Unregister(regId);
  if (res != SWI_STATUS_OK)
    return res;
  return 0;
}

bool CheckBooleanResult(const char *cString, bool bExpectedResult)
{
  int res = 0;
  swi_dset_Iterator_t *set = NULL;
  res = swi_util_Get(cString, &set);
  if (res != SWI_STATUS_OK)
  {
    fprintf(stderr,"Get %s Fail %d\n",cString,res);
    return false;
  }
  res = swi_dset_Next(set);
  if (res != SWI_STATUS_OK)
  {
    fprintf(stderr,"Get Next %s Fail %d\n",cString,res);
    return false;

  }
  if (swi_dset_GetType(set) != SWI_DSET_BOOL)
  {
    fprintf(stderr,"Get %s Fail Wrong Type\n",cString);
    return false;
  }
  if (swi_dset_ToBool(set)!=bExpectedResult)
  {
    fprintf(stderr,"Set %s Fail\n",cString);
    return false;
  }
  return true;
}


bool CheckIntegerResult(const char *cString, int iExpectedResult)
{
  int res = 0;
  swi_dset_Iterator_t *set = NULL;
  res = swi_util_Get(cString, &set);
  if (res != SWI_STATUS_OK)
  {
    fprintf(stderr,"Get %s Fail %d\n",cString,res);
    return false;
  }
  res = swi_dset_Next(set);
  if (res != SWI_STATUS_OK)
  {
    fprintf(stderr,"Get Next %s Fail %d\n",cString,res);
    return false;

  }
  if (swi_dset_GetType(set) != SWI_DSET_INTEGER)
  {
    fprintf(stderr,"Get %s Fail Wrong Type\n",cString);
    return false;
  }
  if (swi_dset_ToInteger(set)!=iExpectedResult)
  {
    fprintf(stderr,"Set %s Fail %d/%d\n",cString,(int)swi_dset_ToInteger(set),iExpectedResult);
    return false;
  }
  return true;
}


bool CheckStringResult(const char *cString, const char *ExpectedResult)
{
    int res = 0;
    swi_dset_Iterator_t *set = NULL;
    res = swi_util_Get(cString, &set);
    if (res != SWI_STATUS_OK)
    {
        fprintf(stderr,"Get %s Fail %d\n",cString,res);
        return false;
    }
    res = swi_dset_Next(set);
     if (res != SWI_STATUS_OK)
    {
        fprintf(stderr,"Get Next %s Fail %d\n",cString,res);
        return false;
    }
    if (swi_dset_GetType(set) != SWI_DSET_STRING)
    {
        fprintf(stderr,"Get %s Fail Wrong Type\n",cString);
        return false;
    }
    if(swi_dset_ToString(set)== ExpectedResult)
    {
        return true;
    }else if(ExpectedResult==NULL)
    {
        if(strlen(swi_dset_ToString(set))>0)
            return false;
        else
            return true;
    }
    if (strcmp(swi_dset_ToString(set), ExpectedResult) != 0)
    {
        fprintf(stderr,"Set %s Fail\n",cString);
        return false;
    }
    return true;
}

static struct option long_options[] = {
              {"apn",  required_argument, 0, 'a'},
              {"interface",  required_argument, 0, 'i'},
              {"password",  required_argument, 0, 'p'},
              {"username",  required_argument, 0, 'u'},
              {"auth",  required_argument, 0, 'x'},
              {"set",  required_argument, 0, 's'},
              {"help",  no_argument, 0, 'h'},
              {0, 0, 0, 0}       };

#define CONFIG_PASSWORD_STRING "config.network.password"
#define CONFIG_USERNAME_STRING "config.network.username"
#define CONFIG_APN_STRING "config.network.apn"
#define CONFIG_AUTH_STRING "config.network.auth"
#define CONFIG_INTF_STRING "config.network.interface"
#define CONFIG_CHANGE_STRING "Changed"
#define CONFIG_NEW_STRING "New"


#define MAX_OPTION_STRING_SIZE 128

enum {
    eOption_Unknown=0,
    eOption_Number,
    eOption_boolean,
    eOption_String,
};
int iGetValueType(char type)
{
    switch(type)
    {
    case 'b':
        return eOption_boolean;
    case 'i':
        return eOption_Number;
    case 's':
        return eOption_String;
     default:
        break;
    }
    return eOption_Unknown;
}
bool PhaseOption(char *argString)
{
    char *semicolon = argString;
    char *semicoloncheck = NULL;
    char szConfigString[MAX_OPTION_STRING_SIZE] = {0};
    char szTypeString[MAX_OPTION_STRING_SIZE] = {0};
    char szValueString[MAX_OPTION_STRING_SIZE] = {0};
    int nItems=0;
    char szPatten[32]={0};
    int res = 0;
    swi_dset_Iterator_t *set = NULL;
    do{
        memset(&szConfigString,0,MAX_OPTION_STRING_SIZE);
        memset(&szTypeString,0,MAX_OPTION_STRING_SIZE);
        memset(&szValueString,0,MAX_OPTION_STRING_SIZE);
        set = NULL;
        semicoloncheck = strchr(semicolon, ';');
        if(semicoloncheck!=NULL)
        {
            snprintf(szPatten,31,"%%%d[^:]:%%%d[^:]:%%%d[^;]",
                MAX_OPTION_STRING_SIZE,MAX_OPTION_STRING_SIZE,MAX_OPTION_STRING_SIZE);
        }
        else
        {
            snprintf(szPatten,31,"%%%d[^:]:%%%d[^:]:%%%ds",
                MAX_OPTION_STRING_SIZE,MAX_OPTION_STRING_SIZE,MAX_OPTION_STRING_SIZE);
        }
        nItems = sscanf( semicolon, szPatten, szConfigString,szTypeString,szValueString);
        if(nItems==3)
        {
            fprintf(stderr,"OK:%s %s %s\n",szConfigString,szTypeString,szValueString);
            switch(iGetValueType(szTypeString[0]))
            {
                case eOption_boolean:
                   {
                        bool bOption  = false;
                        if(strcmp(szValueString,"true")==0)
                        {
                            bOption = true;
                        }
                        else
                        {
                            bOption = false;
                        }
                        res = swi_util_SetBool(szConfigString,bOption);
                        if (res != SWI_STATUS_OK)
                        {
                            fprintf(stderr,"Set %s Fail %d",szConfigString,res);
                            return false;
                        }
                        if (CheckBooleanResult(szConfigString,bOption)==false)
                        {
                            fprintf(stderr,"Set %s Fail\n",szConfigString);
                            return false;
                        }
                        else
                        {
                            fprintf(stderr,"Set %s : %d\n",szConfigString,(int)swi_dset_ToBool(set));
                        }
                    }
                    break;
                case eOption_Number:
                    res = swi_util_SetInteger(szConfigString,atoi(szValueString));
                    if (res != SWI_STATUS_OK)
                    {
                        fprintf(stderr,"Set %s Fail %d",szConfigString,res);
                        return false;
                    }

                    if (CheckIntegerResult(szConfigString,atoi(szValueString))==false)
                    {
                        fprintf(stderr,"Set %s Fail\n",szConfigString);
                        return false;
                    }
                    else
                    {
                        fprintf(stderr,"Set %s : %d\n",szConfigString,(int)swi_dset_ToInteger(set));
                    }
                    break;
                case eOption_String:
                    res = swi_util_SetString(szConfigString,szValueString);
                    if (res != SWI_STATUS_OK)
                    {
                        fprintf(stderr,"Set %s Fail %d",szConfigString,res);
                        return false;
                    }

                    if (CheckStringResult(szConfigString,szValueString)==false)
                    {
                        fprintf(stderr,"Set %s Fail\n",szConfigString);
                        return false;
                    }
                    else
                    {
                        fprintf(stderr,"Set %s : %s\n",szConfigString,szValueString);
                    }
                    break;
                default:
                    return false;
            }
        }
        else
        {
            fprintf(stderr,"FAIL(%d):%s\n",nItems,semicolon);
            return false;
        }
        semicolon = strchr(semicolon+1, ';');
        if(semicolon!=NULL)
        {
            semicolon++;
            if(strlen(semicolon)==0)
                break;
        }
    }while(semicolon != NULL);
    return true;
}

int main(int argc , char *argv[])
{
  int res = 0;
  int option = 0;
  swi_util_regId_t regId = NULL;
  int option_index=0;
  res = swi_util_Init();
  char szConfigName[128]={0};
  if(res!=SWI_STATUS_OK)
    fprintf(stderr,"swi_util_Init Fail\n");
  res = dt_util_Register(&regId);
  if(res!=SWI_STATUS_OK)
    fprintf(stderr,"dt_util_Register Fail\n");

  while ((option = getopt_long( argc, argv, "a:i:u:p:s:x:h::",
                                  long_options,  &option_index)) != -1) {
    switch(option)
        {
     case 'a':
        if((optarg==NULL)||(strlen(optarg)==0))
        {
            PrintUsage();
            break;
        }
        fprintf(stderr,"APN:%s\n",optarg);
        snprintf(szConfigName,127,"%s%s",CONFIG_APN_STRING,CONFIG_NEW_STRING);
        res = swi_util_SetString(szConfigName, optarg);
        if (res != SWI_STATUS_OK)
          return 1;
        if (CheckStringResult(szConfigName,optarg)==false)
        {
            fprintf(stderr,"Set APN Fail\n");
            goto quit;
        }
        snprintf(szConfigName,127,"%s%s",CONFIG_APN_STRING,CONFIG_CHANGE_STRING);
        res = swi_util_SetBool(szConfigName, 1);
        if (res != SWI_STATUS_OK)
            return 1;
        break;
     case 'i':
        if((optarg==NULL)||(strlen(optarg)==0))
        {
            PrintUsage();
            break;
        }
        fprintf(stderr,"Interface:%s\n",optarg);
        snprintf(szConfigName,127,"%s%s",CONFIG_INTF_STRING,CONFIG_NEW_STRING);
        res = swi_util_SetString(szConfigName, optarg);
        if (res != SWI_STATUS_OK)
          return 1;
        if (CheckStringResult(szConfigName,optarg)==false)
        {
            fprintf(stderr,"Set Interface Fail\n");
            goto quit;
        }
        snprintf(szConfigName,127,"%s%s",CONFIG_INTF_STRING,CONFIG_CHANGE_STRING);
        res = swi_util_SetBool(szConfigName, 1);
        if (res != SWI_STATUS_OK)
          return 1;
        break;
     case 'u':
        fprintf(stderr,"Username:%s\n",optarg);
        snprintf(szConfigName,127,"%s%s",CONFIG_USERNAME_STRING,CONFIG_NEW_STRING);
         if(optarg==NULL)
            res = swi_util_SetString(szConfigName, "");
         else
            res = swi_util_SetString(szConfigName, optarg);
        if (res != SWI_STATUS_OK)
        {
            goto quit;
        }
        
        if (CheckStringResult(szConfigName,optarg)==false)
        {
            fprintf(stderr,"Set Username Fail\n");
            goto quit;
        }
        snprintf(szConfigName,127,"%s%s",CONFIG_USERNAME_STRING,CONFIG_CHANGE_STRING);
        res = swi_util_SetBool(szConfigName, 1);
        if (res != SWI_STATUS_OK)
          return 1;
        break;
      case 'p':
        fprintf(stderr,"Password:%s\n",optarg);
        snprintf(szConfigName,127,"%s%s",CONFIG_PASSWORD_STRING,CONFIG_NEW_STRING);
        if(optarg==NULL)
           res = swi_util_SetString(szConfigName, "");
        else
            res = swi_util_SetString(szConfigName, optarg);
        if (res != SWI_STATUS_OK)
        {
            goto quit;
        }
        if (CheckStringResult(szConfigName,optarg)==false)
        {
            fprintf(stderr,"Set Password Fail\n");
            goto quit;
        }
        snprintf(szConfigName,127,"%s%s",CONFIG_PASSWORD_STRING,CONFIG_CHANGE_STRING);
        res = swi_util_SetBool(szConfigName, 1);
        if (res != SWI_STATUS_OK)
          return 1;
        break;
      case 'x':
            fprintf(stderr,"Authentication:%s\n",optarg);
            snprintf(szConfigName,127,"%s%s",CONFIG_AUTH_STRING,CONFIG_NEW_STRING);
            if((atoi(optarg)>=0)&&(atoi(optarg)<3))
            res = swi_util_SetInteger(szConfigName, atoi(optarg));
            if (res != SWI_STATUS_OK)
              return 1;
            if (CheckIntegerResult(szConfigName,atoi(optarg))==false)
            {
                fprintf(stderr,"Set Authentication Fail\n");
                goto quit;
            }
            snprintf(szConfigName,127,"%s%s",CONFIG_AUTH_STRING,CONFIG_CHANGE_STRING);
             res = swi_util_SetBool(szConfigName, 1);
            if (res != SWI_STATUS_OK)
                return 1;
            break;
      case 's':
        fprintf(stderr,"String:%s\n",optarg);
        if(PhaseOption(optarg)==false)
        {
            PrintUsage();
            goto quit;
        }
        break;
     case 'h':
     default:
        PrintUsage();
        break;
        }
  }

quit:
  dt_util_Unregister(regId);
  res = swi_util_Destroy();
  if (res != SWI_STATUS_OK)
    return 1;
  return 0;
  
}
