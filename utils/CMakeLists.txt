cmake_minimum_required(VERSION 2.8)
CMAKE_POLICY(SET CMP0054 NEW)

# name of the project
PROJECT(Utility)

# set module directory
#SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_scripts/modules")

# allow easier if-else syntax
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

# output the system name
MESSAGE(STATUS "Configuring utils for a ${CMAKE_SYSTEM} system")

include_directories (. ${CMAKE_SOURCE_DIR}/../glm/glm)

# set a variable for the source code
set(utilities_src
3dmesh.cpp
3dmodel.cpp
basicmodelloader.cpp
objloader.cpp
triface.cpp
matrix3x3.cpp
aabb3.cpp
)


# set a variable for the header files
#set(utilities_hdr dynamicarray.h
#log.h
#perftimer.h)
add_definitions(-std=c++11 -g)
add_library (Utility ${utilities_src})

add_executable(test test.cpp)
target_link_libraries(test Utility)

