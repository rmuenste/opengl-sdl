cmake_minimum_required(VERSION 2.8)
#CMAKE_POLICY(SET CMP0054 NEW)

project(SDL_GL)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )    

# allow easier if-else syntax
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

# output the system name
MESSAGE(STATUS "Configuring OpenGL-SDL for a ${CMAKE_SYSTEM} system")

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

find_package(OpenGL)
message(STATUS "Operating system: ${CMAKE_SYSTEM_NAME}  OpenGL lib: ${OPENGL_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

if(NOT EXISTS ${CMAKE_BINARY_DIR}/shaders)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders)
endif (NOT EXISTS ${CMAKE_BINARY_DIR}/shaders)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/meshes ${CMAKE_BINARY_DIR}/meshes)

add_subdirectory(utils)
add_subdirectory(gl3w)
add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(assimp)

