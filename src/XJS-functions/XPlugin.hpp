//
//  XPlugin.hpp
//  XJS
//
//  Created by Jonas Böttiger on 12.05.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#ifndef XPlugin_hpp
#define XPlugin_hpp

#include <jsapi.h>

void initPlugin(JSContext * context, JS::HandleObject global);

#endif /* XPlugin_hpp */
