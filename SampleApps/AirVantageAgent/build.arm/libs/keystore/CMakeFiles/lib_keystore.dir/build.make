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
include libs/keystore/CMakeFiles/lib_keystore.dir/depend.make

# Include the progress variables for this target.
include libs/keystore/CMakeFiles/lib_keystore.dir/progress.make

# Include the compile flags for this target's objects.
include libs/keystore/CMakeFiles/lib_keystore.dir/flags.make

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o: libs/keystore/CMakeFiles/lib_keystore.dir/flags.make
libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/keystore/keystore.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_keystore.dir/keystore.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/keystore/keystore.c

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_keystore.dir/keystore.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/keystore/keystore.c > CMakeFiles/lib_keystore.dir/keystore.c.i

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_keystore.dir/keystore.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/keystore/keystore.c -o CMakeFiles/lib_keystore.dir/keystore.c.s

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.requires:
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.requires

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.provides: libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.requires
	$(MAKE) -f libs/keystore/CMakeFiles/lib_keystore.dir/build.make libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.provides.build
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.provides

libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.provides.build: libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o

# Object files for target lib_keystore
lib_keystore_OBJECTS = \
"CMakeFiles/lib_keystore.dir/keystore.c.o"

# External object files for target lib_keystore
lib_keystore_EXTERNAL_OBJECTS =

runtime/lib/libkeystore.so: libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o
runtime/lib/libkeystore.so: libs/keystore/CMakeFiles/lib_keystore.dir/build.make
runtime/lib/libkeystore.so: libs/keystore/CMakeFiles/lib_keystore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library ../../runtime/lib/libkeystore.so"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib_keystore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/keystore/CMakeFiles/lib_keystore.dir/build: runtime/lib/libkeystore.so
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/build

libs/keystore/CMakeFiles/lib_keystore.dir/requires: libs/keystore/CMakeFiles/lib_keystore.dir/keystore.c.o.requires
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/requires

libs/keystore/CMakeFiles/lib_keystore.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore && $(CMAKE_COMMAND) -P CMakeFiles/lib_keystore.dir/cmake_clean.cmake
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/clean

libs/keystore/CMakeFiles/lib_keystore.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/keystore /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/keystore/CMakeFiles/lib_keystore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/keystore/CMakeFiles/lib_keystore.dir/depend

