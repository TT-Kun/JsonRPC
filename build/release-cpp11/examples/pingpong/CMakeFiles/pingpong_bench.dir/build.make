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
include examples/pingpong/CMakeFiles/pingpong_bench.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include examples/pingpong/CMakeFiles/pingpong_bench.dir/compiler_depend.make

# Include the progress variables for this target.
include examples/pingpong/CMakeFiles/pingpong_bench.dir/progress.make

# Include the compile flags for this target's objects.
include examples/pingpong/CMakeFiles/pingpong_bench.dir/flags.make

examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o: examples/pingpong/CMakeFiles/pingpong_bench.dir/flags.make
examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o: /root/workspace/jsonRPC/muduo-master/muduo/examples/pingpong/bench.cc
examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o: examples/pingpong/CMakeFiles/pingpong_bench.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/jsonRPC/muduo-master/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o -MF CMakeFiles/pingpong_bench.dir/bench.cc.o.d -o CMakeFiles/pingpong_bench.dir/bench.cc.o -c /root/workspace/jsonRPC/muduo-master/muduo/examples/pingpong/bench.cc

examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong_bench.dir/bench.cc.i"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/jsonRPC/muduo-master/muduo/examples/pingpong/bench.cc > CMakeFiles/pingpong_bench.dir/bench.cc.i

examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong_bench.dir/bench.cc.s"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/jsonRPC/muduo-master/muduo/examples/pingpong/bench.cc -o CMakeFiles/pingpong_bench.dir/bench.cc.s

# Object files for target pingpong_bench
pingpong_bench_OBJECTS = \
"CMakeFiles/pingpong_bench.dir/bench.cc.o"

# External object files for target pingpong_bench
pingpong_bench_EXTERNAL_OBJECTS =

bin/pingpong_bench: examples/pingpong/CMakeFiles/pingpong_bench.dir/bench.cc.o
bin/pingpong_bench: examples/pingpong/CMakeFiles/pingpong_bench.dir/build.make
bin/pingpong_bench: lib/libmuduo_net.a
bin/pingpong_bench: lib/libmuduo_base.a
bin/pingpong_bench: examples/pingpong/CMakeFiles/pingpong_bench.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/jsonRPC/muduo-master/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/pingpong_bench"
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pingpong_bench.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/pingpong/CMakeFiles/pingpong_bench.dir/build: bin/pingpong_bench
.PHONY : examples/pingpong/CMakeFiles/pingpong_bench.dir/build

examples/pingpong/CMakeFiles/pingpong_bench.dir/clean:
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong && $(CMAKE_COMMAND) -P CMakeFiles/pingpong_bench.dir/cmake_clean.cmake
.PHONY : examples/pingpong/CMakeFiles/pingpong_bench.dir/clean

examples/pingpong/CMakeFiles/pingpong_bench.dir/depend:
	cd /root/workspace/jsonRPC/muduo-master/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/jsonRPC/muduo-master/muduo /root/workspace/jsonRPC/muduo-master/muduo/examples/pingpong /root/workspace/jsonRPC/muduo-master/build/release-cpp11 /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong /root/workspace/jsonRPC/muduo-master/build/release-cpp11/examples/pingpong/CMakeFiles/pingpong_bench.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/pingpong/CMakeFiles/pingpong_bench.dir/depend

