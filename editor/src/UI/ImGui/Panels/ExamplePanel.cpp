#include "../../../../include/UI/ImGui/Panels/ExamplePanel.h"
#include "../../../../../vendor/imgui/imgui.h"

ExamplePanel::ExamplePanel(const std::string& name, ImGuiState& state)
    : panelName(name), state(state) {}

std::unique_ptr<ExamplePanel> CreatePanel(const std::string& panelName,
                                          ImGuiState state) {
    return std::make_unique<ExamplePanel>(panelName, state);
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
