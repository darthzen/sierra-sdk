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

# Utility rule file for testspecific.

# Include the progress variables for this target.
include tests/customfiles/CMakeFiles/testspecific.dir/progress.make

tests/customfiles/CMakeFiles/testspecific: runtime/lua/agent/defaultconfig.lua

runtime/lua/agent/defaultconfig.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/customfiles/defaultconfig.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/agent/defaultconfig.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/tests/customfiles && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/customfiles/defaultconfig.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/runtime/lua/agent/defaultconfig.lua

testspecific: tests/customfiles/CMakeFiles/testspecific
testspecific: runtime/lua/agent/defaultconfig.lua
testspecific: tests/customfiles/CMakeFiles/testspecific.dir/build.make
.PHONY : testspecific

# Rule to build all files generated by this target.
tests/customfiles/CMakeFiles/testspecific.dir/build: testspecific
.PHONY : tests/customfiles/CMakeFiles/testspecific.dir/build

tests/customfiles/CMakeFiles/testspecific.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/tests/customfiles && $(CMAKE_COMMAND) -P CMakeFiles/testspecific.dir/cmake_clean.cmake
.PHONY : tests/customfiles/CMakeFiles/testspecific.dir/clean

tests/customfiles/CMakeFiles/testspecific.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/customfiles /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/tests/customfiles /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/tests/customfiles/CMakeFiles/testspecific.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/customfiles/CMakeFiles/testspecific.dir/depend

