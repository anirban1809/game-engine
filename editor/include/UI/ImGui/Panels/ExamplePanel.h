#ifndef __EXAMPLEPANEL_H__
#define __EXAMPLEPANEL_H__

#include "../ImGuiPanel.h"
#include <string>

class ExamplePanel : public ImGuiPanel {
   public:
    ExamplePanel(const std::string& panelName);
    ~ExamplePanel() override = default;

    void Render() override;

   private:
    std::string panelName;
    bool showDetails = false;
    float sliderValue = 0.5f;
};

#endif  // __EXAMPLEPANEL_H__