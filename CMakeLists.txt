cmake_minimum_required(VERSION 3.6)
project(ForestSim)

set(CMAKE_CXX_STANDARD 11)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORSE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORSE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORSE)

add_subdirectory(shared/glfw-3.1.1)

set(SOURCE_FILES
        main.cpp)
set(UTIL_SRC
        src/utils/GlErrorCheck.cpp
        src/utils/MeshConsolidator.cpp
        src/utils/ObjFileDecoder.cpp
        src/utils/ShaderProgram.cpp
        src/utils/Window.cpp)

add_executable(ForestSim ${SOURCE_FILES} ${UTIL_SRC})
target_include_directories(ForestSim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/utils)

find_package(OpenGL REQUIRED)
MESSAGE( STATUS "OpenGL: " ${OPENGL_INCLUDE_DIR})

# Only add include directory if not MacOS
# Cmake does a lot of wierd stuff to make .frameworks work on MacOS, so this call fails there
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(ForestSim ${OPENGL_INCLUDE_DIR})
endif()

target_include_directories(ForestSim PUBLIC "${CMAKE_SOURCE_DIR}/include/")

# Add Lua
set(LUA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared/lua-5.3.1/src)
target_include_directories(ForestSim PUBLIC ${LUA_DIRECTORY})
set(LUA_LIBRARY ${LUA_DIRECTORY}/liblua.a)
# create a custom target called build_lua that is part of ALL
# and will run each time you type make
add_custom_target(build_lua ALL
        COMMAND ${CMAKE_MAKE_PROGRAM}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared/lua-5.3.1/
        COMMENT "Original lua makefile target")

# now create an imported static target
add_library(lua STATIC IMPORTED)
# Import target "lua" for configuration
set_property(TARGET lua APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(lua PROPERTIES
        IMPORTED_LOCATION_NOCONFIG "${LUA_LIBRARY}")
add_dependencies(lua build_lua)

# Add gl3w
set(GL3W_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared/gl3w)

# Add imgui
set(IMGUI_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared/imgui)

target_include_directories(ForestSim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shared)

target_link_libraries(ForestSim glfw)
target_link_libraries(ForestSim ${OPENGL_gl_LIBRARY})
target_link_libraries(ForestSim lua)
