# Install script for directory: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk

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
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/tests/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/serialframework/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/checks/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/lpack/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/luatobin/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/messaging/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/shell/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/luasocket/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/ntp/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/sched/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/utils/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/web/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/rpc/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/coxpcall/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/lfs/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/loader/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/lua/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/serial/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/luacdb/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/persist/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/lua-yajl/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/racon/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da/cmake_install.cmake")
  INCLUDE("/root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/stagedb/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

