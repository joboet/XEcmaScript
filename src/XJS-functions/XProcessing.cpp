//
//  XProcessing.cpp
//  XJS
//
//  Created by Jonas Böttiger on 13.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#include "XProcessing.hpp"
#include <XPLMProcessing.h>
#include "log.hpp"
#include <vector>

JS::PersistentRootedObject * instances;
uint32_t * numInstances;

struct info {
    JSContext * context;
    JS::PersistentRootedObject * global;
    uint8_t id;
};

float XJS_INTERN_FlightLoopCallback(float elapsedSinceLastCall, float elapsedSinceLastFlightLoop, int counter, void * refconIn) {
    info * refcon = (info*)refconIn;
    
    JSAutoRequest request(refcon->context);
    JSAutoCompartment compartment(refcon->context, refcon->global->get());
    
    JS::AutoValueArray<3> args(refcon->context);
    args[0].setDouble(elapsedSinceLastCall);
    args[1].setDouble(elapsedSinceLastFlightLoop);
    args[2].setInt32(counter);
    
    try {
        JS::RootedValue rval(refcon->context);
        JS::RootedValue objectValue(refcon->context);
        JS_GetElement(refcon->context, *instances, refcon->id, &objectValue);
        catchError(refcon->context);
        if (!objectValue.isObjectOrNull()) throw "Not initialised correctly";
        JS::RootedObject object(refcon->context, objectValue.toObjectOrNull());
        if (!JS_CallFunctionName(refcon->context, object, "onUpdate", args, &rval)) catchError(refcon->context);
        if (!rval.isNumber()) {catchError(refcon->context); throw "Wrong return type";}
        return rval.toNumber();
    } catch (std::string error) {
        Xlog << error;
        return 0.;
    }
}

void XProcessingProcessorDestructor(JSFreeOp * ops, JSObject * obj) {
    info * refcon = (info*)JS_GetPrivate(obj);
    XPLMUnregisterFlightLoopCallback(XJS_INTERN_FlightLoopCallback, (void*)refcon);
    // We leak the global object persistent, as deleting it crashes, as the object it refers to is already deleted (It's only a few bytes)
    delete refcon;
}

JSClassOps XProcessingProcessorOps = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    XProcessingProcessorDestructor,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass XProcessingProcessorClass = {
    "Processor",
    JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
    &XProcessingProcessorOps
};

bool XProcessingProcessorGetElapsedTime(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    float elapsedTime = XPLMGetElapsedTime();
    argv.rval().setDouble(elapsedTime);
    return true;
}

bool XProcessingProcessorGetElapsedCycles(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    int elapsedCycles = XPLMGetCycleNumber();
    argv.rval().setInt32(elapsedCycles);
    return true;
}

JSFunctionSpec XProcessingProcessorFunctions[] = {
    JS_FN("getElapsedTime", XProcessingProcessorGetElapsedTime, 0, JSPROP_PERMANENT | JSPROP_READONLY),
    JS_FN("getElapsedCycles", XProcessingProcessorGetElapsedCycles, 0, JSPROP_PERMANENT | JSPROP_READONLY),
    JS_FS_END
};

bool XProcessingProcessorConstructor(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    try {
        JS::RootedObject Processor(context, JS_NewObject(context, &XProcessingProcessorClass));
        catchError(context);
        JS_DefineFunctions(context, Processor, XProcessingProcessorFunctions);
        catchError(context);
        
        if (!argv[0].isObjectOrNull()) throw "No function provided";
        JS::RootedObject onUpdate(context, argv[0].toObjectOrNull());
        if (!JS::IsCallable(onUpdate)) throw "No function provided";
        
        JS_DefineProperty(context, Processor, "onUpdate", onUpdate, JSPROP_PERMANENT);
        catchError(context);
        
        info * refcon = new info();
        refcon->context = context;
        refcon->global = new JS::PersistentRootedObject(context, JS_GetGlobalForObject(context, Processor));
        refcon->id = *numInstances;
        JS_DefineElement(context, *instances, *numInstances, Processor, JSPROP_ENUMERATE);
        (*numInstances)++;
        
        JS_SetPrivate(Processor, (void*)refcon);
        
        XPLMRegisterFlightLoopCallback(XJS_INTERN_FlightLoopCallback, 2., (void*)refcon);
        
        argv.rval().setObjectOrNull((Processor).get());
        return true;
    } catch (std::string error) {
        JS_ReportErrorUTF8(context, "%s", error.c_str());
        return false;
    }
}

void XProcessingDestructor(JSFreeOp * ops, JSObject * object) {
    delete instances;
    delete numInstances;
}

JSClassOps XProcessingClassOps = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    XProcessingDestructor,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass XProcessingClass = {
    "XProcessing",
    JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE,
    &XProcessingClassOps
};

void initXProcessing(JSContext * context, JS::Handle<JSObject*> global) {
    try {
        JS::RootedObject XProcessing(context, JS_DefineObject(context, global, "XProcessing", &XProcessingClass));
        catchError(context);
        numInstances = new uint32_t;
        instances = new JS::PersistentRootedObject(context, JS_NewArrayObject(context, 64));
        catchError(context);
        
        JS::RootedObject Processor(context, JS_NewObject(context, &XProcessingProcessorClass));
        catchError(context);
        JS_InitClass(context, XProcessing, Processor, &XProcessingProcessorClass, XProcessingProcessorConstructor, 1, nullptr, nullptr, nullptr, nullptr);
    } catch (std::string error) {
        Xlog << error;
    }
    
}
