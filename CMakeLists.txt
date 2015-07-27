project(SDL_GL)

cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

include(FindPkgConfig)

if(NOT WIN32)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
else(NOT WIN32 )
  find_package(SDL2 REQUIRED)
endif(NOT WIN32 )

find_package(OpenGL)
message(STATUS "OpenGL lib: ${OPENGL_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory(gl3w)
add_subdirectory(core)
add_subdirectory(tests)

