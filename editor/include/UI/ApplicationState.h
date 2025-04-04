#ifndef __APPLICATIONSTATE_H__
#define __APPLICATIONSTATE_H__

#include "../../../lib/include/Core/Types.h"
#include "../Core/FileSystem.h"

#include <vector>
#include <string>

class ApplicationState {
   public:
    ApplicationState(FileSystem& fs, std::vector<std::string>& assets)
        : fs(fs), assets(assets) {}
    bool isFrameBufferPanelHovered = false;
    int stateValue = 0;
    std::vector<std::string> nodes = {"Entity 1", "Entity 2", "Camera 1"};
    uint32 selectedNode;
    FileSystem& fs;
    bool openLoadObjectPanel = false;
    std::vector<std::string>& assets;

   private:
};

#endif  // __APPLICATIONSTATE_H__