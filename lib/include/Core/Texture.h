#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Types.h"
#include <string>
class Texture {
   public:
    Texture();
    uint32 LoadTexture(const std::string& path);
};

#endif  // __TEXTURE_H__