//
// Created by Sam Deakin on 2016-11-29.
//

#include "BasicGround.hpp"
#include "GlErrorCheck.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;
using namespace glm;

GLfloat raw_vertices[] = {
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f
};

GLfloat raw_normals[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
};

GLfloat raw_colour[] = {
        0.2275f, 0.3725f, 0.0431f,
        0.2275f, 0.3725f, 0.0431f,
        0.2275f, 0.3725f, 0.0431f,
        0.2275f, 0.3725f, 0.0431f,
        0.2275f, 0.3725f, 0.0431f,
        0.2275f, 0.3725f, 0.0431f
};

GLfloat raw_shininess[] = {
        20, 20, 20, 20, 20, 20
};

BasicGround::BasicGround() {
    program = NULL;
}

BasicGround::~BasicGround() {}


void BasicGround::init(ShaderProgram *program) {
    this->program = program;

    m_uniform_P = program->getUniformLocation("P");
    m_uniform_V = program->getUniformLocation("V");
    m_uniform_M_common = program->getUniformLocation("M_common");
    m_uniform_lightDirection = program->getUniformLocation("lightDirection");
    m_uniform_lightColour = program->getUniformLocation("lightColour");
    m_uniform_ambientIntensity = program->getUniformLocation("ambientIntensity");
    m_uniform_shadowMatrix = program->getUniformLocation("bias_P_V_shadow");
    m_uniform_shadowTexture = program->getUniformLocation("shadow");

    glGenVertexArrays(1, &m_VAO);
    // We could make this one call, but it's not
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_NBO);
    glGenBuffers(1, &m_MBO);
    glGenBuffers(1, &m_KdBO);
    glGenBuffers(1, &m_KsBO);
    glGenBuffers(1, &m_NsBO);

    glBindVertexArray(m_VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, raw_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(0, 0);

    // Model Transform
    // We basically want this plane to be inescapable
    mat4 modelTransform = glm::scale(mat4(1.0f), vec3(10000.0f, 10000.0f, 10000.0f));
    glBindBuffer(GL_ARRAY_BUFFER, m_MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), value_ptr(modelTransform), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 0 * 4));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 1 * 4));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 2 * 4));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 3 * 4));
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    // Normal Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, raw_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(5, 0);

    // Colour Buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_KdBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, raw_colour, GL_STATIC_DRAW);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(6, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_KsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, raw_colour, GL_STATIC_DRAW);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(7, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_NsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, raw_shininess, GL_STATIC_DRAW);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(8, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

void BasicGround::render(glm::mat4 P, glm::mat4 V, Light &light, glm::mat4 shadowMat, GLuint shadowTexture) {
    program->enable();

    glBindVertexArray(m_VAO);

    glUniformMatrix4fv(m_uniform_P, 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(m_uniform_V, 1, GL_FALSE, value_ptr(V));
    glUniformMatrix4fv(m_uniform_shadowMatrix, 1, GL_FALSE, value_ptr(shadowMat));

    // Bind the texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glBindTexture(GL_TEXTURE2, shadowTexture);
    glUniform1i(m_uniform_shadowTexture, 2);

    // This one is only needed because instanced models also use this shader
    mat4 M_common = mat4();
    glUniformMatrix4fv(m_uniform_M_common, 1, GL_FALSE, value_ptr(M_common));

    // Upload Light data
    vec4 lightDirection = V * vec4(light.lightDirection, 0.0f);
    glUniform3fv(m_uniform_lightDirection, 1, value_ptr(vec3(lightDirection)));
    glUniform3fv(m_uniform_lightColour, 1, value_ptr(light.lightColour));
    glUniform1f(m_uniform_ambientIntensity, light.ambientIntensity);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);

    glBindVertexArray(0);

    program->disable();

    CHECK_GL_ERRORS;
}

void BasicGround::renderForDepth(GLint uniform_M_common) {
    glBindVertexArray(m_VAO);

    mat4 M_common = mat4();
    glUniformMatrix4fv(m_uniform_M_common, 1, GL_FALSE, value_ptr(M_common));
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
    glBindVertexArray(0);
}

ShaderType BasicGround::getShaderType() {
    return ShaderType::PHUONG_UNTEXTURED;
}
