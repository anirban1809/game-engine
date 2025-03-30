#ifndef __APPLICATIONSTATE_H__
#define __APPLICATIONSTATE_H__

#include "../../../lib/include/Core/Types.h"

#include <vector>
#include <string>

class ApplicationState {
   public:
    bool isFrameBufferPanelHovered = false;
    int stateValue = 0;
    std::vector<std::string> nodes = {"Entity 1", "Entity 2", "Camera 1"};
    uint32 selectedNode;

   private:
};

#endif  // __APPLICATIONSTATE_H__