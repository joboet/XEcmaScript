#ifndef plugin_hpp
#define plugin_hpp

#include <string>

#include <jsapi.h>
#include <nlohmann/json.hpp>

#include "error.h"
#include "XJS-functions.hpp"
#include "files.hpp"
#include <vector>

enum jsPluginType {
    JS_GLOBAL,
    JS_AIRCRAFT,
    JS_SCENERY
};

class jsPlugin {
public:
    jsPlugin(std::string pathToConfig, jsPluginError * errorOut);
    ~jsPlugin();
    void jsPluginEnable();
    void jsPluginDisable();
    std::string name;
private:
    std::string path;
    jsPluginType type;
    bool enabled;
    JSRuntime * runtime;
    JSContext * context;
    JS::PersistentRootedObject * global;
};

#endif /* plugin_hpp */
