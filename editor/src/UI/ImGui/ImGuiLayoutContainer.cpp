#include "../../../include/UI/ImGui/ImGuiLayoutContainer.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../../../lib/include/Core/Types.h"

#include <iostream>

ImGuiLayoutContainer::ImGuiLayoutContainer(int r, int c)
    : LayoutContainer(r, c) {}

void ImGuiLayoutContainer::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 globalPosition = ImGui::GetWindowPos();
    ImVec2 glfwPos = viewport->Pos;
    ImVec2 relativePos(globalPosition.x - glfwPos.x,
                       globalPosition.y - glfwPos.y);

    float W = viewport->Size.x;
    float H = viewport->Size.y;

    int N = rows;     // total rows
    int M = columns;  // total columns

    float cellWidth = W / M;
    float cellHeight = H / N;

    int row = 0;
    int col = 0;

    int i = 1;
    for (const auto& panel : panels) {
        int colSpan = colspans[i - 1];

        // Wrap to next row if the current panel can't fit in remaining columns
        if (col + colSpan > M) {
            row++;
            col = 0;
        }

        // If we're out of rows, stop rendering
        if (row >= N) break;

        float xpos = col * cellWidth;
        float ypos = row * cellHeight;
        float width = colSpan * cellWidth;
        float height = cellHeight;

        ImGui::SetNextWindowPos(
            ImVec2(viewport->Pos.x + xpos, viewport->Pos.y + ypos),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height));

        panel->Render();

        // Advance column by colSpan
        col += colSpan;
        i++;
    }
}