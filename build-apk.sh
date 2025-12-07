#!/bin/bash

# Build Docker image
docker build -t tienlen-android .

# Run container and build APK
docker run --rm -v "$(pwd):/workspace" tienlen-android /usr/local/bin/build-android.sh

echo ""
echo "To extract the APK, it's located at: ./android/app/build/outputs/apk/release/app-release.apk"
