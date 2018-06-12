#include "XUtilities.hpp"
#include "log.hpp"
#include <XPLMUtilities.h>

JSClass XUtilitiesClass = {
    "XUtilities",
    JSCLASS_HAS_PRIVATE
};

bool XUtilitiesLog(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    try {
        if (!argv[0].isString()) throw "Wrong value type provided, expected string";
        const char * string = JS_EncodeString(context, argv[0].toString());
        XPLMDebugString(string);
        argv.rval().setUndefined();
    } catch (std::string error) {
        JS_ReportErrorUTF8(context, "%s", error.c_str());
        return false;
    }
    return true;
}

JSFunctionSpec XUtilitiesFunctions[] = {
    JS_FN("log", XUtilitiesLog, 1, JSPROP_PERMANENT | JSPROP_READONLY),
    JS_FS_END
};

void initXUtilities(JSContext * context, JS::HandleObject global) {
    try {
        JS::RootedObject XUtilities(context, JS_DefineObject(context, global, "XUtilities", &XUtilitiesClass));
        catchError(context);
        if (!JS_DefineFunctions(context, XUtilities, XUtilitiesFunctions)) catchError(context);
        
    } catch (std::string error) {
        Xlog << error;
    }
}
