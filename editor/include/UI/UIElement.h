#ifndef __UIPANEL_H__
#define __UIPANEL_H__

class UIElement {
   public:
    virtual ~UIElement() = default;
    virtual void Render() = 0;
    void SetXPosition(float);
    void SetYPosition(float);
    void SetWidth(float);
    void SetHeight(float);
    void SetIsChildElement(bool);

   protected:
    float xposition;
    float yposition;
    float width;
    float height;
    bool isChildElement = false;
};

#endif  // __UIPANEL_H__