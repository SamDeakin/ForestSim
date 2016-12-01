//
// Created by Sam Deakin on 2016-11-30.
//

#ifndef FORESTSIM_MESHOBJECT_HPP
#define FORESTSIM_MESHOBJECT_HPP

#include "RenderObject.hpp"
#include <string>
#include <vector>

class MeshObject: public RenderObject {
public:
    MeshObject(std::string objFile, int density);
    virtual ~MeshObject();

    virtual void init(ShaderProgram *program) override;
    virtual void render(glm::mat4 P, glm::mat4 V) override;
    virtual ShaderType getShaderType() override;

protected:
    // Populates the object attributes
    void loadMesh();
    std::string filepath;
    std::string objectName;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvCoords;

    int density;
private:
    ShaderProgram *program;
    ShaderType type;
};


#endif //FORESTSIM_MESHOBJECT_HPP
