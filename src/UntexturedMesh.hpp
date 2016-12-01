//
// Created by Sam Deakin on 2016-11-30.
//

#ifndef FORESTSIM_UNTEXTUREDMESH_HPP
#define FORESTSIM_UNTEXTUREDMESH_HPP

#include "MeshObject.hpp"

class UntexturedMesh: public MeshObject {
public:
    UntexturedMesh(std::string filepath);
    ~UntexturedMesh();

    void init(ShaderProgram *program) override;
    void render(glm::mat4 P, glm::mat4 V) override;
    ShaderType getShaderType() override;

private:
    ShaderProgram *program;
};


#endif //FORESTSIM_UNTEXTUREDMESH_HPP
