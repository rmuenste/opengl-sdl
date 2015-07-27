if(NOT WIN32)
  include_directories(. ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/gl3w/include  /usr/include/SDL2)
else(NOT WIN32)  
  include_directories(. ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR})
endif(NOT WIN32)

add_executable(singlepoint singlepoint.cpp)

if(NOT WIN32)
  target_link_libraries(singlepoint gl3w core ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
else(NOT WIN32)  
  target_link_libraries(singlepoint gl3w core ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})
endif(NOT WIN32)



