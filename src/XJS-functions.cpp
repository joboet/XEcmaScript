
//
//  XJS-functions.cpp
//  XJS
//
//  Created by Jonas Böttiger on 10.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

#include "XJS-functions.hpp"
#include "XJS-functions/XData.hpp"
#include "XJS-functions/XProcessing.hpp"
#include "XJS-functions/XUtilities.hpp"



jsPluginError initXJSClasses(JSContext * context, JS::Handle<JSObject*> global) {
    initXData(context, global);
    initXProcessing(context, global);
    initXUtilities(context, global);
    return XJS_NO_ERROR;
}
