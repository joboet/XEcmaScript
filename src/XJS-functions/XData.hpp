#ifndef XData_hpp
#define XData_hpp

#include <jsapi.h>
#include "error.h"

jsPluginError initXData(JSContext * context, JS::Handle<JSObject*> global);

#endif /* XData_hpp */
