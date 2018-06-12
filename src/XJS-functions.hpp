#ifndef XJS_functions_hpp
#define XJS_functions_hpp

#include "error.h"
#include <jsapi.h>

jsPluginError initXJSClasses(JSContext * context, JS::Handle<JSObject*> global);

#endif /* XJS_functions_hpp */
