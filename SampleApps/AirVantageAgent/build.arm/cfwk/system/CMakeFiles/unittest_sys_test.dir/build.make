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
include cfwk/system/CMakeFiles/unittest_sys_test.dir/depend.make

# Include the progress variables for this target.
include cfwk/system/CMakeFiles/unittest_sys_test.dir/progress.make

# Include the compile flags for this target's objects.
include cfwk/system/CMakeFiles/unittest_sys_test.dir/flags.make

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o: cfwk/system/CMakeFiles/unittest_sys_test.dir/flags.make
cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/cfwk/system/sys_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/unittest_sys_test.dir/sys_test.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/cfwk/system/sys_test.c

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unittest_sys_test.dir/sys_test.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/cfwk/system/sys_test.c > CMakeFiles/unittest_sys_test.dir/sys_test.c.i

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unittest_sys_test.dir/sys_test.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/cfwk/system/sys_test.c -o CMakeFiles/unittest_sys_test.dir/sys_test.c.s

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.requires:
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.requires

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.provides: cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.requires
	$(MAKE) -f cfwk/system/CMakeFiles/unittest_sys_test.dir/build.make cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.provides.build
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.provides

cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.provides.build: cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o

# Object files for target unittest_sys_test
unittest_sys_test_OBJECTS = \
"CMakeFiles/unittest_sys_test.dir/sys_test.c.o"

# External object files for target unittest_sys_test
unittest_sys_test_EXTERNAL_OBJECTS =

runtime/bin/sys_test: cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o
runtime/bin/sys_test: cfwk/system/CMakeFiles/unittest_sys_test.dir/build.make
runtime/bin/sys_test: runtime/lib/libSwi_System.so
runtime/bin/sys_test: runtime/lib/libSwi_log.so
runtime/bin/sys_test: runtime/lib/libEmp.so
runtime/bin/sys_test: runtime/lib/libSwi_DSet.so
runtime/bin/sys_test: static-libs/libpointerlist.a
runtime/bin/sys_test: runtime/lib/libyajl.so
runtime/bin/sys_test: cfwk/system/CMakeFiles/unittest_sys_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../runtime/bin/sys_test"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unittest_sys_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cfwk/system/CMakeFiles/unittest_sys_test.dir/build: runtime/bin/sys_test
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/build

cfwk/system/CMakeFiles/unittest_sys_test.dir/requires: cfwk/system/CMakeFiles/unittest_sys_test.dir/sys_test.c.o.requires
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/requires

cfwk/system/CMakeFiles/unittest_sys_test.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system && $(CMAKE_COMMAND) -P CMakeFiles/unittest_sys_test.dir/cmake_clean.cmake
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/clean

cfwk/system/CMakeFiles/unittest_sys_test.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/cfwk/system /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/cfwk/system/CMakeFiles/unittest_sys_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cfwk/system/CMakeFiles/unittest_sys_test.dir/depend

