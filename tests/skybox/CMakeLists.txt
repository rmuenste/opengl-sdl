include_directories(. ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

add_executable(skybox skybox.cpp)

target_link_libraries(skybox  Utility gl3w core ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})



