#include "plugins.hpp"

#include "../helpers/widget/widget.h"
#include "../log.hpp"

window * pluginsWindow;

void initPlugins() {
  pluginsWindow = new window(200, 200, 200, 200, "XEcmaScript plugins");
  Xlog << "Initialized plugins window";
}

bool shown = false;

void toggleWindow(void * ref) {
  Xlog << "Debug";
  if (shown) {
    pluginsWindow->hide();
    shown = false;
  } else {
    pluginsWindow->show();
    shown = true;
  }
}
