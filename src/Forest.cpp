#include "Forest.hpp"

#include "ShaderProgram.hpp"

#include "OpenGLImport.hpp"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

using namespace std;
using namespace glm;

Forest::Forest() :
    m_z_held(false),
    m_x_held(false),
    m_c_held(false),
    m_mouse1_held(false),
    m_mouse2_held(false),
    m_mouse3_held(false) {
    // Empty
}

Forest::~Forest() {}

void Forest::init() {
    // Set the background colour.
    glClearColor(1.0, 0.0, 1.0, 1.0);

    // Build the shader
    m_shader.generateProgramObject();
    m_shader.attachVertexShader(
            getAssetFilePath("VertexShader.glsl").c_str());
    m_shader.attachFragmentShader(
            getAssetFilePath("FragmentShader.glsl").c_str());
    m_shader.link();

    m_skybox.init();
    m_camera.init(m_framebufferWidth, m_framebufferHeight);
}

void Forest::appLogic() {

}

void Forest::guiLogic() {
//    if( !show_gui ) {
//        return;
//    }

    static bool firstRun(true);
    static bool showDebugWindow(true);
    if (firstRun) {
        ImGui::SetNextWindowPos(ImVec2(50, 50));
        firstRun = false;
    }
    ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
    float opacity(0.5f);

    ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
    if( ImGui::Button( "Quit Application" ) ) {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }

    ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
    ImGui::End();
}

void Forest::draw() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_skybox.render(m_P(), m_V());
}

void Forest::cleanup() {

}

mat4 Forest::m_P() {
    return m_camera.P();
}

mat4 Forest::m_V() {
    return m_camera.V();
}

//-- Virtual callback methods
bool Forest::cursorEnterWindowEvent(int entered) {

}

bool Forest::mouseMoveEvent(double xPos, double yPos) {
    bool eventHandled(false);

    double dx = m_mousex - xPos;
    double dy = m_mousey - yPos;

    if (mouse1_held()) {

    }

    if (mouse2_held()) {
        rotateCamera(dx, dy);
    }

    if (mouse3_held()) {

    }

    // Do this after so interaction functions have access to new and old coordinates
    m_mousex = xPos;
    m_mousey = yPos;

    return eventHandled;
}

bool Forest::mouseButtonInputEvent(int button, int actions, int mods) {
    bool eventHandled(false);

    if (!ImGui::IsMouseHoveringAnyWindow()) {
        // First change held buttons
        if (actions == GLFW_PRESS) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    m_mouse1_held = true;
                    break;
                case GLFW_MOUSE_BUTTON_MIDDLE:
                case GLFW_MOUSE_BUTTON_4: // Because 3 is wierd on osx
                    m_mouse3_held = true;
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    m_mouse2_held = true;
                    break;
            }
        } else if (actions == GLFW_RELEASE) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    m_mouse1_held = false;
                    break;
                case GLFW_MOUSE_BUTTON_MIDDLE:
                case GLFW_MOUSE_BUTTON_4:
                    m_mouse3_held = false;
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    m_mouse2_held = false;
                    break;
            }

        }
    }

    return eventHandled;
}

bool Forest::mouseScrollEvent(double xOffSet, double yOffSet) {

}

bool Forest::windowResizeEvent(int width, int height) {

}

bool Forest::keyInputEvent(int key, int action, int mods) {
    bool eventHandled(false);

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_Z:
                m_z_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_X:
                m_x_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_C:
                m_c_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(m_window, GL_TRUE);
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_Z:
                m_z_held = false;
                eventHandled = true;
                break;
            case GLFW_KEY_X:
                m_x_held = false;
                eventHandled = true;
                break;
            case GLFW_KEY_C:
                m_c_held = false;
                eventHandled = true;
                break;
            default:
                break;
        }
    }

    return eventHandled;
}

bool Forest::mouse1_held() {
    return m_mouse1_held || m_z_held;
}
bool Forest::mouse2_held() {
    return m_mouse2_held || m_c_held;
}
bool Forest::mouse3_held() {
    return m_mouse3_held || m_x_held;
}

void Forest::rotateCamera(double dx, double dy) {
    quat rx = quat(vec3(0.0, dx * -0.005, 0.0));
    quat ry = quat(vec3(dy * 0.005, 0.0, 0.0));
    m_camera.rotate(rx, ry);
}
