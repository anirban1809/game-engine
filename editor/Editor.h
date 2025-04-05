#include <Core/Application.h>
#include <Core/Events.h>
#include "Core/FrameBuffer.h"
#include "Core/VertexContainer.h"
#include "Core/Logger.h"
#include "UI/UIEngine.h"
#include "UI/ImGui/ImGuiLayer.h"
#include "UI/ImGui/ImGuiLayoutContainer.h"
#include "UI/ApplicationState.h"
#include "Core/FileSystem.h"

#include <filesystem>
#include <iostream>
#include <memory>

class Editor : public Application {
   public:
    Editor(int width, int height, const char *title);

   protected:
    VertexContainer *container;
    Shader *shader;
    std::vector<float> stage;
    std::vector<uint32> indices;
    bool value = false;
    bool leftMouseDown = false;
    double lastX = 0.0f;
    double lastY = 0.0f;
    bool firstMouse = false;
    float yaw = -90.0f;  // Initialized so that the initial front is along -Z.
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed = 1.0f;
    UIEngine uiEngine;
    ImGuiLayer *imGuiBackend;
    FrameBuffer *scenebuffer;
    ApplicationState state;
    FileSystem fs;
    std::vector<std::string> assets;

    void DefineUI();

    void OnInit();

    void OnKeyPressed(int key);
    void OnMousePressed(int button);
    void OnMouseReleased(int button);
    void OnMouseMoved(double xpos, double ypos);

    void OnUpdate();

    void OnRender();
    void OnExit();
};
