#pragma once

#include "../ImGuiPanel.h"
#include <string>
#include "../../../../../vendor/glm/glm.hpp"

class FramebufferPanel : public ImGuiPanel {
   public:
    FramebufferPanel(const std::string& title, unsigned int textureID,
                     const glm::ivec2& size);
    void Render() override;

    void SetTexture(unsigned int id, const glm::ivec2& size);

   private:
    std::string title;
    unsigned int textureID;
    glm::ivec2 textureSize;
};
