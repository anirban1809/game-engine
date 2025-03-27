// src/UI/UIManager.cpp
#include "../../include/UI/UIManager.h"

void UIManager::AddPanel(std::unique_ptr<UIPanel> panel) {
    panels.emplace_back(std::move(panel));
}

void UIManager::Render() {
    for (const auto& panel : panels) { panel->Render(); }
}
