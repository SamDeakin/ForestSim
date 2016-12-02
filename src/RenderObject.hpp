//
// Created by Sam Deakin on 2016-11-29.
//

#ifndef FORESTSIM_RENDEROBJECT_HPP
#define FORESTSIM_RENDEROBJECT_HPP

#include "ShaderProgram.hpp"
#include "Light.hpp"
#include <glm/glm.hpp>

enum class ShaderType {
    UNIMPLEMENTED = 0, // Use this type for objects that shouldn't be rendered
    PHUONG_TEXTURED = 1,
    PHUONG_UNTEXTURED = 2,
    CUSTOM = 3 // Use this type for objects that create their own specific shader
};

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
    virtual void render(glm::mat4 P, glm::mat4 V, Light &light) = 0;

    // Get the required shader type for this object
    virtual ShaderType getShaderType();

    // Functions for changing the model transform
    // A sublcass may or may not be affected by these
    void rotate(char axis, double amount);
    void scale(glm::vec3 amount);
    void translate(glm::vec3 amount);

    // Functions for setting variations of objects
    virtual void setScaleVariance(glm::vec3 variance);
    virtual void setRotationVariance(glm::vec3 min, glm::vec3 max);
protected:
    glm::mat4 M;
};


#endif //FORESTSIM_RENDEROBJECT_HPP
