//
//  error.cpp
//  XJS
//
//  Created by Jonas Böttiger on 12.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#include "error.h"

std::string parseError(jsPluginError error) {
    if (error == XJS_NO_ERROR) return "No error";
    if (error == XJS_CONFIG_ERROR) return "Config error";
    if (error == XJS_CONTEXT_INIT_ERROR) return "Context error";
    if (error == XJS_GLOBAL_INIT_ERROR) return "Global error";
    if (error == XJS_COMPILE_ERROR) return "Error when compiling script";
    if (error == XJS_NO_FUNCTIONS) return "Missing functions";
    return ("Unknown error: " + std::to_string(error));
}

void catchError(JSContext * context) {
    JS::RootedValue errorVal(context);
    if (!JS_IsExceptionPending(context)) return;
    if (!JS_GetPendingException(context, &errorVal)) return;
    JS_ClearPendingException(context);
    if (!errorVal.isObject()) return;
    JS::RootedObject errorObj(context, errorVal.toObjectOrNull());
    JSErrorReport * error = JS_ErrorFromException(context, errorObj);
    if (!error) throw "Unknown error";
    if (!error->filename) error->filename = "";
    std::string line = JS_EncodeString(context, JS_NewUCStringCopyZ(context, error->linebuf()));
    std::string message = std::string("In file ") + error->filename + ", line " + std::to_string(error->lineno) + ": \n" + line + "\n" + error->message().c_str();
    throw message;
}
