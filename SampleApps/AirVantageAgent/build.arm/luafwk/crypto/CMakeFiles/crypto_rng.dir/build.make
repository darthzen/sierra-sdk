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
include luafwk/crypto/CMakeFiles/crypto_rng.dir/depend.make

# Include the progress variables for this target.
include luafwk/crypto/CMakeFiles/crypto_rng.dir/progress.make

# Include the compile flags for this target's objects.
include luafwk/crypto/CMakeFiles/crypto_rng.dir/flags.make

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o: luafwk/crypto/CMakeFiles/crypto_rng.dir/flags.make
luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/crypto/lrng.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/crypto_rng.dir/lrng.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/crypto/lrng.c

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/crypto_rng.dir/lrng.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/crypto/lrng.c > CMakeFiles/crypto_rng.dir/lrng.c.i

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/crypto_rng.dir/lrng.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/crypto/lrng.c -o CMakeFiles/crypto_rng.dir/lrng.c.s

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.requires:
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.requires

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.provides: luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.requires
	$(MAKE) -f luafwk/crypto/CMakeFiles/crypto_rng.dir/build.make luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.provides.build
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.provides

luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.provides.build: luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o

# Object files for target crypto_rng
crypto_rng_OBJECTS = \
"CMakeFiles/crypto_rng.dir/lrng.c.o"

# External object files for target crypto_rng
crypto_rng_EXTERNAL_OBJECTS =

runtime/lua/crypto/rng.so: luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o
runtime/lua/crypto/rng.so: luafwk/crypto/CMakeFiles/crypto_rng.dir/build.make
runtime/lua/crypto/rng.so: runtime/lib/libtomcrypt.so
runtime/lua/crypto/rng.so: runtime/lib/libtommath.so
runtime/lua/crypto/rng.so: luafwk/crypto/CMakeFiles/crypto_rng.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module ../../runtime/lua/crypto/rng.so"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crypto_rng.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
luafwk/crypto/CMakeFiles/crypto_rng.dir/build: runtime/lua/crypto/rng.so
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/build

luafwk/crypto/CMakeFiles/crypto_rng.dir/requires: luafwk/crypto/CMakeFiles/crypto_rng.dir/lrng.c.o.requires
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/requires

luafwk/crypto/CMakeFiles/crypto_rng.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto && $(CMAKE_COMMAND) -P CMakeFiles/crypto_rng.dir/cmake_clean.cmake
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/clean

luafwk/crypto/CMakeFiles/crypto_rng.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/crypto /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/luafwk/crypto/CMakeFiles/crypto_rng.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/crypto/CMakeFiles/crypto_rng.dir/depend

