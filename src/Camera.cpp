//
// Created by Sam Deakin on 2016-11-29.
//

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace glm;

Camera::Camera() {}

void Camera::init(int framebufferWidth, int framebufferHeight) {
    std::cout << framebufferWidth << " " << framebufferHeight << std::endl;
    m_P = perspective(
            radians(60.0f),
            float(framebufferWidth) / float(framebufferHeight),
            0.1f, 1000.0f);

    m_V_rot = quat();
    m_V_scale = mat4();
    m_V_trans = mat4();
    m_V_origin = lookAt(vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, 10.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera() {}

glm::mat4 Camera::P() {
    return m_P;
}

glm::mat4 Camera::V() {
    return mat4_cast(m_V_rot) * m_V_scale * m_V_trans * m_V_origin;
}

void Camera::move(vec3 dist) {
    vec4 adjustedDist = vec4(dist, 0.0f) * mat4_cast(m_V_rot);
    mat4 trans = translate(mat4(), vec3(adjustedDist));

    m_V_trans = trans * m_V_trans;
}

void Camera::rotate(quat rot_x, quat rot_y) {
    // This ordering is very specific
    // We want up and down rotation to take place relative to the current angle
    // But left/right should be relative to world
    m_V_rot = rot_y * m_V_rot * rot_x;
}

void Camera::scale(vec3 amount) {
    m_V_scale = glm::scale(m_V_scale, amount);
}
