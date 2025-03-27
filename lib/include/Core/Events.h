#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "../../../vendor/glfw-3.4.bin.MACOS/include/GLFW/glfw3.h"

enum KeyEvent {
    KEY_A = GLFW_KEY_A,
    KEY_B = GLFW_KEY_B,
    KEY_C = GLFW_KEY_C,
    KEY_D = GLFW_KEY_D,
    KEY_E = GLFW_KEY_E,
    KEY_F = GLFW_KEY_F,
    KEY_W = GLFW_KEY_W,
    KEY_S = GLFW_KEY_S
};

enum MouseEvent {
    MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};
;

#endif  // __EVENTS_H__