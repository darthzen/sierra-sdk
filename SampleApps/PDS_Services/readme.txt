This documents provide information about the PDS application.
This application supports:

    1. Set GPS Service State
    2. Get GPS Service State
    3. Start Tracking Session
    4. End Tracking Session
    5. Set SERVICE Auto Tracking
    6. Set PORT Auto Tracking 

************
Compilation
************
- By default, the application is compiled for i86 platform.
  "make"

- The application can be compiled for different platform using the CPU option:
  "make CPU=<platform>"
  where <platform> = arm9 in case 0f ARM and
                     ppc  in case of Power PC.
- For arm9 and powerpc, platform specific tool chains are required. These must be
  installed to the $(HOME) directory once, and can be used for all SDK workspaces.
  If they do not exist, 
  "make CPU=arm9" and
  "make CPU=ppc"  will simply fail.

*****
SDK
*****
- The SDK executable is assumed to be located at the default path for the specified
  platform:
    ../../../build/bin/i386/slqssdk
    ../../../build/bin/arm9/slqssdk
    ../../../build/bin/ppc/slqssdk

- Changing the default path requires modifying SDKPATHDEFAULT in
  src/callhandling.c and re-compiling the application.

**********
Execution
**********
The following operations can be performed using the application:

    a)  Set GPS Service State
        - User can set GPS service state to enable or disable.

    b)  Get GPS Service State
        - User can get GPS service state whether it is enable or disable.

    c)  Start Tracking Session
        - User can start a tracking session and get NMEA data.
                  
    d)  End Tracking Session
        - User can end a tracking session.
      
    e)  Set SERVICE Auto Tracking
        - User can set sevice auto tracking. 
        - This provides alternate way of option 3 and option 4.
        
    f)  Set PORT Auto Tracking
        - User can set port auto tracking.
          Port auto tracking normally enabled in device by default
          It can be disabled or enabled using option 6 in application.
          If Port autotracking is set then only option 1,2 and 6 off application
          can be executed successfully 
