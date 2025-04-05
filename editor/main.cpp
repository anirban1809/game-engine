#include "Editor.h"
int main() {
    Editor *app = new Editor(2560, 1330, "Editor");
    app->Run();
    return 0;
}
