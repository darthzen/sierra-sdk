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
CMAKE_BINARY_DIR = /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default

# Utility rule file for test_modbusserializer.

# Include the progress variables for this target.
include luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/progress.make

luafwk/serialframework/CMakeFiles/test_modbusserializer: runtime/lua/tests/modbusserializer.lua

runtime/lua/tests/modbusserializer.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/serialframework/test/modbusserializer.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tests/modbusserializer.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/serialframework && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/serialframework/test/modbusserializer.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tests/modbusserializer.lua

test_modbusserializer: luafwk/serialframework/CMakeFiles/test_modbusserializer
test_modbusserializer: runtime/lua/tests/modbusserializer.lua
test_modbusserializer: luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/build.make
.PHONY : test_modbusserializer

# Rule to build all files generated by this target.
luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/build: test_modbusserializer
.PHONY : luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/build

luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/serialframework && $(CMAKE_COMMAND) -P CMakeFiles/test_modbusserializer.dir/cmake_clean.cmake
.PHONY : luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/clean

luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/serialframework /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/serialframework /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/serialframework/CMakeFiles/test_modbusserializer.dir/depend

