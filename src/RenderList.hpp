//
// Created by Sam Deakin on 2016-11-30.
//

#ifndef FORESTSIM_RENDERLIST_HPP
#define FORESTSIM_RENDERLIST_HPP

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <map>
#include <vector>

/*
 * A RenderList is simply a list of RenderObjects
 * init(map<shadertype,shader>) must be called to initialize all objects
 * they will all be rendered to the screen when render(P, V) is called
 */
class RenderList {
public:
    RenderList();

    // Note that destroying this also destroys all child RenderObjects
    ~RenderList();

    void addRenderObject(RenderObject *object);

    void init(std::map<ShaderType,ShaderProgram*> &shaders);
    void render(glm::mat4 P, glm::mat4 V);

    size_t size();
private:
    std::vector<RenderObject*> objects;
};


#endif //FORESTSIM_RENDERLIST_HPP
