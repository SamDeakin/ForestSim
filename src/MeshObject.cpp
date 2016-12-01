//
// Created by Sam Deakin on 2016-11-30.
//

#include "MeshObject.hpp"
#include "UntexturedMesh.hpp"
#include "ObjFileDecoder.hpp"

using namespace std;
using namespace glm;

MeshObject::~MeshObject() {}

MeshObject *MeshObject::MakeMeshObject(string objFile, int textured) {
    string fullname = string("Assets/") + objFile;

    if (textured) {
        return NULL;
    } else {
        return new UntexturedMesh(fullname);
    }
}

MeshObject::MeshObject() {}

void MeshObject::loadMesh() {
    ObjFileDecoder::decode(filepath.c_str(), objectName, positions, normals, uvCoords);
}
