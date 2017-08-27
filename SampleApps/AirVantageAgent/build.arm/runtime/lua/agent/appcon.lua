-------------------------------------------------------------------------------
-- Copyright (c) 2012 Sierra Wireless and others.
-- All rights reserved. This program and the accompanying materials
-- are made available under the terms of the Eclipse Public License v1.0
-- which accompanies this distribution, and is available at
-- http://www.eclipse.org/legal/epl-v10.html
--
-- Contributors:
--     Laurent Barthelemy for Sierra Wireless - initial API and implementation
-------------------------------------------------------------------------------

local sched = require"sched"
local persist = require"persist"
local os = require "os"
local config = require"agent.config"
local lfs = require"lfs"
local socket  = require"socket"
local log = require"log"
local pairs = pairs
local tonumber = tonumber
local string = string

local apps_path = (LUA_AF_RW_PATH or lfs.currentdir().."/").."apps/"
local M = {}

--[[
Application list:
list[id] = { id ="app_id", autostart=true/false}
--]]

local application_list = {}

local function loadAppList()
    application_list = persist.load("ApplicationList") or {}
end

-- an application/path is runnable if file named "run" is in installation folder.
local function is_path_runnable(path)
    local res = lfs.attributes(path.."/run", "mode")
    return res=="file"
end

local function is_app_runnable(id)
    return is_path_runnable(apps_path..id)
end

-- check whether the application's directory still exists
local function does_app_path_exist(id)
    local res = lfs.attributes(apps_path..id, "mode")
    return res=='directory'
end

--helper function to send command to the appmon_daemon through socket.
local function sndcmd(str)
    if not str then return nil , "internal error: bad request" end
    str = str.."\n"
    local skt = socket.tcp()
    skt:settimeout(20)
    local port = config.appcon.port and tonumber(config.appcon.port) or 4242
    local host = "localhost"
    local res, err = skt:connect(host, port)
    if not res then
        skt :close()
        local fmt = "Cannot connect to appmon daemon on localhost [%s] and port [%s] , error=[%s]"
        local msg = string.format(fmt, tostring(host), tostring(port), err or "unknown")
        log('APPCON', 'ERROR', msg)
        return nil, msg
    end
    res,err = skt:send(str)
    if res~= #str then
        skt :close();
        local fmt = "Cannot send command to appmon daemon, err=%q"
        local msg = string.format(fmt, err or "unknown")
        log('APPCON', 'ERROR', msg)
        return nil, msg
    end
    --each command got a response, response separator is "\n", so receive("*l")
    res, err = skt:receive("*l")
    skt:close()
    log('APPCON', 'DEBUG', "Query %q -> Response %q", str, tostring(res))
    return res, err
end

-- escape lua magic character to analyse appmon_daemon status command result
local function escapemagicchars(str)
    --put % at the first place!
    local magicchars={"%", "(",")",".","+","-","*", "?", "[", "]", "^", "$"}
    for k,v in pairs(magicchars) do
        str = str:gsub("%"..v,function() return "%"..v end)
    end
    return str
end

--setup the application in appmon_daemon (app is not automatically started afterwards).
local function setupapp(id)
    log("APPCON", "DEBUG", "Setup app id=%s", id or "nil")
    local appsdir = apps_path..id
    return sndcmd("setup "..appsdir.." "..appsdir.."/run")
end

--- Start an application if it is runnable.
-- If the application is not runnable, or the application is already started, it has no effect.
-- @param id the string used as unique id when application was installed
-- @return "ok" string in case of success, nil+error message otherwise
local function start(id)
    local app =  application_list[id]
    if not app then return nil, "Unknown app"
    elseif not is_app_runnable(id) then
        log('APPCON', 'ERROR', "attempt to start non-runnable app %q", id)
        return nil, "Not runnable"
    elseif not app.app_id_daemon then
        local msg = string.format("Runnable app %q not setup in daemon", id)
        log('APPCON', 'ERROR', msg)
        return nil, msg
    end
    log("APPCON", "DEBUG", "Starting application %s", id)
    local res, err = sndcmd("start "..app.app_id_daemon)
    if not res then
        log("APPCON", "ERROR", "Cannot start application %q: %s", id, err or "unknown error")
    end
    return res, err
end

--- Stop an application if it is runnable.
-- If the application is not runnable, or the application is already stopped, it has no effect.
-- @param id the string used as unique id when application was installed
-- @return "ok" string in case of success, nil+error message otherwise
local function stop(id)
    local app =  application_list[id]
    if not app then return nil, "Unknown app"
    elseif not is_app_runnable(id) then
        log('APPCON', 'ERROR', "attempt to stop non-runnable app %q", id)
        return nil, "Not runnable"
    elseif not app.app_id_daemon then
        local msg = string.format("Runnable app %q not setup in daemon", id)
        log('APPCON', 'ERROR', msg)
        return nil, msg
    else
        return sndcmd("stop "..app.app_id_daemon)
    end
end

-- init app does two things:
-- 1: if app is already setup in appmon_daemon, it retrieves the app id in appmon_daemon to act on it later,
--     otherwise it setup the app in appmon_daemon
--    (This is needed to deal with the case where RA reboots but appmon_daemon have not)
-- 2: starts
local function initapp(app)
    log('APPCON', 'DEBUG', "Initializing application %q", app.id)
    local listres, err = sndcmd("list")
    if not listres then
        return nil, "Appmon Daemon list command failed, err="..(err or "unknown")
    end
    local appsfile = escapemagicchars(apps_path..app.id.."/run")
    local curr_id=listres:match(".*appname=%[(%d*)%] privileged=%[%d*%] prog=%["..appsfile.."%].*");
    if not curr_id then
        local res, err = setupapp(app.id)
        if not tonumber(res) then
            return nil, "Appmon Daemon setup command failed, cannot get app id, err="..(err or "unknown")
        else
            curr_id=res
        end
    end
    app.app_id_daemon=curr_id
    if app.autostart then
        log('APPCON', 'DEBUG', "Auto-start application %q", app.id)
        return start(app.id)
    else return "ok" end
end

-- Copies recursively src folder content to dst folder.
-- if a file or folder in src already exists (either as a folder or as a file) in dst, then
-- it is removed (rm -rf) from dst folder before copying it from src.
--@param src path as a string to src folder
--@param dst path as a string to dst folder
--@return "ok" on success, (nil,error) otherwise
--Note: this functions is recursive (i.e. one internal function is called recursively)
--Huge copy (with very much imbricated folders) could lead to some issues...
local function recursive_copy_overwrite(src, dst)
    local function iterate_files_to_copy(src, dst)
        local mode,err
        for file in lfs.dir(src) do
            if file~="." and file ~= ".." then
                local srcpath = src.."/"..file
                local dstpath = dst.."/"..file --may not exist
                mode, err = lfs.attributes(srcpath, "mode")
                if not mode then return nil, string.format("cannot use file %s in source folder, err=%s", srcpath, tostring(err)) end
                if lfs.attributes(dstpath, "mode") then
                    local res, err = os.execute("rm -rf "..dstpath)
                    if res~= 0 then return nil, "cannot remove existing files prior to copying new file with same path" end
                    if mode == "directory" then iterate_files_to_copy(srcpath, dstpath)
                    elseif mode ~= "file" and mode ~= "link" then return nil, string.format("cannot use file %s in source folder, file type %s not supported", srcpath, mode) end
                end
            end
        end
        return "ok"
    end

    local res, err = iterate_files_to_copy(src, dst)
    if not res then return nil, err or "cannot iterate over files to copy" end
    --copy files
    res, err = os.execute("cp -p -r "..src.."/* "..dst)
    if res ~= 0 then return nil, err or "cannot copy install files"
    else return "ok" end
end

--- Install an application in appcon.
-- The application can be runnable or not, it is decided dynamically checking for run file.
-- @param id string to use a unique id for the app.
-- Note that if the application id is already used, the application will be overwritten/reinstalled
-- by the last call to install.
-- @param data_path string representing absolute path to application folder
-- @param autostart boolean value to set autostart mode of the application
-- @param purge boolean value to purge application runtime folder before reinstallation
-- @return "ok" string in case of success, nil+error message otherwise
local function install(id, data_path, autostart, purge)
    log('APPCON', 'INFO', "Installing application %q", id)
    local res,err
    local pattern = '^[%a%d_%-]+$'
    --checks
    if not data_path or "directory"~= lfs.attributes(data_path, "mode") then return nil, "Invalid data" end
    --check id chars
    if not string.match(id, pattern) then return nil, "Invalid id name of application" end

    res = os.execute("chmod -R 775 "..data_path.."/*")
    if res~= 0 then log("APPCON", "WARNING", "Can't set file permission on app data: %s", tostring(res or "unknown error")) end

    if application_list[id] then
        log("APPCON", "WARNING", "Install: Application_id[%s] already used, the application will be overwritten", id)
        local app = application_list[id]
        if app.app_id_daemon then --clean the "old version" of the app in the daemon
            log('APPCON', 'DEBUG', "Re-installing runnable application %q", id)
            stop(id)--do not complain if stop is already etc, remove cmd
            res, err = sndcmd("remove "..app.app_id_daemon)
            if not res or not res:match("^ok") then
                log('APPCON', 'ERROR', "Re-installing application %q: failed to remove it from appmon_daemon, res:%s, error:%s", id, tostring(res), tostring(err))
            end
            app.app_id_daemon=nil
        end
        if purge then --clean the app folder if required
            log("APPCON", "INFO", "Install: purging folder for Application_id[%s] before reinstall", id)
            res = os.execute("rm -rf "..apps_path..id.."/*")
            if res~= 0 then return nil, "Error while purging app runtime dir for reinstall" end
        end
    else
        --no app registered, preventive clean
        os.execute("rm -rf "..apps_path..id)
        res = os.execute("mkdir "..apps_path..id)
        if res~= 0 then return nil, "Error while creating app runtime dir" end
    end
    res, err = recursive_copy_overwrite(data_path, apps_path..id)
    if not res then return nil, err end

    res = os.execute("chmod 775 "..apps_path..id)
    if res~= 0 then log("APPCON", "WARNING", "Can't set file permission on app folder: %s", tostring(res or "unknown error")) end

    --fill app list
    local app = { id=id, autostart = autostart and true or false }
    application_list[id] = app
    if is_path_runnable(apps_path..id) then
        log("APPCON", "DEBUG", "Installing runnable app")
        res, err = setupapp(id)
        if not res then return nil, "Cannot setup application in Application Monitoring Daemon: "..(err or "unknown error") end
        app.app_id_daemon=res
        --start app depending on autostart
        if autostart then
            log("APPCON", "DEBUG", "Auto-starting app")
            start(id)
        end
    end
    persist.save("ApplicationList", application_list);
    return "ok"
end


--- Return the status of an application.
-- if the application is runnable, then it returns the status sent by status command of appmon_daemon (see appmon_daemon doc)
-- else only returns "Not runnable"
-- @param id the string used as unique id when application was installed
-- @return status string in case of success, nil+error message otherwise
local function status(id)
    local app = application_list[id]
    if not app then return nil, "Unknown app"
    elseif not is_app_runnable(id) then
        return "Not runnable"
    elseif not app.app_id_daemon then
        return "Runnable app not setup in daemon."
    else
        local res,err = sndcmd("status "..app.app_id_daemon, true)
        if res then
            res = res :match "appname=%[%d-%] privileged=%[%d-%] (.*)"
            return res or nil,  not res and "error while getting application info from appmon_daemon: status command parsing error" or nil
        else return nil, err or "error while getting application info from appmon_daemon: status command failed" end
    end
end

--- Uninstall an application.
-- First: stop the application if running, then remove permanently the whole folder where the application was installed.
-- @param id the string used as unique id when application was installed
-- @return "ok" string in case of success, nil+error message otherwise
local function uninstall(id)
    log('APPCON', 'INFO', "Uninstalling application %q", id)
    local res,err
    local app = application_list[id]
    if not app then return nil, "Unknown app" end
    --stop app and remove it from appmon_daemon
    if app.app_id_daemon then
        stop(app)
        res, err = sndcmd("remove "..app.app_id_daemon)
        if not res or not res:match("^ok") then
            log('APPCON', 'ERROR', "Uninstalling application %q: failed to remove it from appmon_daemon, res:%s, error:%s", id, tostring(res), tostring(err))
            err=string.format("failed to remove application removed from appmon_daemon: %s %s", tostring(res), tostring(err))
            res=nil
        end
    end
    local appdir=apps_path..app.id
    --remove files on filesystem
    if lfs.attributes(appdir) then
        res, err = os.execute("rm -rf "..appdir)
        if res~=0  then
            log('APPCON', 'INFO', "Uninstalling application %q: failed to remove application install folder: ", id, appdir)
            err=string.format("failed to remove application install folder: %s %s", tostring(res), tostring(err))
            res=nil
        end
    end

    --remove app from internal state
    application_list[id] = nil
    persist.save("ApplicationList", application_list);
    return res and "ok" or nil, err
end

--- Configure an application.
-- Set autostart parameter of an runnable application.
-- if the application is not runnable, or the application is already stopped, it has no effect.
-- @param id the string used as unique id when application was installed
-- @param autostart boolean value to set autostart mode of the application
-- @return "ok" string in case of success, nil+error message otherwise
local function configure(id, autostart)
    local app = application_list[id]
    if not app then return nil, "Unknown app" end
    if not is_app_runnable(id) then
        return nil, "Not Runnable"
    else
        app.autostart = autostart
        persist.save("ApplicationList", application_list);
        return "ok"
    end
end

--- get application info from daemon.
--used in treemgr handler to expose appcon data.
--@param id the string used as unique id when application was installed
--@return table containing application attributes when operation is successful,
--nil+error message otherwise
local function attributes(id)
    local app = application_list[id]
    if not app then return nil, "Unknown app" end
    local result = {
        autostart = app.autostart;
        runnable = is_app_runnable(id) }
    if app.app_id_daemon then
        local status_string, err = sndcmd("status "..app.app_id_daemon, true)
        if status_string then
            for k, v in status_string :gmatch "(%w+)=%[(.-)%]" do
                result[k] = tonumber(v) or v
            end
        end
    end
    return result
end


--- Get installed applications list.
-- @return table listing the installed application, nil+error message otherwise
local function list()
    local res = {}
    for k,v in pairs(application_list) do
        res[k]={autostart=v.autostart, runnable=is_app_runnable(k) }
    end
    return res
end

--- Init function to be called by ReadyAgent initializer module.
-- Loads the installed application list, inits each application and starts all applications
--  with autostart parameter set to true.
-- @return "ok" string in case of success, nil+error of message otherwise
local function init()
    local res, err = os.execute("mkdir -p "..apps_path)
    assert( 0 == res, string.format("ApplicationContainer cannot create apps runtime directory, err=%s", tostring(err)))

    loadAppList();
    local res, err
    -- init applications: needed for any runnable app.
    local function initapps()
        log('APPCON', 'DETAIL', "Initializing applications")
        for id, app in pairs(application_list) do
            if not does_app_path_exist(id) then
                log('APPCON', 'ERROR', "Application %q misses its directory %s%s", id, apps_path, id)
                local res, err = uninstall(id)
                if not res then log('APPCON', 'ERROR', "Auto uninstall of Application %q failed, error: %s", id, tostring(err)) end
            elseif is_app_runnable(id) then
                res, err = initapp(app)
                if not res then log("APPCON","ERROR","Error while init app %s, err=%s", app.id, err or "unknown") end
            end
        end
    end
    persist.save("ApplicationList", application_list);
    sched.sigrunonce("ReadyAgent", "InitDone",  initapps )
    return "ok"
end

M.init = init
M.install = install
M.uninstall = uninstall
M.start = start
M.stop = stop
M.status = status
M.list = list
M.configure = configure
M.attributes = attributes

--temporary fix to have appcon api as a global
agent.appcon=M

return M;
