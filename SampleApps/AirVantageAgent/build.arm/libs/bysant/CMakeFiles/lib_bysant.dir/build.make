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
include libs/bysant/CMakeFiles/lib_bysant.dir/depend.make

# Include the progress variables for this target.
include libs/bysant/CMakeFiles/lib_bysant.dir/progress.make

# Include the compile flags for this target's objects.
include libs/bysant/CMakeFiles/lib_bysant.dir/flags.make

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o: libs/bysant/CMakeFiles/lib_bysant.dir/flags.make
libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysants.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_bysant.dir/bysants.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysants.c

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_bysant.dir/bysants.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysants.c > CMakeFiles/lib_bysant.dir/bysants.c.i

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_bysant.dir/bysants.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysants.c -o CMakeFiles/lib_bysant.dir/bysants.c.s

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.requires:
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.requires

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.provides: libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.requires
	$(MAKE) -f libs/bysant/CMakeFiles/lib_bysant.dir/build.make libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.provides.build
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.provides

libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.provides.build: libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o: libs/bysant/CMakeFiles/lib_bysant.dir/flags.make
libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysantd.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_bysant.dir/bysantd.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysantd.c

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_bysant.dir/bysantd.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysantd.c > CMakeFiles/lib_bysant.dir/bysantd.c.i

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_bysant.dir/bysantd.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysantd.c -o CMakeFiles/lib_bysant.dir/bysantd.c.s

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.requires:
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.requires

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.provides: libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.requires
	$(MAKE) -f libs/bysant/CMakeFiles/lib_bysant.dir/build.make libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.provides.build
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.provides

libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.provides.build: libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o: libs/bysant/CMakeFiles/lib_bysant.dir/flags.make
libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_common.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_bysant.dir/bysant_common.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_common.c

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_bysant.dir/bysant_common.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_common.c > CMakeFiles/lib_bysant.dir/bysant_common.c.i

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_bysant.dir/bysant_common.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_common.c -o CMakeFiles/lib_bysant.dir/bysant_common.c.s

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.requires:
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.requires

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.provides: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.requires
	$(MAKE) -f libs/bysant/CMakeFiles/lib_bysant.dir/build.make libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.provides.build
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.provides

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.provides.build: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o: libs/bysant/CMakeFiles/lib_bysant.dir/flags.make
libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_class.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_bysant.dir/bysant_class.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_class.c

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_bysant.dir/bysant_class.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_class.c > CMakeFiles/lib_bysant.dir/bysant_class.c.i

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_bysant.dir/bysant_class.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant/bysant_class.c -o CMakeFiles/lib_bysant.dir/bysant_class.c.s

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.requires:
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.requires

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.provides: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.requires
	$(MAKE) -f libs/bysant/CMakeFiles/lib_bysant.dir/build.make libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.provides.build
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.provides

libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.provides.build: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o: libs/bysant/CMakeFiles/lib_bysant.dir/flags.make
libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/common/awt_endian.c
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o   -c /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/common/awt_endian.c

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.i"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/common/awt_endian.c > CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.i

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.s"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && /root/toolchain/arm/bin/arm-none-linux-gnueabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/common/awt_endian.c -o CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.s

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.requires:
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.requires

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.provides: libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.requires
	$(MAKE) -f libs/bysant/CMakeFiles/lib_bysant.dir/build.make libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.provides.build
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.provides

libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.provides.build: libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o

# Object files for target lib_bysant
lib_bysant_OBJECTS = \
"CMakeFiles/lib_bysant.dir/bysants.c.o" \
"CMakeFiles/lib_bysant.dir/bysantd.c.o" \
"CMakeFiles/lib_bysant.dir/bysant_common.c.o" \
"CMakeFiles/lib_bysant.dir/bysant_class.c.o" \
"CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o"

# External object files for target lib_bysant
lib_bysant_EXTERNAL_OBJECTS =

runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/build.make
runtime/lib/libbysant.so: libs/bysant/CMakeFiles/lib_bysant.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library ../../runtime/lib/libbysant.so"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib_bysant.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/bysant/CMakeFiles/lib_bysant.dir/build: runtime/lib/libbysant.so
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/build

libs/bysant/CMakeFiles/lib_bysant.dir/requires: libs/bysant/CMakeFiles/lib_bysant.dir/bysants.c.o.requires
libs/bysant/CMakeFiles/lib_bysant.dir/requires: libs/bysant/CMakeFiles/lib_bysant.dir/bysantd.c.o.requires
libs/bysant/CMakeFiles/lib_bysant.dir/requires: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_common.c.o.requires
libs/bysant/CMakeFiles/lib_bysant.dir/requires: libs/bysant/CMakeFiles/lib_bysant.dir/bysant_class.c.o.requires
libs/bysant/CMakeFiles/lib_bysant.dir/requires: libs/bysant/CMakeFiles/lib_bysant.dir/__/common/awt_endian.c.o.requires
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/requires

libs/bysant/CMakeFiles/lib_bysant.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant && $(CMAKE_COMMAND) -P CMakeFiles/lib_bysant.dir/cmake_clean.cmake
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/clean

libs/bysant/CMakeFiles/lib_bysant.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/libs/bysant /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/libs/bysant/CMakeFiles/lib_bysant.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/bysant/CMakeFiles/lib_bysant.dir/depend

