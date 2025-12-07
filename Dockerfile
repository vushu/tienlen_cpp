FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install basic dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    wget \
    unzip \
    openjdk-17-jdk \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Set Java environment
ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV PATH=$PATH:$JAVA_HOME/bin

# Install Android SDK Command Line Tools
ENV ANDROID_SDK_ROOT=/opt/android-sdk
ENV ANDROID_HOME=$ANDROID_SDK_ROOT
ENV PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin:$ANDROID_SDK_ROOT/platform-tools

RUN mkdir -p $ANDROID_SDK_ROOT/cmdline-tools && \
    cd $ANDROID_SDK_ROOT/cmdline-tools && \
    wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip && \
    unzip commandlinetools-linux-11076708_latest.zip && \
    mv cmdline-tools latest && \
    rm commandlinetools-linux-11076708_latest.zip

# Accept licenses and install Android SDK components
RUN yes | sdkmanager --licenses && \
    sdkmanager "platform-tools" "platforms;android-34" "build-tools;34.0.0" && \
    sdkmanager "ndk;26.1.10909125"

ENV ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/ndk/26.1.10909125
ENV PATH=$PATH:$ANDROID_NDK_ROOT

# Install Gradle
ENV GRADLE_VERSION=8.5
RUN wget https://services.gradle.org/distributions/gradle-${GRADLE_VERSION}-bin.zip && \
    unzip gradle-${GRADLE_VERSION}-bin.zip -d /opt && \
    rm gradle-${GRADLE_VERSION}-bin.zip

ENV GRADLE_HOME=/opt/gradle-${GRADLE_VERSION}
ENV PATH=$PATH:$GRADLE_HOME/bin

# Set working directory
WORKDIR /workspace

# Create build script
RUN echo '#!/bin/bash\n\
set -e\n\
\n\
echo "Building Tien Len for Android ARM64..."\n\
\n\
# Verify we have the source\n\
if [ ! -f "/workspace/CMakeLists.txt" ]; then\n\
    echo "Error: CMakeLists.txt not found in /workspace"\n\
    echo "Make sure you mounted your source code with: -v \$(pwd):/workspace"\n\
    exit 1\n\
fi\n\
\n\
# Create build directory\n\
rm -rf /workspace/build-android\n\
mkdir -p /workspace/build-android\n\
cd /workspace/build-android\n\
\n\
# Configure CMake for Android ARM64\n\
cmake /workspace \\\n\
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \\\n\
    -DANDROID_ABI=arm64-v8a \\\n\
    -DANDROID_PLATFORM=android-24 \\\n\
    -DANDROID_STL=c++_shared \\\n\
    -DCMAKE_BUILD_TYPE=Release \\\n\
    -GNinja\n\
\n\
# Build the native library\n\
ninja\n\
\n\
echo "Native library built successfully!"\n\
\n\
# Create Android project structure if it doesn'\''t exist\n\
if [ ! -d "/workspace/android" ]; then\n\
    echo "Creating Android project structure..."\n\
    mkdir -p /workspace/android/app/src/main/jniLibs/arm64-v8a\n\
    mkdir -p /workspace/android/app/src/main/assets\n\
    mkdir -p /workspace/android/app/src/main/java/com/bamhaven/tienlen\n\
    \n\
    # Copy native libraries\n\
    find /workspace/build-android -name "*.so" -exec cp {} /workspace/android/app/src/main/jniLibs/arm64-v8a/ \\;\n\
    \n\
    # Copy libc++_shared.so from NDK (required by SDL3)\n\
    cp $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so /workspace/android/app/src/main/jniLibs/arm64-v8a/\n\
    \n\
    # Copy SDL3 Android Java source files\n\
    mkdir -p /workspace/android/app/src/main/java/org/libsdl/app\n\
    cp /workspace/thirdparty/SDL/android-project/app/src/main/java/org/libsdl/app/*.java /workspace/android/app/src/main/java/org/libsdl/app/\n\
    \n\
    # Create basic Android project files\n\
    cat > /workspace/android/settings.gradle << '\''EOF'\''\n\
pluginManagement {\n\
    repositories {\n\
        google()\n\
        mavenCentral()\n\
        gradlePluginPortal()\n\
    }\n\
}\n\
dependencyResolutionManagement {\n\
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)\n\
    repositories {\n\
        google()\n\
        mavenCentral()\n\
    }\n\
}\n\
rootProject.name = "TienLen"\n\
include '\'':app'\''\n\
EOF\n\
    \n\
    cat > /workspace/android/build.gradle << '\''EOF'\''\n\
plugins {\n\
    id '\''com.android.application'\'' version '\''8.2.0'\'' apply false\n\
}\n\
EOF\n\
    \n\
    cat > /workspace/android/gradle.properties << '\''EOF'\''\n\
org.gradle.jvmargs=-Xmx2048m\n\
android.useAndroidX=true\n\
android.enableJetifier=true\n\
EOF\n\
    \n\
    cat > /workspace/android/app/build.gradle << '\''EOF'\''\n\
plugins {\n\
    id '\''com.android.application'\''\n\
}\n\
\n\
android {\n\
    namespace '\''com.bamhaven.tienlen'\''\n\
    compileSdk 34\n\
\n\
    defaultConfig {\n\
        applicationId '\''com.bamhaven.tienlen'\''\n\
        minSdk 24\n\
        targetSdk 29\n\
        versionCode 1\n\
        versionName '\''1.0'\''\n\
        ndk {\n\
            abiFilters '\''arm64-v8a'\''\n\
        }\n\
    }\n\
    \n\
    androidResources {\n\
        noCompress '\''resources.arsc'\''\n\
    }\n\
    \n\
    lint {\n\
        disable '\''ExpiredTargetSdkVersion'\''\n\
    }\n\
\n\
    buildTypes {\n\
        release {\n\
            minifyEnabled false\n\
            proguardFiles getDefaultProguardFile('\''proguard-android-optimize.txt'\''), '\''proguard-rules.pro'\''\n\
        }\n\
    }\n\
    \n\
    compileOptions {\n\
        sourceCompatibility JavaVersion.VERSION_17\n\
        targetCompatibility JavaVersion.VERSION_17\n\
    }\n\
    \n\
    sourceSets {\n\
        main {\n\
            jniLibs.srcDirs = ['\''src/main/jniLibs'\'']\n\
        }\n\
    }\n\
}\n\
\n\
dependencies {\n\
    implementation '\''androidx.appcompat:appcompat:1.6.1'\''\n\
}\n\
EOF\n\
    \n\
    cat > /workspace/android/app/src/main/AndroidManifest.xml << '\''EOF'\''\n\
<?xml version="1.0" encoding="utf-8"?>\n\
<manifest xmlns:android="http://schemas.android.com/apk/res/android">\n\
    <application\n\
        android:allowBackup="true"\n\
        android:extractNativeLibs="true"\n\
        android:icon="@android:drawable/sym_def_app_icon"\n\
        android:label="Tien Len"\n\
        android:theme="@android:style/Theme.NoTitleBar.Fullscreen">\n\
        <activity\n\
            android:name=".MainActivity"\n\
            android:exported="true"\n\
            android:configChanges="orientation|screenSize|keyboardHidden"\n\
            android:screenOrientation="landscape">\n\
            <intent-filter>\n\
                <action android:name="android.intent.action.MAIN" />\n\
                <category android:name="android.intent.category.LAUNCHER" />\n\
            </intent-filter>\n\
        </activity>\n\
    </application>\n\
</manifest>\n\
EOF\n\
    \n\
    cat > /workspace/android/app/src/main/java/com/bamhaven/tienlen/MainActivity.java << '\''EOF'\''\n\
package com.bamhaven.tienlen;\n\
\n\
public class MainActivity extends org.libsdl.app.SDLActivity {\n\
    protected String[] getLibraries() {\n\
        return new String[] {\n\
            "SDL3",\n\
            "tienlen"\n\
        };\n\
    }\n\
}\n\
EOF\nfi\n\
\n\
# Build APK\n\
cd /workspace/android\n\
gradle assembleRelease\n\
\n\
echo ""\n\
echo "APK built successfully!"\n\
echo "Output: /workspace/android/app/build/outputs/apk/release/app-release.apk"\n\
' > /usr/local/bin/build-android.sh

RUN chmod +x /usr/local/bin/build-android.sh

# Entry point
CMD ["/bin/bash"]
