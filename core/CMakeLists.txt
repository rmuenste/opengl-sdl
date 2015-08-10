include_directories(. ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

set(src_core 
basicshader.cpp  
basicshader.hpp  
sdl_gl_application.cpp
sdl_gl_application.hpp
shaderloader.cpp
shaderloader.hpp
textureloader.cpp
textureloader.hpp
texture.hpp
texture.cpp
transform.hpp
transform.cpp
perspectivetransform.hpp
perspectivetransform.cpp
camera.hpp
camera.cpp
)

add_library(core ${src_core})

#if(NOT WIN32)
#  target_link_libraries(core gl3w ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
#else(NOT WIN32)  
#  target_link_libraries(core gl3w ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})
#endif(NOT WIN32)


