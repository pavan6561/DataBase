# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pavan.balija/vsomeip_method

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pavan.balija/vsomeip_method/build

# Include any dependencies generated for this target.
include CMakeFiles/service.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/service.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/service.dir/flags.make

CMakeFiles/service.dir/service-example.cpp.o: CMakeFiles/service.dir/flags.make
CMakeFiles/service.dir/service-example.cpp.o: ../service-example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pavan.balija/vsomeip_method/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/service.dir/service-example.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/service.dir/service-example.cpp.o -c /home/pavan.balija/vsomeip_method/service-example.cpp

CMakeFiles/service.dir/service-example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/service.dir/service-example.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pavan.balija/vsomeip_method/service-example.cpp > CMakeFiles/service.dir/service-example.cpp.i

CMakeFiles/service.dir/service-example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/service.dir/service-example.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pavan.balija/vsomeip_method/service-example.cpp -o CMakeFiles/service.dir/service-example.cpp.s

# Object files for target service
service_OBJECTS = \
"CMakeFiles/service.dir/service-example.cpp.o"

# External object files for target service
service_EXTERNAL_OBJECTS =

service: CMakeFiles/service.dir/service-example.cpp.o
service: CMakeFiles/service.dir/build.make
service: /usr/local/lib/libvsomeip3.so.3.1.20
service: CMakeFiles/service.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pavan.balija/vsomeip_method/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable service"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/service.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/service.dir/build: service

.PHONY : CMakeFiles/service.dir/build

CMakeFiles/service.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/service.dir/cmake_clean.cmake
.PHONY : CMakeFiles/service.dir/clean

CMakeFiles/service.dir/depend:
	cd /home/pavan.balija/vsomeip_method/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pavan.balija/vsomeip_method /home/pavan.balija/vsomeip_method /home/pavan.balija/vsomeip_method/build /home/pavan.balija/vsomeip_method/build /home/pavan.balija/vsomeip_method/build/CMakeFiles/service.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/service.dir/depend

