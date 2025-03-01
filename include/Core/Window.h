#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"

class Window {
public:
  Window(int width, int height, const char *title);
  ~Window();

  bool ShouldClose() const;
  void PollEvents();
  void SwapBuffers();

private:
  GLFWwindow *window;
};

#endif // __WINDOW_H__