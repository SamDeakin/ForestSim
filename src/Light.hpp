//
// Created by Sam Deakin on 2016-12-01.
//

#ifndef FORESTSIM_LIGHT_HPP
#define FORESTSIM_LIGHT_HPP

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 lightDirection;
    glm::vec3 lightColour;
    float ambientIntensity;
};


#endif //FORESTSIM_LIGHT_HPP
