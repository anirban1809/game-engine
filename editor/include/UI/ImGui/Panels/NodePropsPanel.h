#ifndef __NODEPROPSPANEL_H__
#define __NODEPROPSPANEL_H__

#include "../ImGuiPanel.h"
#include "../../ApplicationState.h"

class NodePropsPanel : public ImGuiPanel {
   public:
    NodePropsPanel(ApplicationState&);
    void Render();

   private:
    ApplicationState& state;
};
#endif  // __NODEPROPSPANEL_H__