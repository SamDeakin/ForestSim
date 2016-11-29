#include "Forest.hpp"

#include <imgui/imgui.h>

Forest::Forest() {

}

Forest::~Forest() {}

void Forest::init() {

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

