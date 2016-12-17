project(GLSteeve) #name of the project
add_definitions(-std=c++11)

cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE shaderFiles "./*.glsl")
add_custom_target(shaders SOURCES ${shaderFiles})
INCLUDE_DIRECTORIES(third-party/include)
INCLUDE_DIRECTORIES(glimac/include)

add_subdirectory(glimac)

aux_source_directory(. SRC_LIST) #add all source file from current directory in the SRC_LIST variable
set(LIB_LIST mingw32 glimac SDL glew32s opengl32 glu32 freeglut) #add the libraries options in the LIB_LIST variable

add_executable(${PROJECT_NAME} ${SRC_LIST}) #set an executable from the file of SRC_LIST
target_link_libraries(${PROJECT_NAME} ${LIB_LIST}) #link the libraries with the executable
