//
// Created by Sam Deakin on 2016-12-02.
//

#ifndef FORESTSIM_FXAARENDERER_HPP
#define FORESTSIM_FXAARENDERER_HPP

#include "TextureRenderer.hpp"
#include "OpenGLImport.hpp"

class FXAARenderer : public TextureRenderer {
public:
    void render(GLuint texture, GLint renderMode);
    void init() override;

private:
    GLint m_uniform_renderMode;
};


#endif //FORESTSIM_FXAARENDERER_HPP
