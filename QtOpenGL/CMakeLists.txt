cmake_minimum_required(VERSION 2.8.5)
project(KIWK-I-MAC) #name of the project
add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fPIE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_WIDGETS_LIB -DQT_MULTIMEDIA_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_XML_LIB -DQT_CORE_LIB")

FILE(GLOB_RECURSE shaderFiles "shaders/*.glsl")
add_custom_target(shaders SOURCES ${shaderFiles})
INCLUDE_DIRECTORIES(third-party/assimp/include)
INCLUDE_DIRECTORIES(third-party/Qt/include)
INCLUDE_DIRECTORIES(third-party/Qt/include/QtOpenGL)
INCLUDE_DIRECTORIES(third-party/Qt/include/QtGui)
INCLUDE_DIRECTORIES(third-party/Qt/include/QtCore)
INCLUDE_DIRECTORIES(third-party/Qt/include/QtWidgets)
INCLUDE_DIRECTORIES(third-party/Qt/include/QtMultimedia)
INCLUDE_DIRECTORIES(include)

aux_source_directory(src SRC_LIST) #add all source file from current directory in the SRC_LIST variable
aux_source_directory(. SRC_LIST) #add all source file from current directory in the SRC_LIST variable

set(LIB_LIST -Lthird-party/Qt/lib -lQt5Gui -lQt5OpenGL -lQt5Core -lQt5DBus -lQt5Widgets -lQt5Multimedia -lQt5Xml -lGL -lm -lpthread -lassimp -Lplatforms -lqxcb
             -Wl,-rpath,third-party/Qt/lib -Wl,-rpath,third-party/assimp/lib -Wl,-rpath,platforms)

add_executable(${PROJECT_NAME} ${SRC_LIST}) #set an executable from the file of SRC_LIST
target_link_libraries(${PROJECT_NAME} ${LIB_LIST}) #link the libraries with the executable
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH true)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "third-party/Qt/lib;third-party/assimp/lib")
  