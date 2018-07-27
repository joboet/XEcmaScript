#!/usr/bin/env bash

if [ $TRAVIS_OS_NAME == osx ]
then
  brew install ccache boost yasm gawk ccache python autoconf@2.13
  export AUTOCONF=autoconf213
  export LOADERPATH="@loader_path"
fi
if [ $TRAVIS_OS_NAME == linux ]
then
  sudo apt-get install pkg-config libboost-filesystem-dev perl yasm binutils-avr autoconf2.13
  export AUTOCONF=autoconf2.13
  export LOADERPATH="\$ORIGIN"
fi

if [ ! -d deps ]
then

mkdir deps
cd deps
mkdir lib
mkdir include
curl http://developer.x-plane.com/wp-content/plugins/code-sample-generation/sample_templates/XPSDK301.zip > SDK.zip
unzip SDK.zip
rm SDK.zip
git clone -b esr52 --depth 1 https://github.com/mozilla/gecko-dev.git
cd gecko-dev/js/src/
$AUTOCONF
mkdir buildRLS
cd buildRLS
../configure --prefix=/usr/local --disable-js-shell --disable-tests
make -j4
make install
cd ../../../../
rm -R gecko-dev
install_name_tool -id $LOADERPATH/lib/libmozjs.dylib /usr/local/lib/libmozjs-52.dylib

fi
