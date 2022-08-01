#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_shared_Debug
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED=1 ../../
make -j8
# make install
