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
CMAKE_SOURCE_DIR = /home/marat/VUZ/OS/OS_lab2/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marat/VUZ/OS/OS_lab2/build

# Include any dependencies generated for this target.
include CMakeFiles/mainAsync.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mainAsync.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mainAsync.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mainAsync.dir/flags.make

CMakeFiles/mainAsync.dir/mainAsync.cpp.o: CMakeFiles/mainAsync.dir/flags.make
CMakeFiles/mainAsync.dir/mainAsync.cpp.o: /home/marat/VUZ/OS/OS_lab2/src/mainAsync.cpp
CMakeFiles/mainAsync.dir/mainAsync.cpp.o: CMakeFiles/mainAsync.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marat/VUZ/OS/OS_lab2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mainAsync.dir/mainAsync.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mainAsync.dir/mainAsync.cpp.o -MF CMakeFiles/mainAsync.dir/mainAsync.cpp.o.d -o CMakeFiles/mainAsync.dir/mainAsync.cpp.o -c /home/marat/VUZ/OS/OS_lab2/src/mainAsync.cpp

CMakeFiles/mainAsync.dir/mainAsync.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mainAsync.dir/mainAsync.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marat/VUZ/OS/OS_lab2/src/mainAsync.cpp > CMakeFiles/mainAsync.dir/mainAsync.cpp.i

CMakeFiles/mainAsync.dir/mainAsync.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mainAsync.dir/mainAsync.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marat/VUZ/OS/OS_lab2/src/mainAsync.cpp -o CMakeFiles/mainAsync.dir/mainAsync.cpp.s

# Object files for target mainAsync
mainAsync_OBJECTS = \
"CMakeFiles/mainAsync.dir/mainAsync.cpp.o"

# External object files for target mainAsync
mainAsync_EXTERNAL_OBJECTS =

mainAsync: CMakeFiles/mainAsync.dir/mainAsync.cpp.o
mainAsync: CMakeFiles/mainAsync.dir/build.make
mainAsync: CMakeFiles/mainAsync.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marat/VUZ/OS/OS_lab2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mainAsync"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mainAsync.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mainAsync.dir/build: mainAsync
.PHONY : CMakeFiles/mainAsync.dir/build

CMakeFiles/mainAsync.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mainAsync.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mainAsync.dir/clean

CMakeFiles/mainAsync.dir/depend:
	cd /home/marat/VUZ/OS/OS_lab2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marat/VUZ/OS/OS_lab2/src /home/marat/VUZ/OS/OS_lab2/src /home/marat/VUZ/OS/OS_lab2/build /home/marat/VUZ/OS/OS_lab2/build /home/marat/VUZ/OS/OS_lab2/build/CMakeFiles/mainAsync.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mainAsync.dir/depend

