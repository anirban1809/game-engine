#include "../../../../include/UI/ImGui/Panels/NodePropsPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>

NodePropsPanel::NodePropsPanel(ApplicationState& s) : state(s) {}

void NodePropsPanel::Render() {
    ImGui::Begin("Node Properties");
    ImGui::Text("Currently Selected: %s",
                state.nodes[state.selectedNode].c_str());
    ImGui::End();
}