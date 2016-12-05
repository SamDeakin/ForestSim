#include "ObjFileDecoder.hpp"
using namespace glm;

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <glm/gtx/string_cast.hpp>
using namespace std;

#include "Exception.hpp"

const vec3 defaultKd = vec3(1.0f, 1.0f, 1.0f);
const vec3 defaultKs = vec3(1.0f, 1.0f, 1.0f);
const float defaultNs = 100;

//---------------------------------------------------------------------------------------
bool ObjFileDecoder::decode(
        const char * objFilePath,
        std::string & objectName,
        std::vector<glm::vec3> & positions,
        std::vector<glm::vec3> & normals,
        std::vector<glm::vec2> & uvCoords,
        GLint & texdata,
        std::vector<glm::vec3> & kd,
        std::vector<glm::vec3> & ks,
        std::vector<float> & shininess
) {

	// Empty containers, and start fresh before inserting data from .obj file
	positions.clear();
	normals.clear();
	uvCoords.clear();

    ifstream in(objFilePath, std::ios::in);
    in.exceptions(std::ifstream::badbit);

    if (!in) {
        stringstream errorMessage;
        errorMessage << "Unable to open .obj file " << objFilePath
            << " within method ObjFileDecoder::decode" << endl;

        throw Exception(errorMessage.str().c_str());
    }

    string currentLine;
    int positionIndexA, positionIndexB, positionIndexC;
    int normalIndexA, normalIndexB, normalIndexC;
    int uvCoordIndexA, uvCoordIndexB, uvCoordIndexC;
    vector<vec3> temp_positions;
    vector<vec3> temp_normals;
    vector<vec2> temp_uvCoords;

	objectName = "";

    std::map<std::string,std::tuple<glm::vec3,glm::vec3,float>> materials;
    string currentMaterial = "";

    bool textured = false;

    while (!in.eof()) {
        try {
            getline(in, currentLine);
        } catch (const ifstream::failure &e) {
            in.close();
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw Exception(errorMessage.str());
        }
	    if (currentLine.substr(0, 2) == "o ") {
		    // Get entire line excluding first 2 chars.
		    istringstream s(currentLine.substr(2));
		    s >> objectName;

	    } else if (currentLine.substr(0, 2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(currentLine.substr(2));
            glm::vec3 vertex;
            s >> vertex.x;
            s >> vertex.y;
            s >> vertex.z;
            temp_positions.push_back(vertex);

        } else if (currentLine.substr(0, 3) == "vn ") {
            // Normal data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(currentLine.substr(2));
            vec3 normal;
            s >> normal.x;
            s >> normal.y;
            s >> normal.z;
            temp_normals.push_back(normal);

        } else if (currentLine.substr(0, 3) == "vt ") {
            // Texture coordinate data on this line.
            // Get entire line excluding first 2 chars.
            istringstream s(currentLine.substr(2));
            vec2 textureCoord;
            s >> textureCoord.s;
            s >> textureCoord.t;
            temp_uvCoords.push_back(textureCoord);

        } else if (currentLine.substr(0, 7) == "mtllib ") {
            // Load the material data
            istringstream s(currentLine.substr(7));
            string mtlfile;
            s >> mtlfile;

            textured = loadMTL(objFilePath, mtlfile.c_str(), texdata, materials);
        } else if (currentLine.substr(0, 7) == "usemtl ") {
            // Set the current material
            istringstream s(currentLine.substr(7));
            string mat;
            s >> mat;

            currentMaterial = mat;
        } else if (currentLine.substr(0, 2) == "f ") {
            // Face index data on this line.

            int index;

            // sscanf will return the number of matched index values it found
            // from the pattern.
            int numberOfIndexMatches = sscanf(currentLine.c_str(), "f %d/%d/%d",
                                              &index, &index, &index);

            if (numberOfIndexMatches == 3) {
                // Line contains indices of the pattern vertex/uv-cord/normal.
                sscanf(currentLine.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &positionIndexA, &uvCoordIndexA, &normalIndexA,
                       &positionIndexB, &uvCoordIndexB, &normalIndexB,
                       &positionIndexC, &uvCoordIndexC, &normalIndexC);



                // Adjust if they're negative
                if (uvCoordIndexA < 0) {
                    int uv_size = temp_uvCoords.size();

                    uvCoordIndexA = uv_size + uvCoordIndexA;
                    uvCoordIndexB = uv_size + uvCoordIndexB;
                    uvCoordIndexC = uv_size + uvCoordIndexC;
                } else {
                    // .obj file uses indices that start at 1, so subtract 1 so they start at 0.
                    uvCoordIndexA--;
                    uvCoordIndexB--;
                    uvCoordIndexC--;
                }

                uvCoords.push_back(temp_uvCoords[uvCoordIndexA]);
                uvCoords.push_back(temp_uvCoords[uvCoordIndexB]);
                uvCoords.push_back(temp_uvCoords[uvCoordIndexC]);

            } else if (numberOfIndexMatches == 1) {
                // Line contains just vertices
                // Get their positions and generate some face normals for them
                sscanf(currentLine.c_str(), "f %d %d %d",
                        &positionIndexA, &positionIndexB, &positionIndexC);

                // Create some face normals for each vertex
                vec3 norm = normalize(cross(temp_positions[positionIndexB - 1] - temp_positions[positionIndexA - 1], temp_positions[positionIndexC - 1] - temp_positions[positionIndexA - 1]));
                temp_normals.push_back(norm);
                normalIndexA = normalIndexB = normalIndexC = temp_normals.size();
            } else {
                // Line contains indices of the pattern vertex//normal.
                sscanf(currentLine.c_str(), "f %d//%d %d//%d %d//%d",
		               &positionIndexA, &normalIndexA,
                       &positionIndexB, &normalIndexB,
                       &positionIndexC, &normalIndexC);
            }

            // Here we adjust them if the indices are negative
            // If one is negative we assume they all will be
            if (positionIndexA < 0) {
                int position_size = temp_positions.size();
                int normal_size = temp_normals.size();

                positionIndexA = position_size + positionIndexA;
                positionIndexB = position_size + positionIndexB;
                positionIndexC = position_size + positionIndexC;
                normalIndexA = normal_size + normalIndexA;
                normalIndexB = normal_size + normalIndexB;
                normalIndexC = normal_size + normalIndexC;
            } else {
                positionIndexA--;
                positionIndexB--;
                positionIndexC--;
                normalIndexA--;
                normalIndexB--;
                normalIndexC--;
            }

            positions.push_back(temp_positions[positionIndexA]);
            positions.push_back(temp_positions[positionIndexB]);
            positions.push_back(temp_positions[positionIndexC]);

            normals.push_back(temp_normals[normalIndexA]);
            normals.push_back(temp_normals[normalIndexB]);
            normals.push_back(temp_normals[normalIndexC]);

            // We also add the material too
            // We push three times so the data can be uploaded as one value per vertex without modification
            auto iterator = materials.find(currentMaterial);
            if (iterator == materials.end()) {
                // If the material is not found then push the default
                for (int i = 0; i < 3; i++) {
                    kd.push_back(defaultKd);
                    ks.push_back(defaultKs);
                    shininess.push_back(defaultNs);
                }
            } else {
                auto mat = iterator->second;
                for (int i = 0; i < 3; i++) {
                    kd.push_back(get<0>(mat));
                    ks.push_back(get<1>(mat));
                    shininess.push_back(get<2>(mat));
                }
            }
        }
    }

    in.close();

	if (objectName.compare("") == 0) {
		// No 'o' object name tag defined in .obj file, so use the file name
		// minus the '.obj' ending as the objectName.
		const char * ptr = strrchr(objFilePath, '/');
		objectName.assign(ptr+1);
		size_t pos = objectName.find('.');
		objectName.resize(pos);
	}

    return textured;
}

//---------------------------------------------------------------------------------------
// Deprecated
//void ObjFileDecoder::decode(
//		const char * objFilePath,
//		std::string & objectName,
//        std::vector<vec3> & positions,
//        std::vector<vec3> & normals
//) {
//    std::vector<vec2> uvCoords;
//    decode(objFilePath, objectName, positions, normals, uvCoords);
//}

//---------------------------------------------------------------------------------------
bool ObjFileDecoder::loadMTL(
        const char * objFilePath,
        const char * mtlReference,
        GLint & texdata,
        std::map<std::string,std::tuple<glm::vec3,glm::vec3,float>> & mtlData
) {
    string path;
    pathMinusFilename(objFilePath, &path);

    string mtlpath = path + mtlReference;

    cout << "Loading material file " << mtlpath << endl;

    ifstream in(mtlpath, std::ios::in);
    in.exceptions(std::ifstream::badbit);
    if (!in) {
        // We fail gracefully here. Just log the message and then return with an empty mtlData
        cout << "Unable to open .mtl file " << mtlpath
                     << " within method ObjFileDecoder::decode" << endl;
        mtlData.clear();
        return false;
    }

    string currentLine;
    string currentmtl = "";
    vec3 currentKd;
    vec3 currentKs;
    float currentShine;
    bool textureSet = false;

    while (!in.eof()) {
        try {
            getline(in, currentLine);
        } catch (const ifstream::failure &e) {
            in.close();
            stringstream errorMessage;
            errorMessage << "Error calling getline() -- " << e.what() << endl;
            throw Exception(errorMessage.str());
        }
        if (currentLine.substr(0, 3) == "Kd ") {
            istringstream s(currentLine.substr(3));
            vec3 Kd;
            s >> Kd.r;
            s >> Kd.g;
            s >> Kd.b;
            currentKd = Kd;

            cout << "    Kd: " << to_string(currentKd) << endl;
        } else if (currentLine.substr(0, 3) == "Ks ") {
            istringstream s(currentLine.substr(3));
            vec3 Ks;
            s >> Ks.r;
            s >> Ks.g;
            s >> Ks.b;
            currentKs = Ks;

            cout << "    Ks: " << to_string(currentKs) << endl;
        } else if (currentLine.substr(0, 3) == "Ns ") {
            istringstream s(currentLine.substr(3));
            float Ns;
            s >> Ns;
            currentShine = Ns;

            cout << "    Ns: " << currentShine << endl;
        } else if (currentLine.substr(0, 7) == "map_Kd ") {
            // TODO ignore for now
        } else if (currentLine.substr(0, 7) == "newmtl ") {
            // Add this material to the map
            mtlData[currentmtl] = make_tuple(currentKd, currentKs, currentShine);

            // The next material
            istringstream s(currentLine.substr(7));
            s >> currentmtl;

            cout << "New material " << currentmtl << endl;

            currentKd = defaultKd;
            currentKs = defaultKs;
            currentShine = defaultNs;
        } // Else ignore others
    }
    // Add the final material to the map
    if (currentmtl != "") {
        mtlData[currentmtl] = make_tuple(currentKd, currentKs, currentShine);
    }
    return textureSet;
}

//---------------------------------------------------------------------------------------
void ObjFileDecoder::pathMinusFilename(
        const char * filepath,
        std::string * path
) {
    string stringpath = string(filepath);
    size_t last_slash = stringpath.find_last_of("/");
    *path = stringpath.substr(0, last_slash + 1);
}

//---------------------------------------------------------------------------------------
GLint ObjFileDecoder::loadTexture(
        std::string & filepath
) {
    return 0; // TODO
}
