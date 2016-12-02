//
// Created by Sam Deakin on 2016-11-30.
//

#ifndef FORESTSIM_MESHOBJECT_HPP
#define FORESTSIM_MESHOBJECT_HPP

#include "RenderObject.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include <string>
#include <vector>

class MeshObject: public RenderObject {
public:
    MeshObject(std::string objFile, int density);
    virtual ~MeshObject();

    virtual void init(ShaderProgram *program) override;
    virtual void render(glm::mat4 P, glm::mat4 V, Light &light) override;
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
    GLint m_uniform_material_kd;
    GLint m_uniform_material_ks;
    GLint m_uniform_material_shine;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_NBO;
    GLuint m_CBO;
    GLuint m_MBO;

    Material mat;
};


#endif //FORESTSIM_MESHOBJECT_HPP
