#include "../../../include/UI/ImGui/ImGuiPanel.h"
#include "../../../../vendor/imgui/imgui.h"
#include <iostream>

void ImGuiPanel::SetPositionAndSize() {
    ImGui::SetNextWindowPos(ImVec2(xposition, yposition), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ImGuiPanel::CenterPanel(float w, float h) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    float W = ((2560.0f - w) / 2);
    float H = ((1330.0f - h) / 2);

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + W, viewport->Pos.y + H),
                            ImGuiCond_Once);
}

void ImGuiPanel::SetSize(float w, float h) {
    ImGui::SetNextWindowSize({w, h}, ImGuiCond_Once);
}