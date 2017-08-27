This documents provide information about the Voice application.
This application supports:

    1.  Dial a Call
    2.  Answer Call
    3.  End a Call
    4.  End Held or Waiting Call
    5.  End Active Call and Accept Held or Waiting Call
    6.  Hold Active Call and Accept Waiting or Held Call
    7.  Hold All Calls Except a Specified One
    8.  Make a Conference Call
    9.  End All Calls
    10. Release a Specified Call
    11. Set Supplementary Service
    12. Start DTMF On Call In Progress
    13. Stop DTMF On Call In Progress
    14. Get CLIR Service Status
    15. Get CLIP Service Status
    16. Send Flash[ONLY FOR CDMA]

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

    a)  Dial a Call
        - User can dial a call.
        - The application will prompt the user to provide input for whether he
          wants to display his phone number and what type of call he wants to 
          dial voice or voice call over IP.

    b)  Answer Call
        - User can answer a incoming call.
        - The application will prompt the user to provide input for which call 
          he wants to answer.Input will be the call ID.

    c)  End a Call
        - User can end a incoming call or outgoing call.
        - The application will prompt the user to provide input for which call 
          he wants to end.Input will be the call ID.
                  
    d)  End Held or Waiting Call
        - User can end a call which is on hold or waiting.
        - The application will prompt the user to provide input for which call 
          he wants to end.Input will be the call ID.
                  
    e)  End Active Call and Accept Held or Waiting Call
        - User can put all active call on hold and simultaneously accept all held 
          and waiting call.
        
    f)  Hold Active Call and Accept Waiting or Held Call
        - User can hold all active call and simultaneously accept all held and 
          waiting call.

    g)  Hold All Calls Except a Specified One
        - User can hold all active call except a specified one for that the 
          application will prompt the user to provide input for which call 
          he does not want to hold.Input will be the call ID.
          
    h)  Make a Conference Call
        - User can put all in progress call into conference call.
        
    i)  End All Calls
        - When user is in conference call he can end all the calls which are
          in conference.

    j)  Release a Specified Call
        - When user is in conference call he can end a specified in conference.
        - The application will prompt the user to provide input for which call 
          he wants to end.Input will be the call ID.        

    k)  Set Supplementary Service
        - User can set supplementary service through the application.
        - User will be asked to modify one of the following service's settings:
          1.Call Forwarding Services
          2.Call Barring Services
          3.Call Waiting Services
          4.Auto Answer
          5.Preferred Privacy[ONLY FOR CDMA]
    
    l)  Start DTMF On Call In Progress
        - User can start Dual-tone multi-frequency.
             
    m)  Stop DTMF On Call In Progress
        - User can stop Dual-tone multi-frequency 
             
    n)  Get CLIR Service Status
        - User can get status of Calling Line Identification Restriction.
    
    o)  Get CLIP Service Status
        - User can get status of Calling Line Identification Presentation.
           
    p)  Send Flash[ONLY FOR CDMA]
        - User can send flash to a phone number which is in progress.
        - The application will prompt the user to provide input for which call 
           he wants to end.Input will be the call ID.