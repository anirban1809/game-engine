#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include <functional>

class Window {
   public:
    using KeyCallback = std::function<void(int key, int action)>;
    using MouseCallback = std::function<void(int button, int action)>;

    Window(int width, int height, const char *title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();

    void SetKeyCallback(KeyCallback callback);
    void SetMouseCallback(MouseCallback callback);
    GLFWwindow *GetGLFWWindow();

   private:
    GLFWwindow *window;
    static void KeyCallbackInternal(GLFWwindow *window, int key, int scancode,
                                    int action, int mods);
    static void MouseCallbackInternal(GLFWwindow *window, int button,
                                      int action, int mods);
    KeyCallback keyCallback;
    MouseCallback mouseCallback;
};

#endif  // __WINDOW_H__