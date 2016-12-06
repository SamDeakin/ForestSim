//
// Created by Sam Deakin on 2016-12-05.
//

#include "SSAORenderer.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <iostream>
#include <lodepng/lodepng.h>
#include "GlErrorCheck.hpp"

using namespace std;
using namespace glm;

void SSAORenderer::render(GLuint sceneTexture, GLuint depthTexture, GLint renderMode) {
    m_quad_program.enable();

    glBindVertexArray(m_quad_VAO);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, sceneTexture);
    glUniform1i(m_uniform_sceneTexture, 6);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(m_uniform_depthTexture, 1);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glUniform1i(m_uniform_noiseTexture, 5);

    glUniform2fv(m_uniform_kernel, 32, m_kernel.data());
    glUniform1i(m_uniform_renderMode, renderMode);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    m_quad_program.disable();
}

void SSAORenderer::init() {
    // Init shader
    m_quad_program.generateProgramObject();
    m_quad_program.attachVertexShader(
            getAssetFilePath("TextureRendererVS.glsl").c_str());
    m_quad_program.attachFragmentShader(
            getAssetFilePath("SSAORendererFS.glsl").c_str());
    m_quad_program.link();
    m_uniform_sceneTexture = m_quad_program.getUniformLocation("sceneTexture");
    m_uniform_renderMode = m_quad_program.getUniformLocation("renderMode");
    m_uniform_depthTexture = m_quad_program.getUniformLocation("depthTexture");
    m_uniform_kernel = m_quad_program.getUniformLocation("kernel");
    m_uniform_noiseTexture = m_quad_program.getUniformLocation("noiseTexture");

    // Init quad
    initQuadObject();

    mt19937 rng(time(NULL));
    uniform_real_distribution<float> gen(-1.0f, 1.0f);

    // Also init 32 by 32 texture for varying the coordinates, and fill it with noise texture
    std::vector<unsigned char> data;
    unsigned error;
    unsigned height;
    unsigned width;

    cout << "Loading noise texture" << endl;
    error = lodepng::decode(data, width, height, string("Assets/Noise.png"));
    if (error) {
        string s = string("Error: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    glGenTextures(1, &m_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Also fill the kernel
    for (int i = 0; i < 32; i++) {
        float x = gen(rng);
        float y = gen(rng);
        cout << x << " " << y << endl;
        m_kernel.push_back(x);
        m_kernel.push_back(y);
    }
}
