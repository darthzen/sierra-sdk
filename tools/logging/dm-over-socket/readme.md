# Capture DM log over socket with AR7594

## Setup

Connect LAN port on AR7594 board with Linux host
Expected to see interface similar to below
```
ifconfig enx00116b738e27
enx00116b738e27: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.225.59  netmask 255.255.255.0  broadcast 192.168.225.255
        inet6 fe80::c027:8489:aaa:5c78  prefixlen 64  scopeid 0x20<link>
        ether 00:11:6b:73:8e:27  txqueuelen 1000  (Ethernet)
        RX packets 148519  bytes 184560394 (184.5 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 126383  bytes 9071218 (9.0 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
### Run python script attached to apply filter and receive DM log
./dm-logger.py 2500 ../3/tools/logging/dm/filters/MC7xxx_LTE_QOS_filter.sqf out.dm

### Run logger on the modem via adb

#### Capture with IPv4 socket
adb shell diag_socket_log -a 192.168.225.59
#### Capture with IPv6 socket
adb shell diag_socket_log -a fe80::c027:8489:aaa:5c78%bridge0

## Convert captured log with swilogutil on Windows
swilogutil convert out.dm out.dlf

## Notes
- The python script works on both IPv4 & IPv6 sockets
- Remember to append %bridge0 when start IPv6 logger

## Test Environment
- Python 2.7.12+
- AR7594 firmware SWI9X40A_01.05.03.00
- Ubuntu 16.10
- Linux kernel 4.8.0-41

## Example IPv4 Session
### adb log
```
adb shell diag_socket_log -a 192.168.225.59
diag_socket_log: Fail creating directory for diag_socket.pid file
 diag_socket_log: Unable to open Pid File,errno: 2
 ODL feature will loose data
diag_socket_log: Diag_LSM_Init succeeded.
diag: In diag_register_socket_cb, registered socket callback function
diag_socket_log: Trying to connect to address: 192.168.225.59, port: 2500
diag_socket_log: Successful connect to address: 192.168.225.59, port number: 2500
 logging switched                                                            
```

### python script log
```
./dm-logger.py 2500 ../3/tools/logging/dm/filters/MC7xxx_LTE_QOS_filter.sqf out.dm
waiting for connection...
sending filter to modem...
pulling log from modem..., ctrl-c to terminate
^Cctrl-c received, exiting...
```

## Example IPv6 Session
### adb log
```
adb shell diag_socket_log -a fe80::c027:8489:aaa:5c78%bridge0 
diag_socket_log: Fail creating directory for diag_socket.pid file
 diag_socket_log: Unable to open Pid File,errno: 2
 ODL feature will loose data
diag_socket_log: Diag_LSM_Init succeeded.
diag: In diag_register_socket_cb, registered socket callback function
diag_socket_log: Trying to connect to address: fe80::c027:8489:aaa:5c78%bridge0, port: 2500
diag_socket_log: Successful connect to address: fe80::c027:8489:aaa:5c78%bridge0, port number: 2500
 logging switched 
```

### python script log
```
./dm-logger.py 2500 ../3/tools/logging/dm/filters/MC7xxx_LTE_QOS_filter.sqf out.dm
waiting for connection...
sending filter to modem...
pulling log from modem..., ctrl-c to terminate
^Cctrl-c received, exiting...
```