#!/bin/bash
#
# Intended usage: Capturing Diagnostic Message (DM) logs from a wireless modem's DM port. The script
# can be used as a standalone application (local logging), or as a "server" application (remote logging).
#
# Local logging: If specified, a DM filter is sent to the device prior to logging. Received DM messages
# are written to a log file in the current working directory.
#
# Remote logging: If specified, a DM filter is sent to the device prior to logging. Received DM messages
# are routed to a remote client via a TCP/IP connection.
#
# NB: /bin/sh must point to /bin/bash, not /bin/dash or the select will fail.

# default TCP port for remote logging
NETPORTDFLT=23000
arch="i386"

usage() {
    echo
    echo "Local Logging:"
    echo "./dmcapture -l [-d device] [-f filter] [-o logfile]:"
    echo "      - Logs DM packets to \"logfile\" if specified, or to a script generated "
    echo "        log file stored in the current working directory if not specified."
    echo
    echo "Remote Logging:"
    echo "./dmcapture [-d device] [-f filter] [-p netport] [-r rhost]"
    echo "      - Establishes a TCP connection with a remote machine \"rhost\" using port number "
    echo "        \"netport\", or a default port number of 23000 if not specified. DM packets are "
    echo "        exchanged over the TCP connection."
    echo "      - Without the -r option, acts as a server application waiting for an incoming "
    echo "        connection request. Otherwise, acts as a client and attempts to establish a "
    echo "        connection with \"rhost\"."
    echo
    echo "OPTIONS:"
    echo "-l            - Local logging if specified, remote logging otherwise"
    echo "-a arch       - Specify architecture if not x86: arm, ppc, mips"
    echo "-d device     - ttyUSB port for DM logging"
    echo "-f filter     - (Optional) DM filter to send to the device prior to logging"
    echo "-p netport    - Remote logging TCP port (defaults to 23000)"
    echo "-r rhost      - (Optional) remote host to connect to"
    echo "-o logfile    - fully qualified DM log (output) file name"
    echo
}

while [ $# -gt 0 ]
do
    case "$1" in
    -a) arch=$2;;
    -d) dev=$2;;
    -f) filter=$2;;
    -h) usage; exit;;
    -l) ltype="local";;
    -o) logfile=$2;;
    -p) netport=$2;;
    -r) rhost=$2;;
    --help) usage; exit;
    esac
    shift
done

# write incoming DM packets to local file
local_logging() {
    if [ -z "$logfile" ]; then logfile=dmlog-$(date +%F-%R).raw ; fi
    echo Log messages: $dev "->" $logfile
    cat $dev > $logfile
    exit
}

# Receive incoming DM packets from a remote host and send them to the device;
# Send device originated DM packets to the remote host
remote_logging() {
    echo Log messages: $dev "->" socket: $netport
    if [ -z $rhost ]; then ./bin/remserial$arch -p $netport $dev
    else ./bin/remserial$arch -r $rhost -p $netport $dev; fi
    exit
}

# write filter to the modem's DM port
write_filter() {
    echo Write Filter: $filter "->" $dev
    if [ -s $filter ]; then
        #cat $filter > $dev
        ./bin/split-sqf$arch $filter $dev
    else
        echo Skip writing of non-existent or zero length filter file!
    fi
}

# -- mandatory command line arguments --
if [ -z "$dev" ]; then
    usage
    exit
#Ensure line discipline does not lead to unexpected behaviour
elif [ -c "$dev" ]; then
    stty -F $dev raw -echo
else
    echo "Invalid device \"$dev\" specified! Exiting DM logging tool."
    exit
fi

# -- optional command line arguments --
# TCP port for remote logging
if [ -z "$netport" ]; then netport=$NETPORTDFLT; fi
# write DM filter to device prior to reading DM packets
if [ -n "$filter" ]; then write_filter; fi
# DM logging
if [ -n "$ltype" ]; then local_logging; else remote_logging; fi

