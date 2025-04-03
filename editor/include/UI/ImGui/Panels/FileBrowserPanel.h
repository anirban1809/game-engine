#ifndef __FILEBROWSERPANEL_H__
#define __FILEBROWSERPANEL_H__

#include "../ImGuiPanel.h"
#include "UI/ApplicationState.h"

class FileBrowserPanel : public ImGuiPanel {
   public:
    FileBrowserPanel(ApplicationState&);
    void Render();
    void RenderTree(const std::string& path, const FileSystemItem& item);

   private:
    ApplicationState& state;
    std::string current;
};

#endif  // __FILEBROWSERPANEL_H__
