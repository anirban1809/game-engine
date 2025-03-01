#include "../../include/Core/Application.h"
#include <iostream>

Application::Application(int width, int height, const char *title) {
    window = new Window(width, height, title);
    renderer = new Renderer();
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
