#include "../../../../include/UI/ImGui/Panels/AssetLibraryPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>

AssetLibraryPanel::AssetLibraryPanel(ApplicationState& s) : state(s) {}

void AssetLibraryPanel::Render() {
    SetPositionAndSize();

    ImGui::Begin("Asset Library");

    for (auto const& item : state.assets) { ImGui::Text("%s", item.c_str()); }

    ImGui::End();
}
