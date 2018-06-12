#ifndef error_h
#define error_h

#include <string>
#include <jsapi.h>

enum jsPluginError {
    XJS_NO_ERROR,
    XJS_CONFIG_ERROR,
    XJS_CONTEXT_INIT_ERROR,
    XJS_GLOBAL_INIT_ERROR,
    XJS_COMPILE_ERROR,
    XJS_NO_FUNCTIONS
};

std::string parseError(jsPluginError error);

void catchError(JSContext * context);

#endif /* error_h */
