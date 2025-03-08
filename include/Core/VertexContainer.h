#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Camera.h"
#include "Types.h"
#include <vector>
#include "../vendor/glm/glm.hpp"
class VertexContainer {
   public:
    ~VertexContainer();

    void Init(std::vector<glm::vec3> &vertexBuffer, uint32 vertexBufferSize,
              std::vector<uint32> &indexBuffer, uint32 indexBufferSize);

    void UpdateVertexBuffer(const std::vector<glm::vec3> &vertices,
                            uint32 size);

    void Bind() const;
    void Draw(uint32 programId);
    void Unbind() const;

    void AttachCamera(Camera *camera);

   private:
    uint32 VAO;
    uint32 VBO;
    uint32 EBO;
    const glm::vec3 *vertices;
    uint32 *indices;
    uint32 vertexSize;
    uint32 indexSize;
    Camera *camera;
};

#endif  // __VERTEX_H__