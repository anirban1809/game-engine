#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../vendor/glm/glm.hpp"
#include "Core/Types.h"

enum Parameters {
    XPosition,
    YPosition,
    ZPosition,
    LightColor

};

class Light {
   public:
    void UpdateLightPositionX(float factor);
    void UpdateLightPositionY(float factor);
    void UpdateLightPositionZ(float factor);
    void SetPositionY(float position);
    void SetColor(glm::vec3 color);
    void SetPosition(glm::vec3 position);

    glm::vec3 GetPosition();
    float* GetParameterPointer(Parameters parameter);
    glm::vec3 GetColor();

   private:
    glm::vec3 lightColor;
    glm::vec3 lightPosition;
};

#endif  // __LIGHT_H__