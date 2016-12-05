//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_BASICGROUND_HPP
#define FORESTSIM_BASICGROUND_HPP

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"
#include "Light.hpp"
#include <glm/glm.hpp>
#include <vector>

class BasicGround: public RenderObject {
public:
    BasicGround();
    virtual ~BasicGround();

    virtual void init(ShaderProgram *program) override;
    virtual void render(glm::mat4 P, glm::mat4 V, Light &light, glm::mat4 shadowMat, GLuint shadowTexture) override;
    void renderForDepth(GLint uniform_M_common) override;

    ShaderType getShaderType() override;
private:
    glm::mat4 M;

    ShaderProgram *program;
    GLint m_uniform_P;
    GLint m_uniform_V;
    GLint m_uniform_M_common;
    GLint m_uniform_lightDirection;
    GLint m_uniform_lightColour;
    GLint m_uniform_ambientIntensity;
    GLint m_uniform_shadowMatrix;
    GLint m_uniform_shadowTexture;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_NBO;
    GLuint m_MBO;
    GLuint m_KdBO;
    GLuint m_KsBO;
    GLuint m_NsBO;
};


#endif //FORESTSIM_BASICGROUND_HPP
