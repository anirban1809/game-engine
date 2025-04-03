#include "../../../../include/UI/ImGui/Panels/FileBrowserPanel.h"
#include "../../../../../vendor/imgui/imgui.h"
#include <iostream>

FileBrowserPanel::FileBrowserPanel(ApplicationState& s) : state(s) {}

void FileBrowserPanel::RenderTree(const std::string& path,
                                  const FileSystemItem& item) {
    if (item.type == ItemType::ITEM_DIRECTORY) {
        if (ImGui::TreeNode(item.name.c_str())) {
            auto items = state.fs.GetItems(path + "/" + item.name);
            for (auto const& childItem : items) {
                RenderTree(path + "/" + item.name, childItem);
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text("%s", item.name.c_str());
        if (ImGui::IsItemClicked()) {
            std::cout << "item: " + item.name << std::endl;
        }
    }
}

void FileBrowserPanel::Render() {
    SetPositionAndSize();

    ImGui::Begin("File Browser");
    std::string path = "/Users/anirban/Documents/Code/engine";
    auto items = state.fs.GetItems(path);

    for (auto const& item : items) { RenderTree(path, item); }

    ImGui::End();
}
