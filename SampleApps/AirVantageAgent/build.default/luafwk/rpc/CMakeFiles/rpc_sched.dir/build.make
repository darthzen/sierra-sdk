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

# Utility rule file for rpc_sched.

# Include the progress variables for this target.
include luafwk/rpc/CMakeFiles/rpc_sched.dir/progress.make

luafwk/rpc/CMakeFiles/rpc_sched: runtime/lua/rpc/sched.lua

runtime/lua/rpc/sched.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/rpc/sched.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/rpc/sched.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/rpc && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/rpc/sched.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/rpc/sched.lua

rpc_sched: luafwk/rpc/CMakeFiles/rpc_sched
rpc_sched: runtime/lua/rpc/sched.lua
rpc_sched: luafwk/rpc/CMakeFiles/rpc_sched.dir/build.make
.PHONY : rpc_sched

# Rule to build all files generated by this target.
luafwk/rpc/CMakeFiles/rpc_sched.dir/build: rpc_sched
.PHONY : luafwk/rpc/CMakeFiles/rpc_sched.dir/build

luafwk/rpc/CMakeFiles/rpc_sched.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/rpc && $(CMAKE_COMMAND) -P CMakeFiles/rpc_sched.dir/cmake_clean.cmake
.PHONY : luafwk/rpc/CMakeFiles/rpc_sched.dir/clean

luafwk/rpc/CMakeFiles/rpc_sched.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/rpc /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/rpc /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/rpc/CMakeFiles/rpc_sched.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/rpc/CMakeFiles/rpc_sched.dir/depend

