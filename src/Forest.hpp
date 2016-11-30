#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "Skybox.hpp"
#include "Camera.hpp"

class Forest : public Window {
public:
    Forest();
    virtual ~Forest();

protected:
    virtual void init() override;
    virtual void appLogic() override;
    virtual void guiLogic() override;
    virtual void draw() override;
    virtual void cleanup() override;

    //-- Virtual callback methods
    virtual bool cursorEnterWindowEvent(int entered) override;
    virtual bool mouseMoveEvent(double xPos, double yPos) override;
    virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
    virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
    virtual bool windowResizeEvent(int width, int height) override;
    virtual bool keyInputEvent(int key, int action, int mods) override;

private:
    ShaderProgram m_shader;
    Skybox m_skybox;
    Camera m_camera;

    // Transforms
    glm::mat4 m_P();
    glm::mat4 m_V(); // Function to aggregate the parts of the V matrix

    // Movement interaction variables and functions
    double m_mousex;
    double m_mousey;

    bool mouse1_held();
    bool m_z_held;
    bool m_mouse1_held;

    bool mouse2_held();
    bool m_c_held;
    bool m_mouse2_held;

    bool mouse3_held();
    bool m_x_held;
    bool m_mouse3_held;
    void rotateCamera(double dx, double dy);
};
