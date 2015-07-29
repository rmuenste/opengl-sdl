project(SDL_GL)

cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

if(NOT WIN32)
  find_package(SDL2 REQUIRED)
else(NOT WIN32 )
  find_package(SDL2 REQUIRED)
endif(NOT WIN32 )

find_package(OpenGL)
message(STATUS "OpenGL lib: ${OPENGL_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/shaders)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders)
endif (NOT EXISTS ${CMAKE_BINARY_DIR}/shaders)


add_subdirectory(gl3w)
add_subdirectory(core)
add_subdirectory(tests)

