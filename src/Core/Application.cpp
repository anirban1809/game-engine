#include "../../include/Core/Application.h"
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

    // Load a custom font (adjust the path and size as needed)
    ImFont* customFont = io.Fonts->AddFontFromFileTTF(
        "/Users/anirban/Downloads/SF-Pro.ttf", 20.0f);
    if (customFont == nullptr) {
        // Handle error: the font file couldn't be loaded.
        fprintf(stderr, "Could not load custom font!\n");
    }

    // Optionally, set the custom font as the default font for all windows
    io.FontDefault = customFont;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!window->ShouldClose()) {
        window->PollEvents();

        OnUpdate();  // Custom update logic

        renderer->Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        OnRender();  // Custom rendering logic

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->SwapBuffers();
    }
}
