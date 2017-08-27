/*******************************************************************************
 * Copyright (c) 2012 Sierra Wireless and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Laurent Barthelemy for Sierra Wireless - initial API and implementation
 *******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <limits.h>
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

#define ASSET_ID "TOTO"
#define NUMBER_MAX_ID 10000
#define SIZE_ALLOC 5 /* size allocated for id number */
static volatile char waiting_notification = 1;
static char result = 1;

swi_status_t get_path_element(int first, const char* pathPtr, char** remainingPath, char** varName);

/* add random id number in to str: */
char* addTicketId(const char* str)
{
  sleep(1);  /*delay 1 second for random id number*/
  srand ( time(NULL) );
  int size = strlen(str);
  char *cmd = malloc(sizeof(char)*size + SIZE_ALLOC + 1);
  if(!cmd)
  {
    printf("av_test : Malloc cmd failed \n");
    return NULL;
  }
  unsigned int id = rand() % NUMBER_MAX_ID;  /*generate random id number between 0-NUMBER_MAX_ID*/
  sprintf(cmd, str, id);
  return cmd;
}

static void exec_lua_code(const char* cmd)
{
  lua_State* L;
  const char* str1 =
      "local sched = require 'sched'\n"
      "local rpc = require 'rpc'\n"
      "local os = require 'os'\n"
      "function invoke(...)\n"
      "local client = rpc.newclient()\n"
      "client.call(client, ...)\n"
      "os.exit(0)\n"
      "end\n"
      "sched.run(invoke, 'agent.asscon.sendcmd', 'TOTO', ";
  const char* str3 = "sched.loop()\n";

  /*concatenate str1 + cmd + str2 in other to create lua_script*/
  int size = strlen(str1) + strlen(cmd) + strlen(str3);
  char* lua_script = malloc(sizeof(char)*size+1);
  if(!lua_script)
  {
    printf("av_test : Malloc lua_script failed \n");
    return;
  }
  strcpy(lua_script, str1);
  strcat(lua_script, cmd);
  strcat(lua_script, str3);

  if (fork() != 0)
  {
    //parent process, just return now
    free(lua_script);
    return;
  }
  //child process: execute the lua script
  L = lua_open();
  luaL_openlibs(L);
  int res = luaL_dostring(L, (const char*)lua_script);
  if (res)
    printf("exec_lua_code failed: %d\n", res);

  lua_close(L);
  free(lua_script);
  exit(0);
}

//sendDataCommandMap: not used for now
//"sched.run(invoke, 'agent.asscon.sendcmd', assetID, 'SendData', { Path = 'TOTO.sub.path', Body = { Command = 'plop',  Args = {foo = 'bar'}, __class = 'AWT-DA::Command' },  TicketId = 75, Type = 2,  __class = 'AWT-DA::Message' })\n"


static void updateNotificationCb(swi_av_Asset_t *asset, const char* componentNamePtr, const char* versionPtr,
    const char *updateFilePathPtr, swi_dset_Iterator_t* customParams, void *userDataPtr)
{
  if (strcmp("my_pkg", componentNamePtr) || strcmp("my_version", versionPtr)
      || strcmp(updateFilePathPtr, "/toto/my_file") || strcmp((const char *) userDataPtr, "userData"))
  {
    result = 81;
    waiting_notification = 0;
    return;
  }

  if (!customParams)
  {
    result = 82;
    waiting_notification = 0;
    return;
  }
  swi_status_t res;

  /*check received values*/
  double float_val = 0;
  res = swi_dset_GetFloatByName(customParams, "float", &float_val);
  if (res != SWI_STATUS_OK || float_val != 0.23)
  {
    result = 83;
    waiting_notification = 0;
    return;
  }

  const char* string_val = NULL;
  res = swi_dset_GetStringByName(customParams, "foo", &string_val);
  if (res != SWI_STATUS_OK || strcmp(string_val, "bar"))
  {
    result = 83;
    waiting_notification = 0;
    return;
  }

  int64_t int_val = 0;
  res = swi_dset_GetIntegerByName(customParams, "num", &int_val);
  if (res != SWI_STATUS_OK || int_val != 42)
  {
    result = 83;
    waiting_notification = 0;
    return;
  }

  result = 0;
  waiting_notification = 0;
}

static void dwcb_DataWritting(swi_av_Asset_t *asset, ///< [IN] the asset receiving the data
    const char *pathPtr, ///< [IN] the path targeted by the data sent by the server.
    swi_dset_Iterator_t* data, ///< [IN] the data iterator containing the received data.
                               ///<      The data contained in the iterator will be automatically released when the callback returns.
    int ack_id,                              ///< [IN] the id to be used to acknowledge the received data.
                                             ///<      If ack_id=0 then there is no need to acknowledge.
    void *userDataPtr)
{
  SWI_LOG("AV_TEST", DEBUG, "dwcb_DataWritting: pathPtr=%s, ack_id=%d\n", pathPtr, ack_id);

  if (strcmp(pathPtr, "sub.path"))
  {
    result = 102;
    waiting_notification = 0;
    return;
  }

  if (data == NULL )
  {
    result = 104;
    waiting_notification = 0;
    return;
  }
  char *val1 = NULL;

  if (SWI_STATUS_OK != swi_dset_GetStringByName(data, "foo", (const char**) &val1))
  {
    result = 105;
    waiting_notification = 0;
    return;
  }

  if (val1 == NULL || strcmp(val1, "bar"))
  {
    result = 106;
    waiting_notification = 0;
    return;
  }

  if (ack_id)
    swi_av_Acknowledge(ack_id, 42, "some error msg", "now", 0);

  swi_dset_Destroy(data);
  result = 0;
  waiting_notification = 0;
  return;
}

static void dwcb_DataWrittingList(swi_av_Asset_t *asset, ///< [IN] the asset receiving the data
    const char *pathPtr, ///< [IN] the path targeted by the data sent by the server.
    swi_dset_Iterator_t* data, ///< [IN] the data iterator containing the received data.
                               ///<      The data contained in the iterator will be automatically released when the callback returns.
    int ack_id,                              ///< [IN] the id to be used to acknowledge the received data.
                                             ///<      If ack_id=0 then there is no need to acknowledge.
    void *userDataPtr)
{
  SWI_LOG("AV_TEST", DEBUG, "dwcb_DataWrittingList: pathPtr=%s, ack_id=%d\n", pathPtr, ack_id);

  if (strcmp(pathPtr, "sub.path"))
  {
    result = 112;
    waiting_notification = 0;
    return;
  }

  if (data == NULL )
  {
    result = 114;
    waiting_notification = 0;
    return;
  }

  swi_status_t res = SWI_STATUS_OK;
  bool valueOK = true;
  res = swi_dset_Next(data);
  while (res == SWI_STATUS_OK && valueOK)
  {
    swi_dset_Type_t type = swi_dset_GetType(data);

    switch (type)
    {
      case SWI_DSET_INTEGER:
        if (swi_dset_ToInteger(data) != 42)
          valueOK = false;
        break;

      case SWI_DSET_STRING:
        if (strcmp(swi_dset_ToString(data), "bar"))
          valueOK = false;
        break;

      default:
        break;
    }
    res = swi_dset_Next(data);
  }

  if (!valueOK)
  {
    result = 125;
    waiting_notification = 0;
    return;
  }

  if (ack_id)
    swi_av_Acknowledge(ack_id, 42, "some error msg", "now", 0);

  swi_dset_Destroy(data);
  result = 0;
  waiting_notification = 0;
  return;
}

static void dwcb_DataCommandList(swi_av_Asset_t *asset, ///< [IN] the asset receiving the data
    const char *pathPtr, ///< [IN] the path targeted by the data sent by the server.
    swi_dset_Iterator_t* data, ///< [IN] the data iterator containing the received data.
                               ///<      The data contained in the iterator will be automatically released when the callback returns.
    int ack_id,                              ///< [IN] the id to be used to acknowledge the received data.
                                             ///<      If ack_id=0 then there is no need to acknowledge.
    void *userDataPtr)
{
  SWI_LOG("AV_TEST", DEBUG, "dwcb_DataCommandList: pathPtr=%s, ack_id=%d\n", pathPtr, ack_id);

  if (strcmp(pathPtr, "commands.plop.sub.path"))
  {
    result = 122;
    waiting_notification = 0;
    return;
  }

  if (data == NULL )
  {
    result = 124;
    waiting_notification = 0;
    return;
  }

  swi_status_t res = SWI_STATUS_OK;
  bool valueOK = true;
  res = swi_dset_Next(data);
  while (res == SWI_STATUS_OK && valueOK)
  {
    swi_dset_Type_t type = swi_dset_GetType(data);

    switch (type)
    {
      case SWI_DSET_INTEGER:
        if (swi_dset_ToInteger(data) != 42)
          valueOK = false;
        break;
      case SWI_DSET_STRING:
        if (strcmp(swi_dset_ToString(data), "bar"))
          valueOK = false;
        break;
      default:
        break;
    }
    res = swi_dset_Next(data);
  }

  if (!valueOK)
  {
    result = 125;
    waiting_notification = 0;
    return;
  }

  if (ack_id)
    swi_av_Acknowledge(ack_id, 42, "some error msg", "now", 0);

  swi_dset_Destroy(data);
  result = 0;
  waiting_notification = 0;
  return;
}

static int test_1_Init_Destroy()
{
  swi_status_t res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
    return res;

  return 0;
}

static int test_2_TriggerPolicy()
{
  swi_status_t res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

//trigger default policy
  res = swi_av_TriggerPolicy(NULL );
  if (res != SWI_STATUS_OK)
    return res;

//trigger one existing policy
  res = swi_av_TriggerPolicy("now");
  if (res != SWI_STATUS_OK)
    return res;

//trigger "never" policy: this must fail
  res = swi_av_TriggerPolicy("never");
  if (res == SWI_STATUS_OK)
    return res;

//test using unknown policy
  res = swi_av_TriggerPolicy("plop");
  if (res == SWI_STATUS_OK)
    return res;

  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
    return res;
  return 0;
}

static int test_3_ConnectToServer()
{
  swi_status_t res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

//test using requesting SYNC connexion
  res = swi_av_ConnectToServer(SWI_AV_CX_SYNC);
  if (res != SWI_STATUS_OK)
    return res;

  SWI_LOG("AV_TEST", DEBUG, "sync done\n");

//test using 0 latency: async but "immediate" connection
  res = swi_av_ConnectToServer(0);
  if (res != SWI_STATUS_OK)
    return res;

//test using correct latency
  res = swi_av_ConnectToServer(10);
  if (res != SWI_STATUS_OK)
    return res;

//test using too big latency:
//expected behavior here: rejected
  res = swi_av_ConnectToServer((unsigned int) INT_MAX + 1);
  if (res != SWI_STATUS_WRONG_PARAMS)
    return res;

  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
    return res;

  return 0;
}

static int test_4_asset_Create_Start_Destroy()
{
  swi_status_t res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  swi_av_Asset_t* asset;
  /*
   res = swi_av_asset_Create(&asset, NULL);
   if (res != SWI_STATUS_WRONG_PARAMS)
   return 1;

   res = swi_av_asset_Create(NULL, "test_asset");
   if (res != SWI_STATUS_WRONG_PARAMS)
   return 1;
   */
  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;
  res = swi_av_asset_Start(NULL );
  if (res != SWI_STATUS_CONTEXT_IS_CORRUPTED)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Destroy(NULL );
  if (res != SWI_STATUS_CONTEXT_IS_CORRUPTED)
    return res;

  res = swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
    return res;

  return 0;
}

static int test_5_asset_pushData()
{

  swi_status_t res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return 1;

  swi_av_Asset_t* asset;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

//"long" path
  res = swi_av_asset_PushInteger(asset, "titi.test.toto1", "now", SWI_AV_TSTAMP_AUTO, 42);
  if (res != SWI_STATUS_OK)
    return res;
//"short" path
  res = swi_av_asset_PushInteger(asset, "titi.toto2", "now", SWI_AV_TSTAMP_AUTO, 43);
  if (res != SWI_STATUS_OK)
    return res;
//"shortest" path
  res = swi_av_asset_PushInteger(asset, "toto3", "now", SWI_AV_TSTAMP_AUTO, 44);
  if (res != SWI_STATUS_OK)
    return res;
//"shortest" path, no timestamp
  res = swi_av_asset_PushInteger(asset, "toto4", "now", SWI_AV_TSTAMP_NO, 45);
  if (res != SWI_STATUS_OK)
    return res;
//"shortest" path, no timestamp, no policy
  res = swi_av_asset_PushInteger(asset, "toto5", NULL, SWI_AV_TSTAMP_AUTO, 46);
  if (res != SWI_STATUS_OK)
    return res;
//"shortest" path, manual timestamp, no policy
  res = swi_av_asset_PushInteger(asset, "toto6", NULL, 23, 47);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_PushFloat(asset, "toto7", "now", SWI_AV_TSTAMP_AUTO, 47.455555);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_PushString(asset, "toto8", "now", SWI_AV_TSTAMP_AUTO, "foo");
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_PushString(asset, "toto8", "now", SWI_AV_TSTAMP_AUTO, NULL );
  if (res != SWI_STATUS_WRONG_PARAMS)
    return res;

  res = swi_av_TriggerPolicy("*");
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
    return res;

  return 0;
}

static int test_6_Acknowledge()
{
  swi_status_t res;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_Acknowledge(0, 0, "BANG BANG BANG", "now", 0);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_TriggerPolicy("now");
  if (res != SWI_STATUS_OK)
    return res;
  return 0;
}

static int test_7_path_utils()
{

  swi_status_t res = SWI_STATUS_OK;

  char* var = NULL;
  char *remain = NULL;
  const char* path = "toto.titi.tata";
  res = get_path_element(0, path, &remain, &var);
  SWI_LOG("AV_TEST", DEBUG, "last: var=[%s], remain=[%s]\n", var, remain);
  if (res != SWI_STATUS_OK)
    return res;
  if(var == NULL)
  {
      if(remain!=NULL)
          free(remain);
      return res;
  }
  if (strcmp("tata", var))
    return res;

  if(remain==NULL)
  {
      free(var);
      return res;
  }
  if (strcmp("toto.titi", remain))
    return res;

  free(var);
  free(remain);

  res = get_path_element(1, path, &remain, &var);
  SWI_LOG("AV_TEST", DEBUG, "first: var=[%s], remain=[%s]\n", var, remain);
  if (res != SWI_STATUS_OK)
    return res;
  if (strcmp("titi.tata", remain))
    return res;
  if (strcmp("toto", var))
    return res;

  free(var);
  free(remain);

  path = "foobarfoobar";

  res = get_path_element(1, path, &remain, &var);
  SWI_LOG("AV_TEST", DEBUG, "first: var=[%s], remain=[%s]\n", var, remain);
  if (res != SWI_STATUS_OK)
    return res;
  if (remain == NULL)
  {
    if (var!=NULL)
        free(var);
    return res;
  }
  if (strcmp("", remain))
    return res;
  if (strcmp("foobarfoobar", var))
    return res;

  free(var);
  free(remain);

  res = get_path_element(0, path, &remain, &var);
  SWI_LOG("AV_TEST", DEBUG, "last: var=[%s], remain=[%s]\n", var, remain);
  if (res != SWI_STATUS_OK)
    return res;

  if (remain == NULL)
  {
      if (var!=NULL)
          free(var);
      return res;
  }
  if (strcmp("", remain))
    return res;

  if(var == NULL)
  {
      free(remain);
      return res;
  }
  if (strcmp("foobarfoobar", var))
    return res;

  free(var);
  free(remain);

//todo test cas with bad content in path: .toto, toto., ttu..titi, etc

  return 0;
}

static int test_8_UpdateNotification()
{
  swi_status_t res;
  swi_av_Asset_t* asset = NULL;
  waiting_notification = 1;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;
  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;
  res = swi_av_RegisterUpdateNotification(asset, (swi_av_updateNotificationCB) updateNotificationCb, "userData");
  if (res != SWI_STATUS_OK)
    return res;

  const char *cmd_SoftwareUpdate ="'SoftwareUpdate', { 'TOTO.my_pkg', 'my_version', '/toto/my_file', {foo='bar', num=42, float=0.23}})\n";
  exec_lua_code(cmd_SoftwareUpdate);
  SWI_LOG("AV_TEST", DEBUG, "exec_lua_code SoftwareUpdate done\n");
  while (waiting_notification)
    ;

  swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
    return res;

  return result;
}

static int test_9_TableManipulation()
{
  swi_status_t res;
  swi_av_Asset_t* asset = NULL;
  swi_av_Table_t *table = NULL;
  const char *columns[] = { "column1", "column2", "column3" };

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_Create(asset, &table, "test", 3, columns, "now", STORAGE_RAM, 0);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_PushInteger(table, 1234);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_PushFloat(table, 1234.1234);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_PushString(table, "test");
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_PushString(table, "fake push");
  if (res != SWI_STATUS_VALUE_OUT_OF_BOUND)
    return res;

  res = swi_av_table_PushRow(table);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_table_Destroy(table);
  if (res != SWI_STATUS_OK)
    return res;
  swi_av_asset_Destroy(asset);
  return 0;
}

static int test_10_asset_receiveDataWriting()
{
  swi_status_t res = SWI_STATUS_OK;
  waiting_notification = 1;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  swi_av_Asset_t* asset;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_RegisterDataWrite(asset, dwcb_DataWritting, NULL );
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

  /*command sends to TOTO asset*/
  const char* str = "'SendData', { Path = 'TOTO.sub.path', Body = { foo = 'bar' }, TicketId = %u, Type = 5, __class = 'AWT-DA::Message' })\n";
  char* cmd_SendDataWriting = addTicketId(str);

  exec_lua_code(cmd_SendDataWriting);
  SWI_LOG("AV_TEST", DEBUG, "exec_lua_code SendDataWriting done\n");
  while (waiting_notification)
    ;

  res = swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataWriting);
      return res;
  }
  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataWriting);
      return res;
  }
  free(cmd_SendDataWriting);
  return result;
}

static int test_11_asset_receiveDataWritingList()
{
  swi_status_t res = SWI_STATUS_OK;
  waiting_notification = 1;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  swi_av_Asset_t* asset;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_RegisterDataWrite(asset, dwcb_DataWrittingList, NULL );
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

  const char* str = "'SendData', { Path = 'TOTO.sub.path', Body = { 42, 'bar' }, TicketId = %u, Type = 5, __class = 'AWT-DA::Message' })\n";
  char* cmd_SendDataWrittingList = addTicketId(str);

  exec_lua_code(cmd_SendDataWrittingList);
  SWI_LOG("AV_TEST", DEBUG, "exec_lua_code SendDataWrittingList done\n");
  while (waiting_notification)
    ;

  res = swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataWrittingList);
      return res;
  }
  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataWrittingList);
      return res;
  }
  free(cmd_SendDataWrittingList);
  return result;
}

static int test_12_asset_receiveDataCommandList()
{
  swi_status_t res = SWI_STATUS_OK;
  waiting_notification = 1;

  res = swi_av_Init();
  if (res != SWI_STATUS_OK)
    return res;

  swi_av_Asset_t* asset;

  res = swi_av_asset_Create(&asset, ASSET_ID);
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_RegisterDataWrite(asset, dwcb_DataCommandList, NULL );
  if (res != SWI_STATUS_OK)
    return res;

  res = swi_av_asset_Start(asset);
  if (res != SWI_STATUS_OK)
    return res;

  const char* str = "'SendData', { Path = 'TOTO.sub.path', Body = { Command = 'plop',  Args = {42, 'bar'}, __class = 'AWT-DA::Command' },  TicketId = %u, Type = 2,  __class = 'AWT-DA::Message' })\n";
  char* cmd_SendDataCommandList = addTicketId(str);

  exec_lua_code(cmd_SendDataCommandList);
  SWI_LOG("AV_TEST", DEBUG, "exec_lua_code SendDataCommandList done\n");
  while (waiting_notification)
    ;

  res = swi_av_asset_Destroy(asset);
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataCommandList);
      return res;
  }
  res = swi_av_Destroy();
  if (res != SWI_STATUS_OK)
  {
      free(cmd_SendDataCommandList);
      return res;
  }

  free(cmd_SendDataCommandList);
  return result;
}

int main(int argc, char *argv[])
{
  INIT_TEST("AV_TEST");
//  swi_log_setlevel(DEBUG, "AV", NULL );

  CHECK_TEST(test_1_Init_Destroy());
  CHECK_TEST(test_2_TriggerPolicy());
  CHECK_TEST(test_3_ConnectToServer());
  CHECK_TEST(test_4_asset_Create_Start_Destroy());
  CHECK_TEST(test_5_asset_pushData());
  CHECK_TEST(test_6_Acknowledge());
  CHECK_TEST(test_7_path_utils());
  CHECK_TEST(test_8_UpdateNotification());
  CHECK_TEST(test_9_TableManipulation());
  CHECK_TEST(test_10_asset_receiveDataWriting());
  CHECK_TEST(test_11_asset_receiveDataWritingList());
  CHECK_TEST(test_12_asset_receiveDataCommandList());

//preventive clean if one previous test failed
//  swi_av_Destroy();

  return 0;
}
