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

# Utility rule file for agent_provisioning.

# Include the progress variables for this target.
include agent/agent/CMakeFiles/agent_provisioning.dir/progress.make

agent/agent/CMakeFiles/agent_provisioning: runtime/lua/agent/provisioning.lua

runtime/lua/agent/provisioning.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent/provisioning.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/agent/provisioning.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/agent/agent && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent/provisioning.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/agent/provisioning.lua

agent_provisioning: agent/agent/CMakeFiles/agent_provisioning
agent_provisioning: runtime/lua/agent/provisioning.lua
agent_provisioning: agent/agent/CMakeFiles/agent_provisioning.dir/build.make
.PHONY : agent_provisioning

# Rule to build all files generated by this target.
agent/agent/CMakeFiles/agent_provisioning.dir/build: agent_provisioning
.PHONY : agent/agent/CMakeFiles/agent_provisioning.dir/build

agent/agent/CMakeFiles/agent_provisioning.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/agent/agent && $(CMAKE_COMMAND) -P CMakeFiles/agent_provisioning.dir/cmake_clean.cmake
.PHONY : agent/agent/CMakeFiles/agent_provisioning.dir/clean

agent/agent/CMakeFiles/agent_provisioning.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/agent/agent /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/agent/agent /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/agent/agent/CMakeFiles/agent_provisioning.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : agent/agent/CMakeFiles/agent_provisioning.dir/depend

