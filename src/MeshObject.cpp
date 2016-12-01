//
// Created by Sam Deakin on 2016-11-30.
//

#include "MeshObject.hpp"
#include "ObjFileDecoder.hpp"
#include "GlErrorCheck.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

MeshObject::MeshObject(string filepath, int density) {
    this->filepath = string("Assets/") + filepath;
    cout << this->filepath << endl;
    this->loadMesh();

    this->density = density;

    // Check if this mesh is textured or untextured
    // TODO
    type = ShaderType::PHUONG_UNTEXTURED;

    // Log
    cout << "Loaded mesh: " << filepath << endl;
    cout << "    Vertices: " << positions.size() << endl;
    cout << "    Normals: " << normals.size() << endl;
    cout << "    uvCoords: " << uvCoords.size() << endl;
}

MeshObject::~MeshObject() {}

void MeshObject::loadMesh() {
    ObjFileDecoder::decode(filepath.c_str(), objectName, positions, normals, uvCoords);
}

void MeshObject::init(ShaderProgram *program) {
    this->program = program;

    // Init OpenGL objects
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    // Normal Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    if (type == ShaderType::PHUONG_UNTEXTURED) {
        // This is inefficient, but it has to be done
        vec3 colour = vec3(1.0f, 1.0f, 1.0f);
        vec3 *col_arr = new vec3[positions.size()];
        for (int i = 0; i < positions.size(); i++) {
            col_arr[i] = colour;
        }
        // Colour Buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), col_arr, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
        delete[] col_arr;
    }

    // Model Transform
    glBindBuffer(GL_ARRAY_BUFFER, m_MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), value_ptr(M), GL_STATIC_DRAW);
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

void MeshObject::render(glm::mat4 P, glm::mat4 V) {
    // Render the mesh
    program->enable();

    glBindVertexArray(m_VAO);

    glUniformMatrix4fv(m_uniform_P, 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(m_uniform_V, 1, GL_FALSE, value_ptr(V));

    glDrawArraysInstanced(GL_TRIANGLES, 0, positions.size(), 1);

    glBindVertexArray(0);

    program->disable();
}

ShaderType MeshObject::getShaderType() {
    return type;
}
