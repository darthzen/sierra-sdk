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
include luafwk/m3da/CMakeFiles/bysant_core.dir/depend.make

# Include the progress variables for this target.
include luafwk/m3da/CMakeFiles/bysant_core.dir/progress.make

# Include the compile flags for this target's objects.
include luafwk/m3da/CMakeFiles/bysant_core.dir/flags.make

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o: luafwk/m3da/CMakeFiles/bysant_core.dir/flags.make
luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/m3da/bysant/bysant_core.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/m3da/bysant/bysant_core.c

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bysant_core.dir/bysant/bysant_core.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/m3da/bysant/bysant_core.c > CMakeFiles/bysant_core.dir/bysant/bysant_core.c.i

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bysant_core.dir/bysant/bysant_core.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/m3da/bysant/bysant_core.c -o CMakeFiles/bysant_core.dir/bysant/bysant_core.c.s

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.requires:
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.requires

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.provides: luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.requires
	$(MAKE) -f luafwk/m3da/CMakeFiles/bysant_core.dir/build.make luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.provides.build
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.provides

luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.provides.build: luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o

# Object files for target bysant_core
bysant_core_OBJECTS = \
"CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o"

# External object files for target bysant_core
bysant_core_EXTERNAL_OBJECTS =

runtime/lua/m3da/bysant/core.so: luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o
runtime/lua/m3da/bysant/core.so: luafwk/m3da/CMakeFiles/bysant_core.dir/build.make
runtime/lua/m3da/bysant/core.so: runtime/lib/libbysant_lua.so
runtime/lua/m3da/bysant/core.so: runtime/lib/libbysant.so
runtime/lua/m3da/bysant/core.so: luafwk/m3da/CMakeFiles/bysant_core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module ../../runtime/lua/m3da/bysant/core.so"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bysant_core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
luafwk/m3da/CMakeFiles/bysant_core.dir/build: runtime/lua/m3da/bysant/core.so
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/build

luafwk/m3da/CMakeFiles/bysant_core.dir/requires: luafwk/m3da/CMakeFiles/bysant_core.dir/bysant/bysant_core.c.o.requires
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/requires

luafwk/m3da/CMakeFiles/bysant_core.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da && $(CMAKE_COMMAND) -P CMakeFiles/bysant_core.dir/cmake_clean.cmake
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/clean

luafwk/m3da/CMakeFiles/bysant_core.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/m3da /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/m3da/CMakeFiles/bysant_core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/m3da/CMakeFiles/bysant_core.dir/depend

