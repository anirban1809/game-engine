#ifndef __UIPANEL_H__
#define __UIPANEL_H__

class UIPanel {
   public:
    virtual ~UIPanel() = default;
    virtual void Render() = 0;
};

#endif  // __UIPANEL_H__