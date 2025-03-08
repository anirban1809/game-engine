#ifndef __SCALE_H__
#define __SCALE_H__

#include "Loaders/Wavefront.h"
#include <vector>

namespace Transformers {
class Scale {
   public:
    static void Apply(WavefrontObjLoader* obj, float scaleFactor);
    static void Apply(WavefrontObjLoader* obj, float scaleFactorX,
                      float scaleFactorY);
    static void Apply(WavefrontObjLoader* obj, float scaleFactorX,
                      float scaleFactorY, float scaleFactorZ);
};
}  // namespace Transformers

#endif  // __SCALE_H__