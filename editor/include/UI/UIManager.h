#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include <vector>
#include <memory>
#include "UIPanel.h"

class UIManager {
   public:
    UIManager() = default;
    ~UIManager() = default;

    void AddPanel(std::unique_ptr<UIPanel> panel);
    void Render();

   private:
    std::vector<std::unique_ptr<UIPanel>> panels;
};

#endif  // __UIMANAGER_H__