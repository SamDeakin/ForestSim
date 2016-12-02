//
// Created by Sam Deakin on 2016-11-29.
//

#include <glm/gtc/matrix_transform.hpp>
#include "RenderObject.hpp"

RenderObject::RenderObject() {
    M = glm::mat4();
}

RenderObject::~RenderObject() {}

ShaderType RenderObject::getShaderType() {
    return ShaderType::UNIMPLEMENTED;
}

void RenderObject::rotate(char axis, double amount) {
    glm::vec3 axis_vec3 = glm::vec3(0.0f);
    switch(axis) {
        case 'x':
            axis_vec3 = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        case 'y':
            axis_vec3 = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case 'z':
            axis_vec3 = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
    }
    M = glm::rotate(M, float(amount), axis_vec3);
}

void RenderObject::scale(glm::vec3 amount) {
    M = glm::scale(M, amount);
}

void RenderObject::translate(glm::vec3 amount) {
    M = glm::translate(M, amount);
}

// These are so subclasses can respond if needed
void RenderObject::setScaleVariance(glm::vec3 variance) {}
void RenderObject::setRotationVariance(glm::vec3 min, glm::vec3 max) {}
