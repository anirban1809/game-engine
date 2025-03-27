#include "../../../../include/UI/ImGui/Panels/ExamplePanel.h"
#include "../../../../../vendor/imgui/imgui.h"

ExamplePanel::ExamplePanel(const std::string& name) : panelName(name) {}

std::unique_ptr<ExamplePanel> CreatePanel(const std::string& panelName) {
    return std::make_unique<ExamplePanel>(panelName);
}

void ExamplePanel::Render() {
    ImGui::Begin(panelName.c_str());

    if (ImGui::Checkbox("Show Details", &showDetails)) {
        // You could trigger events or log state here
    }

    ImGui::SliderFloat("Slider", &sliderValue, 0.0f, 1.0f);

    if (showDetails) { ImGui::Text("Current value: %.2f", sliderValue); }

    ImGui::End();
}
