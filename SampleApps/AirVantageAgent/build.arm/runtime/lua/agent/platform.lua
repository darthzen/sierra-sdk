-------------------------------------------------------------------------------
-- Copyright (c) 2012 Sierra Wireless and others.
-- All rights reserved. This program and the accompanying materials
-- are made available under the terms of the Eclipse Public License v1.0
-- which accompanies this distribution, and is available at
-- http://www.eclipse.org/legal/epl-v10.html
--
-- Contributors:
--     Laurent Barthelemy for Sierra Wireless - initial API and implementation
--     Fabien Fleutot     for Sierra Wireless - initial API and implementation
-------------------------------------------------------------------------------

--- ReadyAgent port on Linux : porting and wrappers functions


local M = { }
local tm = require "agent.treemgr"
function M.getupdateplatformcomponent()
        return {
            ReadyAgent    = _READYAGENTRELEASE,
        }
    end

function M.getdeviceid()
    return tm.get("system.cellular.hw_info.imei")
end

local function setkey(key)
    local log = require "log"
    --check key
    if not key or string.len(key) < 1 or key == "NOTSET" then
        log("GENERAL", "ERROR", "Cannot set empty key!")
        return
    end
    --set key
    local provisioning = require "agent.provisioning"
    provisioning.password(key)
    log("GENERAL", "INFO", "key set")
end

local function push_fw_ver()
    local firmware_version = "system.sw_info.fw_ver"
    local sys_asset = require"agent.devman".asset
    local dt = require"devicetree"
    dt.init()
    local fw_ver = dt.get(firmware_version)
    log("GENERAL", "INFO", "sending firmare revision to portal")
    sys_asset:pushdata("", {[firmware_version]=fw_ver}, "now")
end

--------------------------------------------------------------------------------
-- platform.init must return non false/nil value for success
-- or nil+error message
function M.init()
    local log = require "log"
    local treemgr = require "agent.treemgr"
    local config = require "agent.config"
    local sched = require "sched"

    --security is off: return now
    if not config.server.authentication and not config.server.encryption then
        return "ok"
    end

    --security is on: verify if crypto.key is installed
    local crypto_file = io.open("crypto/crypto.key")
    local key_count = 0
    if crypto_file then
        for keys in crypto_file:lines() do key_count = key_count + 1 end
    end

    --if crypto.key is OK
    if crypto_file and key_count >= 5 then
        log("GENERAL", "INFO", "'crypto.key' and all keys present")
    --if crypto.key is not present or incomplete (re)create crypto.key
    else
        log("GENERAL", "INFO", "generating 'crypto.key'")
        setkey( treemgr.get("system.cellular.sn") )
    end

    --delay by 60s for network registration
    sched.sigrun("updateresult", "*",
        function() sched.wait(60); push_fw_ver() end)

    sched.sigrunonce("ReadyAgent", "InitDone", push_fw_ver)

    return 'ok'
end

return M

