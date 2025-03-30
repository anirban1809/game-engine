#include "../../../../include/UI/ImGui/Panels/ScenePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>
ScenePropsPanel::ScenePropsPanel(ApplicationState& s) : state(s) {}

void ScenePropsPanel::Render() {
    ImGui::Begin("Scene Properties");

    for (int i = 0; i < state.nodes.size(); i++) {
        if (ImGui::TreeNode(state.nodes[i].c_str())) { ImGui::TreePop(); }
        if (ImGui::IsItemClicked()) { state.selectedNode = i; }
    }

    ImGui::End();
}