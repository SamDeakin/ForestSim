//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_CAMERA_HPP
#define FORESTSIM_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
    Camera();
    void init(int framebufferWidth, int framebufferHeight);
    ~Camera();

    glm::mat4 P();
    glm::mat4 V();

    void move(glm::vec3 dist);
    void rotate(glm::quat rx, glm::quat ry);
    void scale(glm::vec3 amount); // Probably not used

    void reset();
private:
    glm::mat4 m_P;
    glm::mat4 m_V_origin; // Starting point
    glm::quat m_V_rot;
    glm::mat4 m_V_trans;
    glm::mat4 m_V_scale;
};


#endif //FORESTSIM_CAMERA_HPP
