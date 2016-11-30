//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_BASICGROUND_HPP
#define FORESTSIM_BASICGROUND_HPP

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <vector>

class BasicGround: public RenderObject {
public:
    BasicGround();
    virtual ~BasicGround();

    virtual void init(ShaderProgram *program) override;
    virtual void render(glm::mat4 P, glm::mat4 V) override;
private:
    glm::mat4 M;

    ShaderProgram *program;
    GLint m_uniform_P;
    GLint m_uniform_V;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_NBO;
    GLuint m_CBO;
    GLuint m_MBO;
};


#endif //FORESTSIM_BASICGROUND_HPP
