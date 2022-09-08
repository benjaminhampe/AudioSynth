#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/win64_Release_shared
mkdir -p $BUILD
cd $BUILD
cmake -G "MSYS Makefiles" \
   -DCMAKE_PREFIX_PATH=C:/SDK/Qt5.12.3/5.12.3/mingw73_64/lib/cmake \
   -DBUILD_SHARED=1 \
   -DCMAKE_BUILD_TYPE=Release ../../
mingw32-make -j4