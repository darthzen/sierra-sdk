This documents provide information about the GOBI Image management application.
This application supports:

    1.  Displaying information pertaining to valid firmware images located on
        the host.

    2.  Displaying information pertaining to the firmware images on the device.

    3.  Displaying detailed information pertaining to a specific firmware image on the device.

    4.  Downloading firmware to a GOBI device.

    5.  Deleting a firmware image from the device.

    6.  Activating a firmware image resident on the device.
        Note: AMSS image selection can be specified by updating carrierImageOverride.txt
              to specify the version of build ID of the desired AMSS image corresponding
              to the carrier. If the entry is invalid i.e. no corresponding image exists
              on the device, no image will be selected, application will prompt with error
              "Corresponding MODEM image does not exist on the device".
              If carrierImageOverride.txt is not present in current working directory or no
              carrier AMSS image is specified, then the latest AMSS image for the selected
              carrier will be selected.

    7    Display Raw Images( build id of UQCN and AMSS images) present in device

    8    Activating from Raw Images avilable on device

    9    SL9090 image download supported by application ( option 4 and 6 can be used to download or
         switch images)

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
  src/gobiimgmgmt.c and re-compiling the application.

**********
Execution
**********
The following operations can be performed using the application:

    a)  Fetching the information about images located on the host machine.
        - The application will prompt the user to specify the path for the image folders.
        - The path can either be a fully qualified path or a relative path.
        - The maximum path length is 510 characters (bytes).

    b)  Fetching the information about the images located on the device.

    c)  Fetching the detailed information about a specific image located on the device.
        - Application will display all the images loaded on the device.
        - User will be prompted to select the image id of the image for which detailed information needs to be retrieved.

    d)  Download a new image to the device
        - The application will display list of images located on the device including
          the active image.
        - Application will then prompt the user to specify the path for image folders.
        - The path can either be a fully qualified path or a relative path.
        - The maximum path length is 510 characters (bytes).
        - The application will then display list of valid images located at the path
          specified by user.
        - User will be prompted to select the image to be downloaded from the list
          displayed in last step.

    e)  Delete an image from the device.
        - The application will display list of images located on the device including
          the active image.
        - User will be prompted to select the image to be deleted from device from the
          list displayed in last step.

    f) Activating an image located on the device.
        - The application will display list of images located on the device including
          the active image.
        - User will be prompted to select the image to be activated on the device
          from the list displayed in last step.

    g) Displaying build id of images avilable device

    h) Activating images by selecting images index of UQCN and AMSS separately.
