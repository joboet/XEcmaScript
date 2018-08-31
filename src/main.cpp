#include <XPLMPlugin.h>
#include <XPLMProcessing.h>
#include <string>
#include <jsapi.h>
#include <js/Initialization.h>
#include "files.hpp"
#include "manager.hpp"
#include "log.hpp"
#include "XJS-functions/XGraphics.hpp"
#include "interface/plugins.hpp"
#include "interface/menu.hpp"

pluginManager * manager;

jsPluginType type;

float initScripts(float, float, int, void *) {
    Xlog << "Initialising plugin manager";
    manager = new pluginManager(type);
    initMenus();
    initPlugins();
    return 0;
}

PLUGIN_API int XPluginStart ( char * outName, char * outSignature, char * outDescription ) {
    strcpy(outName, "XEcmaScript");
    strcpy(outSignature, "JoBo.Plugin.XEcmaScript");
    strcpy(outDescription, "Ecmascript engine for X-Plane");

    XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);
    XPLMEnableFeature("XPLM_USE_NATIVE_WIDGET_WINDOWS", 1);

    initPath();

    if (!(getPluginPath().find("Custom Scenery") == std::string::npos)) {
        type = JS_SCENERY;
    } else if (!(getPluginPath().find("Aircraft") == std::string::npos)) {
        type = JS_AIRCRAFT;
    } else {
        type = JS_GLOBAL;
    }

    JS_Init();

    if (!JS_IsInitialized()) return 0;

    initOpenGL();

    XPLMRegisterFlightLoopCallback(initScripts, -1, NULL);

    return 1;
}

PLUGIN_API void XPluginStop (void) {
    if (manager) delete manager;
    JS_ShutDown();
}

PLUGIN_API void XPluginEnable (void) {
}

PLUGIN_API void XPluginDisable (void) {

}

PLUGIN_API void XPluginReceiveMessage (XPLMPluginID inFrom, int inMessage, void * inParam) {

}
