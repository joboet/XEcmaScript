//
//  XProcessing.hpp
//  XJS
//
//  Created by Jonas Böttiger on 13.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef XProcessing_hpp
#define XProcessing_hpp

#include <jsapi.h>
#include "error.h"

void initXProcessing(JSContext * context, JS::Handle<JSObject*> global);

#endif /* XProcessing_hpp */
