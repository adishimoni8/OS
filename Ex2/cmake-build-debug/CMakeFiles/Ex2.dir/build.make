# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /home/guy/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/213.7172.20/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/guy/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/213.7172.20/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/guy/Documents/OS/Ex2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guy/Documents/OS/Ex2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Ex2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Ex2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Ex2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ex2.dir/flags.make

CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o: CMakeFiles/Ex2.dir/flags.make
CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o: ../Tests/test1.in.cpp
CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o: CMakeFiles/Ex2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o -MF CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o.d -o CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o -c /home/guy/Documents/OS/Ex2/Tests/test1.in.cpp

CMakeFiles/Ex2.dir/Tests/test1.in.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex2.dir/Tests/test1.in.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guy/Documents/OS/Ex2/Tests/test1.in.cpp > CMakeFiles/Ex2.dir/Tests/test1.in.cpp.i

CMakeFiles/Ex2.dir/Tests/test1.in.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex2.dir/Tests/test1.in.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guy/Documents/OS/Ex2/Tests/test1.in.cpp -o CMakeFiles/Ex2.dir/Tests/test1.in.cpp.s

CMakeFiles/Ex2.dir/Exception.cpp.o: CMakeFiles/Ex2.dir/flags.make
CMakeFiles/Ex2.dir/Exception.cpp.o: ../Exception.cpp
CMakeFiles/Ex2.dir/Exception.cpp.o: CMakeFiles/Ex2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Ex2.dir/Exception.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Ex2.dir/Exception.cpp.o -MF CMakeFiles/Ex2.dir/Exception.cpp.o.d -o CMakeFiles/Ex2.dir/Exception.cpp.o -c /home/guy/Documents/OS/Ex2/Exception.cpp

CMakeFiles/Ex2.dir/Exception.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex2.dir/Exception.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guy/Documents/OS/Ex2/Exception.cpp > CMakeFiles/Ex2.dir/Exception.cpp.i

CMakeFiles/Ex2.dir/Exception.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex2.dir/Exception.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guy/Documents/OS/Ex2/Exception.cpp -o CMakeFiles/Ex2.dir/Exception.cpp.s

CMakeFiles/Ex2.dir/QuantumTimer.cpp.o: CMakeFiles/Ex2.dir/flags.make
CMakeFiles/Ex2.dir/QuantumTimer.cpp.o: ../QuantumTimer.cpp
CMakeFiles/Ex2.dir/QuantumTimer.cpp.o: CMakeFiles/Ex2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Ex2.dir/QuantumTimer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Ex2.dir/QuantumTimer.cpp.o -MF CMakeFiles/Ex2.dir/QuantumTimer.cpp.o.d -o CMakeFiles/Ex2.dir/QuantumTimer.cpp.o -c /home/guy/Documents/OS/Ex2/QuantumTimer.cpp

CMakeFiles/Ex2.dir/QuantumTimer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex2.dir/QuantumTimer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guy/Documents/OS/Ex2/QuantumTimer.cpp > CMakeFiles/Ex2.dir/QuantumTimer.cpp.i

CMakeFiles/Ex2.dir/QuantumTimer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex2.dir/QuantumTimer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guy/Documents/OS/Ex2/QuantumTimer.cpp -o CMakeFiles/Ex2.dir/QuantumTimer.cpp.s

CMakeFiles/Ex2.dir/UThread.cpp.o: CMakeFiles/Ex2.dir/flags.make
CMakeFiles/Ex2.dir/UThread.cpp.o: ../UThread.cpp
CMakeFiles/Ex2.dir/UThread.cpp.o: CMakeFiles/Ex2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Ex2.dir/UThread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Ex2.dir/UThread.cpp.o -MF CMakeFiles/Ex2.dir/UThread.cpp.o.d -o CMakeFiles/Ex2.dir/UThread.cpp.o -c /home/guy/Documents/OS/Ex2/UThread.cpp

CMakeFiles/Ex2.dir/UThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex2.dir/UThread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guy/Documents/OS/Ex2/UThread.cpp > CMakeFiles/Ex2.dir/UThread.cpp.i

CMakeFiles/Ex2.dir/UThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex2.dir/UThread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guy/Documents/OS/Ex2/UThread.cpp -o CMakeFiles/Ex2.dir/UThread.cpp.s

CMakeFiles/Ex2.dir/uthreads.cpp.o: CMakeFiles/Ex2.dir/flags.make
CMakeFiles/Ex2.dir/uthreads.cpp.o: ../uthreads.cpp
CMakeFiles/Ex2.dir/uthreads.cpp.o: CMakeFiles/Ex2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Ex2.dir/uthreads.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Ex2.dir/uthreads.cpp.o -MF CMakeFiles/Ex2.dir/uthreads.cpp.o.d -o CMakeFiles/Ex2.dir/uthreads.cpp.o -c /home/guy/Documents/OS/Ex2/uthreads.cpp

CMakeFiles/Ex2.dir/uthreads.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex2.dir/uthreads.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guy/Documents/OS/Ex2/uthreads.cpp > CMakeFiles/Ex2.dir/uthreads.cpp.i

CMakeFiles/Ex2.dir/uthreads.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex2.dir/uthreads.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guy/Documents/OS/Ex2/uthreads.cpp -o CMakeFiles/Ex2.dir/uthreads.cpp.s

# Object files for target Ex2
Ex2_OBJECTS = \
"CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o" \
"CMakeFiles/Ex2.dir/Exception.cpp.o" \
"CMakeFiles/Ex2.dir/QuantumTimer.cpp.o" \
"CMakeFiles/Ex2.dir/UThread.cpp.o" \
"CMakeFiles/Ex2.dir/uthreads.cpp.o"

# External object files for target Ex2
Ex2_EXTERNAL_OBJECTS =

Ex2: CMakeFiles/Ex2.dir/Tests/test1.in.cpp.o
Ex2: CMakeFiles/Ex2.dir/Exception.cpp.o
Ex2: CMakeFiles/Ex2.dir/QuantumTimer.cpp.o
Ex2: CMakeFiles/Ex2.dir/UThread.cpp.o
Ex2: CMakeFiles/Ex2.dir/uthreads.cpp.o
Ex2: CMakeFiles/Ex2.dir/build.make
Ex2: CMakeFiles/Ex2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Ex2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ex2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ex2.dir/build: Ex2
.PHONY : CMakeFiles/Ex2.dir/build

CMakeFiles/Ex2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Ex2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Ex2.dir/clean

CMakeFiles/Ex2.dir/depend:
	cd /home/guy/Documents/OS/Ex2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guy/Documents/OS/Ex2 /home/guy/Documents/OS/Ex2 /home/guy/Documents/OS/Ex2/cmake-build-debug /home/guy/Documents/OS/Ex2/cmake-build-debug /home/guy/Documents/OS/Ex2/cmake-build-debug/CMakeFiles/Ex2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Ex2.dir/depend

