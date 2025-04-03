#ifndef __LAYOUTCONTAINER_H__
#define __LAYOUTCONTAINER_H__

#include "UIElement.h"

#include <vector>
#include <memory>
#include <string>

enum FlowDirection { FLOW_HORIZONTAL, FLOW_VERTICAL };

class LayoutContainer : public UIElement {
   public:
    LayoutContainer(int, int);
    void AddElement(std::shared_ptr<UIElement> panel, int);
    void Render() override;

    void SetFlowDirection(FlowDirection direction);

    void SetGap(float);
    std::string name;

   protected:
    std::vector<std::shared_ptr<UIElement>> elements;
    std::vector<int> colspans;
    float gap;
    float rows;
    float columns;

   private:
    FlowDirection direction = FlowDirection::FLOW_HORIZONTAL;
};
#endif  // __LAYOUTCONTAINER_H__
