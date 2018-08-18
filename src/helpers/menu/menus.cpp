#include "menus.hpp"
#include "../../log.hpp"

menu::menu(std::string name, menutype type, menu * parent) {
  XPLMMenuID parentID;
  int item;
  if (parent) {
    parentID = parent->ID;
    item = parent->appendLabel(name).index;
  } else {
    if (type == XP_MENU_AIRCRAFT) {
      parentID = XPLMFindAircraftMenu();
    } else {
      parentID = XPLMFindPluginsMenu();
    }
    item = XPLMAppendMenuItem(parentID, name.c_str(), NULL, 0);
  }

  ID = XPLMCreateMenu(name.c_str(), parentID, item, &this->callback, (void*)this);
}

menu::~menu() {
  XPLMDestroyMenu(ID);
  for (auto i = itemlist.begin(); i != itemlist.end(); i++) {
    i->destroy();
  }
}

menuitem menu::appendLabel(std::string name) {
  int index = XPLMAppendMenuItem(ID, name.c_str(), NULL, 0);
  return menuitem(ID, index);
}

menuitem menu::appendItem(std::string name, menuCallback callback, void * ref) {
  item * menuitemref = new item();
  menuitemref->callback = callback;
  menuitemref->ref = ref;
  int index = XPLMAppendMenuItem(ID, name.c_str(), menuitemref, 0);
  if (index < 0) Xlog << "Failed to initialize menu item " + name;
  return menuitem(ID, index);
}

void menu::appendSeparator() {
  XPLMAppendMenuSeparator(ID);
}

void menu::callback(void * menuref, void * itemref) {
  if (itemref) {
    item * menuitem = (item*)itemref;
    menuitem->callback(menuitem->ref);
  }
}



menuitem::menuitem(XPLMMenuID ID, int index) {
  this->ID = ID;
  this->index = index;
}

menuitem::~menuitem() {

}

void menuitem::enable() {
  XPLMEnableMenuItem(ID, index, 1);
}

void menuitem::disable() {
  XPLMEnableMenuItem(ID, index, 0);
}
