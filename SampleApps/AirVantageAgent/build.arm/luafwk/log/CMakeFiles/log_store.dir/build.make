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
include luafwk/log/CMakeFiles/log_store.dir/depend.make

# Include the progress variables for this target.
include luafwk/log/CMakeFiles/log_store.dir/progress.make

# Include the compile flags for this target's objects.
include luafwk/log/CMakeFiles/log_store.dir/flags.make

luafwk/log/CMakeFiles/log_store.dir/log_store.c.o: luafwk/log/CMakeFiles/log_store.dir/flags.make
luafwk/log/CMakeFiles/log_store.dir/log_store.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_store.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object luafwk/log/CMakeFiles/log_store.dir/log_store.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/log_store.dir/log_store.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_store.c

luafwk/log/CMakeFiles/log_store.dir/log_store.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/log_store.dir/log_store.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_store.c > CMakeFiles/log_store.dir/log_store.c.i

luafwk/log/CMakeFiles/log_store.dir/log_store.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/log_store.dir/log_store.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_store.c -o CMakeFiles/log_store.dir/log_store.c.s

luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.requires:
.PHONY : luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.requires

luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.provides: luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.requires
	$(MAKE) -f luafwk/log/CMakeFiles/log_store.dir/build.make luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.provides.build
.PHONY : luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.provides

luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.provides.build: luafwk/log/CMakeFiles/log_store.dir/log_store.c.o

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o: luafwk/log/CMakeFiles/log_store.dir/flags.make
luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_storeflash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/log_store.dir/log_storeflash.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_storeflash.c

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/log_store.dir/log_storeflash.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_storeflash.c > CMakeFiles/log_store.dir/log_storeflash.c.i

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/log_store.dir/log_storeflash.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/log_storeflash.c -o CMakeFiles/log_store.dir/log_storeflash.c.s

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.requires:
.PHONY : luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.requires

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.provides: luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.requires
	$(MAKE) -f luafwk/log/CMakeFiles/log_store.dir/build.make luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.provides.build
.PHONY : luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.provides

luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.provides.build: luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o

# Object files for target log_store
log_store_OBJECTS = \
"CMakeFiles/log_store.dir/log_store.c.o" \
"CMakeFiles/log_store.dir/log_storeflash.c.o"

# External object files for target log_store
log_store_EXTERNAL_OBJECTS =

runtime/lua/log/store.so: luafwk/log/CMakeFiles/log_store.dir/log_store.c.o
runtime/lua/log/store.so: luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o
runtime/lua/log/store.so: luafwk/log/CMakeFiles/log_store.dir/build.make
runtime/lua/log/store.so: luafwk/log/CMakeFiles/log_store.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module ../../runtime/lua/log/store.so"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/log_store.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
luafwk/log/CMakeFiles/log_store.dir/build: runtime/lua/log/store.so
.PHONY : luafwk/log/CMakeFiles/log_store.dir/build

luafwk/log/CMakeFiles/log_store.dir/requires: luafwk/log/CMakeFiles/log_store.dir/log_store.c.o.requires
luafwk/log/CMakeFiles/log_store.dir/requires: luafwk/log/CMakeFiles/log_store.dir/log_storeflash.c.o.requires
.PHONY : luafwk/log/CMakeFiles/log_store.dir/requires

luafwk/log/CMakeFiles/log_store.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log && $(CMAKE_COMMAND) -P CMakeFiles/log_store.dir/cmake_clean.cmake
.PHONY : luafwk/log/CMakeFiles/log_store.dir/clean

luafwk/log/CMakeFiles/log_store.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/log/CMakeFiles/log_store.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/log/CMakeFiles/log_store.dir/depend

