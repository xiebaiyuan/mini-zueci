#!/bin/bash
set -ex
OHOS_SDK=/Users/xiebaiyuan/Library/OpenHarmony/Sdk/12

CURRENT_DIR=$(cd `dirname $0`; pwd) # get the absolute path of the current directory
echo "OHOS_SDK: ${OHOS_SDK}"

OHOS_STL=c++_shared
#TOOLCHAIN=clang
#BCCODE_RELEASE=ON
#BDC_DEBUG=OFF
#PRINT_DEBUG=OFF


rm -rf ${CURRENT_DIR}/ohos_release
mkdir -p ${CURRENT_DIR}/ohos_release
mkdir -p ${CURRENT_DIR}/ohos_release/withsymble
mkdir -p ${CURRENT_DIR}/ohos_release/withsymble/armeabi-v7a
mkdir -p ${CURRENT_DIR}/ohos_release/withsymble/arm64-v8a
mkdir -p ${CURRENT_DIR}/ohos_release/armeabi-v7a
mkdir -p ${CURRENT_DIR}/ohos_release/arm64-v8a

#cd ${CURRENT_DIR}/core



# build v7 ....
rm -rf build_v7
mkdir -p build_v7
cd build_v7

cmake -DOHOS_NDK=${OHOS_NDK} \
      -DOHOS_TOOLCHAIN=clang \
      -DOHOS_STL=$OHOS_STL \
      -DOHOS_ABI=armeabi-v7a \
      -DCMAKE_TOOLCHAIN_FILE="${OHOS_SDK}/native/build/cmake/ohos.toolchain.cmake" \
      -DZUECI_STATIC=ON\
      -DZUECI_DEBUG=OFF\
      -DZUECI_NOOPT=OFF\
      -DZUECI_SANITIZE=OFF\
      -DZUECI_TEST=OFF\
      -DZUECI_TEST_ICONV=OFF\
      -DZUECI_STATIC=ON\
      .. -G "Ninja"


ninja

#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

#cp libiconv.so ${CURRENT_DIR}/ohos_release/withsymble/v7/
#nm -D --defined-only libiconv.so
#${OHOS_NDK}/toolchains/aarch64-linux-OHOS-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-OHOS-strip -s libiconv.so
#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so
#
cp libzueci-static.a ${CURRENT_DIR}/ohos_release/armeabi-v7a/

cd -

# build_v8
rm -rf build_v8
mkdir -p build_v8
cd build_v8
cmake -DOHOS_NDK=${OHOS_NDK} \
      -DBUILDING_OHOS=ON \
      -DOHOS_TOOLCHAIN=clang \
      -DOHOS_STL=$OHOS_STL \
      -DOHOS_ABI=arm64-v8a \
      -DCMAKE_TOOLCHAIN_FILE="${OHOS_SDK}/native/build/cmake/ohos.toolchain.cmake" \
      -DZUECI_STATIC=ON\
      -DZUECI_DEBUG=OFF\
      -DZUECI_NOOPT=OFF\
      -DZUECI_SANITIZE=OFF\
      -DZUECI_TEST=OFF\
      -DZUECI_TEST_ICONV=OFF\
      -DZUECI_STATIC=ON\
      .. -G "Ninja"

ninja
#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

#cp libiconv.so ${CURRENT_DIR}/ohos_release/withsymble/v8/
#nm -D --defined-only libiconv.so
#${OHOS_NDK}/toolchains/aarch64-linux-OHOS-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-OHOS-strip -s libiconv.so
#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

cp libzueci-static.a ${CURRENT_DIR}/ohos_release/arm64-v8a/



cd -