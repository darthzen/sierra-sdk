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

# Utility rule file for doxygen_gen.

# Include the progress variables for this target.
include doc/doxygen/CMakeFiles/doxygen_gen.dir/progress.make

doc/doxygen/CMakeFiles/doxygen_gen:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen && cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/doc/doxygen && doxygen /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/doc/doxygen/Doxyfile
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen && rm -rf /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen/C_User_API_doc
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen && mv /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/doc/doxygen/C_User_API_doc /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen

doxygen_gen: doc/doxygen/CMakeFiles/doxygen_gen
doxygen_gen: doc/doxygen/CMakeFiles/doxygen_gen.dir/build.make
.PHONY : doxygen_gen

# Rule to build all files generated by this target.
doc/doxygen/CMakeFiles/doxygen_gen.dir/build: doxygen_gen
.PHONY : doc/doxygen/CMakeFiles/doxygen_gen.dir/build

doc/doxygen/CMakeFiles/doxygen_gen.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen && $(CMAKE_COMMAND) -P CMakeFiles/doxygen_gen.dir/cmake_clean.cmake
.PHONY : doc/doxygen/CMakeFiles/doxygen_gen.dir/clean

doc/doxygen/CMakeFiles/doxygen_gen.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/doc/doxygen /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/doc/doxygen/CMakeFiles/doxygen_gen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/doxygen/CMakeFiles/doxygen_gen.dir/depend

