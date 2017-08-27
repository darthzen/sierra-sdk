This documents provide information about the MC7xxx Image management application.
This application supports:

    1.  Displaying information pertaining to a valid firmware image located on
        the host.

    2.  Displaying information pertaining to the firmware image on the device.

    3.  Downloading firmware to a MC7xxx device. The candidate image must
        contain a valid Sierra Package String in the CWE header.

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

- Changing the default path requires modifying SDKPATHDEFAULT in
  src/mc77xximgmgmt.c and re-compiling the application.

**********
Execution
**********
The following operations can be performed using the application:

    a)  Fetching the information about an image located on the host machine.
        - The application will prompt the user to specify the path to the image.
        - The path can either be a fully qualified path or a relative path.
        - The maximum path length is 510 characters (bytes).
        - If there is missing information in the fields that are displayed,
          then the firmware image does not contain valid Sierra Package String
          in the CWE header, and it will not be possible to download this image
          to the device.

    b)  Fetching the information about the image running on the device.

    c)  Download a new image to the device
        - The application will prompt the user to specify the path for image.
        - The path can either be a fully qualified path or a relative path.
        - The maximum path length is 510 characters (bytes).
        - The application will not perform a firmware download if the firmware
          image resident on the device and the image located at the user
          specified path are same.
