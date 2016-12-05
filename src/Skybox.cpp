#include <lodepng/lodepng.h>
#include "Skybox.hpp"
#include "OpenGLImport.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace glm;

GLfloat skyboxCube[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

Skybox::Skybox(std::string texture) {
    this->texture_name = string("Assets/") + texture;
}

void Skybox::init() {
    // We start by creating our filename strings
    // We have no complicated searching logic, filenames have to follow this
    // They should also all have the same dimensions
    string backString = texture_name + "Back2048.png";
    string downString = texture_name + "Down2048.png";
    string frontString = texture_name + "Front2048.png";
    string leftString = texture_name + "Left2048.png";
    string rightString = texture_name + "Right2048.png";
    string upString = texture_name + "Up2048.png";

    // Load the files
    unsigned error;

    error = lodepng::decode(back, width, height, backString);
    if (error) {
        string s = string("back: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    // Now do that another 5 times... so shitty...
    error = lodepng::decode(down, width, height, downString);
    if (error) {
        string s = string("down: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(front, width, height, frontString);
    if (error) {
        string s = string("front: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(left, width, height, leftString);
    if (error) {
        string s = string("left: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(right, width, height, rightString);
    if (error) {
        string s = string("right: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(up, width, height, upString);
    if (error) {
        string s = string("up: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    // Load the shader program
    m_shader.generateProgramObject();
    m_shader.attachVertexShader("Assets/SkyboxVS.glsl");
    m_shader.attachFragmentShader("Assets/SkyboxFS.glsl");
    m_shader.link();

    // Get locations
    m_uniform_P = m_shader.getUniformLocation("P");
    m_uniform_V = m_shader.getUniformLocation("V");
    m_uniform_texture = m_shader.getUniformLocation("skybox");
    m_uniform_sun_colour = m_shader.getUniformLocation("sunColour");

    // Upload textures and stuff
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, back.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, down.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, front.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, left.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, right.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, up.data());
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Upload cube
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxCube), &skyboxCube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glBindVertexArray(0);
}

Skybox::~Skybox() {}

// Render the skybox
void Skybox::render(mat4 P, mat4 V, Light &light) {
    mat4 V_wout_trans = mat4(mat3(V));

    m_shader.enable();

    // We upload the sun colour to tint the skybox
    glUniform3fv(m_uniform_sun_colour, 1, value_ptr(light.lightColour));

    glEnable(GL_TEXTURE_CUBE_MAP);
    glDepthMask(GL_FALSE);
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glUniformMatrix4fv(m_uniform_P, 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(m_uniform_V, 1, GL_FALSE, value_ptr(V_wout_trans));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDisable(GL_TEXTURE_CUBE_MAP);
    glDepthMask(GL_TRUE);

    m_shader.disable();
}
