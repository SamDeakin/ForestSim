//
// Created by Sam Deakin on 2016-11-30.
//

#include "RenderList.hpp"

using namespace std;
using namespace glm;

RenderList::RenderList() {}

RenderList::~RenderList() {
    for (auto object : objects) {
        delete object;
    }
}

void RenderList::addRenderObject(RenderObject *object) {
    objects.push_back(object);
}

void RenderList::init(std::map<ShaderType,ShaderProgram*> &shaders) {
    for (auto object : objects) {
        ShaderType type = object->getShaderType();
        if (type == ShaderType::UNIMPLEMENTED) {
            throw "RenderList::init: Passed ShaderType::UNIMPLEMENTED";
        } else if (type == ShaderType::CUSTOM) {
            // Just call init with NULL
            object->init(NULL);
        } else {
            // This will throw if the type is not found
            object->init(shaders.at(type));
        }
    }
}

void RenderList::render(mat4 P, mat4 V, Light &light) {
    for (auto object : objects) {
        object->render(P, V, light);
    }
}

size_t RenderList::size() {
    return objects.size();
}
