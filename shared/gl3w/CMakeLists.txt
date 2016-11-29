# Create the gl3w library

set(GL3W_SOURCE GL/gl3w.c)
add_library(gl3w ${GL3W_SOURCE})
target_include_directories(gl3w PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(gl3w PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
