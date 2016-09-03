include_directories(./include)

add_library(gl3w src/gl3w.c)

if (UNIX)
  target_link_libraries(gl3w dl)
else()
  target_link_libraries(gl3w ${OPENGL_LIBRARIES})
endif()
