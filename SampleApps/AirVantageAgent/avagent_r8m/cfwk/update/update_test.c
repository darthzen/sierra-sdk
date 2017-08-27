/*******************************************************************************
 * Copyright (c) 2012 Sierra Wireless and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Laurent Barthelemy for Sierra Wireless - initial API and implementation
 *     Romain Perier      for Sierra Wireless - initial API and implementation
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include "swi_update.h"
#include "swi_log.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "testutils.h"

static const char *lua_script =
"local sched = require 'sched'\n"
"local rpc = require 'rpc'\n"
"local os = require 'os'\n"
"function invoke(...)\n"
" local client = rpc.newclient()\n"
" client.call(client, ...)\n"
" os.exit(0)\n"
"end\n"
"sched.run(invoke, 'agent.update.localupdate', '/tmp/update_package.tar.gz', false)\n"
"sched.loop()\n";


static const char *manifest_content =
"{\n"
"  version = \"1.1\",\n"
"\n"
"  components =\n"
"  {\n"
"    {   name = \"@sys.appcon.my_app\",\n"
"        location = \"update.txt\",\n"
"        depends = {},\n"
"        provides = { my_app=\"1.1\"},\n"
"        version = \"1.1\"\n"
"    }\n"
"  }\n"
"}\n";


static volatile uint8_t waiting_update_notification = 1;


static swi_status_t statusNotification(swi_update_Notification_t* indPtr)
{

  SWI_LOG("UPDATE_TEST", INFO, "statusNotification: %p\n", indPtr->eventDetails);
  const char* details = indPtr->eventDetails != NULL ? indPtr->eventDetails : "";
  switch (indPtr->event)
  {
    case SWI_UPDATE_NEW:
      SWI_LOG("UPDATE_TEST", INFO, "new update\n");
      break;
      case SWI_UPDATE_DOWNLOAD_IN_PROGRESS:
      SWI_LOG("UPDATE_TEST", INFO, "download in progress: [%s]\n", details);
      break;
    case SWI_UPDATE_DOWNLOAD_OK:
      SWI_LOG("UPDATE_TEST", INFO, "download ok\n");
      break;
    case SWI_UPDATE_CRC_OK:
      SWI_LOG("UPDATE_TEST", INFO, "crc ok\n");

      //      swi_update_Request_t request = (count++)%2;
      //SWI_LOG("UPDATE_TEST", INFO,  "sending req:%d\n", request);
      //res = swi_update_Request(request);
      //SWI_LOG("UPDATE_TEST", INFO, "swi_update_Request: %d\n", res)
      break;
    case SWI_UPDATE_UPDATE_IN_PROGRESS:
      SWI_LOG("UPDATE_TEST", INFO, "update in progress for [%s]\n", details);
      break;
    case SWI_UPDATE_FAILED:
      //update finished, end test
      waiting_update_notification = 0;
      SWI_LOG("UPDATE_TEST", ERROR, "update failed: [%s]\n", details);
      break;
    case SWI_UPDATE_SUCCESSFUL:
      //update finished, end test
      waiting_update_notification = 0;
      SWI_LOG("UPDATE_TEST", INFO, "update successful: [%s]\n", details);
      break;
    case SWI_UPDATE_PAUSED:
      SWI_LOG("UPDATE_TEST", INFO, "update paused\n");
      break;
    default:
      //update event "error", end test
      waiting_update_notification = 0;
      SWI_LOG("UPDATE_TEST", WARNING, "unknown event: [%d]\n", indPtr->event);
      break;
  }

//  swi_status_t res = swi_update_Request(SWI_UPDATE_REQ_RESUME);
//  SWI_LOG("UPDATE_TEST", INFO, "swi_update_Request: %d\n", res)

  return SWI_STATUS_OK;
}

static int test_update_Init()
{
  swi_status_t res;

  res = swi_update_Init();
  if (res != SWI_STATUS_OK)
    return 1;

  res = swi_update_Init();
  if (res != SWI_STATUS_OK)
    return 1;

  res = swi_update_Init();
  if (res != SWI_STATUS_OK)
    return 1;
  return 0;
}

static int test_update_Destroy()
{
  swi_status_t res;

  res = swi_update_Destroy();
  if (res != SWI_STATUS_OK)
    return 1;

  res = swi_update_Destroy();
  if (res != SWI_STATUS_OK)
    return 1;
  return 0;
}

// swi_update_Request must be used when an update is in progress, so
// its test is likely to be put in  statusNotification callback test/use.

//static int test_update_Request()
//{
//  swi_status_t res;
//
//  res = swi_update_Request(SWI_UPDATE_REQ_PAUSE);
//  if (res != SWI_STATUS_OK)
//    return 1;
//  return 0;
//}

static int test_update_RegisterStatusNotification()
{
  swi_status_t res;

  res = swi_update_RegisterStatusNotification(NULL);
  if (res != SWI_STATUS_OK)
    return 1;

  res = swi_update_RegisterStatusNotification(statusNotification);
  if (res != SWI_STATUS_OK)
    return 1;

  return 0;
}

static void exec_lua_code()
{
  lua_State* L;

  if (fork() != 0)
    return;
  L = lua_open();
  luaL_openlibs(L);
  (void)luaL_dostring(L, lua_script);
  lua_close(L);
}

static void generate_package()
{
  FILE *file;

  file = fopen("/tmp/Manifest", "w");
  fwrite(manifest_content, 1, strlen(manifest_content), file);
  fclose(file);

  file = fopen("/tmp/update.txt", "w");
  fwrite("test update", 1, strlen("test update"), file);
  fclose(file);

  int ret = system("cd /tmp; tar czpf update_package.tar.gz Manifest update.txt 2>/dev/null");
  if (ret != -1) {
    SWI_LOG("UPDATE_TEST", ERROR, "%s: unpacking package failed\n", __FUNCTION__);
    abort();
  }
}

int main(void)
{
  INIT_TEST("UPDATE_TEST");

  CHECK_TEST(test_update_Init());
  //CHECK_TEST(test_update_Request());
  CHECK_TEST(test_update_RegisterStatusNotification());

  //waiting_update_notification is set to 0 at the end of an update and on update event error
  generate_package();
  exec_lua_code();
  while(waiting_update_notification)
    usleep(1000 * 100);
  CHECK_TEST(test_update_Destroy());

  return 0;
}
