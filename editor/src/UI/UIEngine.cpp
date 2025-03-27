// UIEngine.cpp
#include "../../include/UI/UIEngine.h"

UIEngine::UIEngine(std::unique_ptr<UIBackend> backend)
    : backend(std::move(backend)) {}

void UIEngine::Init(void* windowHandle) { backend->Init(windowHandle); }

void UIEngine::BeginFrame() { backend->BeginFrame(); }

void UIEngine::RenderPanels() { uiManager.Render(); }

void UIEngine::EndFrame() { backend->EndFrame(); }

void UIEngine::Shutdown() { backend->Shutdown(); }

UIManager& UIEngine::GetUIManager() { return uiManager; }
