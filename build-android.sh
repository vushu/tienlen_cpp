#!/bin/bash
set -e

echo "Building Tien Len for Android ARM64..."

# Verify we have the source
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found"
    echo "Please run this script from the project root"
    exit 1
fi

# Clean and create build directory
rm -rf build-android
mkdir -p build-android
cd build-android

# Configure CMake for Android ARM64
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-24 \
    -DANDROID_STL=c++_shared \
    -DCMAKE_BUILD_TYPE=Release \
    -GNinja

# Build the native library
ninja

echo "Native library built successfully!"

# Copy native libraries to Android project
echo "Copying native libraries to Android project..."
mkdir -p ../android/app/src/main/jniLibs/arm64-v8a

# Copy all built .so files
find . -name "*.so" -exec cp -v {} ../android/app/src/main/jniLibs/arm64-v8a/ \;

# Copy libc++_shared.so from NDK (required by SDL3)
cp -v $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so ../android/app/src/main/jniLibs/arm64-v8a/

# Build APK
echo "Building APK..."
cd ../android
gradle assembleRelease

echo ""
echo "APK built successfully!"
echo "Output: android/app/build/outputs/apk/release/app-release-unsigned.apk"