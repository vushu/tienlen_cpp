# Android Build Instructions

## Prerequisites
- Docker installed on your system

## Building the APK

### Method 1: Using the build script (Recommended)
```bash
./build-apk.sh
```

### Method 2: Manual build
```bash
# Build the Docker image
docker build -t tienlen-android .

# Run the container and build
docker run --rm -v "$(pwd):/workspace" tienlen-android /usr/local/bin/build-android.sh
```

## Output
The APK will be generated at:
```
./android/app/build/outputs/apk/release/app-release.apk
```

## Architecture
- Target: ARM64 (arm64-v8a)
- Minimum Android SDK: 24 (Android 7.0)
- Target Android SDK: 34

## Build Process
The Docker container will:
1. Cross-compile the C++ code using Android NDK
2. Create an Android project structure
3. Copy native libraries to the Android project
4. Build the APK using Gradle

## Customization
You can modify the following in the Dockerfile:
- `ANDROID_PLATFORM`: Change minimum Android version
- `ANDROID_ABI`: Add other architectures (armeabi-v7a, x86_64, etc.)
- Android SDK/NDK versions

## Troubleshooting

### Permission Issues
If you encounter permission issues, ensure the build script is executable:
```bash
chmod +x build-apk.sh
```

### Clean Build
To clean previous builds:
```bash
rm -rf build-android android
```

### Interactive Container
To debug or explore the container:
```bash
docker run --rm -it -v "$(pwd):/workspace" tienlen-android /bin/bash
```
