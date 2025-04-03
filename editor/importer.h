#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <unordered_map>

struct Vertex {
    aiVector3D position;
    aiVector3D normal;
    aiVector3D texCoords;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal &&
               texCoords == other.texCoords;
    }
};

struct VertexHasher {
    std::size_t operator()(const Vertex& v) const {
        auto h1 = std::hash<float>{}(v.position.x) ^
                  std::hash<float>{}(v.position.y) ^
                  std::hash<float>{}(v.position.z);
        auto h2 = std::hash<float>{}(v.normal.x) ^
                  std::hash<float>{}(v.normal.y) ^
                  std::hash<float>{}(v.normal.z);
        auto h3 = std::hash<float>{}(v.texCoords.x) ^
                  std::hash<float>{}(v.texCoords.y);
        return h1 ^ h2 ^ h3;
    }
};

class Importer {
   public:
    static bool Load(
        const std::string& path,
        std::vector<float>& vertexBuffer,  // [x, y, z, u, v, nx, ny, nz]
        std::vector<unsigned int>& indexBuffer) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cerr << "Assimp error: " << importer.GetErrorString()
                      << std::endl;
            return false;
        }

        std::unordered_map<Vertex, unsigned int, VertexHasher> uniqueVertices;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                if (face.mNumIndices != 3) continue;  // skip non-triangle faces

                for (unsigned int j = 0; j < 3; ++j) {
                    unsigned int idx = face.mIndices[j];

                    Vertex v;
                    v.position = mesh->mVertices[idx];
                    v.normal = mesh->HasNormals() ? mesh->mNormals[idx]
                                                  : aiVector3D(0, 0, 0);
                    v.texCoords = mesh->HasTextureCoords(0)
                                      ? mesh->mTextureCoords[0][idx]
                                      : aiVector3D(0, 0, 0);

                    if (uniqueVertices.count(v) == 0) {
                        uniqueVertices[v] =
                            static_cast<unsigned int>(vertexBuffer.size() / 8);

                        vertexBuffer.push_back(v.position.x);
                        vertexBuffer.push_back(v.position.y);
                        vertexBuffer.push_back(v.position.z);

                        vertexBuffer.push_back(v.texCoords.x);
                        vertexBuffer.push_back(v.texCoords.y);

                        vertexBuffer.push_back(v.normal.x);
                        vertexBuffer.push_back(v.normal.y);
                        vertexBuffer.push_back(v.normal.z);
                    }

                    indexBuffer.push_back(uniqueVertices[v]);
                }
            }
        }

        return true;
    }
};
#endif  // __IMPORTER_H__