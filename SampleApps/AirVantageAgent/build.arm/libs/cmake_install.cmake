# Install script for directory: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/runtime")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/common/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/luasignal/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/libtommath/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/libtomcrypt/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/cdb/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/yajl/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

