#include "window.hpp"

window::window(int left, int top, int right, int bottom, std::string name, bool translucent) :
  widget(left, top, right, bottom, name, xpWidgetClass_MainWindow, false){
  int intranslucent = translucent ? xpMainWindowStyle_Translucent : xpMainWindowStyle_MainWindow;
  XPSetWidgetProperty(ID, xpProperty_MainWindowType, intranslucent);
  XPSetWidgetProperty(ID, xpProperty_MainWindowHasCloseBoxes, 1);
}

window::~window() {
  
}