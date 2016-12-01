//
// Created by Sam Deakin on 2016-11-30.
//

#include "MeshObject.hpp"
#include "ObjFileDecoder.hpp"

#include <iostream>

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
    // TODO
}

void MeshObject::render(glm::mat4 P, glm::mat4 V) {
    // Render the mesh
    // TODO
}

ShaderType MeshObject::getShaderType() {
    return type;
}
