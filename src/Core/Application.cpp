#include "../../include/Core/Application.h"
#include "Core/FrameBuffer.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "../../../vendor/imgui/imgui.h"
#include "../../../vendor/imgui/imgui_impl_glfw.h"
#include "../../../vendor/imgui/imgui_impl_opengl3.h"

Application::Application(int width, int height, const char* title) {
    window = new Window(width, height, title);
    renderer = new Renderer();

    // Set up key event callback
    window->SetKeyCallback([this](int key, int action) {
        if (action == GLFW_REPEAT || GLFW_PRESS) {
            OnKeyPressed(key);  // Call the abstracted method
        }
    });

    window->SetMouseButtonCallback([this](int button, int action) {
        if (action == GLFW_PRESS) {
            OnMousePressed(button);  // Call the abstracted method
        }

        if (action == GLFW_RELEASE) {
            OnMouseReleased(button);  // Call the abstracted method
        }
    });

    window->SetMouseCursorCallback(
        [this](double xpos, double ypos) { OnMouseMoved(xpos, ypos); });
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete window;
    delete renderer;
}

void Application::Run() {
    OnInit();  // Custom initialization

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Load a custom font (adjust the path and size as needed)
    ImFont* customFont = io.Fonts->AddFontFromFileTTF(
        "/Users/anirban/Downloads/SF-Pro.ttf", 20.0f);
    if (customFont == nullptr) {
        fprintf(stderr, "Could not load custom font!\n");
    }

    io.FontDefault = customFont;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    FrameBuffer sceneBuffer(1920.0f, 1080.0f);
    while (!window->ShouldClose()) {
        window->PollEvents();

        OnUpdate();  // Custom update logic

        renderer->Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(
            ImVec2(viewport->Pos.x + 570, viewport->Pos.y + 10),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_Once);

        ImGui::Begin("My Scene", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        sceneBuffer.RescaleFrameBuffer(window_width, window_height);
        glViewport(0, 0, window_width, window_height);

        ImVec2 position = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureID)sceneBuffer.GetFrameTexture(),
            ImVec2(position.x, position.y),
            ImVec2(position.x + window_width, position.y + window_height),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        // Secondary window for additional UI (like debug info)

        ImGui::SetNextWindowPos(
            ImVec2(viewport->Pos.x + 1580, viewport->Pos.y + 10),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(330, 1000), ImGuiCond_Once);
        ImGui::Begin("Secondary Window", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("This is the secondary ImGui window.");
        // Add more UI elements here as needed
        ImGui::End();

        sceneBuffer.Bind();

        glViewport(0, 0, window_width,
                   window_height);  // Set the viewport for the FBO
        renderer->Clear();          // Clear the FBO's buffers
        OnRender();                 // Custom rendering logic
        ImGui::Render();
        sceneBuffer.Unbind();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        window->SwapBuffers();
    }
}
