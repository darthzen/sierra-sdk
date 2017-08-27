# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm

# Include any dependencies generated for this target.
include libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/depend.make

# Include the progress variables for this target.
include libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/progress.make

# Include the compile flags for this target's objects.
include libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/flags.make

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/flags.make
libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/SerialFramework/Modbus/modbus_serializer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_modbus.dir/modbus_serializer.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/SerialFramework/Modbus/modbus_serializer.c

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_modbus.dir/modbus_serializer.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/SerialFramework/Modbus/modbus_serializer.c > CMakeFiles/lib_modbus.dir/modbus_serializer.c.i

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_modbus.dir/modbus_serializer.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/SerialFramework/Modbus/modbus_serializer.c -o CMakeFiles/lib_modbus.dir/modbus_serializer.c.s

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.requires:
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.requires

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.provides: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.requires
	$(MAKE) -f libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/build.make libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.provides.build
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.provides

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.provides.build: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o

# Object files for target lib_modbus
lib_modbus_OBJECTS = \
"CMakeFiles/lib_modbus.dir/modbus_serializer.c.o"

# External object files for target lib_modbus
lib_modbus_EXTERNAL_OBJECTS =

static-libs/libmodbus.a: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o
static-libs/libmodbus.a: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/build.make
static-libs/libmodbus.a: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../../../static-libs/libmodbus.a"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && $(CMAKE_COMMAND) -P CMakeFiles/lib_modbus.dir/cmake_clean_target.cmake
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib_modbus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/build: static-libs/libmodbus.a
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/build

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/requires: libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/modbus_serializer.c.o.requires
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/requires

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus && $(CMAKE_COMMAND) -P CMakeFiles/lib_modbus.dir/cmake_clean.cmake
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/clean

libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/SerialFramework/Modbus /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/SerialFramework/Modbus/CMakeFiles/lib_modbus.dir/depend

