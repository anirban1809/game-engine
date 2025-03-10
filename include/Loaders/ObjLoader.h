#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include "../vendor/glm/glm.hpp"
#include "Core/Types.h"
#include <string>
#include <vector>

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
    void SetIndices(const std::vector<uint32>& indices);
    void AddMaterial(const Material& material);
    std::string name;

   private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<uint32> indices;
    std::vector<Material> materials;
};

class ObjLoader {
   public:
    void LoadObjectFile(const std::string& filename);
    void LoadMaterialFile();

   private:
    std::string objectFile;
    std::string materialFile;
    std::vector<Object> objects;
};

#endif  // __OBJLOADER_H__