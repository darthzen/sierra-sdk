This documents provide information about the SMS application.
This application supports:
    1. Being notified of new incoming SMS messages
    2. Sending SMS messages in both text and raw format
    3. Reading the SMS message list
    4. Reading a single SMS from the SMS message list.

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

    1. Register to be nofified of  new incoming SMS messages

    2. Send a SMS message

    3. Display the list of SMS messages stored on the device and/or UIM

    4. Read a SMS message stored on the device and/or UIM
