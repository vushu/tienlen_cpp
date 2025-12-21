#!/bin/bash
set -e

echo "Building Android APK using Docker..."

# Build Docker image if it doesn't exist
if ! docker images | grep -q tienlen-android-builder; then
    echo "Building Docker image..."
    docker build -t tienlen-android-builder .
fi

# Run build in container
docker run --rm \
    -v "$(pwd):/workspace" \
    tienlen-android-builder \
    /workspace/build-android-simple.sh

echo ""
echo "Build complete!"
echo "APK: android/app/build/outputs/apk/release/app-release-unsigned.apk"