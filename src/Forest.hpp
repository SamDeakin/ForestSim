#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "Skybox.hpp"

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

    // Transforms
    glm::mat4 m_P;
    glm::mat4 m_V(); // Function to aggregate the parts of the V matrix
    glm::mat4 m_V_origin; // Starting point
    glm::quat m_V_rot;
    glm::mat4 m_V_trans;
    glm::mat4 m_V_scale;
};
