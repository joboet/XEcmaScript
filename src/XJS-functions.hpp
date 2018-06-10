//
//  XJS-functions.hpp
//  XJS
//
//  Created by Jonas Böttiger on 10.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef XJS_functions_hpp
#define XJS_functions_hpp

#include "error.h"
#include <jsapi.h>

jsPluginError initXJSClasses(JSContext * context, JS::Handle<JSObject*> global);

#endif /* XJS_functions_hpp */
