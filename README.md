# XEcmaScript
An EcmaScript (JavaScript) scripting environment for the X-Plane flight simulator

# Requirements
- CMake
- Mozilla Spidermoney version 52
- Boost
- pkg-config
- X-Plane SDK

# How to build
First, install Spidermonkey using the instructions provided on (https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/Build_Documentation). This may require some experimenting to get a successfull build. The debug build is not needed. 

Then, install the other dependencies. I recommend using a package manager such as Homebrew on MacOS, or you can build them from source if you wish to do so. The X-Plane SDK can be downloaded from (https://developer.x-plane.com/sdk/).

Use the CMake GUI to configure the build, specifying the path to the X-Plane SDK. After generating the build files, use make in a terminal to build the plugin.

# API
A rough, incomplete sketch of the API can be found in the API.rtf.

# Disclaimer
This plugin is a work-in-progress and as such, things may change. Therefore, it should not be used for scripting purposes. Currently, the plugin is for interested C++ developers only.

# Current status
Currently, the bare structure of the plugin is done. Simple JavaScript plugins can be executed successfully, such as setting DataRefs in X-Plane. However, most of the API is not implemented yet, and the API itself is not thought out completely.
