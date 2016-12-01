//
// Created by Sam Deakin on 2016-11-30.
//

#include "UntexturedMesh.hpp"

using namespace std;
using namespace glm;

UntexturedMesh::UntexturedMesh(std::string filepath) {
    this->filepath = filepath;
}

UntexturedMesh::~UntexturedMesh() {}

void UntexturedMesh::init(ShaderProgram *program) {
    loadMesh();
}

void UntexturedMesh::render(mat4 P, mat4 V) {
    // Unimplemented
}

ShaderType UntexturedMesh::getShaderType() {
    return ShaderType::PHUONG_UNTEXTURED;
}
