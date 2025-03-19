#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"
#include <functional>

class Window {
   public:
    using KeyCallback = std::function<void(int key, int action)>;
    using MouseButtonCallback = std::function<void(int button, int action)>;
    using MouseCursorCallback = std::function<void(double xpos, double ypos)>;

    Window(int width, int height, const char *title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();

    void SetKeyCallback(KeyCallback callback);
    void SetMouseButtonCallback(MouseButtonCallback callback);
    void SetMouseCursorCallback(MouseCursorCallback callback);
    GLFWwindow *GetGLFWWindow();

   private:
    GLFWwindow *window;
    static void KeyCallbackInternal(GLFWwindow *window, int key, int scancode,
                                    int action, int mods);
    static void MouseButtonCallbackInternal(GLFWwindow *window, int button,
                                            int action, int mods);

    static void MouseCursorCallbackInternal(GLFWwindow *window, double xpos,
                                            double ypos);

    KeyCallback keyCallback;
    MouseButtonCallback mouseButtonCallback;
    MouseCursorCallback mouseCursorCallback;
};

#endif  // __WINDOW_H__