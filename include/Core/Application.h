#ifndef __APPLICATION_H__
#define __APPLICATION_H__

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

   private:
    Window *window;
    Renderer *renderer;
};

#endif  // __APPLICATION_H__