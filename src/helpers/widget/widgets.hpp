#ifndef widgets_hpp
#define widgets_hpp

#include <Widgets/XPWidgets.h>

#include <string>

class widget {
protected:
public:
  widget(int left, int top, int right, int bottom, std::string name, XPWidgetClass type, bool visible = true);
  ~widget();
  XPWidgetID ID;
  widget * parent = NULL;
  virtual void attach(widget * widget);
  virtual void show();
  virtual void hide();
};

#endif