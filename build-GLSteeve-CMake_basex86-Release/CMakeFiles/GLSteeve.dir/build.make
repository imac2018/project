# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.5

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\steev\Plus\IMAC\Git\project\GLSteeve

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release

# Include any dependencies generated for this target.
include CMakeFiles/GLSteeve.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GLSteeve.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GLSteeve.dir/flags.make

CMakeFiles/GLSteeve.dir/main.cpp.obj: CMakeFiles/GLSteeve.dir/flags.make
CMakeFiles/GLSteeve.dir/main.cpp.obj: CMakeFiles/GLSteeve.dir/includes_CXX.rsp
CMakeFiles/GLSteeve.dir/main.cpp.obj: C:/Users/steev/Plus/IMAC/Git/project/GLSteeve/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GLSteeve.dir/main.cpp.obj"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\GLSteeve.dir\main.cpp.obj -c C:\Users\steev\Plus\IMAC\Git\project\GLSteeve\main.cpp

CMakeFiles/GLSteeve.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLSteeve.dir/main.cpp.i"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\steev\Plus\IMAC\Git\project\GLSteeve\main.cpp > CMakeFiles\GLSteeve.dir\main.cpp.i

CMakeFiles/GLSteeve.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLSteeve.dir/main.cpp.s"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\steev\Plus\IMAC\Git\project\GLSteeve\main.cpp -o CMakeFiles\GLSteeve.dir\main.cpp.s

CMakeFiles/GLSteeve.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/GLSteeve.dir/main.cpp.obj.requires

CMakeFiles/GLSteeve.dir/main.cpp.obj.provides: CMakeFiles/GLSteeve.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\GLSteeve.dir\build.make CMakeFiles/GLSteeve.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/GLSteeve.dir/main.cpp.obj.provides

CMakeFiles/GLSteeve.dir/main.cpp.obj.provides.build: CMakeFiles/GLSteeve.dir/main.cpp.obj


# Object files for target GLSteeve
GLSteeve_OBJECTS = \
"CMakeFiles/GLSteeve.dir/main.cpp.obj"

# External object files for target GLSteeve
GLSteeve_EXTERNAL_OBJECTS =

GLSteeve.exe: CMakeFiles/GLSteeve.dir/main.cpp.obj
GLSteeve.exe: CMakeFiles/GLSteeve.dir/build.make
GLSteeve.exe: glimac/libglimac.a
GLSteeve.exe: CMakeFiles/GLSteeve.dir/linklibs.rsp
GLSteeve.exe: CMakeFiles/GLSteeve.dir/objects1.rsp
GLSteeve.exe: CMakeFiles/GLSteeve.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GLSteeve.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\GLSteeve.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GLSteeve.dir/build: GLSteeve.exe

.PHONY : CMakeFiles/GLSteeve.dir/build

CMakeFiles/GLSteeve.dir/requires: CMakeFiles/GLSteeve.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/GLSteeve.dir/requires

CMakeFiles/GLSteeve.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\GLSteeve.dir\cmake_clean.cmake
.PHONY : CMakeFiles/GLSteeve.dir/clean

CMakeFiles/GLSteeve.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\steev\Plus\IMAC\Git\project\GLSteeve C:\Users\steev\Plus\IMAC\Git\project\GLSteeve C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release C:\Users\steev\Plus\IMAC\Git\project\build-GLSteeve-CMake_basex86-Release\CMakeFiles\GLSteeve.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GLSteeve.dir/depend

