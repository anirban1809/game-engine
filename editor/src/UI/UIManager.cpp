// src/UI/UIManager.cpp
#include "../../include/UI/UIManager.h"
#include "../../include/UI/LayoutContainer.h"
#include <iostream>

void UIManager::AddPanel(std::shared_ptr<UIPanel> panel) {
    panels.emplace_back(panel);
}

void UIManager::Render() {
    for (const std::shared_ptr<UIPanel>& panel : panels) { panel->Render(); }
    for (std::shared_ptr<LayoutContainer>& container : containers) {
        container->Render();
    }
}

void UIManager::AddLayoutContainer(
    std::shared_ptr<LayoutContainer> layoutContainer) {
    containers.push_back(layoutContainer);
}