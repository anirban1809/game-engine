#ifndef __SCENEPROPSPANEL_H__
#define __SCENEPROPSPANEL_H__

#include "../ImGuiPanel.h"
#include "../../ApplicationState.h"
#include <memory>

class ScenePropsPanel : public ImGuiPanel {
   public:
    ScenePropsPanel(ApplicationState&);
    void Render();

   private:
    ApplicationState& state;
};

#endif  // __SCENEPROPSPANEL_H__