#include "Forest.hpp"

#include "ShaderProgram.hpp"
#include "RenderList.hpp"

#include "OpenGLImport.hpp"
#include "GlErrorCheck.hpp"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <map>
#include <GlErrorCheck.hpp>
#include <OpenGL/OpenGL.h>

using namespace std;
using namespace glm;

#define DEPTH_TEXTURE_SIZE 8192

Forest::Forest() :
    m_z_held(false),
    m_x_held(false),
    m_c_held(false),
    m_mouse1_held(false),
    m_mouse2_held(false),
    m_mouse3_held(false),
    m_w_held(false),
    m_a_held(false),
    m_s_held(false),
    m_d_held(false),
    m_shift_held(false),
    m_FXAA_enabled(true),
    m_FXAA_renderMode(3),
    m_skybox_enabled(true),
    m_shadow_enabled(true),
    m_show_shadow(false) {
    // Empty
}

Forest::~Forest() {
    if (list) {
        delete list;
    }
}

Forest::Forest(RenderList *list) : Forest() {
    this->list = list;
}

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

    light.ambientIntensity = 0.2f;
    light.lightColour = vec3(1.0f, 1.0f, 1.0f);
    light.lightDirection = normalize(vec3(0.0f, -1.0f, -1.0f));
    m_P_light = glm::ortho(-1500.0f, 1500.0f, -1500.0f, 2750.0f, -500.0f, 2500.0f);
    m_V_light = glm::lookAt(-light.lightDirection * 1000.0f, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    m_to_tex = glm::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f
    );

    // Initialize our shadow shader
    m_depth_only.generateProgramObject();
    m_depth_only.attachVertexShader(
            getAssetFilePath("DepthOnlyVS.glsl").c_str());
    m_depth_only.attachFragmentShader(
            getAssetFilePath("DepthOnlyFS.glsl").c_str());
    m_depth_only.link();
    m_uniform_shadow_M_common = m_depth_only.getUniformLocation("M_common");
    m_uniform_shadow_V = m_depth_only.getUniformLocation("V");
    m_uniform_shadow_P = m_depth_only.getUniformLocation("P");

    if (list) {
        // Create shader list for rendered objects
        map<ShaderType,ShaderProgram*> shaders;
        shaders.insert(pair<ShaderType,ShaderProgram*>(ShaderType::PHUONG_UNTEXTURED, &m_phuong_untextured));

        list->init(shaders);
    }

    // Initialize the extra frame buffer
    glGenFramebuffers(1, &m_scene_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_scene_FBO);

    glGenTextures(1, &m_sceneTexture);
    glBindTexture(GL_TEXTURE_2D, m_sceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // TODO change these to filter better later
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenRenderbuffers(1, &m_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_sceneTexture, 0);

    GLenum toBeDrawn = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &toBeDrawn);

    CHECK_GL_ERRORS;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw "ERROR: Framebuffer created incorrectly";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Initialize the shadow render buffer stuff
    glGenFramebuffers(1, &m_shadow_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_FBO);

    glGenTextures(1, &m_shadow_tex);
    glBindTexture(GL_TEXTURE_2D, m_shadow_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    CHECK_GL_ERRORS;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw "ERROR: Framebuffer created incorrectly";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate our lowest depth value texture
    glGenTextures(1, &m_lowestDepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_lowestDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    // Set that one pixel to -1;
    float pixels = 1;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixels);
    CHECK_GL_ERRORS;

    glBindTexture(GL_TEXTURE_2D, 0);

    m_quadRenderer.init();
    m_FXAA_renderer.init();
}

void Forest::appLogic() {
    double dx = 0;
    double dy = 0;
    double speed = 1.0;
    if (m_shift_held) {
        speed = 10.0;
    }

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

    ImGui::InputInt("FXAA debug setting", &m_FXAA_renderMode, 1, 1);
    if (m_FXAA_renderMode > 4) {
        m_FXAA_renderMode = 4;
    } else if (m_FXAA_renderMode < 0) {
        m_FXAA_renderMode = 0;
    }

    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
}

void Forest::draw() {
    mat4 P = m_P();
    mat4 V = m_V();
    mat4 bias_P_V_shadow = m_to_tex * m_P_light * m_V_light;

    glEnable(GL_DEPTH_TEST);

    // First we draw to our shadow FBO
    // Bind the FBO
    if (!m_show_shadow) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_FBO);
        glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    m_depth_only.enable();
    // Upload our uniforms
    glUniformMatrix4fv(m_uniform_shadow_P, 1, GL_FALSE, value_ptr(m_P_light));
    glUniformMatrix4fv(m_uniform_shadow_V, 1, GL_FALSE, value_ptr(m_V_light));

    m_ground.renderForDepth(m_uniform_shadow_M_common);
    if (list) {
        list->renderForDepth(m_uniform_shadow_M_common);
    }
    m_depth_only.disable();

    if (!m_show_shadow) {
        // Second we draw to our FBO
        glBindFramebuffer(GL_FRAMEBUFFER, m_scene_FBO);
        glViewport(0, 0, 1920, 1080);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_skybox_enabled) {
            m_skybox.render(P, V);
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);

        GLuint shadow_texture = m_shadow_tex;
        if (!m_shadow_enabled) {
            shadow_texture = m_lowestDepthTexture;
        }

        m_ground.render(P, V, light, bias_P_V_shadow, shadow_texture);

        if (list) {
            list->render(P, V, light, bias_P_V_shadow, shadow_texture);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (m_FXAA_enabled) {
            m_FXAA_renderer.render(m_sceneTexture, m_FXAA_renderMode);
        } else {
            m_quadRenderer.render(m_sceneTexture);
        }
    }
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
    return false;
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
        if (m_shift_held) {
            moveCamera(dx * 10, dy * 10);
        } else {
            moveCamera(dx, dy);
        }
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

    if (m_shift_held) {
        yOffSet *= 10;
    }

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
            case GLFW_KEY_R:
                resetPosition();
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
            case GLFW_KEY_F:
                m_FXAA_enabled = !m_FXAA_enabled;
                eventHandled = true;
                break;
            case GLFW_KEY_EQUAL:
                m_FXAA_renderMode++;
                eventHandled = true;
                break;
            case GLFW_KEY_MINUS:
                m_FXAA_renderMode--;
                eventHandled = true;
                break;
            case GLFW_KEY_B:
                m_skybox_enabled = !m_skybox_enabled;
                eventHandled = true;
                break;
            case GLFW_KEY_M:
                m_shadow_enabled = !m_shadow_enabled;
                eventHandled = true;
                break;
            case GLFW_KEY_N:
                m_show_shadow = !m_show_shadow;
                eventHandled = true;
                break;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                m_shift_held = true;
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
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                m_shift_held = false;
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
    m_camera.move(vec3(dx, 0.0f, dy));
}

void Forest::zoomCamera(double dx) {
    // Implemented as moving forward (-z)
    m_camera.move(vec3(0.0f, 0.0f, float(dx) * 0.1f));
}

void Forest::moveCamera(double dx, double dy) {
    m_camera.move(vec3(float(dx) * 0.1f, -float(dy) * 0.1f, 0.0f));
}

void Forest::resetPosition() {
    m_camera.reset();
}
