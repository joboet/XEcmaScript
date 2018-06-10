//
//  XGraphics.cpp
//  XJS
//
//  Created by Jonas Böttiger on 15.04.18.
//  Copyright © 2018 CoffeeBlend. All rights reserved.
//

/*#include "XGraphics.hpp"
#include <vector>
#include <functional>

class openglWrapper {
private:
    //static std::vector<std::function<void(...)>> panelFunctions;
    //static std::vector<std::function<void(...)>> screenFunctions;
    
    static uint16_t matrixLevel;
    
    static void translate(double x, double y, double z) {
        glTranslated(x, y, z);
    }
    
    static void rotate(double x, double y, double z) {
        glRotated(x, 1, 0, 0);
        glRotated(y, 0, 1, 0);
        glRotated(z, 0, 0, 1);
    }
    static void scale(double x, double y, double z) {
        glScaled(x, y, z);
    }
    static void reset() {
        glLoadIdentity();
    }
    
    static void push() {
        glPushMatrix();
        matrixLevel++;
    }
    
    static void pop() {
        if (matrixLevel) {
            glPopMatrix();
            matrixLevel--;
        }
    }
    
    static void executeFunctions(std::vector<std::function<void(...)>> &functions) {
        for (uint16_t i = 0; i < functions.size(); i++) {
            functions[i];
        }
    }
public:
    static void panel() {
        
    }
    static void screen() {
        
    }
    
    
};

uint16_t openglWrapper::matrixLevel = 0;

JSClass XGraphicsClass = {
    "XGraphics",
    JSCLASS_HAS_PRIVATE,
};
*/
void initOpenGL() {
    
}
/*
void initXGraphics(JSContext * context, JS::HandleObject global) {
    JS::PersistentRootedObject * XGraphics = new JS::PersistentRootedObject(context, JS_NewObject(context, &XGraphicsClass));
    JS_DefineProperty(context, global, "XGraphics", *XGraphics, JSPROP_PERMANENT | JSPROP_READONLY);
    
}*/
