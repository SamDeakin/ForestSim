//
// Created by Sam Deakin on 2016-11-30.
//

#ifndef FORESTSIM_MESHOBJECT_HPP
#define FORESTSIM_MESHOBJECT_HPP

#include "RenderObject.hpp"
#include "Light.hpp"
#include <string>
#include <vector>

class MeshObject: public RenderObject {
public:
    MeshObject(std::string objFile, int density);
    virtual ~MeshObject();

    virtual void init(ShaderProgram *program) override;
    virtual void render(glm::mat4 P, glm::mat4 V, Light &light, glm::mat4 shadowMat, GLuint shadowTexture) override;
    virtual void renderForDepth(GLint uniform_M_common) override;
    virtual ShaderType getShaderType() override;

    void setScaleVariance(glm::vec3 variance) override;
    void setRotationVariance(glm::vec3 min, glm::vec3 max) override;
protected:
    // Populates the object attributes
    void loadMesh();
    std::string filepath;
    std::string objectName;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvCoords;
    std::vector<glm::vec3> Kd;
    std::vector<glm::vec3> Ks;
    std::vector<float> Ns;
    std::vector<glm::mat4> instances; // Model transforms for separate instances

    int density;
    glm::vec3 max_scale;
    glm::vec3 min_rot;
    glm::vec3 max_rot;
private:
    ShaderProgram *program;
    ShaderType type;

    GLint m_uniform_P;
    GLint m_uniform_V;
    GLint m_uniform_M_common;
    GLint m_uniform_lightDirection;
    GLint m_uniform_lightColour;
    GLint m_uniform_ambientIntensity;
    GLint m_uniform_shadowMatrix;
    GLint m_uniform_shadowTexture;
    GLint m_uniform_texture;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_NBO;
    GLuint m_MBO;
    GLuint m_KdBO;
    GLuint m_KsBO;
    GLuint m_NsBO;
    GLuint m_UVBO;

    GLuint m_texture;
    std::string m_textureFile;
    unsigned m_textureWidth;
    unsigned m_textureHeight;
};


#endif //FORESTSIM_MESHOBJECT_HPP
