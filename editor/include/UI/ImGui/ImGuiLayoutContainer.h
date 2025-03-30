#ifndef __IMGUILAYOUTCONTAINER_H__
#define __IMGUILAYOUTCONTAINER_H__

#include "../LayoutContainer.h"

class ImGuiLayoutContainer : public LayoutContainer {
   public:
    ImGuiLayoutContainer(int, int);
    void Render();
};

template <typename T, typename... Args>
std::unique_ptr<T> CreateLayoutContainer(Args&&... args) {
    static_assert(std::is_base_of<LayoutContainer, T>::value,
                  "T must inherit from UIPanel");
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif  // __IMGUILAYOUTCONTAINER_H__