# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/nando/rtmidi-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nando/rtmidi-master

# Include any dependencies generated for this target.
include CMakeFiles/rtmidi_static.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rtmidi_static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtmidi_static.dir/flags.make

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o: CMakeFiles/rtmidi_static.dir/flags.make
CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o: RtMidi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nando/rtmidi-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o -c /home/nando/rtmidi-master/RtMidi.cpp

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtmidi_static.dir/RtMidi.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nando/rtmidi-master/RtMidi.cpp > CMakeFiles/rtmidi_static.dir/RtMidi.cpp.i

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtmidi_static.dir/RtMidi.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nando/rtmidi-master/RtMidi.cpp -o CMakeFiles/rtmidi_static.dir/RtMidi.cpp.s

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.requires:

.PHONY : CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.requires

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.provides: CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.requires
	$(MAKE) -f CMakeFiles/rtmidi_static.dir/build.make CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.provides.build
.PHONY : CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.provides

CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.provides.build: CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o


CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o: CMakeFiles/rtmidi_static.dir/flags.make
CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o: rtmidi_c.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nando/rtmidi-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o -c /home/nando/rtmidi-master/rtmidi_c.cpp

CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nando/rtmidi-master/rtmidi_c.cpp > CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.i

CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nando/rtmidi-master/rtmidi_c.cpp -o CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.s

CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.requires:

.PHONY : CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.requires

CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.provides: CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.requires
	$(MAKE) -f CMakeFiles/rtmidi_static.dir/build.make CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.provides.build
.PHONY : CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.provides

CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.provides.build: CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o


# Object files for target rtmidi_static
rtmidi_static_OBJECTS = \
"CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o" \
"CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o"

# External object files for target rtmidi_static
rtmidi_static_EXTERNAL_OBJECTS =

librtmidi_static.a: CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o
librtmidi_static.a: CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o
librtmidi_static.a: CMakeFiles/rtmidi_static.dir/build.make
librtmidi_static.a: CMakeFiles/rtmidi_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nando/rtmidi-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library librtmidi_static.a"
	$(CMAKE_COMMAND) -P CMakeFiles/rtmidi_static.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtmidi_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtmidi_static.dir/build: librtmidi_static.a

.PHONY : CMakeFiles/rtmidi_static.dir/build

CMakeFiles/rtmidi_static.dir/requires: CMakeFiles/rtmidi_static.dir/RtMidi.cpp.o.requires
CMakeFiles/rtmidi_static.dir/requires: CMakeFiles/rtmidi_static.dir/rtmidi_c.cpp.o.requires

.PHONY : CMakeFiles/rtmidi_static.dir/requires

CMakeFiles/rtmidi_static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtmidi_static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtmidi_static.dir/clean

CMakeFiles/rtmidi_static.dir/depend:
	cd /home/nando/rtmidi-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nando/rtmidi-master /home/nando/rtmidi-master /home/nando/rtmidi-master /home/nando/rtmidi-master /home/nando/rtmidi-master/CMakeFiles/rtmidi_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtmidi_static.dir/depend
