#ifndef menus_hpp
#define menus_hpp

#include <string>
#include <list>

#include <XPLMMenus.h>

#include "../pointer/pointer.hpp"

typedef void (*menuCallback)(void*);

enum menutype {
  XP_MENU_PLUGINS,
  XP_MENU_AIRCRAFT
};

struct item {
  menuCallback callback;
  void * ref;
};

class menuitem {
public:
  menuitem(XPLMMenuID ID, int index);
  ~menuitem();
  void enable();
  void disable();
  int index;
private:
  XPLMMenuID ID;
};

class menu {
public:
  menu(std::string name, menutype type, menu * parent = NULL);
  ~menu();
  menuitem appendLabel(std::string name);
  menuitem appendItem(std::string name, menuCallback callback, void * ref);
  void appendSeparator();
  XPLMMenuID ID;
private:
  std::list<safe_pointer<item>> itemlist;
  static void callback(void * menuref, void * itemref);
};

#endif
