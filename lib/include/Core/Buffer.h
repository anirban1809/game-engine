#ifndef __BUFFER_H__
#define __BUFFER_H__
#include "./Types.h"

class VertexBuffer {
   public:
    virtual ~VertexBuffer();

   protected:
    static void Create(float* vb, uint32 size);
};

class IndexBuffer {
   public:
    static void Create(uint32* ib, uint32 size);
    virtual ~IndexBuffer();
};

#endif  // __BUFFER_H__