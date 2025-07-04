# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/jsonRPC/muduo-master/muduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/jsonRPC/muduo-master/build/release-cpp11

# Include any dependencies generated for this target.
include contrib/thrift/CMakeFiles/muduo_thrift.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include contrib/thrift/CMakeFiles/muduo_thrift.dir/compiler_depend.make

# Include the progress variables for this target.
include contrib/thrift/CMakeFiles/muduo_thrift.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/thrift/CMakeFiles/muduo_thrift.dir/flags.make

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o: contrib/thrift/CMakeFiles/muduo_thrift.dir/flags.make
contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o: /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftConnection.cc
contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o: contrib/thrift/CMakeFiles/muduo_thrift.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/jsonRPC/muduo-master/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o -MF CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o.d -o CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o -c /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftConnection.cc

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.i"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftConnection.cc > CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.i

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.s"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftConnection.cc -o CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.s

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o: contrib/thrift/CMakeFiles/muduo_thrift.dir/flags.make
contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o: /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftServer.cc
contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o: contrib/thrift/CMakeFiles/muduo_thrift.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/jsonRPC/muduo-master/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o -MF CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o.d -o CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o -c /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftServer.cc

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_thrift.dir/ThriftServer.cc.i"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftServer.cc > CMakeFiles/muduo_thrift.dir/ThriftServer.cc.i

contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_thrift.dir/ThriftServer.cc.s"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift/ThriftServer.cc -o CMakeFiles/muduo_thrift.dir/ThriftServer.cc.s

# Object files for target muduo_thrift
muduo_thrift_OBJECTS = \
"CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o" \
"CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o"

# External object files for target muduo_thrift
muduo_thrift_EXTERNAL_OBJECTS =

lib/libmuduo_thrift.a: contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftConnection.cc.o
lib/libmuduo_thrift.a: contrib/thrift/CMakeFiles/muduo_thrift.dir/ThriftServer.cc.o
lib/libmuduo_thrift.a: contrib/thrift/CMakeFiles/muduo_thrift.dir/build.make
lib/libmuduo_thrift.a: contrib/thrift/CMakeFiles/muduo_thrift.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/jsonRPC/muduo-master/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../lib/libmuduo_thrift.a"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && $(CMAKE_COMMAND) -P CMakeFiles/muduo_thrift.dir/cmake_clean_target.cmake
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/muduo_thrift.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/thrift/CMakeFiles/muduo_thrift.dir/build: lib/libmuduo_thrift.a
.PHONY : contrib/thrift/CMakeFiles/muduo_thrift.dir/build

contrib/thrift/CMakeFiles/muduo_thrift.dir/clean:
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift && $(CMAKE_COMMAND) -P CMakeFiles/muduo_thrift.dir/cmake_clean.cmake
.PHONY : contrib/thrift/CMakeFiles/muduo_thrift.dir/clean

contrib/thrift/CMakeFiles/muduo_thrift.dir/depend:
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/jsonRPC/muduo-master/muduo /root/workspace/jsonRPC/muduo-master/muduo/contrib/thrift /root/workspace/jsonRPC/muduo-master/build/release-cpp11 /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift /root/workspace/jsonRPC/muduo-master/build/release-cpp11/contrib/thrift/CMakeFiles/muduo_thrift.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contrib/thrift/CMakeFiles/muduo_thrift.dir/depend

