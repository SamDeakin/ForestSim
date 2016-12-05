#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include <tuple>

#include "OpenGLImport.hpp"

class ObjFileDecoder {
public:

    /**
    * Extracts vertex data from a Wavefront .obj file
    * If an object name parameter is present in the .obj file, objectName is set to that,
    * otherwise objectName is set to the name of the .obj file.
    *
    * Returns true if a texture is loaded into texdata, otherwise false
    * This implements a subset of the wavefront file format. The material handling is very limited.
    * We can handle either many materials with different kd/ks/shininess values, or one texture for the entire model
    *
    * [in] objFilePath - path to .obj file
    * [out] objectName - name given to object.
    * [out] positions - positions given in (x,y,z) model space.
    * [out] normals - normals given in (x,y,z) model space.
    * [out] uvCoords - texture coordinates in (u,v) parameter space.
    * [out] texdata - one texture that has been loaded into OpenGL already, covers the entire model
    * [out] kd - array of kd values, one for each vertex
    * [out] ks - array of ks values, one for each vertex
    * [out] shininess - array of shininess values, one for each vertex
    */
    static bool decode(
            const char * objFilePath,
            std::string & objectName,
            std::vector<glm::vec3> & positions,
            std::vector<glm::vec3> & normals,
            std::vector<glm::vec2> & uvCoords,
            std::string & texfile,
            std::vector<glm::vec3> & kd,
            std::vector<glm::vec3> & ks,
            std::vector<float> & shininess
    );

    // Deprecated
//	/**
//	* Extracts vertex data from a Wavefront .obj file
//	* If an object name parameter is present in the .obj file, objectName is set to that,
//	* otherwise objectName is set to the name of the .obj file.
//	*
//	* [in] objFilePath - path to .obj file
//	* [out] objectName - name given to object.
//	* [out] positions - positions given in (x,y,z) model space.
//	* [out] normals - normals given in (x,y,z) model space.
//	*/
//    static void decode(
//		    const char * objFilePath,
//			std::string & objectName,
//            std::vector<glm::vec3> & positions,
//            std::vector<glm::vec3> & normals
//    );

    /**
     * Extracts material data from an mtl file
     * Returns true if a texture was loaded, otherwise false
     *
     * [in] objFilePath - path to .obj file
     * [in] mtlReference - referenced path from the .obj file (Paths are relative so we require both)
     * [out] texdata - one texture that has been loaded into OpenGL already
     * [out] mtlData - map of material name to it's individual kd/ks/shininess values
     */
    static bool loadMTL(
            const char * objFilePath,
            const char * mtlReference,
            std::string & texfile,
            std::map<std::string,std::tuple<glm::vec3,glm::vec3,float>> & mtlData
    );

private:
    /**
     * Extracts the path without a filename
     * It will still end in a slash, or be empty
     * Eg.
     *     Assets/asdf.obj -> Assets/
     *     asdf.obj ->
     *
     * [in] filepath - the full path to the file
     * [out] path - the file path without the file name
     */
    static void pathMinusFilename(
            const char * filepath,
            std::string * path
    );
};


