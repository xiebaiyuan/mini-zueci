#!/bin/bash
set -ex
ANDROID_NDK=/opt/android-ndk-r17c/

export ANDROID_HOST=aarch64-linux-android
#export ANDROID_BUILD=darwin-x86_64
#export ANDROID_ARCH=arm64
#export ANDROID_NDK=$HOME/Library/Android/sdk/ndk-bundle
#export ANDROID_VERSION=24
#export ANDROID_TOOLCHAIN_VERSION=4.9
export ANDROID_SYSROOT=$ANDROID_NDK/platforms/android-$ANDROID_VERSION/arch-$ANDROID_ARCH
#export CFLAGS=--sysroot=$ANDROID_SYSROOT
#export CPPFLAGS=--sysroot=$ANDROID_SYSROOT
#export AR=$ANDROID_HOST-ar
#export RANLIB=$ANDROID_HOST-ranlib
#export PATH=$ANDROID_NDK/toolchains/$ANDROID_HOST-$ANDROID_TOOLCHAIN_VERSION/prebuilt/$ANDROID_BUILD/bin:$PATH


#./configure --host=$ANDROID_HOST --with-sysroot="$ANDROID_SYSROOT"

CURRENT_DIR=$(cd `dirname $0`; pwd) # get the absolute path of the current directory
echo "ANDROID_NDK: ${ANDROID_NDK}"

ANDROID_STL=c++_shared
MIN_ANDROID_API_LEVEL_ARMV7=16
MIN_ANDROID_API_LEVEL_ARMV8=21
#TOOLCHAIN=clang
#BCCODE_RELEASE=ON
#BDC_DEBUG=OFF
#PRINT_DEBUG=OFF


rm -rf ${CURRENT_DIR}/release
mkdir -p ${CURRENT_DIR}/release
mkdir -p ${CURRENT_DIR}/release/withsymble
mkdir -p ${CURRENT_DIR}/release/withsymble/armeabi-v7a
mkdir -p ${CURRENT_DIR}/release/withsymble/arm64-v8a
mkdir -p ${CURRENT_DIR}/release/armeabi-v7a
mkdir -p ${CURRENT_DIR}/release/arm64-v8a

#cd ${CURRENT_DIR}/core



# build v7 ....
rm -rf build_v7
mkdir -p build_v7
cd build_v7

cmake -DANDROID_NDK=${ANDROID_NDK} \
      -DANDROID_TOOLCHAIN=clang \
      -DANDROID_STL=$ANDROID_STL \
      -DANDROID_ABI=armeabi-v7a \
      -DANDROID_NATIVE_API_LEVEL=${MIN_ANDROID_API_LEVEL_ARMV7} \
      -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK}/build/cmake/android.toolchain.cmake" \
      -DZUECI_STATIC=ON\
      -DZUECI_DEBUG=OFF\
      -DZUECI_NOOPT=OFF\
      -DZUECI_SANITIZE=OFF\
      -DZUECI_TEST=OFF\
      -DZUECI_TEST_ICONV=OFF\
      -DZUECI_STATIC=ON\
      ..


make -j16

#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

#cp libiconv.so ${CURRENT_DIR}/release/withsymble/v7/
#nm -D --defined-only libiconv.so
#${ANDROID_NDK}/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-strip -s libiconv.so
#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so
#
cp libzueci-static.a ${CURRENT_DIR}/release/armeabi-v7a/

cd -

# build_v8
rm -rf build_v8
mkdir -p build_v8
cd build_v8
cmake -DANDROID_NDK=${ANDROID_NDK} \
      -DBUILDING_ANDROID=ON \
      -DANDROID_TOOLCHAIN=clang \
      -DANDROID_STL=$ANDROID_STL \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_NATIVE_API_LEVEL=${MIN_ANDROID_API_LEVEL_ARMV8} \
      -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK}/build/cmake/android.toolchain.cmake" \
      -DZUECI_STATIC=ON\
      -DZUECI_DEBUG=OFF\
      -DZUECI_NOOPT=OFF\
      -DZUECI_SANITIZE=OFF\
      -DZUECI_TEST=OFF\
      -DZUECI_TEST_ICONV=OFF\
      -DZUECI_STATIC=ON\
      ..

make -j16

#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

#cp libiconv.so ${CURRENT_DIR}/release/withsymble/v8/
#nm -D --defined-only libiconv.so
#${ANDROID_NDK}/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-strip -s libiconv.so
#file libiconv.so
#objdump -x libiconv.so | grep NEEDED
#ls -lh libiconv.so

cp libzueci-static.a ${CURRENT_DIR}/release/arm64-v8a/



cd -