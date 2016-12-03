//
// Created by Sam Deakin on 2016-12-02.
//

#ifndef FORESTSIM_TEXTURERENDERER_HPP
#define FORESTSIM_TEXTURERENDERER_HPP

#include "OpenGLImport.hpp"
#include "ShaderProgram.hpp"

#include <string>

class TextureRenderer {
public:
    virtual void render(GLint texture);
    virtual void init();

    std::string getAssetFilePath(std::string file);
protected:
    void initQuadObject();

    ShaderProgram m_quad_program;
    GLint m_uniform_sceneTexture;
    GLuint m_quad_VAO;
    GLuint m_quad_VBO;
};


#endif //FORESTSIM_TEXTURERENDERER_HPP
