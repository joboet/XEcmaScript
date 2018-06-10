//
//  plugin.cpp
//  XJS
//
//  Created by Jonas Böttiger on 06.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#include "plugin.hpp"

#include <fstream>
#include <sstream>
#include <cstdio>
#include "log.hpp"

using json = nlohmann::json;

JSClassOps globalclassops = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    JS_GlobalObjectTraceHook
};

JSClass globalclass = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    &globalclassops
};

jsPlugin::jsPlugin(std::string pathToConfig, jsPluginError * errorOut) {
    jsPluginError errortemp = XJS_NO_ERROR;
    if (errorOut == nullptr) {
        errorOut = &errortemp;
    } else {
        *errorOut = XJS_NO_ERROR;
    }
    
try {
    json pluginConfig;
    {
        std::ifstream configFile(pathToConfig);
        std::stringstream configStream;
        configStream << configFile.rdbuf();
        pluginConfig = json::parse(configStream.str().c_str());
    }
    
    
    name = pluginConfig["name"];
    if (pluginConfig["type"].get<std::string>() == "global") {type = JS_GLOBAL;}
    else if (pluginConfig["type"].get<std::string>() == "aircraft") {type = JS_AIRCRAFT;}
    else if (pluginConfig["type"].get<std::string>() == "scenery") {type = JS_SCENERY;}
    else {throw XJS_CONFIG_ERROR;}
    path = pathToConfig.substr(0, pathToConfig.find_last_of('/'));
    
    Xlog << std::string("Loading plugin ") + name;
    
    // Init JavaScript context
    context = JS_NewContext(32 * 1024 * 1024);
    if (!context) {
        *errorOut = XJS_CONTEXT_INIT_ERROR;
        return;
    }
    if (!JS::InitSelfHostedCode(context)) {
        *errorOut = XJS_CONTEXT_INIT_ERROR;
        return;
    }
    
    JSAutoRequest request(context);
    
    JS::CompartmentOptions options;
    global = new JS::PersistentRootedObject(context);
    *global = JS_NewGlobalObject(context, &globalclass, NULL, JS::FireOnNewGlobalHook, options);
    
    if (!global) {
        *errorOut = XJS_GLOBAL_INIT_ERROR;
        return;
    } else if (!*global) {
        *errorOut = XJS_GLOBAL_INIT_ERROR;
        return;
    }
    
    JSAutoCompartment compartment(context, *global);
    JS_InitStandardClasses(context, *global);
    catchError(context);
    initXJSClasses(context, *global);
    catchError(context);
    
    // Compile and execute script files
    for (auto iterator = pluginConfig["files"].begin(); iterator != pluginConfig["files"].end(); iterator++) {
        std::string scriptname = *iterator;
        
        JS::CompileOptions compileoptions(context);
        JS::Rooted<JSScript*> script = JS::RootedScript(context);
        
        JS::RootedValue returnvalue(context);
        FILE * srcfile = fopen((path + boostseparator + scriptname).c_str(), "r");
        if (!JS::Compile(context, compileoptions, srcfile, &script)) {
            catchError(context);
        }
        if (!JS_ExecuteScript(context, script, &returnvalue)) {
            catchError(context);
        }
        fclose(srcfile);
    }
    
    // Check for callback functions and call XPluginStart()
    {
        JS::RootedValue XJSPluginStopHandle(context);
        JS::RootedValue rval(context);
        
        JS::AutoValueArray<1> args(context);
        args[0].set(JS::UndefinedValue());

        if (!JS_GetProperty(context, *global, "XPluginStop", &XJSPluginStopHandle)) {
            throw XJS_NO_FUNCTIONS;
        } else if (!(XJSPluginStopHandle.isObject() && JS::IsCallable(&XJSPluginStopHandle.toObject()))) {
            throw XJS_NO_FUNCTIONS;
        } else if (!JS_CallFunctionName(context, *global, "XPluginStart", args, &rval)) {
            catchError(context);
        }
    }
    Xlog << "Successfully loaded plugin " + name;
} catch (jsPluginError error) {
    *errorOut = error;
    Xlog << "Error in plugin " + name + ": " + parseError(error);
} catch (std::string error) {
    Xlog << "Error in plugin " + name + ": " + error;
}
}

jsPlugin::~jsPlugin() {
    {
        JSAutoRequest request(context);
        JSAutoCompartment compartment(context, global->get());
        JS::RootedValue rval(context);
        JS::AutoValueArray<1> args(context);
        args[0].set(JS::NullValue());
        JS_CallFunctionName(context, *global, "XPluginStop", args, &rval);
    }
    
    delete global;
    JS_DestroyContext(context);
}

void jsPlugin::jsPluginEnable() {
    
}

void jsPlugin::jsPluginDisable() {
    
}
