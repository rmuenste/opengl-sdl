include_directories(. ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR})

add_executable(singlepoint singlepoint.cpp)

target_link_libraries(singlepoint gl3w core ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})



