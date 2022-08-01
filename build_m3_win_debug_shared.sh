#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/win64_Debug_shared
mkdir -p $BUILD
cd $BUILD
cmake -G "MSYS Makefiles" \
   -DCMAKE_PREFIX_PATH=C:/SDK/Qt5.12.3/5.12.3/mingw73_64/lib/cmake \
   -DCMAKE_BUILD_TYPE=Debug \
   -DBUILD_SHARED=1 ../../
mingw32-make -j16
