#include "../../../include/Loaders/ObjLoader.h"
#include "Core/Renderer.h"
#include "Core/Types.h"
#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
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

void Object::SetMaterial(const Material& _material) { material = _material; }

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

std::vector<std::tuple<vec3float, vec2float, vec3float>>
Object::GetAllVertexData() const {
    std::tuple<vec3float, vec2float, vec3float> result[vertices.size()];

    for (int i = 0; i < vertexIndices.size(); i++) {
        uint32 vertexIndexValue = vertexIndices[i];
        uint32 textureIndexValue = textureIndices[i];
        uint32 normalIndexValue = normalIndices[i];
        result[vertexIndexValue] = std::make_tuple(
            // vertices
            vertices[vertexIndexValue][0], vertices[vertexIndexValue][1],
            vertices[vertexIndexValue][2],

            // textures
            texCoords[textureIndexValue][0], texCoords[textureIndexValue][1],

            // normals
            normals[normalIndexValue][0], normals[normalIndexValue][1],
            normals[normalIndexValue][2]);
    }

    std::vector<std::tuple<vec3float, vec2float, vec3float>> output(
        result, result + sizeof(result) / sizeof(result[0]));

    return output;
}

Material Object::GetMaterial() const { return material; }

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
    std::vector<uint32> vertexEntries;

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

        if (type == "usemtl") {
            // TODO: first check if all the materials are loaded
            std::string materialName;
            ss >> materialName;

            for (auto const& material : materials) {
                if (material.name == materialName) {
                    currentObject.SetMaterial(material);
                    break;
                }
            }
        }
    }

    __reset_temp_values;

    this->objects = allObjects;

    for (auto& object : objects) { object.AdjustReusedVertices(); }

    file.close();
}

// A hash functor for a tuple of two uint32 values (texture index, normal
// index).
struct PairHash {
    std::size_t operator()(const std::tuple<uint32, uint32>& key) const {
        uint32 a = std::get<0>(key);
        uint32 b = std::get<1>(key);
        return std::hash<uint32>()(a) ^ (std::hash<uint32>()(b) << 1);
    }
};

// Define a key type for (vertex index, texture index, normal index)
typedef std::tuple<uint32, uint32, uint32> VertexKey;

// Custom hash for VertexKey
struct VertexKeyHash {
    std::size_t operator()(const VertexKey& key) const {
        uint32 a = std::get<0>(key);
        uint32 b = std::get<1>(key);
        uint32 c = std::get<2>(key);
        size_t h1 = std::hash<uint32>()(a);
        size_t h2 = std::hash<uint32>()(b);
        size_t h3 = std::hash<uint32>()(c);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

void Object::AdjustReusedVertices() {
    // New (unified) attribute arrays:
    std::vector<glm::vec3> newVertices;
    std::vector<glm::vec2> newTexCoords;
    std::vector<glm::vec3> newNormals;

    // New index arrays (we keep them separate, but they will end up holding the
    // same values).
    std::vector<uint32> newVertexIndices;
    std::vector<uint32> newTextureIndices;
    std::vector<uint32> newNormalIndices;

    // Map to track each unique (position, texcoord, normal) combination.
    std::unordered_map<VertexKey, uint32, VertexKeyHash> uniqueMap;
    uniqueMap.reserve(vertexIndices.size());

    // Iterate over each face vertex (i.e. each entry in the index arrays).
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        uint32 vIndex = vertexIndices[i];
        uint32 tIndex = textureIndices[i];
        uint32 nIndex = normalIndices[i];

        // Create a key from the current combination.
        VertexKey key = std::make_tuple(vIndex, tIndex, nIndex);

        auto it = uniqueMap.find(key);
        if (it == uniqueMap.end()) {
            // This combination hasn't been seen yet.
            uint32 newIndex = static_cast<uint32>(newVertices.size());
            uniqueMap[key] = newIndex;

            // Duplicate the attributes into the new arrays.
            newVertices.push_back(vertices[vIndex]);
            newTexCoords.push_back(texCoords[tIndex]);
            newNormals.push_back(normals[nIndex]);

            // For this face vertex, store the new index in all three index
            // arrays.
            newVertexIndices.push_back(newIndex);
            newTextureIndices.push_back(newIndex);
            newNormalIndices.push_back(newIndex);
        } else {
            // Reuse the existing unified vertex.
            uint32 unifiedIndex = it->second;
            newVertexIndices.push_back(unifiedIndex);
            newTextureIndices.push_back(unifiedIndex);
            newNormalIndices.push_back(unifiedIndex);
        }
    }

    // Replace the original arrays with the new ones.
    vertices = newVertices;
    texCoords = newTexCoords;
    normals = newNormals;
    vertexIndices = newVertexIndices;
    textureIndices = newTextureIndices;
    normalIndices = newNormalIndices;
}

void ObjLoader::LoadMaterialFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Failed to open object file" << std::endl;
    }

    std::string line;
    Material currentMaterial;
    bool materialCreated;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;

        ss >> type;

        if (type == "#") { continue; }

        if (type == "newmtl") {
            if (!materialCreated) {
                currentMaterial = Material();
                materialCreated = true;
                ss >> currentMaterial.name;
            } else {
                materials.push_back(currentMaterial);
                ss >> currentMaterial.name;
                materialCreated = false;
            }
        }

        if (type == "Ka") {
            float r, g, b;
            ss >> r >> g >> b;
            currentMaterial.ambientColor = glm::vec3(r, g, b);
        }

        if (type == "Kd") {
            float r, g, b;
            ss >> r >> g >> b;
            currentMaterial.diffuseColor = glm::vec3(r, g, b);
        }

        if (type == "Ks") {
            float r, g, b;
            ss >> r >> g >> b;
            currentMaterial.specularColor = glm::vec3(r, g, b);
        }

        if (type == "Ns") { ss >> currentMaterial.shininess; }

        if (type == "d") { ss >> currentMaterial.transparency; }

        if (type == "illum") {
            char mode;
            ss >> mode;

            switch (mode) {
                case '0':
                    currentMaterial.lightingMode = LightingMode::NO_LIGHTING;
                    break;
                case '1':
                    currentMaterial.lightingMode = LightingMode::DIFFUSE_ONLY;
                    break;
                case '2':
                    currentMaterial.lightingMode =
                        LightingMode::SPECULAR_ENABLED;
                    break;
            }
        }

        if (type == "map_Kd") { ss >> currentMaterial.diffuseTextureFile; }

        if (type == "map_Ks") { ss >> currentMaterial.specularMapFile; }

        if (type == "map_Bump") { ss >> currentMaterial.bumpMapFile; }
    }

    materials.push_back(currentMaterial);
}
