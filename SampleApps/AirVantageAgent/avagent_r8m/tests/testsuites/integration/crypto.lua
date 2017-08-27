local u             = require 'unittest'
local math          = require 'math'
local platformAPI   = require 'tester.platformAPI'
local yajl          = require 'yajl'
local pltconf       = {}
local platformTable = nil
local appprefix     = "MihiniModel_"
local p             = p
local sprint        = sprint


local t = u.newtestsuite 'Crypto Integration'
local targetManager = nil

local function registerdevice(devicename, crypto, registrationPassword)

  -- Fill the structure to create the device:
  -- - Device Name
  -- - Serial Number
  -- - Application to use
  -- - Registration Password to use
  local config = {
   name = devicename,
  
   gateway = {
      serialNumber = devicename
   },
   
   applications = {{
      name =  appprefix .. crypto, -- "hmac-sha1_aes-cbc-128" for instance
      type = "Mihini Software",
      revision = crypto
   }},
   
   communication =   {
      m3da =    {
          registrationPassword = registrationPassword
      }
    }
  }

  -- Create the device with the associated model
  print("Registering device " .. devicename .. " using " .. crypto .. " and its registration password is " .. registrationPassword)
  local operationTable,errmess =  platformAPI.registerSystem(platformTable, config)
  
  -- Check device has been successfuly created
  u.assert_not_nil(operationTable)
  u.assert_not_nil(operationTable.uid)
  u.assert_equal(errmess, "200HTTP/1.1 200 OK")
  print("System ".. operationTable.uid .." registered")
  
  -- Activate Device
  local systemID = { uids = {operationTable.uid} }
  local operationActTable,errmess =  platformAPI.activateSystems(platformTable, systemID)
  u.assert_equal(errmess, "200HTTP/1.1 200 OK")
  
  return operationTable.uid
end

local function createSyncJob(uid)
  u.assert_not_nil(uid)
  
  local syncJob = {
    action = "MIHINI_DM_SYNCHRONIZE",
    systems = {
       uids = { uid }
    }
  }
  
  local operationTable,errmess =  platformAPI.syncSystems(platformTable, syncJob)
  u.assert_equal(errmess, "200HTTP/1.1 200 OK")
  print("Synchronize operation created: " .. operationTable.operation)
  local syncJobID = operationTable.operation
  
  
  u.assert_not_nil(syncJobID)
  return syncJobID
end

local function createResetJob(uid)
  u.assert_not_nil(uid)
  
  local resetJob = {
    action = "MIHINI_DM_RESET",
    systems = {
       uids = { uid }
    }
  }
  
  local operationTable,errmess =  platformAPI.resetSystems(platformTable, resetJob)
  u.assert_equal(errmess, "200HTTP/1.1 200 OK", resetJob)
  print("Reset to factory job created created: "..operationTable.operation)
  
  local syncJobID = operationTable.operation
  
  u.assert_not_nil(syncJobID)
  return syncJobID
end


-- server.url = "tcp://localhost:8070"
-- agent.deviceId = "012345678901234"
-- server.authentication = 'hmac-sha1'
-- server.encryption = 'aes-cbc-128'
local function setremoteconf(url, deviceId, auth, encrypt)
  agent.config.server.url = url
  print("Set url= "..url)
  
  print("Set authentication= "..auth)
  if (auth == "None") then auth = nil end
  agent.config.server.authentication = auth
  
  print("Set encryption= "..encrypt)
  if (encrypt == "None") then encrypt = nil end
  agent.config.server.encryption = encrypt
  
  agent.config.agent.deviceId = deviceId
  print("Set deviceId= "..deviceId)
end

local function modifyDefaultConfigFile(url, deviceId, auth, encrypt, path)
  -- load the file
  local defconf = require "agent.defaultconfig"
  
  defconf.server.url = url
  
  if (auth == "None") then auth = nil end
  defconf.server.authentication = auth
  
  if (encrypt == "None") then encrypt = nil end
  defconf.server.encryption = encrypt
  
  defconf.agent.deviceId = deviceId
  
  local s = "local config = {}\nsetfenv(1, config)\n" .. sprint(defconf) .. "\nreturn config"
  local s = "local config = " .. sprint(defconf) .. "\nreturn config"
  local file,err = io.open(path .. "/runtime/lua/agent/defaultconfig.lua", "w")
  u.assert_not_nil(file, err)
  file:write(s)
  file:flush()
  file:close()
  
  print(s)
end

-- Test the AVMS registration of a new device
-- Scenario is:
-- - Create job on server side
-- - Force device to connect to the server
-- - Wait for job end
-- - check results
local function testRegistration(uid, auth, encrypt, key, name)
  local rpcclient
  local rescon, err
  
  -- Create a synchronize job on AVMS
  local syncJobID = createSyncJob(uid)
  u.assert_not_nil(syncJobID)
  
  ---------------------------------------------------
  -- Create device on client side
  -- write in the defaultconfig file
  
  -- reset configuration
  targetManager:install()    
  targetManager:start()
  sched.wait(2)
  rpcclient = targetManager:getrpc(false)
  
--   local f = rpcclient:newexec(setremoteconf)
--   f(pltconf.avmstcp, name, auth, encrypt)
--   sched.wait(1)
--   targetManager:stop()
--   sched.wait(1)
--   targetManager:start()
--   sched.wait(2)
--   rpcclient = targetManager:getrpc(false)
  
  -- generate registration key from registration password
  local function setPassword(key)
    local prov = require 'agent.provisioning'
    prov.registration_password(key)
  end
  
  local f = rpcclient:newexec(setPassword)
  f(key)
  
  sched.wait(1)
  targetManager:stop()
  sched.wait(1)
  
  ---------------------------------------------------
  -- Test first connection to server (key provisioning) and synchronize job execution 
  targetManager:start()
  sched.wait(2)
  rpcclient = targetManager:getrpc(false)
  
  -- connect to server
  rescon, err = rpcclient:call("agent.srvcon.connect")
  
  -- Wait for end of synchronize job on AVMS side
  local status =  platformAPI.waitJobEnd(platformTable, syncJobID, 2*60)
  
  -- stop target
  targetManager:stop()

  -- Check results
  u.assert_not_nil(rescon, err)
  -- TEMP u.assert_equal("ok", rescon) -- "ok" is expected as the only correct result of "agent.srvcon.connect()"
  u.assert_not_nil(status)
  u.assert_equal("SUCCESS", status)
end


-- Test the remote command "Reset To Factory"
-- Scenario is:
-- - Create job on server side
-- - Force device to connect to the server
-- - Reboot the device
-- - check results
local function testResetToFactory(deviceUID, auth, encrypt, key, name)
  
  local syncJobID, resetJobID
  local rescon, err
  local rpcclient
  
  resetJobID = createResetJob(deviceUID)
  
  targetManager:start()
  sched.wait(2)
  rpcclient   = targetManager:getrpc(false)
  rescon, err = rpcclient:call("agent.srvcon.connect")
  

  -- Reset to Factory is acknowledged after the reboot
  targetManager:stop()
  targetManager:start()
  sched.wait(2)
  rpcclient = targetManager:getrpc(false)
  
  -- Reconfigure device
  targetManager:stop()
  sched.wait(1)
  targetManager:start()
  sched.wait(2)
  rpcclient = targetManager:getrpc(false)
  
  local f = rpcclient:newexec(setremoteconf)
  f(pltconf.avmstcp, name, auth, encrypt)
  sched.wait(1)
  targetManager:stop()
  sched.wait(1)
  targetManager:start()
  sched.wait(2)
  rpcclient = targetManager:getrpc(false)
  
  local status =  platformAPI.waitJobEnd(platformTable, resetJobID, 2*60)
  
  targetManager:stop()
  
  -- Check results
  -- TEMP u.assert_not_nil(rescon, err)
  -- TEMP u.assert_equal("ok", rescon) -- "ok" is expected as the only correct result of "agent.srvcon.connect()"
  u.assert_not_nil(status)
  u.assert_equal("SUCCESS", status)
  
  
  -- Create a synchronize job on AVMS  
  syncJobID = createSyncJob(deviceUID)
 
  targetManager:start()
  sched.wait(2)
  rpcclient   = targetManager:getrpc(false)
  rescon, err = rpcclient:call("agent.srvcon.connect")
  
  status =  platformAPI.waitJobEnd(platformTable, syncJobID, 2*60)
  
  targetManager:stop()
  
  
  -- Check results
  u.assert_not_nil(rescon, err)
  u.assert_equal("ok", rescon) -- "ok" is expected as the only correct result of "agent.srvcon.connect()"
  u.assert_not_nil(status)
  u.assert_equal("SUCCESS", status)
end


-- create device environment for testing
-- parameters:
--    - auth shall be sha1 or md5
--    - encrypt shall be 
local function testCombination(auth, encrypt, key, name)
  local rescon, err
  local rpcclient
  local syncJobID
  
  u.assert_not_nil(auth)
  u.assert_not_nil(encrypt)
  u.assert_not_nil(key)
  u.assert_not_nil(name)
  
  u.assert_not_nil(targetManager)
  
  ---------------------------------------------------
  -- Create device on server side and configure device on embedded side
  local deviceUID = registerdevice(name, auth.."_"..encrypt, key)
  u.assert_not_nil(deviceUID)
  
  local path = targetManager.targetdir
  modifyDefaultConfigFile(pltconf.avmstcp, name, auth, encrypt, path)
  
  ---------------------------------------------------
  -- Test registering a device and first connection to AVMS
  testRegistration(deviceUID, auth, encrypt, key, name)
  
  ---------------------------------------------------
  -- Test reading an existing node of the tree manager
  
  
  ---------------------------------------------------
  -- Test reading an existing leaf of the tree manager
  
  
  ---------------------------------------------------
  -- Test reading an unexisting node of the tree manager
  
  
  
  ---------------------------------------------------
  -- Test reset to factory and communication is still available
  -- (crypto infos were not erased)
  -- Create a reset to factory job and then a synchronize job for this device
  -- TEMP testResetToFactory(deviceUID, auth, encrypt, key, name) 
  
  
end

function init(tm)
  targetManager = tm
end

function generatekey()
  local validchar = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0","1","2","3","4","5","6","7","8","9"}
  -- generate a random key length
  local keylength = math.random(4, 12)
  
  -- generate a random provisioning key key
  local provisioningkey = ""
  for i=1,keylength do 
    local newchar = validchar[math.random(1,#validchar)]
    provisioningkey = provisioningkey..newchar
  end
  
  print("Provisionning password generated: "..provisioningkey)
  
  return provisioningkey
end


function t:setup()
  u.assert_not_nil(targetManager)
  u.assert_not_nil(targetManager.config)
  u.assert_not_nil(targetManager.config.integConfig)
  pltconf = targetManager.config.integConfig
  p(pltconf)
--   {
--   deviceSN     = "CA1222104501002",
--   serverURL    = "http://dev-airlink.m2mop.net/api/",
--   userName     = "administrator@m2mop.net",
--   password     = "zs18mk--",
--   clientId     = "085d82466f824079a829f301b8a1f492",
--   clientSecret = "6854fa72f98e492db163458085d5b89c"}
  
  local platform,errmess = platformAPI.new(pltconf)
  u.assert_not_nil(platform,errmess)
  platformTable = platform
  print("Platform URL is: "..platformTable.serverURL)
  local accessGranted,errmess = platformAPI.getAccessToken(platformTable)
  u.assert_true(accessGranted,errmess)
  
  -- Delete all devices if existing
  platformAPI.delDevice(platformTable, "testautoS1CBC128")
  platformAPI.delDevice(platformTable, "testautoS1CBC256")
  platformAPI.delDevice(platformTable, "testautoS1CTR128")
  platformAPI.delDevice(platformTable, "testautoS1CTR256")
  platformAPI.delDevice(platformTable, "testautoS1None")
  platformAPI.delDevice(platformTable, "testautoMD5CBC128")
  platformAPI.delDevice(platformTable, "testautoMD5CBC256")
  platformAPI.delDevice(platformTable, "testautoMD5CTR128")
  platformAPI.delDevice(platformTable, "testautoMD5CTR256")
  platformAPI.delDevice(platformTable, "testautoMD5None")
  platformAPI.delDevice(platformTable, "testautoNoneNone")
  
  -- backup defaultconfig file
  os.execute("cp "..targetManager.targetdir.."/runtime/lua/agent/defaultconfig.lua "..targetManager.targetdir.."/runtime/lua/agent/defaultconfig.lua.backup")
  
end

function t:teardown()
  os.execute("mv "..targetManager.targetdir.."/runtime/lua/agent/defaultconfig.lua.backup "..targetManager.targetdir.."/runtime/lua/agent/defaultconfig.lua")
end

function t:test_cryptSha1AesCbc128()
  
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-sha1", "aes-cbc-128", provisioningkey, "testautoS1CBC128")
  
    
  
  
  -- 
end

function t:test_cryptSha1AesCbc256()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-sha1", "aes-cbc-256", provisioningkey, "testautoS1CBC256")
end

function t:test_cryptSha1AesCtr128()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-sha1", "aes-ctr-128", provisioningkey, "testautoS1CTR128")
end

function t:test_cryptSha1AesCtr256()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-sha1", "aes-ctr-256", provisioningkey, "testautoS1CTR256")
end

function t:test_cryptSha1None()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-sha1", "None", provisioningkey, "testautoS1None")
end

function t:test_cryptMd5AesCbc128()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-md5", "aes-cbc-128", provisioningkey, "testautoMD5CBC128")
end

function t:test_cryptMd5AesCbc256()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-md5", "aes-cbc-256", provisioningkey, "testautoMD5CBC256")
end

function t:test_cryptMd5AesCtr128()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-md5", "aes-ctr-128", provisioningkey, "testautoMD5CTR128")
end

function t:test_cryptMd5AesCtr256()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-md5", "aes-ctr-256", provisioningkey, "testautoMD5CTR256")
end

function t:test_cryptMd5None()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("hmac-md5", "None", provisioningkey, "testautoMD5None")
end

function t:test_cryptNoneNone()
    
  local provisioningkey = generatekey()
  
  -- initialize the device
  -- Create device on client side and install agent_provisioning
  -- init defaultconfig.lua file
  -- generate and set random provisioning key
  -- start Mihini
  -- Initiate a connection
  -- Check success
  testCombination("None", "None", provisioningkey, "testautoNoneNone")
end

return {init = init}