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

local config = require 'agent.config'
if not config.get('update.activate') then error "Update is disabled in agent.config" end
--Update module is not activated, calling error will be defaulted in -1 code

local function SoftwareUpdate(asset, data, path, ticketid)

    local update = require "agent.update"

    log("UPDATE", "DETAIL", "SoftwareUpdate cmd: %s, %s, %s, %s", tostring(asset), tostring(data), tostring(path), tostring(ticketid))

    local url = data and (data.url or data[1])
    local signature = data and (data.signature or data[2])
    if not url or not signature then return 551, "Wrong params in SoftwareUpdate command: need package url and package signature" end


    local newupdate = {proto= "m3da", url = url, signature=signature, ticketid = ticketid}
    local res, errcode, errstr = update.notifynewupdate(newupdate)

    if res then
        -- if update accepted the job, return "async" to indicate asynchronous update status acknowledge
        return "async"
    else
        errcode = tonumber(errcode) or 558 -- update rejected by update module
        return errcode, errstr or "update rejected by update module"
    end
end

return SoftwareUpdate
