#include "../../../include/Loaders/ObjLoader.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>

#define __reset                          \
    currentObject.SetVertices(vertices); \
    currentObject.SetNormals(normals);   \
    currentObject.SetTextures(textures); \
    currentObject.SetIndices(indices);   \
    allObjects.push_back(currentObject); \
    vertices = {};                       \
    textures = {};                       \
    normals = {};                        \
    indices = {};                        \
    objectCreated = false;

void Object::SetVertices(const std::vector<glm::vec3>& v) { vertices = v; }
void Object::SetTextures(const std::vector<glm::vec2>& t) { texCoords = t; }
void Object::SetNormals(const std::vector<glm::vec3>& v) { normals = v; }
void Object::SetIndices(const std::vector<uint32>& v) { indices = v; }

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
    std::vector<uint32> indices;
    bool objectCreated = false;

    uint32 localMaxIndex = 0;
    uint32 globalMaxIndex = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;

        ss >> type;

        if (type == "o") {
            if (!objectCreated) {
                currentObject = Object();
                objectCreated = true;
            } else {
                globalMaxIndex = localMaxIndex;
                localMaxIndex = 0;
                __reset;
            }
        }

        if (type == "#") {
            continue;
        }

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

                if (v > localMaxIndex) {
                    localMaxIndex = v;
                }

                indices.push_back(v - globalMaxIndex - 1);
            }
        }
    }

    __reset;

    this->objects = allObjects;
    file.close();
}