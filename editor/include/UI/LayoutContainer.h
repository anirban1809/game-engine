#ifndef __LAYOUTCONTAINER_H__
#define __LAYOUTCONTAINER_H__

#include "UIPanel.h"

#include <vector>
#include <memory>
#include <string>

enum FlowDirection { FLOW_HORIZONTAL, FLOW_VERTICAL };

class LayoutContainer {
   public:
    LayoutContainer(int, int);
    void AddPanel(std::shared_ptr<UIPanel> panel, int);
    virtual void Render();

    void SetFlowDirection(FlowDirection direction);
    void SetGap(float);
    std::string name;

   protected:
    std::vector<std::shared_ptr<UIPanel>> panels;
    float gap;
    int rows;
    int columns;
    std::vector<int> colspans;

   private:
    FlowDirection direction = FlowDirection::FLOW_HORIZONTAL;
};
#endif  // __LAYOUTCONTAINER_H__