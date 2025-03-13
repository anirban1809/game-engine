#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include "../vendor/glm/glm.hpp"
#include "Core/Types.h"
#include <string>
#include <vector>

#define vec3float float, float, float
#define vec2float float, float

enum class LightingMode {
    NO_LIGHTING = 0,
    DIFFUSE_ONLY = 1,
    SPECULAR_ENABLED = 2
};

typedef struct {
    std::string name;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    float transparency;
    LightingMode lightingMode;
    std::string diffuseTextureFile;
    std::string specularMapFile;
    std::string bumpMapFile;
    std::string opacityMapFile;

} Material;

class Object {
   public:
    void SetVertices(const std::vector<glm::vec3>& vertices);
    void SetTextures(const std::vector<glm::vec2>& textures);
    void SetNormals(const std::vector<glm::vec3>& normals);
    void SetVertexIndices(const std::vector<uint32>& indices);
    void SetTextureIndices(const std::vector<uint32>& indices);
    void SetNormalIndices(const std::vector<uint32>& indices);
    void AddMaterial(const Material& material);
    std::vector<std::tuple<vec3float, vec2float>> GetVerticesAndTextures()
        const;

    std::vector<glm::vec3> GetVertices();
    std::vector<glm::vec2> GetTextures();
    std::vector<glm::vec3> GetNormals();
    std::vector<uint32> GetVertexIndices() const;

    std::string name;
    uint32 vertexIndexOffset;
    uint32 textureIndexOffset;
    uint32 normalIndexOffset;

   private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<uint32> vertexIndices;
    std::vector<uint32> textureIndices;
    std::vector<uint32> normalIndices;
    std::vector<Material> materials;
};

class ObjLoader {
   public:
    void LoadObjectFile(const std::string& filename);
    void LoadMaterialFile();
    std::vector<Object> GetObjects();

   private:
    std::string objectFile;
    std::string materialFile;
    std::vector<Object> objects;
};

#endif  // __OBJLOADER_H__