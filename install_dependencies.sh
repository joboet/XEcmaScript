#!/usr/bin/env bash

if [ $TRAVIS_OS_NAME == linux ]
then
  sh -c "$(curl -fsSL https://raw.githubusercontent.com/Linuxbrew/install/master/install.sh)"
  sudo apt-get install build-essential
  export PATH="/home/linuxbrew/.linuxbrew/bin:$PATH"
  brew install patchelf
  brew install gcc@4.9
  export CC=gcc-4.9
  export CXX=g++-4.9
fi

brew install pkg-config perl binutils ccache boost gawk python autoconf@2.13
if [ $TRAVIS_OS_NAME == osx ]
then
  brew install yasm
  brew upgrade cmake
fi

cd deps

$MOZJS_EXISTS=$(pkg-config --libs mozjs-52)

if [ -z "$MOZJS_EXISTS" ]
then

export DEPS_DIR=$(pwd)

if [ $TRAVIS_OS_NAME == linux ]
then
  curl https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.sh > cmake_linux.sh
  sudo sh ./cmake_linux.sh --prefix=/usr/local/bin --skip-license
  cmake --version
fi

curl http://developer.x-plane.com/wp-content/plugins/code-sample-generation/sample_templates/XPSDK301.zip > SDK.zip
unzip -q -u SDK.zip
rm SDK.zip
git clone -b esr52 --depth 1 https://github.com/mozilla/gecko-dev.git
cd gecko-dev/js/src/
autoconf213
mkdir buildRLS
cd buildRLS
../configure --prefix=$DEPS_DIR --disable-tests
make -j4 -s
make install
cd ../../../..
sudo rm -R gecko-dev

if [ $TRAVIS_OS_NAME == osx ]
then
  install_name_tool -id @loader_path/lib/libmozjs.dylib $DEPS_DIR/lib/libmozjs-52.dylib
fi
if [ $TRAVIS_OS_NAME == linux ]
then
  patchelf --set-soname \$ORIGIN/lib/libmozjs.so /usr/local/lib/libmozjs-52.so
fi

fi

cp $DEPS_DIR/lib/pkgconfig/mozjs-52.pc /usr/local/lib/pkgconfig/mozjs-52.pc
