This documents provide information about the SMS application.
This application supports:
    1. Initiating Firmware download on device.
    2. Initiating Firmware Update on device.
    3. Currently CancelSession,GetSessionInfo and SWIOMADM Settings are not supported on device. 


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
- The SDK executable is assumed to be located at the default path:
    ../../../pkgs/sdk/slqssdki386

- Changing the default path requires modifying SDKPATHDEFAULT in
  src/SMSSampelApp.c and re-compiling the application.

**********
Execution
**********
The following operations can be performed using the application:

    1. Start Session
    2. cancel Session
    3. Get Session Info
    4. Get SWIOMADM Setting
    5. Set SWIOMADM Setting



