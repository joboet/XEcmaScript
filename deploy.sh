#!/usr/bin/env bash

export BUILDDIR=$(pwd)/build

if [ $TRAVIS_OS_NAME == osx ]
then
  export PLUGIN_NAME=mac.xpl
  export EXTENSION=.dylib
fi
if [ $TRAVIS_OS_NAME == linux ]
then
  export PLUGIN_NAME=lin.xpl
  export EXTENSION=.so
fi

export MOZJS_LIB_NAME=$(pkg-config --libs-only-l mozjs-52)
export MOZJS_LIB_PATH=$(pkg-config --libs-only-L mozjs-52)
export MOZJS_LIBRARY=${MOZJS_LIB_PATH#"-L"}/lib${MOZJS_LIB_NAME#"-l"}$EXTENSION
echo $MOZJS_LIBRARY

mkdir deploy && cd deploy
mkdir $TRAVIS_OS_NAME && cd $TRAVIS_OS_NAME
mkdir lib
cp $MOZJS_LIBRARY lib/libmozjs$EXTENSION
cp $BUILDDIR/$PLUGIN_NAME ./$PLUGIN_NAME

cd ..
zip -r $TRAVIS_OS_NAME.zip $TRAVIS_OS_NAME
