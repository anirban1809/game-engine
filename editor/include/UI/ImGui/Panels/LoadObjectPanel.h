#ifndef __LOADOBJECTPANEL_H__
#define __LOADOBJECTPANEL_H__

#include "../ImGuiPanel.h"
#include "../../ApplicationState.h"
#include "../../../../../vendor/imgui/imgui.h"
#include "./../../../../../vendor/imgui-filebrowser/imfilebrowser.h"

class LoadObjectPanel : public ImGuiPanel {
   public:
    LoadObjectPanel(ApplicationState& state);
    void Render() override;

   private:
    void RenderTree(const std::string& path, const FileSystemItem& item);
    ApplicationState& state;
    ImGui::FileBrowser fileDialog;
    std::string path = "";
};

#ifdef PLATFORM_MAC
std::string OpenMacFileDialog(bool&);
#endif

#endif  // __LOADOBJECTPANEL_H__
