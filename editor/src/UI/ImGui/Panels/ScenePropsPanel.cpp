#include "../../../../include/UI/ImGui/Panels/ScenePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>
ScenePropsPanel::ScenePropsPanel(ApplicationState& s) : state(s) {}

void ScenePropsPanel::Render() {
    SetPositionAndSize();
    ImGui::Begin("Scene Properties");

    for (int i = 0; i < state.nodes.size(); i++) {
        if (ImGui::TreeNode(state.nodes[i].c_str())) { ImGui::TreePop(); }
        if (ImGui::IsItemClicked()) { state.selectedNode = i; }
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Add Static Mesh")) {
            // Handle Option 1
        }
        if (ImGui::MenuItem("Add Light Object")) {
            // Handle Option 2
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Close")) {
            // Example: Close the window by setting the flag
            // You would need access to the show flag here
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}