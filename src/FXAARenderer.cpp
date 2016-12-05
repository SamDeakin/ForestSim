//
// Created by Sam Deakin on 2016-12-02.
//

#include "FXAARenderer.hpp"

void FXAARenderer::render(GLuint texture, GLint renderMode) {
    m_quad_program.enable();
    glBindVertexArray(m_quad_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(m_uniform_sceneTexture, 0);
    glUniform1i(m_uniform_renderMode, renderMode);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_quad_program.disable();
}

void FXAARenderer::init() {
    // Init shader
    m_quad_program.generateProgramObject();
    m_quad_program.attachVertexShader(
            getAssetFilePath("TextureRendererVS.glsl").c_str());
    m_quad_program.attachFragmentShader(
            getAssetFilePath("FXAARendererFS.glsl").c_str());
    m_quad_program.link();
    m_uniform_sceneTexture = m_quad_program.getUniformLocation("sceneTexture");
    m_uniform_renderMode = m_quad_program.getUniformLocation("renderMode");

    // Init quad
    initQuadObject();
}
