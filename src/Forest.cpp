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

    // Create the Phuong shader for untextured models
    m_phuong_untextured.generateProgramObject();
    m_phuong_untextured.attachVertexShader(
            getAssetFilePath("PhuongUntexturedVS.glsl").c_str());
    m_phuong_untextured.attachFragmentShader(
            getAssetFilePath("PhuongUntexturedFS.glsl").c_str());
    m_phuong_untextured.link();

    m_skybox.init();
    m_camera.init(m_framebufferWidth, m_framebufferHeight);
    m_ground.init(&m_phuong_untextured);
}

void Forest::appLogic() {
    double dx = 0;
    double dy = 0;
    double speed = 1.0;

    if (m_w_held) {
        dy += speed;
    }
    if (m_a_held) {
        dx += speed;
    }
    if (m_s_held) {
        dy -= speed;
    }
    if (m_d_held) {
        dx -= speed;
    }
    walkCamera(dx, dy);
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
    mat4 P = m_P();
    mat4 V = m_V();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_skybox.render(P, V);

    m_ground.render(P, V);
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
        // Nothing for now
    }

    if (mouse2_held()) {
        rotateCamera(dx, dy);
    }

    if (mouse3_held()) {
        moveCamera(dx, dy);
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
    bool eventHandled(false);

    zoomCamera(yOffSet);
    eventHandled = true;

    return eventHandled;
}

bool Forest::windowResizeEvent(int width, int height) {
    bool eventHandled(false);
    return eventHandled;
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
                eventHandled = true;
                break;
            case GLFW_KEY_W:
                m_w_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_A:
                m_a_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_S:
                m_s_held = true;
                eventHandled = true;
                break;
            case GLFW_KEY_D:
                m_d_held = true;
                eventHandled = true;
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
            case GLFW_KEY_W:
                m_w_held = false;
                eventHandled = true;
                break;
            case GLFW_KEY_A:
                m_a_held = false;
                eventHandled = true;
                break;
            case GLFW_KEY_S:
                m_s_held = false;
                eventHandled = true;
                break;
            case GLFW_KEY_D:
                m_d_held = false;
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
    quat ry = quat(vec3(dy * -0.005, 0.0, 0.0));
    m_camera.rotate(rx, ry);
}

void Forest::walkCamera(double dx, double dy) {
    m_camera.move(vec3(dx, 0.0f, -dy));
}

void Forest::zoomCamera(double dx) {
    // Implemented as moving forward (-z)
    m_camera.move(vec3(0.0f, 0.0f, float(dx) * 0.1f));
}

void Forest::moveCamera(double dx, double dy) {
    m_camera.move(vec3(float(dx) * 0.1f, -float(dy) * 0.1f, 0.0f));
}
