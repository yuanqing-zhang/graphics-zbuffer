cmake_minimum_required(VERSION 3.0)

project(OpenGL_TEST)

set( CMAKE_CXX_FLAGS "-std=c++11" )

# Eigen3
set(Eigen3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eigen-3.3.7/share/eigen3/cmake")
find_package(Eigen3 3.3.7 REQUIRED)
message(STATUS "Eigen3 path is ${EIGEN3_INCLUDE_DIRS}")
include_directories(${EIGEN3_INCLUDE_DIRS})


find_package(GLUT REQUIRED)
message(STATUS "GLUT path is ${GLUT_INCLUDE_DIRS}")
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL path is ${OpenGL_INCLUDE_DIR}")
include_directories(${OpenGL_INCLUDE_DIR})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OpenGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OpenGL_FOUND)

add_subdirectory(src)