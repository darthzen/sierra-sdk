RAM Dump Tool
---------------------
This tool supports the capturing of the device RAM contents when the device is in 
boot and hold mode. RAM contents are saved in files written to the current working directory.

************
Compilation
************
- By default, the application is compiled for i386 platform.
  "make"

- The application can be compiled for different platform using the CPU option:
  "make CPU=<platform>"
  where <platform> = arm9 in case 0f ARM and
                     ppc  in case of Power PC.

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
3. High speed mode: within a shell, execute the following (for i386):
   ./ramdumptooli386 /dev/ttyUSB<digit>

   Slow speed mode: within a shell, execute the following (for i386):
   ./ramdumptooli386 -s /dev/ttyUSB<digit>

/dev/ttyUSB<digit> = DM interface ttyUSB device file in boot and hold mode (usually /dev/ttyUSB0)
