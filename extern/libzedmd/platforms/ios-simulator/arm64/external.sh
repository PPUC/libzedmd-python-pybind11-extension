#!/bin/bash

set -e

LIBFRAMEUTIL_SHA=30048ca23d41ca0a8f7d5ab75d3f646a19a90182
SOCKPP_SHA=e6c4688a576d95f42dd7628cefe68092f6c5cd0f

NUM_PROCS=$(sysctl -n hw.ncpu)

echo "Building libraries..."
echo "  LIBFRAMEUTIL_SHA: ${LIBFRAMEUTIL_SHA}"
echo "  SOCKPP_SHA: ${SOCKPP_SHA}"
echo "  NUM_PROCS: ${NUM_PROCS}"
echo ""

if [ -z "${BUILD_TYPE}" ]; then
   BUILD_TYPE="Release"
fi

rm -rf external
mkdir external
cd external

#
# copy libframeutil
#

curl -sL https://github.com/ppuc/libframeutil/archive/${LIBFRAMEUTIL_SHA}.tar.gz -o libframeutil-${LIBFRAMEUTIL_SHA}.tar.gz
tar xzf libframeutil-${LIBFRAMEUTIL_SHA}.tar.gz
mv libframeutil-${LIBFRAMEUTIL_SHA} libframeutil
cd libframeutil
cp include/* ../../third-party/include
cd ..

#
# build sockpp and copy to external
#

curl -sL https://github.com/fpagliughi/sockpp/archive/${SOCKPP_SHA}.tar.gz -o sockpp-${SOCKPP_SHA}.tar.gz
tar xzf sockpp-${SOCKPP_SHA}.tar.gz
mv sockpp-${SOCKPP_SHA} sockpp
cd sockpp
cmake \
   -DSOCKPP_BUILD_SHARED=OFF \
   -DSOCKPP_BUILD_STATIC=ON \
   -DCMAKE_SYSTEM_NAME=iOS \
   -DCMAKE_OSX_SYSROOT=iphonesimulator \
   -DCMAKE_OSX_ARCHITECTURES=arm64 \
   -DCMAKE_OSX_DEPLOYMENT_TARGET=17.0 \
   -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
   -B build
cmake --build build -- -j${NUM_PROCS}
cp -r include/sockpp ../../third-party/include/
cp build/libsockpp.a ../../third-party/build-libs/ios-simulator/arm64/
cd ..
