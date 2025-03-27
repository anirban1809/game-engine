#ifndef __IMGUIPANEL_H__
#define __IMGUIPANEL_H__

#include "../UIPanel.h"

class ImGuiPanel : public UIPanel {
   public:
    virtual ~ImGuiPanel() = default;
    virtual void Render() override = 0;
};

#include <memory>

template <typename T, typename... Args>
std::unique_ptr<T> CreatePanel(Args&&... args) {
    static_assert(std::is_base_of<UIPanel, T>::value,
                  "T must inherit from UIPanel");
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif  // __IMGUIPANEL_H__