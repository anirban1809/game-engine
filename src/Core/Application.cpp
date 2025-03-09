#include "../../include/Core/Application.h"
#include "GLFW/glfw3.h"
#include <iostream>

Application::Application(int width, int height, const char *title) {
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
    delete window;
    delete renderer;
}

void Application::Run() {
    OnInit();  // Custom initialization

    while (!window->ShouldClose()) {
        window->PollEvents();

        OnUpdate();  // Custom update logic

        renderer->Clear();
        OnRender();  // Custom rendering logic
        window->SwapBuffers();
    }
}
