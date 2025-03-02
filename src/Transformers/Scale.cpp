#include <tuple>
#include <vector>
#include "../include/Transformers/Scale.h"
#include "Loaders/Wavefront.h"

void Transformers::Scale::Apply(WavefrontObjLoader* obj, float scaleFactor) {
    for (std::tuple<vec3float>& value : obj->GetVertices()) {
        std::get<0>(value) *= scaleFactor;
        std::get<1>(value) *= scaleFactor;
        std::get<2>(value) *= scaleFactor;
    }
}