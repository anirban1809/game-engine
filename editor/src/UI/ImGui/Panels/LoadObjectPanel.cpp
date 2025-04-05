#include "../../../../include/UI/ImGui/Panels/LoadObjectPanel.h"
#include "../../../../../vendor/imgui/imgui.h"

#include <iostream>
#include <sstream>
LoadObjectPanel::LoadObjectPanel(ApplicationState& state) : state(state) {
    fileDialog.SetTitle("title");
    fileDialog.SetTypeFilters({".h", ".cpp"});
    path = "/Users/anirban/Documents/Code/engine";
}

void LoadObjectPanel::RenderTree(const std::string& path,
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
        if (ImGui::Selectable(item.name.c_str())) {
            ImGui::Text("%s", item.name.c_str());
            if (ImGui::IsItemClicked()) {
                std::cout << "item: " + item.name << std::endl;
            }
        }
    }
}

std::string FormatBytes(size_t bytes) {
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    double value = static_cast<double>(bytes);
    int suffixIndex = 0;

    while (value >= 1024 && suffixIndex < 4) {
        value /= 1024.0;
        ++suffixIndex;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << value << " "
        << suffixes[suffixIndex];
    return oss.str();
}
void LoadObjectPanel::Render() {
    if (!state.openLoadObjectPanel) { return; }

    CenterPanel(600, 400);
    SetSize(700, 400);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::Begin("Load Object File", &state.openLoadObjectPanel);

    // back button
    if (ImGui::Button("Back")) { path = path + "/.."; }

    ImGui::BeginChild("Load", ImVec2(500, 300));
    std::vector<FileSystemItem> items = state.fs.GetItems(path);

    if (ImGui::BeginTable("CollapsibleTable", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Size");

        ImGui::TableHeadersRow();
        for (auto const& item : items) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            if (ImGui::Selectable(item.name.c_str())) {
                ImGui::Text("%s", item.name.c_str());
            }
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (item.type == ITEM_DIRECTORY) {
                    path = path + '/' + item.name;

                } else {
                    if (item.extension == ".obj") {
                        state.assets.push_back(item.name);
                    }
                }
            }

            ImGui::TableSetColumnIndex(1);
            if (item.type != ITEM_FILE) {
                ImGui::Text("-");
            } else {
                ImGui::Text("%s", FormatBytes(item.size).c_str());
            }
        }

        ImGui::EndTable();
    }
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar();
}
