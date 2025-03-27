// UIEngine.h
#pragma once
#include <memory>
#include "UIBackend.h"
#include "UIManager.h"

class UIEngine {
   public:
    UIEngine(std::unique_ptr<UIBackend> backend);
    void Init(void* windowHandle);
    void BeginFrame();
    void RenderPanels();
    void EndFrame();
    void Shutdown();
    UIManager& GetUIManager();

   private:
    std::unique_ptr<UIBackend> backend;
    UIManager uiManager;
};
