#include "../include/Loaders/Wavefront.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

void WavefrontObjLoader::Load(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    fileContent = buffer.str();
}

std::tuple<vec3float> WavefrontObjLoader::ParseVertices(
    const std::string& line) {
    std::tuple<vec3float> point3d;

    std::istringstream iss(line);
    std::string prefix;
    float x, y, z;

    if (iss >> prefix >> x >> y >> z && prefix == "v") {
        point3d = std::make_tuple(x, y, z);  // Store in vector
    }

    return point3d;
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

Triangle WavefrontObjLoader::ParseFaces(const std::string& line) {
    std::vector<FaceVertex> faceVertices;
    std::vector<std::string> values = splitString(line, ' ');

    for (int i = 1; i < values.size(); i++) {
        std::vector<std::string> faceValue = splitString(values[i], '/');

        FaceVertex faceData = {
            .v = (uint32)atoi(faceValue[0].c_str()),
            .vt = (uint32)atoi(faceValue[1].c_str()),
            .vn = (uint32)atoi(faceValue[2].c_str()),
        };

        faceVertices.push_back(faceData);
    }

    return std::make_tuple(faceVertices[0], faceVertices[1], faceVertices[2]);
}

VectorNormal WavefrontObjLoader::ParseNormals(const std::string& line) {
    VectorNormal normal;

    std::istringstream iss(line);
    std::string prefix;
    float x, y, z;

    if (iss >> prefix >> x >> y >> z && prefix == "vn") {
        normal = {.x = x, .y = y, .z = z};
    }

    return normal;
}

VectorTexture WavefrontObjLoader::ParseTextures(const std::string& line) {
    VectorTexture texture;

    std::istringstream iss(line);
    std::string prefix;
    float u, v;

    if (iss >> prefix >> u >> v && prefix == "vt") {
        texture = {.u = u, .v = v};
    }

    return texture;
}

std::vector<std::tuple<vec3float>> WavefrontObjLoader::GetVertices() {
    return vertices;
}

std::vector<Triangle> WavefrontObjLoader::GetTriangles() { return triangles; }

std::vector<VectorTexture> WavefrontObjLoader::GetVectorTextures() {
    return textures;
}

std::vector<VectorNormal> WavefrontObjLoader::GetVectorNormals() {
    return normals;
}

void WavefrontObjLoader::ParseContent() {
    std::istringstream stream(fileContent);
    std::string line;

    while (std::getline(stream, line)) {
        // ignore comments
        if (line.at(0) == '#') {
            continue;
        }

        // parse vertices
        if (line.at(0) == 'v') {
            vertices.push_back(ParseVertices(line));
        }

        // parse vector normals
        if (line.substr(0, 1) == "vn") {
            ParseNormals(line);
        }

        // parse vector textures
        if (line.substr(0, 1) == "vt") {
            normals.push_back(ParseNormals(line));
        }

        if (line.at(0) == 'f') {
            triangles.push_back(ParseFaces(line));
        }
    }
}
