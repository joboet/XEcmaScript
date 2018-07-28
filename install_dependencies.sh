#!/usr/bin/env bash

if [ $TRAVIS_OS_NAME == linux ]
then
  sh -c "$(curl -fsSL https://raw.githubusercontent.com/Linuxbrew/install/master/install.sh)"
  sudo apt-get install build-essential
  export PATH="/home/linuxbrew/.linuxbrew/bin:$PATH"
  export LOADERPATH="\$ORIGIN"
  brew install patchelf
  export CHANGEID=patchelf --set-soname \$ORIGIN/lib/libmozjs.so /usr/local/lib/libmozjs-52.so
fi

if [ $TRAVIS_OS_NAME == osx ]
then
  export LOADERPATH="@loader_path"
  export CHANGEID=install_name_tool -id @loader_path/lib/libmozjs.dylib /usr/local/lib/libmozjs-52.dylib
fi

brew install pkg-config perl binutils ccache boost yasm gawk python autoconf@2.13

if [ ! -d deps ]
then

mkdir deps
cd deps
curl http://developer.x-plane.com/wp-content/plugins/code-sample-generation/sample_templates/XPSDK301.zip > SDK.zip
unzip -qq SDK.zip
rm SDK.zip
git clone -b esr52 --depth 1 https://github.com/mozilla/gecko-dev.git
cd gecko-dev/js/src/
autoconf213
mkdir buildRLS
cd buildRLS
../configure --prefix=/usr/local --disable-tests
make -j4 -s
make install
cd ../../../../
sudo rm -R gecko-dev
bash -c $CHANGEID

fi
