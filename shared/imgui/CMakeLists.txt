# Create the imgui library

set(IMGUI_SOURCE
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp)
add_library(imgui ${IMGUI_SOURCE})
