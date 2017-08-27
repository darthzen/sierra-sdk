

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
    ../../../build/bin/i386/slqssdk
    ../../../build/bin/arm9/slqssdk
    ../../../build/bin/ppc/slqssdk



******************
Auto start feature
******************
- By deafult, Agent won't start automatically
- It needs to enable AGENT_AUTO_START compile option in slqscompile.mak to enable agent auto-start feature