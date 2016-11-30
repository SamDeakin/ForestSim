#include "Forest.hpp"

#include "ShaderProgram.hpp"

#include "OpenGLImport.hpp"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;
using namespace glm;

Forest::Forest() {
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

    m_P = perspective(
            radians(60.0f),
            float(m_framebufferWidth) / float(m_framebufferHeight),
            0.1f, 1000.0f);
    m_V_rot = quat();
    m_V_scale = mat4();
    m_V_trans = mat4();
    m_V_origin = lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
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
    m_skybox.render(m_P, m_V());
}

void Forest::cleanup() {

}

mat4 Forest::m_V() {
    // TODO is this the right order?
    return mat4_cast(m_V_rot) * m_V_scale * m_V_trans * m_V_origin;
}

//-- Virtual callback methods
bool Forest::cursorEnterWindowEvent(int entered) {

}

bool Forest::mouseMoveEvent(double xPos, double yPos) {

}

bool Forest::mouseButtonInputEvent(int button, int actions, int mods) {

}

bool Forest::mouseScrollEvent(double xOffSet, double yOffSet) {

}

bool Forest::windowResizeEvent(int width, int height) {

}

bool Forest::keyInputEvent(int key, int action, int mods) {

}

