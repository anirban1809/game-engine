#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include <vector>
#include <memory>
#include "UIPanel.h"
#include "LayoutContainer.h"

class UIManager {
   public:
    UIManager() = default;
    ~UIManager() = default;

    void AddPanel(std::shared_ptr<UIPanel> panel);
    void AddLayoutContainer(std::shared_ptr<LayoutContainer> layoutContainer);
    void Render();

   private:
    std::vector<std::shared_ptr<UIPanel>> panels;
    std::vector<std::shared_ptr<LayoutContainer>> containers;
};

#endif  // __UIMANAGER_H__