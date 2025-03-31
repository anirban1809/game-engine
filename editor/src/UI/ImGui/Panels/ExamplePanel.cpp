#include "../../../../include/UI/ImGui/Panels/ExamplePanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "../../../../include/UI/ApplicationState.h"
#include <iostream>

ExamplePanel::ExamplePanel(const std::string& name, ApplicationState& state)
    : panelName(name), state(state) {}

std::unique_ptr<ExamplePanel> CreatePanel(const std::string& panelName,
                                          ApplicationState& state) {
    return std::make_unique<ExamplePanel>(panelName, state);
}

void ExamplePanel::Render() {
    SetPositionAndSize();
    ImGui::Begin(panelName.c_str());
    if (ImGui::Checkbox("Show Details", &showDetails)) {
        // You could trigger events or log state here
    }

    ImGui::SliderFloat("Slider", &sliderValue, 0.0f, 1.0f);

    if (ImGui::CollapsingHeader("Settings")) {
        ImGui::Text("Some setting here");
        // ImGui::Checkbox("Enable feature", &enabled);
    }

    if (ImGui::TreeNode("Node 1")) {
        ImGui::Text("This is inside Node 1");

        if (ImGui::TreeNode("Child Node")) {
            ImGui::Text("Nested inside Child Node");
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    if (showDetails) { ImGui::Text("Current value: %.2f", sliderValue); }

    ImGui::End();
}
