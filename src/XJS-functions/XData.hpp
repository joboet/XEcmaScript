//
//  XData.hpp
//  XJS
//
//  Created by Jonas Böttiger on 11.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef XData_hpp
#define XData_hpp

#include <jsapi.h>
#include "error.h"

jsPluginError initXData(JSContext * context, JS::Handle<JSObject*> global);

#endif /* XData_hpp */
