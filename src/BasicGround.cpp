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

BasicGround::BasicGround() {
    program = NULL;
}

BasicGround::~BasicGround() {}


void BasicGround::init(ShaderProgram *program) {
    this->program = program;

    m_uniform_P = program->getUniformLocation("P");
    m_uniform_V = program->getUniformLocation("V");

    glGenVertexArrays(1, &m_VAO);
    // We could make this one call, but it's not
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_NBO);
    glGenBuffers(1, &m_CBO);
    glGenBuffers(1, &m_MBO);

    glBindVertexArray(m_VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6, &raw_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    // Normal Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(raw_normals), &raw_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    // Colour Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(raw_colour), &raw_colour, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    // Model Transform
    mat4 modelTransform = scale(mat4(1.0f), vec3(1000.0f, 1000.0f, 1000.0f));
    glBindBuffer(GL_ARRAY_BUFFER, m_MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), value_ptr(modelTransform), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 0 * 4));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 1 * 4));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 2 * 4));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * 3 * 4));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

void BasicGround::render(glm::mat4 P, glm::mat4 V) {
    program->enable();

    glBindVertexArray(m_VAO);

    glUniformMatrix4fv(m_uniform_P, 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(m_uniform_V, 1, GL_FALSE, value_ptr(V));

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);

    glBindVertexArray(0);

    program->disable();

    CHECK_GL_ERRORS;
}

ShaderType BasicGround::getShaderType() {
    return ShaderType::PHUONG_UNTEXTURED;
}
