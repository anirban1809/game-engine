#ifndef __IMGUIPANEL_H__
#define __IMGUIPANEL_H__

#include "../UIElement.h"

enum LayoutType {
    GRID,
    FLEX,
};

class ImGuiPanel : public UIElement {
   public:
    virtual ~ImGuiPanel() = default;
    virtual void Render() override = 0;
    void SetPositionAndSize();
};

#include <memory>

template <typename T, typename... Args>
std::shared_ptr<T> CreatePanel(Args&&... args) {
    static_assert(std::is_base_of<UIElement, T>::value,
                  "T must inherit from UIPanel");
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif  // __IMGUIPANEL_H__