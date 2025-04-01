#include "../../include/Core/Camera.h"
#include "../../../vendor/glm/gtc/matrix_transform.hpp"

glm::vec3& Camera::GetCameraPosition() { return cameraPosition; }

glm::vec3& Camera::GetCameraLook() { return cameraLook; }

glm::mat4& Camera::GetView() { return view; }

glm::mat4& Camera::GetProjection() { return projection; }

void Camera::SetCameraPosition(float x, float y, float z) {
    cameraPosition[0] = x;
    cameraPosition[1] = y;
    cameraPosition[2] = z;
    UpdateView();
}

void Camera::SetCameraLook(float x, float y, float z) {
    cameraLook[0] = x;
    cameraLook[1] = y;
    cameraLook[2] = z;
    UpdateView();
}

void Camera::UpdateView() {
    view = glm::lookAt(cameraPosition, cameraLook, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetCameraProjection(float fovDeg, float aspectRatio, float near,
                                 float far) {
    projection = glm::perspective(glm::radians(fovDeg), aspectRatio, near, far);
}

// TODO: add better translation capabilities
void Camera::TranslateX(float value) {
    cameraPosition[0] += value;
    UpdateView();
}

void Camera::TranslateZ(float value) {
    cameraPosition[2] += value;
    UpdateView();
}

void Camera::TranslateY(float value) {
    cameraPosition[1] += value;
    UpdateView();
}

void Camera::MoveForward(float amount) {
    cameraPosition += GetCameraLook() * amount;
    UpdateView();
}
