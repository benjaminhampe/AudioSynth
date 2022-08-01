#!/bin/bash
PWD="$(pwd)"
BUILD=$PWD/build/linux64_shared_MinSizeRel
mkdir -p $BUILD
cd $BUILD
CXX=/usr/lib/icecc/bin/g++ CC=/usr/lib/icecc/bin/gcc cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED=1 -DTARGET_ENVIRONMENT=host ../../
ICECC_CARET_WORKAROUND=0 /usr/bin/time -v -p /usr/bin/make -j160
