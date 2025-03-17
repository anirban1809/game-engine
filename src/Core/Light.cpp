#include "../../../include/Core/Light.h"

void Light::UpdateLightPositionX(float factor) { lightPosition[0] += factor; }
void Light::UpdateLightPositionY(float factor) { lightPosition[1] += factor; }
void Light::UpdateLightPositionZ(float factor) { lightPosition[2] += factor; }

void Light::SetColor(glm::vec3 color) { lightColor = color; }

void Light::SetPosition(glm::vec3 position) { lightPosition = position; }

glm::vec3 Light::GetPosition() { return lightPosition; }

glm::vec3 Light::GetColor() { return lightColor; }