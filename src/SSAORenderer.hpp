//
// Created by Sam Deakin on 2016-12-05.
//

#ifndef FORESTSIM_SSAORENDERER_HPP
#define FORESTSIM_SSAORENDERER_HPP

#include "OpenGLImport.hpp"
#include "TextureRenderer.hpp"
#include <glm/glm.hpp>
#include <vector>

class SSAORenderer : public TextureRenderer {
public:
    void render(GLuint sceneTexture, GLuint depthTexture, GLint renderMode);
    void init() override;

private:
    GLint m_uniform_renderMode;
    GLint m_uniform_depthTexture;
    GLint m_uniform_kernel;
    GLint m_uniform_noiseTexture;

    GLuint m_noiseTexture;

    std::vector<glm::vec2> m_kernel;
};


#endif //FORESTSIM_SSAORENDERER_HPP
