//
// Created by Sam Deakin on 2016-11-30.
//

#include "MeshObject.hpp"
#include "ObjFileDecoder.hpp"
#include "GlErrorCheck.hpp"

#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <lodepng/lodepng.h>

using namespace std;
using namespace glm;

MeshObject::MeshObject(string filepath, int density) {
    this->filepath = string("Assets/") + filepath;
    cout << this->filepath << endl;
    this->loadMesh();

    this->density = density;

    // Log
    cout << "Loaded mesh: " << filepath << endl;
    cout << "    Vertices: " << positions.size() << endl;
    cout << "    Normals: " << normals.size() << endl;
    cout << "    uvCoords: " << uvCoords.size() << endl;

    max_scale = vec3(1.0f);
    min_rot = vec3(0.0f);
    max_rot = vec3(0.0f);
}

MeshObject::~MeshObject() {}

void MeshObject::loadMesh() {
    bool textured = ObjFileDecoder::decode(filepath.c_str(), objectName, positions, normals, uvCoords, m_textureFile, Kd, Ks, Ns);

    if (textured) {
        type = ShaderType::PHUONG_TEXTURED;
    } else {
        type = ShaderType::PHUONG_UNTEXTURED;
    }
}

void MeshObject::init(ShaderProgram *program) {
    this->program = program;

    // Here we generate model transforms for all of our instances
    // To hopefully make sure our random numbers are different
    // Seeds are hard
    // Two models with the same z on the first vector and number of vertices will still collide
    mt19937 rng(time(NULL) * positions.size() * positions[0][2]);
    uniform_real_distribution<float> positionrng(-1000, 1000);
    uniform_real_distribution<float> scalerng(1, max_scale.x);
//    uniform_real_distribution<float> rotxrng(min_rot.x, max_rot.x);
    uniform_real_distribution<float> rotyrng(min_rot.y, max_rot.y);
//    uniform_real_distribution<float> rotzrng(min_rot.z, max_rot.z);
    for (int i = 0; i < density; i++) {
        // Just generate that many instances w/ different model transforms
        mat4 instance = mat4();

        // Change rotation
        // We allow separate rotations like this
        quat rotation = quat();
//        rotation = glm::rotate(rotation, rotxrng(rng), vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, rotyrng(rng), vec3(0.0f, 1.0f, 0.0f));
//        rotation = glm::rotate(rotation, rotzrng(rng), vec3(0.0f, 0.0f, 1.0f));
        instance = mat4_cast(rotation) * instance;

        // Change position
        instance = glm::translate(instance, vec3(positionrng(rng), 0.0f, positionrng(rng)));

        // Change scale
        // Changing each dimension separately would be weird, so we just do once for all
        float scaleFactor = scalerng(rng);
        instance = glm::scale(instance, vec3(scaleFactor));

        instances.push_back(instance);
    }

    // Init OpenGL objects
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
    glGenBuffers(1, &m_UVBO);

    glBindVertexArray(m_VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    // Model Transform
    glBindBuffer(GL_ARRAY_BUFFER, m_MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * instances.size(), instances.data(), GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    if (type == ShaderType::PHUONG_UNTEXTURED) {
        glBindBuffer(GL_ARRAY_BUFFER, m_KdBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * Kd.size(), &Kd[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    } else {
        // Bind the uv array
        glBindBuffer(GL_ARRAY_BUFFER, m_UVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvCoords.size(), &uvCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        // Load in the texture
        std::vector<unsigned char> data;
        unsigned error;
        cout << "Loading " << m_textureFile << endl;

        error = lodepng::decode(data, m_textureWidth, m_textureHeight, m_textureFile);
        if (error) {
            string s = string("Error: ") + lodepng_error_text(error);
            cout << s << endl;
            throw s;
        }

        // Now we upload to a texture
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        m_uniform_texture = program->getUniformLocation("Kd");
    }

    // We upload Ks and Ns for both types anyways
    glBindBuffer(GL_ARRAY_BUFFER, m_KsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * Ks.size(), &Ks[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, m_NsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Ns.size(), &Ns[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

void MeshObject::render(glm::mat4 P, glm::mat4 V, Light &light, glm::mat4 shadowMat, GLuint shadowTexture) {
    // Render the mesh
    program->enable();

    glBindVertexArray(m_VAO);
    glCullFace(GL_NONE);
    glDisable(GL_CULL_FACE);

    glUniformMatrix4fv(m_uniform_P, 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(m_uniform_V, 1, GL_FALSE, value_ptr(V));
    glUniformMatrix4fv(m_uniform_M_common, 1, GL_FALSE, value_ptr(M));
    glUniformMatrix4fv(m_uniform_shadowMatrix, 1, GL_FALSE, value_ptr(shadowMat));

    // Bind the texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glBindTexture(GL_TEXTURE2, shadowTexture);
    glUniform1i(m_uniform_shadowTexture, 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindTexture(GL_TEXTURE3, m_texture);
    glUniform1i(m_uniform_texture, 3);

    // Upload Light info
    vec4 lightDirection = V * vec4(light.lightDirection, 0.0f);
    glUniform3fv(m_uniform_lightDirection, 1, value_ptr(vec3(lightDirection)));
    glUniform3fv(m_uniform_lightColour, 1, value_ptr(light.lightColour));
    glUniform1f(m_uniform_ambientIntensity, light.ambientIntensity);

    glDrawArraysInstanced(GL_TRIANGLES, 0, positions.size(), instances.size());

    glBindVertexArray(0);

    program->disable();
}

void MeshObject::renderForDepth(GLint uniform_M_common) {
    // Skip all the extra colour and shader setup steps, just upload M and draw
    glUniformMatrix4fv(uniform_M_common, 1, GL_FALSE, value_ptr(M));
    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, positions.size(), instances.size());
    glBindVertexArray(0);
}

ShaderType MeshObject::getShaderType() {
    return type;
}

void MeshObject::setScaleVariance(glm::vec3 variance) {
    max_scale = variance;
}

void MeshObject::setRotationVariance(glm::vec3 min, glm::vec3 max) {
    min_rot = min;
    max_rot = max;
}

