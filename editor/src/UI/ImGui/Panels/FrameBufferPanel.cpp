#include "../../../../include/UI/ImGui/Panels/FrameBufferPanel.h"
#include "../../../../../vendor/imgui/imgui.h"

FramebufferPanel::FramebufferPanel(const std::string& title,
                                   unsigned int textureID,
                                   const glm::ivec2& size)
    : title(title), textureID(textureID), textureSize(size) {}

void FramebufferPanel::SetTexture(unsigned int id, const glm::ivec2& size) {
    textureID = id;
    textureSize = size;
}

void FramebufferPanel::Render() {
    ImGui::Begin(title.c_str());

    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    // Maintain aspect ratio
    float aspect = static_cast<float>(textureSize.x) / textureSize.y;
    float displayWidth = availableSize.x;
    float displayHeight = displayWidth / aspect;

    if (displayHeight > availableSize.y) {
        displayHeight = availableSize.y;
        displayWidth = displayHeight * aspect;
    }

    ImGui::Image(static_cast<ImTextureID>(textureID),
                 ImVec2(displayWidth, displayHeight), ImVec2(0, 1),
                 ImVec2(1, 0));
    // Flip vertically for OpenGL default

    ImGui::End();
}
