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

# Utility rule file for agent_update_tools.

# Include the progress variables for this target.
include agent/agent/update/CMakeFiles/agent_update_tools.dir/progress.make

agent/agent/update/CMakeFiles/agent_update_tools: runtime/lua/agent/update/tools/createpkg.lua

runtime/lua/agent/update/tools/createpkg.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent/update/tools/createpkg.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../../runtime/lua/agent/update/tools/createpkg.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/agent/agent/update && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent/update/tools/createpkg.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/runtime/lua/agent/update/tools/createpkg.lua

agent_update_tools: agent/agent/update/CMakeFiles/agent_update_tools
agent_update_tools: runtime/lua/agent/update/tools/createpkg.lua
agent_update_tools: agent/agent/update/CMakeFiles/agent_update_tools.dir/build.make
.PHONY : agent_update_tools

# Rule to build all files generated by this target.
agent/agent/update/CMakeFiles/agent_update_tools.dir/build: agent_update_tools
.PHONY : agent/agent/update/CMakeFiles/agent_update_tools.dir/build

agent/agent/update/CMakeFiles/agent_update_tools.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/agent/agent/update && $(CMAKE_COMMAND) -P CMakeFiles/agent_update_tools.dir/cmake_clean.cmake
.PHONY : agent/agent/update/CMakeFiles/agent_update_tools.dir/clean

agent/agent/update/CMakeFiles/agent_update_tools.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent/update /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/agent/agent/update /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.arm/agent/agent/update/CMakeFiles/agent_update_tools.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : agent/agent/update/CMakeFiles/agent_update_tools.dir/depend

