//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_RENDEROBJECT_HPP
#define FORESTSIM_RENDEROBJECT_HPP

#include "ShaderProgram.hpp"
#include <glm/glm.hpp>

/*
 * A simple abstract class that is used to render an object
 * A sublass should take in a ShaderProgram and use it to render itself when render() is called
 */
class RenderObject {
public:
    RenderObject();
    virtual ~RenderObject();

    // One of these shaders should be used to render this object
    virtual void init(ShaderProgram *program) = 0;

    // Draw this object on screen
    virtual void render(glm::mat4 P, glm::mat4 V) = 0;
};


#endif //FORESTSIM_RENDEROBJECT_HPP
