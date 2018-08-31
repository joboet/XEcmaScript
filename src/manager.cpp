#include "manager.hpp"
#include <boost/filesystem.hpp>
#include "log.hpp"

pluginManager::pluginManager(jsPluginType type) {
    this->type = type;
    loadPlugins();
}

pluginManager::~pluginManager() {
    unloadPlugins();
}

void pluginManager::reloadPlugins() {
    unloadPlugins();
    loadPlugins();
}

void pluginManager::reloadPlugin(uint32_t id) {

}

void pluginManager::enablePlugins() {
    for (auto i = plugins.begin(); i != plugins.end(); i++) {
        if (*i != nullptr) (*i)->jsPluginEnable();
    }
}

void pluginManager::disablePlugins() {
    for (auto i = plugins.begin(); i != plugins.end(); i++) {
        if (*i != nullptr) (*i)->jsPluginDisable();
    }
}

jsPlugin * pluginManager::operator[](std::string name) {
    return plugins[names[name]];
}

jsPlugin * pluginManager::operator[](uint32_t id) {
    return plugins[id];
}

uint32_t pluginManager::operator+=(jsPlugin * plugin) {
    plugins.push_back(plugin);
    names[plugin->name] = plugins.size() - 1;
    return plugins.size() - 1;
}

uint32_t pluginManager::operator+=(std::string pathToConfig) {
    jsPlugin * plugin = new jsPlugin(pathToConfig, nullptr);
    plugins.push_back(plugin);
    names[plugin->name] = plugins.size() - 1;
    return plugins.size() - 1;
}

void pluginManager::operator-=(std::string name) {
    uint32_t id = names[name];
    names.erase(names.find(name));
    if (plugins[id] != nullptr) delete plugins[id];
    plugins[id] = nullptr;
}

void pluginManager::operator-=(uint32_t id) {
    if (plugins[id] != nullptr) delete plugins[id];
    plugins[id] = nullptr;
}

void pluginManager::loadPlugins() {
try {
    boost::filesystem::path oldPath = boost::filesystem::current_path();
    std::string nativescriptspath = getPluginPath();
    boost::filesystem::path pluginpath(nativescriptspath.c_str());
    if (pluginpath.has_filename()) pluginpath.remove_filename();
    boost::filesystem::current_path(pluginpath);
    boost::filesystem::path scriptsdir("scripts");
    Xlog << "Loading plugins from " + pluginpath.string();
    for (boost::filesystem::recursive_directory_iterator iterator(scriptsdir); iterator != boost::filesystem::recursive_directory_iterator(); iterator++) {
        if (iterator->path().filename().string() == "plugin" && iterator->path().extension().string() == ".json") {
            Xlog << "Loading config " + iterator->path().string();
            operator+=(iterator->path().string());
        }
    }
    boost::filesystem::current_path(oldPath);
} catch(boost::filesystem::filesystem_error error) {
    Xlog << std::string(error.what());
}
}

void pluginManager::unloadPlugins() {
    names.clear();
    for (auto i = plugins.begin(); i != plugins.end(); i++) {
        if (*i != nullptr) delete *i;
        *i = nullptr;
    }
}
