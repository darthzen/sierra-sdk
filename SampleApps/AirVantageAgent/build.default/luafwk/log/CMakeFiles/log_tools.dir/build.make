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

# Utility rule file for log_tools.

# Include the progress variables for this target.
include luafwk/log/CMakeFiles/log_tools.dir/progress.make

luafwk/log/CMakeFiles/log_tools: runtime/lua/log/tools.lua

runtime/lua/log/tools.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/tools.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/log/tools.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/log && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log/tools.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/log/tools.lua

log_tools: luafwk/log/CMakeFiles/log_tools
log_tools: runtime/lua/log/tools.lua
log_tools: luafwk/log/CMakeFiles/log_tools.dir/build.make
.PHONY : log_tools

# Rule to build all files generated by this target.
luafwk/log/CMakeFiles/log_tools.dir/build: log_tools
.PHONY : luafwk/log/CMakeFiles/log_tools.dir/build

luafwk/log/CMakeFiles/log_tools.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/log && $(CMAKE_COMMAND) -P CMakeFiles/log_tools.dir/cmake_clean.cmake
.PHONY : luafwk/log/CMakeFiles/log_tools.dir/clean

luafwk/log/CMakeFiles/log_tools.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/log /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/log /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/log/CMakeFiles/log_tools.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/log/CMakeFiles/log_tools.dir/depend

