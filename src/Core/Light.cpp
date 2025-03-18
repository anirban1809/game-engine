#include "../../../include/Core/Light.h"

void Light::UpdateLightPositionX(float factor) { lightPosition[0] += factor; }
void Light::UpdateLightPositionY(float factor) { lightPosition[1] += factor; }
void Light::UpdateLightPositionZ(float factor) { lightPosition[2] += factor; }

void Light::SetColor(glm::vec3 color) { lightColor = color; }

void Light::SetPosition(glm::vec3 position) { lightPosition = position; }

glm::vec3 Light::GetPosition() { return lightPosition; }

float* Light::GetParameterPointer(Parameters parameter) {
    switch (parameter) {
        case Parameters::XPosition:
            return &lightPosition[0];
        case Parameters::YPosition:
            return &lightPosition[1];
        case Parameters::ZPosition:
            return &lightPosition[2];
        case Parameters::LightColor:
            return &lightColor[0];
    }
    return nullptr;
}

glm::vec3 Light::GetColor() { return lightColor; }