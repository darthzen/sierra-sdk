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

# Utility rule file for socket.

# Include the progress variables for this target.
include luafwk/luasocket/CMakeFiles/socket.dir/progress.make

luafwk/luasocket/CMakeFiles/socket: runtime/lua/ltn12.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/mime.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket/http.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket/url.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket/ftp.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket/tp.lua
luafwk/luasocket/CMakeFiles/socket: runtime/lua/socket/smtp.lua

runtime/lua/ltn12.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/ltn12.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/ltn12.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/ltn12.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/ltn12.lua

runtime/lua/mime.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/mime.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/mime.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/mime.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/mime.lua

runtime/lua/socket.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket.lua

runtime/lua/socket/http.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/http.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket/http.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/http.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket/http.lua

runtime/lua/socket/url.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/url.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket/url.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/url.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket/url.lua

runtime/lua/socket/ftp.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/ftp.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket/ftp.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/ftp.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket/ftp.lua

runtime/lua/socket/tp.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/tp.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket/tp.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/tp.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket/tp.lua

runtime/lua/socket/smtp.lua: /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/smtp.lua
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../../runtime/lua/socket/smtp.lua"
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && /usr/bin/cmake -E copy_if_different /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket/socket/smtp.lua /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/runtime/lua/socket/smtp.lua

socket: luafwk/luasocket/CMakeFiles/socket
socket: runtime/lua/ltn12.lua
socket: runtime/lua/mime.lua
socket: runtime/lua/socket.lua
socket: runtime/lua/socket/http.lua
socket: runtime/lua/socket/url.lua
socket: runtime/lua/socket/ftp.lua
socket: runtime/lua/socket/tp.lua
socket: runtime/lua/socket/smtp.lua
socket: luafwk/luasocket/CMakeFiles/socket.dir/build.make
.PHONY : socket

# Rule to build all files generated by this target.
luafwk/luasocket/CMakeFiles/socket.dir/build: socket
.PHONY : luafwk/luasocket/CMakeFiles/socket.dir/build

luafwk/luasocket/CMakeFiles/socket.dir/clean:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket && $(CMAKE_COMMAND) -P CMakeFiles/socket.dir/cmake_clean.cmake
.PHONY : luafwk/luasocket/CMakeFiles/socket.dir/clean

luafwk/luasocket/CMakeFiles/socket.dir/depend:
	cd /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/avagent_r8m/luafwk/luasocket /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket /root/ws/SLQSrelease/SLQS04.00.08/SampleApps/AirVantageAgent/build.default/luafwk/luasocket/CMakeFiles/socket.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : luafwk/luasocket/CMakeFiles/socket.dir/depend
