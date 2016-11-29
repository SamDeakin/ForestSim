//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_SKYBOX_HPP
#define FORESTSIM_SKYBOX_HPP

#include <string>
#include <vector>

#include "ShaderProgram.hpp"

class Skybox {
public:
    // Init the skybox with a set of textures
    Skybox(std::string texture = "SkyboxSet1/TropicalSunnyDay/TropicalSunnyDay");
    ~Skybox();

    // Initialize
    void init();

    // Render the skybox
    void render();

private:
    // File name
    std::string texture;

    // Skybox specific shader

    // Skybox textures
    unsigned width;
    unsigned height;
    std::vector<unsigned char> back;
    std::vector<unsigned char> down;
    std::vector<unsigned char> front;
    std::vector<unsigned char> left;
    std::vector<unsigned char> right;
    std::vector<unsigned char> up;
};


#endif //FORESTSIM_SKYBOX_HPP
