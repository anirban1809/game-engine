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
    float G = gap;    // gap in pixels

    // Total gaps in layout: (M+1) vertical and (N+1) horizontal
    float cellWidth = (W - G * (M + 1)) / M;
    float cellHeight = (H - G * (N + 1)) / N;

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

        // Calculate position with gaps
        float xpos = G + col * (cellWidth + G);
        float ypos = G + row * (cellHeight + G);
        float width = colSpan * cellWidth + (colSpan - 1) * G;
        float height = cellHeight;

        float yOffset = row == 0 ? 25.0f : 0.0f;

        ImGui::SetNextWindowPos(
            ImVec2(viewport->Pos.x + xpos, viewport->Pos.y + ypos + yOffset),
            ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height - yOffset));

        panel->Render();

        // Advance column by colSpan
        col += colSpan;
        i++;
    }
}