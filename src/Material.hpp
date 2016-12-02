//
// Created by Sam Deakin on 2016-12-01.
//

#ifndef FORESTSIM_MATERIAL_HPP
#define FORESTSIM_MATERIAL_HPP

#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 kd;
    glm::vec3 ks;
    float shininess;
};


#endif //FORESTSIM_MATERIAL_HPP
