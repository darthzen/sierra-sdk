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

# Utility rule file for devtree2_install.

# Include the progress variables for this target.
include non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/progress.make

non-free/porting/devtree_v2/CMakeFiles/devtree2_install:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/non-free/porting/devtree_v2 && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/non-free/porting/devtree_v2/devtree2.map /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/resources/devtree2.map

devtree2_install: non-free/porting/devtree_v2/CMakeFiles/devtree2_install
devtree2_install: non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/build.make
.PHONY : devtree2_install

# Rule to build all files generated by this target.
non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/build: devtree2_install
.PHONY : non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/build

non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/non-free/porting/devtree_v2 && $(CMAKE_COMMAND) -P CMakeFiles/devtree2_install.dir/cmake_clean.cmake
.PHONY : non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/clean

non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/non-free/porting/devtree_v2 /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/non-free/porting/devtree_v2 /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : non-free/porting/devtree_v2/CMakeFiles/devtree2_install.dir/depend

