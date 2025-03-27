#include "../UIBackend.h"
#include "../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"

class ImGuiLayer : public UIBackend {
   public:
    void Init(void* windowHandle) override;
    void BeginFrame() override;
    void EndFrame() override;
    void Shutdown() override;

   private:
    GLFWwindow* window = nullptr;
    const char* glslVersion = "#version 330";
};