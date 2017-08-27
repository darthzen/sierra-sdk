This documents provide information about the command line firmware download application.
This application supports downloading firmware images for the following category of devices:

    1.  MDM9x00 based devices. For ex: MC7700/10/50.

    2.  MDM9x15 based devices. For ex: MC7355/04/50.

    3.  Gobi3k base devices. For ex: MC8355.

    4.  MDM9x30 based devices. For ex: EM/MC7455

************
Compilation
************
- By default, the application is compiled for i86 platform.
  "make"

- The application can be compiled for different platform using the CPU option:
  "make CPU=<platform>"
  where <platform> = arm9 in case 0f ARM and
                     ppc  in case of Power PC.
- For arm9 and powerpc, platform specific toolchains are required. These must be
  installed to the $(HOME) directory once, and can be used for all SDK workspaces.
  If they do not exist, 
  "make CPU=arm9" and
  "make CPU=ppc"  will simply fail.

*****
SDK
*****
- The SDK executable is assumed to be located at the default path for the specified platform:
    ../../../build/bin/host<arch>/slqssdk

- This is to be specified as a command line parameter. For more refer in application menu
  by  <appname> -h ( or --help ).

**********
Execution
**********
The following operations can be performed using the application:

    a)  Updating the firmware on an MDM 9x00 based device.

    b)  Updating the firmware on an Gobi3k based device. Please note folder path format has
        to be <filepath>/x where x is a number

    c)  Updating the firmware on an MDM 9x15 based device either through a single file or two files.

    d)  Retrieve the usage of the application through <appname> -h

************
Exit Codes
************
Below are the exit codes from the application
0   - Success
100 - Generic FW download error
101 - SDK specific error. Please check syslog for SDK error codes
102 - Error in setting SDK callbacks
103 - Path is too long
104 - Mandatory SDK/FW path not set
105 - Post FW download check indicates that the FW upgrade has failed. 
      For example, PRI not updated as expected, FW version not updated etc
106 - Invalid device
107 - Unable to start the SDK process. Check syslog for SDK error codes
