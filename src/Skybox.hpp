//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_SKYBOX_HPP
#define FORESTSIM_SKYBOX_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "ShaderProgram.hpp"
#include "OpenGLImport.hpp"
#include "Light.hpp"

class Skybox {
public:
    // Init the skybox with a set of textures
    Skybox(std::string texture = "SkyboxSet1/TropicalSunnyDay/TropicalSunnyDay");
    ~Skybox();

    // Initialize
    void init();

    // Render the skybox
    void render(glm::mat4 P, glm::mat4 V, Light &light);

private:
    // File name
    std::string texture_name;

    // Skybox specific shader
    ShaderProgram m_shader;
    GLint m_uniform_P;
    GLint m_uniform_V;
    GLint m_uniform_texture;
    GLint m_uniform_sun_colour;

    // Skybox textures
    unsigned width;
    unsigned height;
    std::vector<unsigned char> back;
    std::vector<unsigned char> down;
    std::vector<unsigned char> front;
    std::vector<unsigned char> left;
    std::vector<unsigned char> right;
    std::vector<unsigned char> up;

    unsigned m_faces = 6;
    GLuint m_texture;
    GLuint m_VAO;
    GLuint m_VBO;
};


#endif //FORESTSIM_SKYBOX_HPP
