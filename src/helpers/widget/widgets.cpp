#include "widgets.hpp"

widget::widget(int left, int top, int right, int bottom, std::string name, XPWidgetClass type, bool visible) {
  XPCreateWidget(left, top, right, bottom, visible, name.c_str(), 1, 0, type);
}

widget::~widget() {
  XPDestroyWidget(ID, 0);
}

void widget::attach(widget * widget) {
  XPPlaceWidgetWithin(widget->ID, ID);
  widget->parent = this;
}

void widget::show() {
  XPShowWidget(ID);
}

void widget::hide() {
  XPHideWidget(ID);
}
