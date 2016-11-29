#include "Forest.hpp"

#include "ShaderProgram.hpp"

#include "OpenGLImport.hpp"

#include <imgui/imgui.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

Forest::Forest() {
    // Empty
}

Forest::~Forest() {}

void Forest::init() {
    // Set the background colour.
    glClearColor(1.0, 1.0, 0.0, 1.0);

    // Build the shader
    m_shader.generateProgramObject();
    m_shader.attachVertexShader(
            getAssetFilePath("VertexShader.glsl").c_str());
    m_shader.attachFragmentShader(
            getAssetFilePath("FragmentShader.glsl").c_str());
    m_shader.link();

    m_skybox.init();
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

}

void Forest::cleanup() {

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

