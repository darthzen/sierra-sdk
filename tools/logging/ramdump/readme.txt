RAM Dump Tool
---------------------
This tool supports the capturing of the device RAM contents when the device is in memory dump mode. RAM contents are saved in files written to the directory
specified by the command line argument

************
Compilation
************
- By default, the application is compiled for i386 Linux platform.
  "make"

- The application can be compiled for different platform using the CPU option:
  "make CPU=<platform>"
  where <platform> = win32 in case of compiling with Cygwin on Windows
                     arm9  in case of ARM and
                     ppc   in case of Power PC.

- For arm9, powerpc, and mips, platform specific tool chains are required. These must be
  installed to the $(HOME) directory once, and can be used for all SDK workspaces.
  If they do not exist, 
  "make CPU=arm9",
  "make CPU=mips",
  "make CPU=mipsel", and
  "make CPU=ppc"  will simply fail.

*******************
 Prior to Execution
*******************
1. Enter the following AT commands:
    at!entercnd="A710"
    at!eroption=0

2. Either reproduce a crash you are investigating, or reset the device

**********
Execution
**********
Within a shell, execute the following (for i386):
   ./ramdumptool_i386 [-d <device>] [-c <comport>] [-o <output directory>] [-noreset] [-s]

  -d <device>
       Optionally specify device type. i.e -d 9x30.
  -c <comport>
       Optional parameter to specify the DM port, example: -c 3 for COM3 on Windows
       or -c 0 for /dev/ttyUSB0 on Linux
  -o <output_directory>
       Optional parameter to specify where to store the crash dump.
       If this is not specified, '<current dir>/MemDumpOut' will be used as default
  -noreset
       Optional Parameter to not reset the device after collecting a crash dump
  -s
       Force slow speed for DMSS protocol. Will have no effect on 9x30 device
  -h
       Display this help menu

/dev/ttyUSB<digit> = DM interface ttyUSB device file in boot and hold mode (usually /dev/ttyUSB0)

-----
NOTE:
-----
  To execute the program in Windows shell command prompt, cygwin dll path needs to be added to Windows environment variable.

************
Known Issues
************
1.
  On Linux, device sometimes does not reset when -noreset option is not specified. This has not been seen on Windows
2.
  When compiling on Linux, the program may sometimes end up with a problem where data read from serial port is corrupted. The cause is still unknown (making some changes in the code unrelated to serial port and the problem goes away)
  This problem does not happen on Windows
3.
  The DM port detection logic on Linux relies on 'udevadm' and is rather simple and has not been tested for more complicated setup.
4.
  The application has not been compiled and tested for arm9 and ppc target.
