#ifndef __SCALE_H__
#define __SCALE_H__

#include "Loaders/Wavefront.h"
#include <vector>

namespace Transformers {
class Scale {
   public:
    static void Apply(WavefrontObjLoader* obj, float scaleFactor);
};
}  // namespace Transformers

#endif  // __SCALE_H__