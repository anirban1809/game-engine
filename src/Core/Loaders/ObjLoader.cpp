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

void Object::AdjustReusedVertices() {
    // Reserve space for the outer map: upper bound is the number of original
    // vertices.
    std::unordered_map<uint32, std::unordered_map<uint32, uint32>> duplicateMap;
    duplicateMap.reserve(vertices.size());

    // Process each face index entry.
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        uint32 origIndex = vertexIndices[i];
        uint32 texIndex = textureIndices[i];

        // Look for an existing mapping for this original vertex.
        auto outerIt = duplicateMap.find(origIndex);
        if (outerIt == duplicateMap.end()) {
            // First time we see this vertex.
            // Create an inner map and reserve a couple of entries.
            std::unordered_map<uint32, uint32> innerMap;
            innerMap.reserve(2);
            // Map the current texture index to the original vertex index.
            innerMap.emplace(texIndex, origIndex);
            duplicateMap.emplace(origIndex, std::move(innerMap));
            // Leave vertexIndices[i] unchanged.
        } else {
            auto& innerMap = outerIt->second;
            auto innerIt = innerMap.find(texIndex);
            if (innerIt != innerMap.end()) {
                // This (vertex, texture) combination was seen before.
                vertexIndices[i] = innerIt->second;
            } else {
                // Found the vertex, but with a new texture index.
                // Duplicate the vertex by pushing a copy.
                uint32 newIndex = static_cast<uint32>(vertices.size());
                vertices.push_back(vertices[origIndex]);
                // Record the new mapping.
                innerMap.emplace(texIndex, newIndex);
                // Update the face index.
                vertexIndices[i] = newIndex;
            }
        }
    }
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

            if (mode == '0') {
                currentMaterial.lightingMode = LightingMode::NO_LIGHTING;
            }

            if (mode == '1') {
                currentMaterial.lightingMode = LightingMode::DIFFUSE_ONLY;
            }

            if (mode == '2') {
                currentMaterial.lightingMode = LightingMode::SPECULAR_ENABLED;
            }
        }

        if (type == "map_Kd") { ss >> currentMaterial.diffuseTextureFile; }

        if (type == "map_Ks") { ss >> currentMaterial.specularMapFile; }

        if (type == "map_Bump") { ss >> currentMaterial.bumpMapFile; }
    }

    materials.push_back(currentMaterial);
}
