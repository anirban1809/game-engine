#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Window.h"
#include "FrameBuffer.h"

class Application {
   public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();  // Main loop

   protected:
    virtual void OnInit(){};    // To be overridden for custom initialization
    virtual void OnUpdate(){};  // Override for updating logic
    virtual void OnRender(){};  // Override for custom rendering
    virtual void OnKeyPressed(int key) {}
    virtual void OnMousePressed(int button) {}
    virtual void OnMouseReleased(int button) {}
    virtual void OnMouseMoved(double xpos, double ypos) {}
    Camera camera;
    Light light;
    Window *window;
    FrameBuffer *sceneBuffer;
    bool IsSceneClicked;
    bool ShowSettingsWindow;

   private:
    Renderer *renderer;
};

#endif  // __APPLICATION_H__
