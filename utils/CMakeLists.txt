
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
quaternion.cpp
quaternion.h
mymath.cpp
mymath.h
matrix4x4.cpp
matrix4x4.h
)


# set a variable for the header files
#set(utilities_hdr dynamicarray.h
#log.h
#perftimer.h)
add_definitions(-std=c++11 -g)
add_library (Utility ${utilities_src})

