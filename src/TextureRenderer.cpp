//
// Created by Sam Deakin on 2016-12-02.
//

#include "TextureRenderer.hpp"

const GLfloat raw_quad_vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
};

void TextureRenderer::init() {
    // Initialize shaders and gl objects for extra rendering
    m_quad_program.generateProgramObject();
    m_quad_program.attachVertexShader(
            getAssetFilePath("TextureRendererVS.glsl").c_str());
    m_quad_program.attachFragmentShader(
            getAssetFilePath("TextureRendererFS.glsl").c_str());
    m_quad_program.link();
    m_uniform_sceneTexture = m_quad_program.getUniformLocation("sceneTexture");

    initQuadObject();
}

void TextureRenderer::initQuadObject() {
    // Create our quad to be rendered
    // This should probably be in a class but fuck it
    glGenVertexArrays(1, &m_quad_VAO);
    glBindVertexArray(m_quad_VAO);

    glGenBuffers(1, &m_quad_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(raw_quad_vertices), raw_quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
}

void TextureRenderer::render(GLint texture) {
    // Now render to screen
    m_quad_program.enable();
    glBindVertexArray(m_quad_VAO);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(m_uniform_sceneTexture, 8);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_quad_program.disable();
}

std::string TextureRenderer::getAssetFilePath(std::string file) {
    return std::string("Assets/") + file;
}
