#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Core/Camera.h"
#include "Core/Light.h"
#include "Renderer.h"
#include "Window.h"

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
    virtual void OnMousePressed(int key) {}
    Camera camera;
    Light light;
    Window *window;

   private:
    Renderer *renderer;
};

#endif  // __APPLICATION_H__