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

# Utility rule file for tester.

# Include the progress variables for this target.
include tests/core/CMakeFiles/tester.dir/progress.make

tests/core/CMakeFiles/tester: runtime/lua/tester/testMain.lua
tests/core/CMakeFiles/tester: runtime/lua/tester/testManager.lua
tests/core/CMakeFiles/tester: runtime/lua/tester/targetManager.lua
tests/core/CMakeFiles/tester: runtime/lua/tester/testerTools.lua
tests/core/CMakeFiles/tester: runtime/lua/tester/platformAPI.lua

runtime/lua/tester/testMain.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testMain.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tester/testMain.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testMain.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tester/testMain.lua

runtime/lua/tester/testManager.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testManager.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tester/testManager.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testManager.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tester/testManager.lua

runtime/lua/tester/targetManager.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/targetManager.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tester/targetManager.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/targetManager.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tester/targetManager.lua

runtime/lua/tester/testerTools.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testerTools.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tester/testerTools.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/testerTools.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tester/testerTools.lua

runtime/lua/tester/platformAPI.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/platformAPI.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/tester/platformAPI.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core/platformAPI.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/tester/platformAPI.lua

tester: tests/core/CMakeFiles/tester
tester: runtime/lua/tester/testMain.lua
tester: runtime/lua/tester/testManager.lua
tester: runtime/lua/tester/targetManager.lua
tester: runtime/lua/tester/testerTools.lua
tester: runtime/lua/tester/platformAPI.lua
tester: tests/core/CMakeFiles/tester.dir/build.make
.PHONY : tester

# Rule to build all files generated by this target.
tests/core/CMakeFiles/tester.dir/build: tester
.PHONY : tests/core/CMakeFiles/tester.dir/build

tests/core/CMakeFiles/tester.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core && $(CMAKE_COMMAND) -P CMakeFiles/tester.dir/cmake_clean.cmake
.PHONY : tests/core/CMakeFiles/tester.dir/clean

tests/core/CMakeFiles/tester.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/tests/core /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/tests/core/CMakeFiles/tester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/core/CMakeFiles/tester.dir/depend

