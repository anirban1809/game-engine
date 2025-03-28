#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Types.h"
class FrameBuffer {
   public:
    FrameBuffer(float width, float height);
    ~FrameBuffer();
    uint32 GetFrameTexture();
    void RescaleFrameBuffer(float width, float height);
    void Bind() const;
    void Unbind() const;
    void Clear() const;

   private:
    uint32 fbo;
    uint32 texture;
    uint32 rbo;
};

#endif  // __FRAMEBUFFER_H__