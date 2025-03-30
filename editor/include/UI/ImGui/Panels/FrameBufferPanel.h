#pragma once

#include "../ImGuiPanel.h"
#include <string>
#include "../../../../../vendor/glm/glm.hpp"
#include "../../../../../lib/include/Core/FrameBuffer.h"
#define GL_SILENCE_DEPRECATION
#include "../ImGuiLayer.h"

class FramebufferPanel : public ImGuiPanel {
   public:
    FramebufferPanel(const std::string& title, ImGuiState& state,
                     FrameBuffer* sceneBuffer, const glm::ivec2& size,
                     const glm::vec2 position);
    void Render() override;

    void SetTexture(unsigned int id, const glm::ivec2& size);

    bool IsHovered() const;
    glm::ivec2 GetMouseFramebufferCoords() const;

   private:
    ImGuiState& state;
    std::string title;
    FrameBuffer* sceneBuffer;
    glm::ivec2 textureSize;
    uint32 textureID;
    bool isHovered = false;
    glm::ivec2 mouseCoords = {0, 0};
};
