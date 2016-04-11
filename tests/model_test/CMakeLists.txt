include_directories(. ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

add_executable(model_test model_test.cpp)

target_link_libraries(model_test  Utility gl3w core ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})



