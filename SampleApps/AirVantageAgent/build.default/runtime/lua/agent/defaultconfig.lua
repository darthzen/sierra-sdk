-------------------------------------------------------------------------------
-- Copyright (c) 2012 Sierra Wireless and others.
-- All rights reserved. This program and the accompanying materials
-- are made available under the terms of the Eclipse Public License v1.0
-- which accompanies this distribution, and is available at
-- http://www.eclipse.org/legal/epl-v10.html
--
-- Contributors:
--     Laurent Barthelemy for Sierra Wireless - initial API and implementation
--     Cuero Bugot        for Sierra Wireless - initial API and implementation
-------------------------------------------------------------------------------

-- Default configuration file for Linux targets

    local config = {}
    setfenv(1, config)

    server = {}

    server.serverId = "AIRVANTAGE"
    server.retrytimes = 10
    server.retryperiod = 60

    -- Determines the protocol, host, port, and optionally other things such
    -- as path, user, password
    server.url = "tcp://eu.airvantage.net:44900"
    --server.url = "tcp://na.airvantage.net:44900"

    --When the device is behind a proxy this settings defines a HTTP proxy. This parameter is only relevant for HTTP transport protocol
    --server.proxy must be a URL starting by "http://".
    --server.proxy = "http://some.proxy.server:port"

    -- Security: authentication is one of "hmac-sha1" or "hmac-md5" (or nil)
    -- Encryption cannot be enabled without authentication. It's of the form
    -- "<cipher>-<chaining>-<length>", where cipher must be "aes", chaining is
    -- either "ctr" or "cbc", length is either "128" or "256".
    server.authentication = 'hmac-sha1'
    server.encryption = 'aes-cbc-128'

    -- Agent auto connection policy
    server.autoconnect = { }
    server.autoconnect.onboot = true -- connect a few seconds after the ReadyAgent started
    server.autoconnect.period = 15 -- period in minute (connect every 15 minutes)
    -- server.autoconnect.cron = "0 0 * * *" -- cron entry (connect once a day at midnight)


    agent = {}
    agent.assetport = 9999 -- connection port, used to communicate with all the local clients
    --Address on which the agent is accepting connection in order to communicate with the assets
    --Pattern is accepted: can be set to "*" to accept connection from any address, by default shell accepts only localhost connection.
    --agent.assetaddress = "*"

    agent.signalport = 18888 -- port used for LUASIGNAL fwk (Linux only)


    -- Shell related settings
    shell = {}
    shell.activate = true
    shell.port = 2000
    shell.editmode = "edit" -- can be "line" if the trivial line by line mode is wanted
    shell.historysize = 30  -- only valid for edit mode,

    -- Time related settings
    time = {}
    -- activate Time Services: periodic polling only for now, sync can always be done using synchronize API on demand.
    time.activate = false
    --timezone: signed integer representing quarter(s) of hour to be added to UTC time (examples: -4 for UTC-1, 18 for UTC+5:45, ...)
    time.timezone = 0
    -- daylight saving time: signed integer representing quarter(s) of hour to be added to UTC time
    time.dst = 0

    time.ntpserver = "pool.ntp.org"
    --polling period for auto time sync
    --whatever ntppolling value, time sync is done on ReadyAgent boot if Time and NetworkManager are activated
    --if ntppolling is set to 0 or nil value, no periodic time sync is done
    --if set to string value, it will be interpreted as a cron entry (see timer.lua doc)
    --else positive number representing minutes is expected to specify periodic time sync
    time.ntppolling = 0

    network = {}
    network.activate = true
    network.isCdma = false
    network.isCdmaNew = false
    network.isCdmaChanged = false
    --TODO Update to true if CDMA profile is needed or false if not required
    network.isCdmaProfileNeeded = false
    --0 for None, 1 for PAP, 2 for CHAP Authentication, 3 for PAP/CHAP
    network.auth = 0
    network.authNew = 0
    network.authChanged = false
    network.username = {}
    network.password = {}
    network.usernameNew = {}
    network.passwordNew = {}
    network.usernameChanged = false
    network.passwordChanged = false
    --network.auth = 1
    --network.username = "username"
    --network.password = "password"

    -- uncomment and update the APN field if necessary, with well configured carrier PRI,
    -- the default APN should work with the carrier
    --network.apn = ""
    network.apnNew = {}
    network.apnChanged = false;

    -- the apn orginal stored will be backup, initiate as empty
    network.apn_backup = ""

    -- Check APN need to be updated before start data call
    network.updateAPN = false
    -- TODO update "mc7354-pdn-1" below
    network.dhcp_cmd = 'route del default; dhclient %s'
    network.dhcp_cmd_release = "ps aux | grep %s | grep -v grep | awk {'print $2'}| xargs kill -9"
    network.interface = 'eth1'
    network.interfaceNew = {}
    network.interfaceChanged = false
    
    log = {}
    log.defaultlevel = "INFO" -- default log level: can be one of NONE, ERROR, WARNING, INFO, DETAIL, DEBUG, ALL. See log.lua for details
    log.moduleslevel = { }
    --log.moduleslevel.GENERAL = "ALL"    -- per module log level
    --log.moduleslevel.SERVER  = "INFO"   -- per module log level
    log.moduleslevel.SCHED     = "INFO"   -- per module log level
    log.moduleslevel.TREEMGR   = "DETAIL" -- per module log level
    log.enablecolors = true

    -- change default format for all logs
    --log.format = "%t %m-%s: %l"
    -- timestampformat specifies strftime format to print date/time.
    -- timestampformat is useful only if %t% is in formater string
    log.timestampformat = "%F %T"


    update = {}
    update.activate = true
    --update package file name to use for local update file detection
    --update.localpkgname="updatepackage.tar.lzma"
    --dwlnotifperiod: number of seconds between update notification during downloads, default value is 2s.
    --update.dwlnotifperiod = 30

    --may need more than 10 minutes for the fw download retry case
    update.timeout = 1200

    -- Device Management Application/Commands
    device = {}
    device.activate = true
    --device.tcprconnect = {addr = '10.41.51.50', port = 2065}

    -- Monitoring system
    monitoring = {}
    monitoring.activate = false

    data = { }
    data.activate = true
    data.policy = { }
    data.policy.default  = { latency = 5, onboot=30 }
    data.policy.hourly   = { latency = 60*60 }
    data.policy.daily    = { latency = 24*60*60 }
    data.policy.never    = { 'manual' }
    data.policy.now      = { latency = 5, onboot=30 }
    data.policy.on_boot  = { onboot=30 }
    return config
