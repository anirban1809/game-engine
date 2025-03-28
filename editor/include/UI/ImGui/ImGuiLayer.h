#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "../UIBackend.h"
#include "../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"

class ImGuiState {
   public:
    bool isFrameBufferPanelHovered = false;
};

class ImGuiLayer : public UIBackend {
   public:
    void Init(void* windowHandle) override;
    ImGuiIO GetIO();
    void BeginFrame() override;
    void EndFrame() override;
    void Shutdown() override;

   private:
    GLFWwindow* window = nullptr;
    const char* glslVersion = "#version 330";
    ImGuiState state;
};
#endif  // __IMGUILAYER_H__