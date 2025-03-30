#include "../../include/UI/LayoutContainer.h"
#include <iostream>

LayoutContainer::LayoutContainer(int r, int c) {
    rows = r;
    columns = c;
}

void LayoutContainer::AddPanel(std::shared_ptr<UIPanel> panel, int colspan) {
    panels.emplace_back(panel);
    colspans.emplace_back(colspan);
}

void LayoutContainer::Render() {
    for (const auto& panel : panels) { panel->Render(); }
}

void LayoutContainer::SetFlowDirection(FlowDirection _direction) {
    direction = _direction;
}

void LayoutContainer::SetGap(float _gap) { gap = _gap; }