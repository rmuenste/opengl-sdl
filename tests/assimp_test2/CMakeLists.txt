include_directories(. ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/core 
  ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR} 
  ${SDL2_IMAGE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/assimp/include)

add_executable(assimp_test2 assimp_test2.cpp)

target_link_libraries(assimp_test2 Utility gl3w core assimp ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})


