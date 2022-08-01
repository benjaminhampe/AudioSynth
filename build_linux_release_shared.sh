#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_shared_MinSizeRel
mkdir -p $BUILD
cd $BUILD
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED=1 ../../
make -j8
# make install
