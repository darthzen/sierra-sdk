-------------------------------------------------------------------------------
-- Copyright (c) 2012 Sierra Wireless and others.
-- All rights reserved. This program and the accompanying materials
-- are made available under the terms of the Eclipse Public License v1.0
-- which accompanies this distribution, and is available at
-- http://www.eclipse.org/legal/epl-v10.html
--
-- Contributors:
--     Romain Perier for Sierra Wireless - initial API and implementation
-------------------------------------------------------------------------------

local airvantage = require 'airvantage'
local u = require 'unittest'
local t = u.newtestsuite("airvantage")
local waiting_notification = 1

local function updateNotificationCb(package, version, url, parameters)
   if package == "my_pkg" and version == "my_version" and url == "/toto/my_file" then
      waiting_notification = 0
   end
end

local function dataWritingCb()
   
end

function t :setup()
   assert(airvantage.init())
end

function t :teardown()
   assert(waiting_notification == 0)
end

function t :test_01_Init()
   -- Initializing the library many times must work
   assert(airvantage.init())
   assert(airvantage.init())
   assert(airvantage.init())
end

function t :test_02_TriggerPolicy()
   -- Trigger default policy
   assert(airvantage.triggerPolicy(nil))
   -- Trigger one existing policy
   assert(airvantage.triggerPolicy("now"))
   -- Trigger "never" policy: must fail
   u.assert_nil(airvantage.triggerPolicy("never"))
   -- Trigger unknown policy: must fail
   u.assert_nil(airvantage.triggerPolicy("plop"))
end

function t: test_03_connectToServer()
   -- Test using sync connection
   assert(airvantage.connectToServer())
   assert(airvantage.connectToServer(nil))
   assert(airvantage.connectToServer(0))

   assert(airvantage.connectToServer(10))
   -- Test using negative latency
   -- expected behavior here: negative value casted into unsigned int
   -- resulting value above INT_MAX, the max value accepted, so it's rejected
   u.assert_nil(airvantage.connectToServer(-5))
   
   -- Test using too big latency:
   -- expected behavior here: rejected
   u.assert_nil(airvantage.connectToServer(2^31 + 1))
end

function t: test_04_Create_Start_Close()
   local asset = airvantage.newAsset("TOTO")
   assert(asset)
   assert(asset:start())
   assert(asset:close())
end

function t: test_05_asset_pushData()
   local asset = airvantage.newAsset("TOTO")
   assert(asset)
   assert(asset:start())

   assert(asset:pushData("titi.test.toto1", 42, "now"))
   assert(asset:pushData("titi.toto2", 43, "now"))
   assert(asset:pushData("toto3", 44, "now"))
   assert(asset:pushData("toto4", 45, "now"))
   assert(asset:pushData("toto5", 46, "now"))
   assert(asset:pushData("toto6", 47, "now"))
   assert(asset:pushData("toto7", 47.455555, "now"))
   assert(asset:pushData("toto8", "foo", "now"))
   assert(asset:pushData("toto8", { nil }, "now"))
   assert(airvantage.triggerPolicy("*"))
   assert(asset:close())
end

function t: test_06_Acknowledge()
   assert(airvantage.acknowledge(0, 0, "BANG BANG BANG", "now", 0))
   assert(airvantage.triggerPolicy("now"))
end

function t: test_07_UpdateNotification()
   local rpc = require 'rpc'
   local asset = airvantage.newAsset("TOTO")
   assert(asset)

   assert(asset:start())
   assert(asset:setUpdateHook(updateNotificationCb))
   rpc.newclient():call('agent.asscon.sendcmd', 'TOTO', 'SoftwareUpdate', { 'TOTO.my_pkg', 'my_version', '/toto/my_file', {foo='bar', num=42, float=0.23}})

   assert(asset:close())
end

function t: test_08_TableManipulation()
   local asset = airvantage.newAsset("TOTO")
   assert(asset)

   assert(asset:start())
   local t = asset:newTable("test", {"column1", "column2", "column3"}, "ram", "now", 0)
   assert(t)

   assert(t:pushRow({ 1234, 1234.1234, "test" }))
   assert(t:pushRow({ 1234, 1234.1234, "test", "test2", "test3" }))
   print("FIXME!!")
   --FIXME: t:pushRow should return an error in case of the pushed data is > the number of column 
   -- or should return a notification explaining the row has been sent automatically
   assert(asset:close())
end
