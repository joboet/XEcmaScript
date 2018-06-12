#include "XData.hpp"
#include <XPLMDataAccess.h>
#include "log.hpp"

int XJS_INTERN_getDatai(void * location) {
    return *((int*)location);
}

void XJS_INTERN_setDatai(void * location, int value) {
    *((int*)location) = value;
}

float XJS_INTERN_getDataf(void * location) {
    return *((float*)location);
}

void XJS_INTERN_setDataf(void * location, float value) {
    *((float*)location) = value;
}

double XJS_INTERN_getDatad(void * location) {
    return *((double*)location);
}

void XJS_INTERN_setDatad(void * location, double value) {
    *((double*)location) = value;
}

int XJS_INTERN_getDatavi(void * location, int * outValues, int inOffset, int inMax) {
    if (outValues) {
        int numCopied = 0;
        for (int i = inOffset; i < (inOffset + inMax); i++) {
            if (i < ((int*)location)[-1]) {
                outValues[numCopied] = ((int*)location)[i];
                numCopied++;
            } else {
                break;
            }
        }
        return numCopied;
    }
    return ((int*)location)[-1];
}

void XJS_INTERN_setDatavi(void * location, int * values, int offset, int count) {
    if (values) {
        int numCopied = 0;
        for (int i = offset; i < (offset + count); i++) {
            if (i < ((int*)location)[-1]) {
                ((int*)location)[i] = values[numCopied];
                numCopied++;
            } else {
                break;
            }
        }
    }
}

int XJS_INTERN_getDatavf(void * location, float * outValues, int inOffset, int inMax) {
    if (outValues) {
        int numCopied = 0;
        for (int i = inOffset; i < (inOffset + inMax); i++) {
            if (i < ((int*)location)[-1]) {
                outValues[numCopied] = ((float*)location)[i];
                numCopied++;
            } else {
                break;
            }
        }
        return numCopied;
    }
    return ((float*)location)[-1];
}

void XJS_INTERN_setDatavf(void * location, float * values, int offset, int count) {
    if (values) {
        int numCopied = 0;
        for (int i = offset; i < (offset + count); i++) {
            if (i < ((float*)location)[-1]) {
                ((float*)location)[i] = values[numCopied];
                numCopied++;
            } else {
                break;
            }
        }
    }
}

int XJS_INTERN_getDatab(void * location, void * outValues, int inOffset, int inMax) {
    if (outValues) {
        int numCopied = 0;
        for (int i = inOffset; i < (inOffset + inMax); i++) {
            if (i < ((char*)location)[-1]) {
                ((char*)outValues)[numCopied] = ((char*)location)[i];
                numCopied++;
            } else {
                break;
            }
        }
        return numCopied;
    }
    return ((char*)location)[-1];
}

void XJS_INTERN_setDatab(void * location, void * values, int offset, int count) {
    if (values) {
        int numCopied = 0;
        for (int i = offset; i < (offset + count); i++) {
            if (i < ((char*)location)[-1]) {
                ((char*)location)[i] = ((char*)values)[numCopied];
                numCopied++;
            } else {
                break;
            }
        }
    }
}








JSClass XDataDataType = {
    "Data",
    JSCLASS_HAS_PRIVATE
};

bool getData(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    const char * data = new char[256];
    int size;
    size = XPLMGetDatab(dataref, (void*)data, 0, 256);
    argv.rval().setString(JS_NewStringCopyN(context, data, size));
    delete [] data;
    return true;
}

bool setData(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    
    JS::RootedObject dataIn(context, argv[0].get().toObjectOrNull());
    uint32_t size = JS_GetStringLength(argv[0].toString());
    char * data = new char[size];
    
    if (argv[0].isString()) {
        JS_EncodeStringToBuffer(context, argv[0].toString(), data, size);
        XPLMSetDatab(dataref, (void*)data, 0, size);
    } else {
        delete [] data;
        JS_ReportErrorUTF8(context, "Data provided is not a string");
        return false;
    }
    
    argv.rval().set(JS::NullValue());
    delete [] data;
    return true;
}

JSFunctionSpec XDataDataFunctionsData[] = {
    JS_FN("get", getData, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setData, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

bool getFloatArr(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    float * data = new float[256];
    int size;
    if (!argv[0].isInt32()) {
        size = XPLMGetDatavf(dataref, data, 0, 256);
    } else {
        XPLMGetDatavf(dataref, data, argv[0].toInt32(), 1);
        argv.rval().setDouble(data[0]);
        delete [] data;
        return true;
    }
    JS::AutoValueArray<256> rval(context);
    for (int i = 0; i < size; i++) {
        rval[i].setDouble(data[i]);
    }
    JSObject * rvalobject = JS_NewArrayObject(context, rval);
    argv.rval().setObject(*rvalobject);
    delete [] data;
    return true;
}

bool setFloatArr(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    
    if (!argv[1].isInt32()) argv[1].setInt32(0);
    
    JS::RootedObject dataIn(context, argv[0].get().toObjectOrNull());
    uint32_t size = 0;
    JS_GetArrayLength(context, dataIn, &size);
    float * data = new float[size];
    
    if (argv[0].isObject()) {
        JS::RootedValue value(context);
        for (uint32_t i = 0; i < size; i++) {
            JS_GetElement(context, dataIn, i, &value);
            data[i] = value.toDouble();
        }
        XPLMSetDatavf(dataref, data, argv[1].toInt32(), size);
    } else {
        delete [] data;
        JS_ReportErrorUTF8(context, "Data provided is not an array");
        return false;
    }
    
    argv.rval().set(JS::NullValue());
    delete [] data;
    return true;
}

JSFunctionSpec XDataDataFunctionsFloatArr[] = {
    JS_FN("get", getFloatArr, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setFloatArr, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};


bool getIntArr(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    int * data = new int[256];
    int size;
    if (!argv[0].isInt32()) {
        size = XPLMGetDatavi(dataref, data, 0, 256);
    } else {
        XPLMGetDatavi(dataref, data, argv[0].toInt32(), 1);
        argv.rval().setInt32(data[0]);
        delete [] data;
        return true;
    }
    JS::AutoValueArray<256> rval(context);
    for (int i = 0; i < size; i++) {
        rval[i].setInt32(data[i]);
    }
    JSObject * rvalobject = JS_NewArrayObject(context, rval);
    argv.rval().setObject(*rvalobject);
    delete [] data;
    return true;
}

bool setIntArr(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    
    if (!argv[1].isInt32()) argv[1].setInt32(0);
    
    JS::RootedObject dataIn(context, argv[0].get().toObjectOrNull());
    uint32_t size = 0;
    JS_GetArrayLength(context, dataIn, &size);
    int * data = new int[size];
    
    if (argv[0].isObject()) {
        JS::RootedValue value(context);
        for (uint32_t i = 0; i < size; i++) {
            JS_GetElement(context, dataIn, i, &value);
            data[i] = value.toInt32();
        }
        XPLMSetDatavi(dataref, data, argv[1].toInt32(), size);
    } else {
        delete [] data;
        JS_ReportErrorUTF8(context, "Data provided is not an array");
        return false;
    }
    
    argv.rval().set(JS::NullValue());
    delete [] data;
    return true;
}

JSFunctionSpec XDataDataFunctionsIntArr[] = {
    JS_FN("get", getIntArr, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setIntArr, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

bool getDouble(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    double data = XPLMGetDatad(dataref);
    JS::RootedValue rval(context);
    rval.setDouble(data);
    argv.rval().set(rval);
    return true;
}

bool setDouble(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    double data = argv[0].toDouble();
    XPLMSetDatad(dataref, data);
    argv.rval().set(JS::NullValue());
    return true;
}

JSFunctionSpec XDataDataFunctionsDouble[] = {
    JS_FN("get", getDouble, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setDouble, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

bool getFloat(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    float data = XPLMGetDataf(dataref);
    JS::RootedValue rval(context);
    rval.setDouble(data);
    argv.rval().set(rval);
    return true;
}

bool setFloat(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    float data = argv[0].toDouble();
    XPLMSetDataf(dataref, data);
    argv.rval().set(JS::NullValue());
    return true;
}

JSFunctionSpec XDataDataFunctionsFloat[] = {
    JS_FN("get", getFloat, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setFloat, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

bool getInt(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    int data = XPLMGetDatai(dataref);
    JS::RootedValue rval(context);
    rval.setInt32(data);
    argv.rval().set(rval);
    return true;
}

bool setInt(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, argv.thisv().get().toObjectOrNull());
    XPLMDataRef dataref = (XPLMDataRef)JS_GetPrivate(Data);
    int data = argv[0].toInt32();
    XPLMSetDatai(dataref, data);
    argv.rval().set(JS::NullValue());
    return true;
}

JSFunctionSpec XDataDataFunctionsInt[] = {
    JS_FN("get", getInt, 0, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("set", setInt, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

bool dataConstructor(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject Data(context, JS_NewObject(context, &XDataDataType));
    const char * name = JS_EncodeString(context, argv[0].toString());
    XPLMDataRef dataref = XPLMFindDataRef(name);
    JS_SetPrivate(Data, dataref);
    
    XPLMDataTypeID type = XPLMGetDataRefTypes(dataref);
    switch (type) {
        case xplmType_Int:
            JS_DefineFunctions(context, Data, XDataDataFunctionsInt);
            break;
        case xplmType_Float:
            JS_DefineFunctions(context, Data, XDataDataFunctionsFloat);
            break;
        case xplmType_Double:
            JS_DefineFunctions(context, Data, XDataDataFunctionsDouble);
            break;
        case xplmType_IntArray:
            JS_DefineFunctions(context, Data, XDataDataFunctionsIntArr);
            break;
        case xplmType_FloatArray:
            JS_DefineFunctions(context, Data, XDataDataFunctionsFloatArr);
            break;
        case xplmType_Data:
            JS_DefineFunctions(context, Data, XDataDataFunctionsData);
            break;
        default:
            JS_ReportErrorUTF8(context, "Unknown dataref type");
            return false;
    }
    
    argv.rval().setObject(*Data);
    
    return true;
}





JSClass XDataClass = {
    "XData",
    JSCLASS_HAS_PRIVATE
};

bool shareData(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject XData(context, argv.thisv().get().toObjectOrNull());
    
    void * refcon;
    
    const char * name = JS_EncodeString(context, argv[0].toString());
    if (argv[1].isInt32()) {
        int * store = new int;
        refcon = (void*)store;
        XPLMRegisterDataAccessor(name, xplmType_Int, 1, XJS_INTERN_getDatai, XJS_INTERN_setDatai, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, (void*)store, (void*)store);
    } else if (argv[1].isDouble()) {
        double * store = new double;
        refcon = (void*)store;
        XPLMRegisterDataAccessor(name, xplmType_Double, 1, nullptr, nullptr, nullptr, nullptr, XJS_INTERN_getDatad, XJS_INTERN_setDatad, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, (void*)store, (void*)store);
    } else if (argv[1].isObject()) {
        JS::RootedObject initValue(context, argv[1].get().toObjectOrNull());
        JS::MutableHandleValue handle(argv[1]);
        JS_GetElement(context, initValue, 0, handle);
        uint32_t size = 0;
        JS_GetArrayLength(context, initValue, &size);
        if (handle.get().isInt32()) {
            int * store = new int[size + 1];
            store[0] = (int)size;
            store++;
            refcon = (void*)store;
            XPLMRegisterDataAccessor(name, xplmType_IntArray, 1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, XJS_INTERN_getDatavi, XJS_INTERN_setDatavi, nullptr, nullptr, nullptr, nullptr, (void*)store, (void*)store);
        } else if (handle.get().isDouble()){
            float * store = new float[size + 1];
            store[0] = (float)size;
            store++;
            refcon = (void*)store;
            XPLMRegisterDataAccessor(name, xplmType_FloatArray, 1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, XJS_INTERN_getDatavf, XJS_INTERN_setDatavf, nullptr, nullptr, (void*)store, (void*)store);
        } else {
            JS_ReportErrorUTF8(context, "Type not recognized");
            return false;
        }
    } else if (argv[1].isString()){
        JSString * string = argv[1].toString();
        size_t size = JS_GetStringLength(string);
        char * store = new char[size + 1];
        store[0] = (char)size;
        store++;
        refcon = (void*)store;
        XPLMRegisterDataAccessor(name, xplmType_Data, 1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, XJS_INTERN_getDatab, XJS_INTERN_setDatab, (void*)store, (void*)store);
    } else {
        JS_ReportErrorUTF8(context, "Type not recognized");
        return false;
    }
    
    JS::RootedValue dataFun(context);
    JS_GetProperty(context, XData, "Data", &dataFun);
    JS::RootedObject dataConstructor(context, dataFun.toObjectOrNull());
    
    JS::AutoValueArray<1> dataargs(context);
    args[0].setString(JS_NewStringCopyZ(context, name));
    
    JS::RootedObject data(context, JS_New(context, dataConstructor, dataargs));
    JS::RootedValue refconvalue(context);
    refconvalue.setPrivate(refcon);
    JS_DefineProperty(context, data, "refcon", refconvalue, JSPROP_READONLY | JSPROP_PERMANENT);
    
    argv.rval().setObject(*data);
    return true;
}

bool unshareData(JSContext * context, unsigned argc, JS::Value * args) {
    JS::CallArgs argv = JS::CallArgsFromVp(argc, args);
    JS::RootedObject data(context, argv[0].toObjectOrNull());
    JS::RootedValue refconstore(context);
    
    JS_GetProperty(context, data, "refcon", &refconstore);
    void * refcon = refconstore.toPrivate();
    
    XPLMDataTypeID type = XPLMGetDataRefTypes(JS_GetPrivate(data));
    XPLMUnregisterDataAccessor(JS_GetPrivate(data));
    
    switch (type) {
        case xplmType_Int:
        {
            int * store = (int*)refcon;
            delete store;
            break;
        }
        case xplmType_Float:
        {
            float * store = (float*)refcon;
            delete store;
            break;
        }
        case xplmType_Double:
        {
            double * store = (double*)refcon;
            delete store;
            break;
        }
        case xplmType_IntArray:
        {
            int * store = (int*)refcon;
            store--;
            delete [] store;
            break;
        }
        case xplmType_FloatArray:
        {
            float * store = (float*)refcon;
            store--;
            delete [] store;
            break;
        }
        case xplmType_Data:
        {
            char * store = (char*)refcon;
            store--;
            delete [] store;
            break;
        }
        default:
            break;
    }
    
    argv.rval().setBoolean(true);
    
    return true;
}

JSFunctionSpec XDataFunctions[] = {
    JS_FN("shareData", shareData, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FN("unshareData", unshareData, 1, JSPROP_PERMANENT || JSPROP_READONLY),
    JS_FS_END
};

jsPluginError initXData(JSContext * context, JS::Handle<JSObject*> global) {
    JS::RootedObject XData(context, JS_DefineObject(context, global, "XData", &XDataClass));
    JS_DefineFunctions(context, XData, XDataFunctions);
    
    JS::RootedObject XDataDataProto(context, JS_NewObject(context, &XDataDataType));
    JS_InitClass(context, XData, XDataDataProto, &XDataDataType, dataConstructor, 1, nullptr, nullptr, nullptr, nullptr);
    
    return XJS_NO_ERROR;
}
