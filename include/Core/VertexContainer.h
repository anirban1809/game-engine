#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Types.h"
class VertexContainer {
   public:
    ~VertexContainer();

    void Init(float *vertexBuffer, uint32 vertexBufferSize, uint32 *indexBuffer,
              uint32 indexBufferSize);

    void UpdateVertexBuffer(float *newVertices, uint32 size);

    void Bind() const;
    void Draw(uint32 programId, float *colors) const;
    void Unbind() const;

   private:
    uint32 VAO;
    uint32 VBO;
    uint32 EBO;
    float *vertices;
    uint32 *indices;
    uint32 vertexSize;
    uint32 indexSize;
};

#endif  // __VERTEX_H__