#include "../../../include/UI/ImGui/ImGuiPanel.h"
#include "../../../../vendor/imgui/imgui.h"
#include <iostream>

void ImGuiPanel::SetPositionAndSize() {
    ImGui::SetNextWindowPos(ImVec2(xposition, yposition), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height));
}
