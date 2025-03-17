#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Camera.h"
#include "Core/Light.h"
#include "Core/Texture.h"
#include "Loaders/ObjLoader.h"
#include "Types.h"
#include <vector>
#include "../vendor/glm/glm.hpp"
class VertexContainer {
   public:
    ~VertexContainer();

    void Init(std::vector<float> &vertexBuffer,
              std::vector<uint32> &indexBuffer, uint32 shaderProgramId);

    void AddObjects(const std::vector<Object> &);
    void UpdateVertexBuffer(const std::vector<float> &vertices, uint32 size);

    void Bind() const;
    void Draw(uint32 programId);
    void Unbind() const;
    void ApplyTexture(uint32, uint32);
    void AttachLight(Light *light);

    void AttachCamera(Camera *camera);

   private:
    uint32 VAO;
    uint32 VBO;
    uint32 EBO;
    const float *vertices;
    uint32 *indices;
    uint32 vertexSize;
    uint32 indexSize;
    Camera *camera;
    Light *light;
    Texture *texture;
    std::vector<uint32> textures;
    std::vector<Object> objects;
};

#endif  // __VERTEX_H__