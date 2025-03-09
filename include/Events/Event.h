#ifndef __EVENT_H__
#define __EVENT_H__

enum class EventType {
    None = 0,

    /**Window Events */
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,

    /* Keyboard Events */
    KeyPressed,
    KeyReleased,

    /**Mouse Events */
    MousePressed,
    MouseReleased,
    MouseMoved,
    MouseScrolled,
};

class Event {};

#endif  // __EVENT_H__