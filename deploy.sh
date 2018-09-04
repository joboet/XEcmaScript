#!/usr/bin/env bash

export DEPS_DIR=$(pwd)/deps
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

export MOZJS_LIBRARY=$DEPS_DIR/lib/libmozjs-52$EXTENSION
export MOZGLUE_LIBRARY=$DEPS_DIR/lib/libmozglue$EXTENSION
echo $MOZJS_LIBRARY
echo $MOZGLUE_LIBRARY

mkdir deploy && cd deploy
mkdir $TRAVIS_OS_NAME && cd $TRAVIS_OS_NAME
mkdir lib
cp $MOZJS_LIBRARY lib/libmozjs$EXTENSION
cp $MOZGLUE_LIBRARY lib/libmozglue$EXTENSION
cp $BUILDDIR/$PLUGIN_NAME ./$PLUGIN_NAME

cd ..
zip -r $TRAVIS_OS_NAME.zip $TRAVIS_OS_NAME
