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

# Utility rule file for stagedb_lua.

# Include the progress variables for this target.
include luafwk/stagedb/CMakeFiles/stagedb_lua.dir/progress.make

luafwk/stagedb/CMakeFiles/stagedb_lua: runtime/lua/stagedb.lua

runtime/lua/stagedb.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/stagedb/stagedb.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/stagedb.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/stagedb && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/stagedb/stagedb.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/stagedb.lua

stagedb_lua: luafwk/stagedb/CMakeFiles/stagedb_lua
stagedb_lua: runtime/lua/stagedb.lua
stagedb_lua: luafwk/stagedb/CMakeFiles/stagedb_lua.dir/build.make
.PHONY : stagedb_lua

# Rule to build all files generated by this target.
luafwk/stagedb/CMakeFiles/stagedb_lua.dir/build: stagedb_lua
.PHONY : luafwk/stagedb/CMakeFiles/stagedb_lua.dir/build

luafwk/stagedb/CMakeFiles/stagedb_lua.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/stagedb && $(CMAKE_COMMAND) -P CMakeFiles/stagedb_lua.dir/cmake_clean.cmake
.PHONY : luafwk/stagedb/CMakeFiles/stagedb_lua.dir/clean

luafwk/stagedb/CMakeFiles/stagedb_lua.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/stagedb /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/stagedb /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/stagedb/CMakeFiles/stagedb_lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/stagedb/CMakeFiles/stagedb_lua.dir/depend

