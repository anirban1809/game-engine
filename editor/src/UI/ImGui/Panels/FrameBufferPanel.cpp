#include "../../../../include/UI/ImGui/Panels/FrameBufferPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "../../../../../lib/include/Core/FrameBuffer.h"
#define GL_SILENCE_DEPRECATION
#include "../../../../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include <iostream>

FramebufferPanel::FramebufferPanel(const std::string& title, ImGuiState& state,
                                   FrameBuffer* sceneBuffer,
                                   const glm::ivec2& size)
    : title(title), sceneBuffer(sceneBuffer), textureSize(size), state(state) {}

void FramebufferPanel::SetTexture(unsigned int id, const glm::ivec2& size) {
    textureID = sceneBuffer->GetFrameTexture();
    textureSize = size;
}

void FramebufferPanel::Render() {
    ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoMove);
    state.isFrameBufferPanelHovered = ImGui::IsWindowHovered();
    // ImGui::Text("const char *fmt, ...");

    // // Get global position of the current ImGui window.
    ImVec2 globalPos = ImGui::GetWindowPos();

    // Get the main viewport position, which usually corresponds to the GLFW
    // window's top-left.
    ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImVec2 glfwPos = mainViewport->Pos;

    // Compute the relative position.
    ImVec2 relativePos(globalPos.x - glfwPos.x, globalPos.y - glfwPos.y);

    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    sceneBuffer->RescaleFrameBuffer(window_width, window_height);

    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImVec2 position = ImGui::GetCursorScreenPos();

    ImGui::Image(static_cast<ImTextureID>(
                     static_cast<uintptr_t>(sceneBuffer->GetFrameTexture())),
                 ImVec2(window_width, window_height), ImVec2(0, 1),
                 ImVec2(1, 0));

    ImGui::End();
}

bool FramebufferPanel::IsHovered() const { return isHovered; }

glm::ivec2 FramebufferPanel::GetMouseFramebufferCoords() const {
    return mouseCoords;
}
