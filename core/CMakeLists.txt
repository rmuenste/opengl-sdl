include_directories(. ${CMAKE_SOURCE_DIR}/core ${CMAKE_SOURCE_DIR}/utils ${CMAKE_SOURCE_DIR}/gl3w/include  ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/assimp/include)

set(src_core 
basicshader.cpp  
basicshader.hpp  
sdl_gl_application.cpp
sdl_gl_application.hpp
shaderloader.cpp
shaderloader.hpp
simpleshader.cpp
simpleshader.hpp
textureloader.cpp
textureloader.hpp
texture.hpp
texture.cpp
light.hpp
light.cpp
gameobject.hpp
gameobject.cpp
transform.hpp
transform.cpp
phongshader.cpp  
phongshader.hpp  
phong_dir.cpp  
phong_dir.hpp  
phong.cpp  
phong.hpp  
shadowmapshader.cpp  
shadowmapshader.hpp  
normallines.cpp  
normallines.hpp  
textureshader.cpp  
textureshader.hpp  
twopassshadow.cpp  
twopassshadow.hpp  
rendermanager.hpp
rendermanager.cpp
texturemanager.hpp
texturemanager.cpp
material.cpp  
material.hpp  
perspectivetransform.hpp
perspectivetransform.cpp
camera.hpp
camera.cpp
mesh.cpp
mesh.hpp
resourcemanager.hpp
resourcemanager.cpp
)

add_library(core ${src_core})

#if(NOT WIN32)
target_link_libraries(core Utility assimp)
#else(NOT WIN32)  
#  target_link_libraries(core gl3w ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})
#endif(NOT WIN32)


