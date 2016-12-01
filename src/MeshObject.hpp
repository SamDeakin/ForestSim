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
    virtual ~MeshObject();

    virtual void init(ShaderProgram *program) override = 0;
    virtual void render(glm::mat4 P, glm::mat4 V) override = 0;
    virtual ShaderType getShaderType() override = 0;

    // Use this to construct a MeshObject
    // This will create the correct subclass
    static MeshObject *MakeMeshObject(std::string objFile, int textured);

protected:
    MeshObject();

    // Populates the object attributes
    void loadMesh();
    std::string filepath;
    std::string objectName;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvCoords;
};


#endif //FORESTSIM_MESHOBJECT_HPP
