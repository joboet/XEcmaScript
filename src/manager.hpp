//
//  manager.hpp
//  XJS
//
//  Created by Jonas Böttiger on 11.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef manager_hpp
#define manager_hpp

#include <vector>
#include <map>
#include "plugin.hpp"
#include "files.hpp"
#include "error.h"

class pluginManager {
public:
    pluginManager(jsPluginType type);
    ~pluginManager();
    void reloadPlugins();
    void reloadPlugin(uint32_t id);
    void enablePlugins();
    void disablePlugins();
    jsPlugin * operator[](std::string name);
    jsPlugin * operator[](uint32_t id);
    uint32_t operator+=(jsPlugin * plugin);
    uint32_t operator+=(std::string pathToConfig);
    void operator-=(std::string name);
    void operator-=(uint32_t id);
private:
    jsPluginType type;
    void loadPlugins();
    void unloadPlugins();
    std::map<std::string, uint32_t> names;
    std::vector<jsPlugin*> plugins;
};

#endif /* manager_hpp */
