# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/brandon/Dev/guitesting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/brandon/Dev/guitesting

# Include any dependencies generated for this target.
include src/swig/CMakeFiles/theme.dir/depend.make

# Include the progress variables for this target.
include src/swig/CMakeFiles/theme.dir/progress.make

# Include the compile flags for this target's objects.
include src/swig/CMakeFiles/theme.dir/flags.make

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o: src/swig/CMakeFiles/theme.dir/flags.make
src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o: src/swig/themeLUA_wrap.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/brandon/Dev/guitesting/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o"
	cd /home/brandon/Dev/guitesting/src/swig && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/theme.dir/themeLUA_wrap.cxx.o -c /home/brandon/Dev/guitesting/src/swig/themeLUA_wrap.cxx

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/theme.dir/themeLUA_wrap.cxx.i"
	cd /home/brandon/Dev/guitesting/src/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/brandon/Dev/guitesting/src/swig/themeLUA_wrap.cxx > CMakeFiles/theme.dir/themeLUA_wrap.cxx.i

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/theme.dir/themeLUA_wrap.cxx.s"
	cd /home/brandon/Dev/guitesting/src/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/brandon/Dev/guitesting/src/swig/themeLUA_wrap.cxx -o CMakeFiles/theme.dir/themeLUA_wrap.cxx.s

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.requires:
.PHONY : src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.requires

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.provides: src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.requires
	$(MAKE) -f src/swig/CMakeFiles/theme.dir/build.make src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.provides.build
.PHONY : src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.provides

src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.provides.build: src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o
.PHONY : src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.provides.build

src/swig/themeLUA_wrap.cxx: src/swig/theme.i
	$(CMAKE_COMMAND) -E cmake_progress_report /home/brandon/Dev/guitesting/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Swig source"
	cd /home/brandon/Dev/guitesting/src/swig && /usr/bin/swig -lua -c++ -outdir /home/brandon/Dev/guitesting/src/swig -c++ -o /home/brandon/Dev/guitesting/src/swig/themeLUA_wrap.cxx /home/brandon/Dev/guitesting/src/swig/theme.i

# Object files for target theme
theme_OBJECTS = \
"CMakeFiles/theme.dir/themeLUA_wrap.cxx.o"

# External object files for target theme
theme_EXTERNAL_OBJECTS =

src/swig/theme.so: src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o
src/swig/theme.so: src/swig/CMakeFiles/theme.dir/build.make
src/swig/theme.so: src/swig/CMakeFiles/theme.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module theme.so"
	cd /home/brandon/Dev/guitesting/src/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/theme.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/swig/CMakeFiles/theme.dir/build: src/swig/theme.so
.PHONY : src/swig/CMakeFiles/theme.dir/build

src/swig/CMakeFiles/theme.dir/requires: src/swig/CMakeFiles/theme.dir/themeLUA_wrap.cxx.o.requires
.PHONY : src/swig/CMakeFiles/theme.dir/requires

src/swig/CMakeFiles/theme.dir/clean:
	cd /home/brandon/Dev/guitesting/src/swig && $(CMAKE_COMMAND) -P CMakeFiles/theme.dir/cmake_clean.cmake
.PHONY : src/swig/CMakeFiles/theme.dir/clean

src/swig/CMakeFiles/theme.dir/depend: src/swig/themeLUA_wrap.cxx
	cd /home/brandon/Dev/guitesting && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/brandon/Dev/guitesting /home/brandon/Dev/guitesting/src/swig /home/brandon/Dev/guitesting /home/brandon/Dev/guitesting/src/swig /home/brandon/Dev/guitesting/src/swig/CMakeFiles/theme.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/swig/CMakeFiles/theme.dir/depend

