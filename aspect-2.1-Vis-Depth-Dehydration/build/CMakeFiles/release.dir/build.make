# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/pylith-2.2.1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /usr/local/pylith-2.2.1-linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build

# Utility rule file for release.

# Include the progress variables for this target.
include CMakeFiles/release.dir/progress.make

CMakeFiles/release:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "switching to RELEASE mode..."
	/usr/local/pylith-2.2.1-linux-x86_64/bin/cmake -D CMAKE_BUILD_TYPE=Release .
	/usr/local/pylith-2.2.1-linux-x86_64/bin/cmake -E echo "***"
	/usr/local/pylith-2.2.1-linux-x86_64/bin/cmake -E echo "*** Switched to Release mode. Now recompile with:  \$$ make"
	/usr/local/pylith-2.2.1-linux-x86_64/bin/cmake -E echo "***"

release: CMakeFiles/release
release: CMakeFiles/release.dir/build.make

.PHONY : release

# Rule to build all files generated by this target.
CMakeFiles/release.dir/build: release

.PHONY : CMakeFiles/release.dir/build

CMakeFiles/release.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/release.dir/cmake_clean.cmake
.PHONY : CMakeFiles/release.dir/clean

CMakeFiles/release.dir/depend:
	cd /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build /home/peach/Desktop/Yinuo/aspect-2.1-Vis-Depth/build/CMakeFiles/release.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/release.dir/depend

