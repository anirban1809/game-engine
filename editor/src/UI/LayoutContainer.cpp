#include "../../include/UI/LayoutContainer.h"

LayoutContainer::LayoutContainer(int r, int c) {
    rows = r;
    columns = c;
}

void LayoutContainer::AddElement(std::shared_ptr<UIElement> element,
                                 int colspan) {
    element->SetIsChildElement(true);
    elements.emplace_back(element);
    colspans.emplace_back(colspan);
}

void LayoutContainer::Render() {
    for (const auto& element : elements) { element->Render(); }
}

void LayoutContainer::SetFlowDirection(FlowDirection _direction) {
    direction = _direction;
}

void LayoutContainer::SetGap(float _gap) { gap = _gap; }
