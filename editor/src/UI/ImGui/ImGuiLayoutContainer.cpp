#include "../../../include/UI/ImGui/ImGuiLayoutContainer.h"
#include "../../../../vendor/imgui/imgui.h"
#include "../../../../lib/include/Core/Types.h"

#include <iostream>

ImGuiLayoutContainer::ImGuiLayoutContainer(int r, int c)
    : LayoutContainer(r, c) {}

void ImGuiLayoutContainer::Render() {
    float X, Y, W, H;
    if (!isChildElement) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 globalPosition = ImGui::GetWindowPos();
        ImVec2 glfwPos = viewport->Pos;
        ImVec2 relativePos(globalPosition.x - glfwPos.x,
                           globalPosition.y - glfwPos.y);

        X = viewport->Pos.x;
        Y = viewport->Pos.y;
        W = viewport->Size.x;
        H = viewport->Size.y;
    } else {
        X = xposition;
        Y = yposition;
        W = width;
        H = height;
    }

    int N = rows;     // total rows
    int M = columns;  // total columns
    float G = gap;    // gap in pixels

    // Total gaps in layout: (M+1) vertical and (N+1) horizontal
    float cellWidth = (W - G * (M + 1)) / M;
    float cellHeight = (H - G * (N + 1)) / N;

    int row = 0;
    int col = 0;

    int i = 1;
    for (const auto& panel : elements) {
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
        float w = colSpan * cellWidth + (colSpan - 1) * G;
        float h = cellHeight;

        float yOffset = row == 0 ? 14.0f : 0.0f;

        panel->SetXPosition(X + xpos);
        panel->SetYPosition(Y + ypos + yOffset);

        panel->SetHeight(h - yOffset);
        panel->SetWidth(w);

        panel->Render();

        // Advance column by colSpan
        col += colSpan;
        i++;
    }
}