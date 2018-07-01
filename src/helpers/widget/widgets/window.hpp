#ifndef window_hpp
#define window_hpp

#include <Widgets/XPWidgets.h>
#include <Widgets/XPStandardWidgets.h>

#include <string>

#include "../widgets.hpp"

class window: public widget {
public:
  window(int left, int top, int right, int bottom, std::string name, bool translucent = false);
  ~window();
private:
};

#endif