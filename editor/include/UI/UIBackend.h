#ifndef __UIBackend_H__
#define __UIBackend_H__

class UIBackend {
   public:
    virtual ~UIBackend() = default;

    virtual void Init(void* windowHandle) = 0;  // generic window pointer
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Shutdown() = 0;
};

#endif  // __UIBackend_H__