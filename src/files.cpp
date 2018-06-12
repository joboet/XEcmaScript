#include "files.hpp"

const char * separator;
std::string pathToPlugin;

void initPath() {
    char * pathToPluginTemp = new char[256];
    XPLMGetPluginInfo(XPLMGetMyID(), nullptr, pathToPluginTemp, nullptr, nullptr);
    pathToPlugin = pathToPluginTemp;
    
    separator = XPLMGetDirectorySeparator();
}

std::string getPluginPath() {
    return pathToPlugin;
}

std::string joinPath(std::string first, std::string second) {
    std::string result = first;
    result += separator;
    result += second;
    return result;
}

char getseparator() {
    return *separator;
}
