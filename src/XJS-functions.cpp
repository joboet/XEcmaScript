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
