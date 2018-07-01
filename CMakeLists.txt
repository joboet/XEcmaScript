cmake_minimum_required(VERSION 3.11.1)

# Build system configuration

set(CMAKE_CXX_STANDARD 17)

# Packages

find_package(PkgConfig)

# Project configuration

project(XEcmaScript)

set(XEcmaScript_VERSION_MAJOR 0)
set(XEcmaScript_VERSION_MINOR 01)
set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

# Plugin configuration

file(GLOB_RECURSE PLUGIN_SRC "src/*.h" "src/*.hpp" "src/*.c" "src/*.cpp" "src/*.tpp")
add_library(plugin SHARED ${PLUGIN_SRC})

set_target_properties(plugin PROPERTIES SUFFIX ".xpl")
set_target_properties(plugin PROPERTIES PREFIX "")

# XSDK configuration

#set(XSDK_DIR CACHE PATH "Path to the X-Plane SDK")
set(XSDK_DIR "/Users/Jonas/Documents/SDK")
set(XSDK_LIBRARIES "${XSDK_DIR}/Libraries/")
set(XSDK_INCLUDE "${XSDK_DIR}/CHeaders/")
add_definitions(-DXPLM200 -DXPLM210 -DXPLM300 -DXPLM301)

# OS-Specifics

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  add_definitions(-DAPL=1)
  set(XSDK_LIBRARIES "-F ${XSDK_LIBRARIES}/Mac -framework XPLM -framework XPWidgets")
  set_target_properties(plugin PROPERTIES OUTPUT_NAME "mac")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_definitions(-DLIN=1)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -nodefaultlibs -undefined_warning")
  set_target_properties(plugin PROPERTIES OUTPUT_NAME "lin")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_definitions(-DWIN=1)
  link_directories("${XSDK_LIBRARIES}/Win/")
  set(XSDK_LIBRARIES "XPLM_64 lXPWidgets_64")
  set_target_properties(plugin PROPERTIES OUTPUT_NAME "win")
endif()

# CMake Header configuration

configure_file("${PROJECT_SOURCE_DIR}/cmake.h.in" "${PROJECT_BINARY_DIR}/cmake.h")

# Modules

pkg_check_modules(MOZJS REQUIRED mozjs-52)

target_link_libraries(plugin ${MOZJS_LIBRARIES})

target_link_libraries(plugin ${XSDK_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(plugin ${Boost_LIBRARIES})

# Headers
include_directories(${Boost_INCLUDE})
include_directories(${XSDK_INCLUDE})
include_directories(${MOZJS_INCLUDE})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
