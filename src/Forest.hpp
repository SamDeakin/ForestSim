#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "Skybox.hpp"
#include "Camera.hpp"
#include "BasicGround.hpp"
#include "RenderList.hpp"
#include "Light.hpp"

class Forest : public Window {
public:
    Forest();
    virtual ~Forest();

    // Constructor to init with a scene of objects
    Forest(RenderList *list);

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
    BasicGround m_ground;

    // Extra shaders
    ShaderProgram m_phuong_untextured;

    // For rendering to FBO and then to screen in quad after
    ShaderProgram m_quad_program;
    GLuint m_scene_FBO;
    GLuint m_sceneTexture;
    GLuint m_depthBuffer;
    GLint m_uniform_sceneTexture;
    GLuint m_quad_VAO;
    GLuint m_quad_VBO;

    // Transforms
    glm::mat4 m_P();
    glm::mat4 m_V(); // Function to aggregate the parts of the V matrix

    // Objects to be rendered
    Light light;
    RenderList *list;

    // Movement interaction variables and functions
    double m_mousex;
    double m_mousey;

    bool m_w_held;
    bool m_a_held;
    bool m_s_held;
    bool m_d_held;
    void walkCamera(double dx, double dy);

    bool m_shift_held;
    void zoomCamera(double dx);

    bool mouse1_held();
    bool m_z_held;
    bool m_mouse1_held;
    // No movement action here

    bool mouse2_held();
    bool m_c_held;
    bool m_mouse2_held;
    void rotateCamera(double dx, double dy);

    bool mouse3_held();
    bool m_x_held;
    bool m_mouse3_held;
    void moveCamera(double dx, double dy);

    void resetPosition();
};
