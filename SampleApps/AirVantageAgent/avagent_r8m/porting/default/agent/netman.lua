local config = require "agent.config"
local sched = require 'sched'
local log = require "log"
local data_call = require"data_call"
local os           = os
local string       = require'string'

module 'agent.netman'

function init()
   if config.get('network.isCdma') == false then
   -- only override apn for 3GPP connection (2G/3G/LTE)
   local cur_apn = data_call.get_apn(config.get('network.isCdma'))
   log("NETMAN", "INFO", "current apn %s", cur_apn)

      if config.get('network.apn') ~= nil then
         data_call.set_apn( config.get('network.apn'),config.get('network.isCdma'))
         log("NETMAN", "INFO", "backup apn %s", cur_apn)
         config.set('network.apn_backup', cur_apn)
      end
   end
   if config.get('network.isCdma') == true then
       if config.get('network.isCdmaProfileNeeded') == true then       
           -- only override apn for CDMA if profile needed
           local cur_apn = data_call.get_apn(config.get('network.isCdma'))
           log("NETMAN", "INFO", "current apn %s", cur_apn)
           if cur_apn ~= config.get('network.apn') then
               data_call.set_apn( config.get('network.apn'),config.get('network.isCdma'))
               log("NETMAN", "INFO", "backup apn %s", cur_apn)
               config.set('network.apn_backup', cur_apn)
           end
       end
   end

     -- Notify that the network is ready...
    sched.sigonce("Agent", "InitDone", function() sched.signal("NETMAN", "CONNECTED", "Default") end)
    return "ok"
end

function withnetwork(action, ...)
    local c = config.get
    local connected = 0

    if data_call.is_connected() == 0 then
        log("NETMAN", "INFO", "starting data call with apn %s", c'network.apn')

        -- retry 3 times, 30s apart
        for i=1,3 do
            UpdateParameters()
            local connected = data_call.connect(
                    config.get('network.isCdma'),
                    config.get('network.auth'),
                    config.get('network.username'),
                    config.get('network.password')
                )
            if connected == 1 then
                log("NETMAN", "INFO", "connected, running dhcp");
                os.execute( string.format( c'network.dhcp_cmd_release', c'network.interface') )
                os.execute( string.format( c'network.dhcp_cmd', c'network.interface' ) )
                return action(...)
            else
                log("NETMAN", "INFO", "data call failed, retry in 30s")
                sched.wait(30)
            end
        end
        log("NETMAN", "ERROR", "data call failed");
        return nil, 'data call failed'
    else
        log("NETMAN", "INFO", "already connected");
        log("NETMAN", "INFO", "connected, running dhcp");
        os.execute( string.format( c'network.dhcp_cmd_release', c'network.interface') )
        os.execute( string.format( c'network.dhcp_cmd', c'network.interface' ) )
        return action(...)
    end

end

function UpdateParameters()
    local cur_apn = data_call.get_apn(config.get('network.isCdma'))
    if config.get("network.apnChanged") then
        if config.get('network.apnNew') then
        log("NETMAN", "INFO", "Update new APN:%s",config.get('network.apnNew'))
        data_call.set_apn( config.get('network.apnNew'),config.get('network.isCdma'));
        config.set('network.apn', config.get('network.apnNew'))
        config.set('network.apn_backup', cur_apn);
        else
        log("NETMAN", "INFO", "Update new APN:nil")
        data_call.set_apn( '',config.get('network.isCdma'));
        config.set('network.apn', '')
        config.set('network.apn_backup', cur_apn);
        end
        config.set('network.apnChanged', false)
        sched.wait(10)
    end
    if config.get("network.usernameChanged") then
        if config.get('network.usernameNew') then
        log("NETMAN", "INFO", "Update username;%s",config.get('network.usernameNew'))
        config.set('network.username', config.get('network.usernameNew'))
        else
        log("NETMAN", "INFO", "Update username:nil")
        end
        config.set('network.usernameChanged', false)
    end
    if config.get("network.passwordChanged") then
        if config.get('network.passwordNew') then
        log("NETMAN", "INFO", "Update password:%s",config.get('network.passwordNew'))
        config.set('network.password', config.get('network.passwordNew'))
        else
        log("NETMAN", "INFO", "Update password:nil")
        end
        config.set('network.passwordChanged', false)
    end
    if config.get("network.authChanged") then
        if config.get('network.authNew') then
        log("NETMAN", "INFO", "Update auth:%d",config.get('network.authNew'))
        config.set('network.auth', config.get('network.authNew'))
        else
        log("NETMAN", "INFO", "Update auth:nil")
        end
        config.set('network.authChanged', false)
    end
    if config.get("network.interfaceChanged") then
        if config.get('network.interfaceNew') then
        log("NETMAN", "INFO", "Update interface:%s",config.get('network.interfaceNew'))
        config.set('network.interface', config.get('network.interfaceNew'))
        else
        log("NETMAN", "INFO", "Update interface:nil")
        config.set('network.interface', '')
        end
        config.set('network.interfaceChanged', false)
    end
end