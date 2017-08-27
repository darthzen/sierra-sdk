#*******************************************************************************
# Copyright (c) 2012 Sierra Wireless and others.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )

# specify the cross compiler (https://github.com/raspberrypi/tools.git)
SET(CMAKE_C_COMPILER   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-g++)
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/)
SET(CMAKE_LD   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-ld)


# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#*******************************************************************************
# Copyright (c) 2012 Sierra Wireless and others.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )

# specify the cross compiler (https://github.com/raspberrypi/tools.git)
SET(CMAKE_C_COMPILER   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-g++)
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/)
SET(CMAKE_LD   $ENV{HOME}/toolchain/arm64bit/be/aarch64ebv8-marvell-linux-gnu-5.2.1_x86_64_20151117/bin/aarch64_be-marvell-linux-gnu-ld)


# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
