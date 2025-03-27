#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../../../vendor/glm/glm.hpp"
#include <tuple>

class Camera {
   public:
    glm::vec3 GetCameraPosition();
    glm::vec3 GetCameraLook();
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void SetCameraPosition(float x, float y, float z);
    void SetCameraLook(float x, float y, float z);
    void MoveForward(float amount);

    glm::mat4 SetProjection(float fov, float aspectRatio, float near,
                            float far);
    void UpdateView();
    void SetCameraProjection(float fovDeg, float aspectRatio, float near,
                             float far);

    void TranslateX(float value);
    void TranslateY(float value);
    void TranslateZ(float value);

   private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraLook;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif  // __CAMERA_H__