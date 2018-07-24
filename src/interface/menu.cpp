#include "menu.hpp"

#include "plugins.hpp"

menu * mainMenu;

void initMenus() {
  mainMenu = new menu("XEcmaScript", XP_MENU_PLUGINS);
  mainMenu->appendItem("Plugins", toggleWindow, (void*)mainMenu);
}
