#include "../../../include/Loaders/ObjLoader.h"
#include "Core/Types.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>

/**code to reset temporary values set during computation reuse as and when
 * necessary
 */
#define __reset_temp_values                           \
    currentObject.SetVertices(vertices);              \
    currentObject.SetNormals(normals);                \
    currentObject.SetTextures(textures);              \
    currentObject.SetVertexIndices(vertexIndices);    \
    currentObject.SetTextureIndices(textureIndices);  \
    currentObject.SetNormalIndices(normalIndices);    \
    currentObject.vertexIndexOffset = vertexoffset;   \
    currentObject.textureIndexOffset = textureoffset; \
    currentObject.normalIndexOffset = normaloffset;   \
    allObjects.push_back(currentObject);              \
    vertices = {};                                    \
    textures = {};                                    \
    normals = {};                                     \
    vertexIndices = {};                               \
    textureIndices = {};                              \
    normalIndices = {};                               \
    objectCreated = false;

void Object::SetVertices(const std::vector<glm::vec3>& v) { vertices = v; }
void Object::SetTextures(const std::vector<glm::vec2>& t) { texCoords = t; }
void Object::SetNormals(const std::vector<glm::vec3>& v) { normals = v; }
void Object::SetVertexIndices(const std::vector<uint32>& v) {
    vertexIndices = v;
}

void Object::SetTextureIndices(const std::vector<uint32>& v) {
    textureIndices = v;
}

void Object::SetNormalIndices(const std::vector<uint32>& v) {
    normalIndices = v;
}

std::vector<uint32> Object::GetVertexIndices() const { return vertexIndices; }

std::vector<glm::vec3> Object::GetVertices() { return vertices; }
std::vector<glm::vec2> Object::GetTextures() { return texCoords; }
std::vector<glm::vec3> Object::GetNormals() { return normals; }

std::vector<Object> ObjLoader::GetObjects() { return objects; }

std::vector<std::tuple<vec3float, vec2float>> Object::GetVerticesAndTextures()
    const {
    std::tuple<vec3float, vec2float> result[vertices.size()];

    for (int i = 0; i < vertexIndices.size(); i++) {
        uint32 vertexIndexValue = vertexIndices[i];
        uint32 textureIndexValue = textureIndices[i];
        result[vertexIndexValue] = std::make_tuple(
            vertices[vertexIndexValue][0], vertices[vertexIndexValue][1],
            vertices[vertexIndexValue][2], texCoords[textureIndexValue][0],
            texCoords[textureIndexValue][1]);
    }

    std::vector<std::tuple<vec3float, vec2float>> output(
        result, result + sizeof(result) / sizeof(result[0]));

    return output;
}

void ObjLoader::LoadObjectFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Failed to open object file" << std::endl;
    }

    std::string line;

    Object currentObject;

    std::vector<Object> allObjects;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<uint32> vertexIndices;
    std::vector<uint32> textureIndices;
    std::vector<uint32> normalIndices;
    uint32 vertexoffset;
    uint32 textureoffset;
    uint32 normaloffset;

    bool objectCreated = false;

    uint32 localMaxVertexIndex = 0;
    uint32 globalMaxVertexIndex = 0;

    uint32 localMaxTextureIndex = 0;
    uint32 globalMaxTextureIndex = 0;

    uint32 localMaxNormalIndex = 0;
    uint32 globalMaxNormalIndex = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;

        ss >> type;

        if (type == "o") {
            if (!objectCreated) {
                currentObject = Object();
                objectCreated = true;
            } else {
                globalMaxVertexIndex = localMaxVertexIndex;
                localMaxVertexIndex = 0;

                globalMaxTextureIndex = localMaxTextureIndex;
                localMaxTextureIndex = 0;

                globalMaxNormalIndex = localMaxNormalIndex;
                localMaxNormalIndex = 0;
                __reset_temp_values;
            }
        }

        if (type == "#") { continue; }

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }

        if (type == "vn") {
            float x, y, z;
            ss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }

        if (type == "vt") {
            float u, v;
            ss >> u >> v;
            textures.emplace_back(u, v);
        }

        if (type == "f") {
            char slash;
            for (int i = 0; i < 3; i++) {
                uint32 v, vt, vn;
                ss >> v >> slash >> vt >> slash >> vn;

                if (v > localMaxVertexIndex) { localMaxVertexIndex = v; }
                if (vt > localMaxTextureIndex) { localMaxTextureIndex = vt; }
                if (vn > localMaxNormalIndex) { localMaxNormalIndex = vn; }

                vertexIndices.push_back(v - globalMaxVertexIndex - 1);
                textureIndices.push_back(vt - globalMaxTextureIndex - 1);
                normalIndices.push_back(vn - globalMaxNormalIndex - 1);
            }

            vertexoffset = globalMaxVertexIndex;
            textureoffset = globalMaxTextureIndex;
            normaloffset = globalMaxNormalIndex;
        }
    }

    __reset_temp_values;

    this->objects = allObjects;
    file.close();
}