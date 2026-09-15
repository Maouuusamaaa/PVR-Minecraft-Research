# PVR Minecraft Bridge — EXP-001 Implementation Build Guide

## Purpose

This document describes how to build the minimal Android ARM64 native bridge for EXP-001.

The bridge is a **research-only, controlled native library** that:
- Initializes with a logging marker when loaded into the Minecraft Android process
- Logs context information (PID, TID) to logcat
- Contains NO RenderDragon hooks, BGFX interception, or renderer code
- Is reversible and isolated from Minecraft's core functionality

## Target Specifications

- **Android Version**: 13+
- **Architecture**: ARM64 (arm64-v8a)
- **Target Device**: itel P55 5G
- **ABI**: arm64-v8a
- **Output**: `libpvr_minecraft_bridge.so` (shared library)

## Prerequisites

### On Linux/macOS (Build Host)

1. **Android NDK** (r25 or later recommended)
   - Download: https://developer.android.com/ndk/downloads
   - Set `ANDROID_NDK_ROOT` environment variable:
     ```bash
     export ANDROID_NDK_ROOT=/path/to/android-ndk-r25c
     ```

2. **CMake** (3.22+)
   ```bash
   cmake --version
   ```

3. **C/C++ Compiler** (included in NDK)

### Verification

```bash
echo $ANDROID_NDK_ROOT
ls $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/
```

## Build Steps

### 1. Prepare Build Directory

```bash
cd /path/to/PVR-Minecraft-Research/BRIDGE-IMPL
mkdir -p build
cd build
```

### 2. Configure CMake for Android ARM64

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-33 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_ANDROID_STL=c++_shared
```

**Explanation of flags:**
- `CMAKE_TOOLCHAIN_FILE`: Tells CMake to use the Android NDK toolchain
- `ANDROID_ABI`: Target architecture (arm64-v8a for 64-bit ARM)
- `ANDROID_PLATFORM`: Minimum Android API level (33 = Android 13)
- `CMAKE_BUILD_TYPE`: Release build (optimized)
- `CMAKE_ANDROID_STL`: C++ standard library (c++_shared allows linking liblog.so)

### 3. Build the Library

```bash
cmake --build . --config Release
```

**Output:**
```
[ 50%] Building C object CMakeFiles/pvr_minecraft_bridge.dir/src/bridge_init.c.o
[100%] Building C object CMakeFiles/pvr_minecraft_bridge.dir/src/bridge_log.c.o
[100%] Linking C shared library libpvr_minecraft_bridge.so
[100%] Built target pvr_minecraft_bridge
```

The built library is located at:
```
build/libpvr_minecraft_bridge.so
```

### 4. Verify the Build

```bash
file libpvr_minecraft_bridge.so
```

**Expected output:**
```
libpvr_minecraft_bridge.so: ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked, with debug_info
```

### 5. Run Static Checks (Optional)

```bash
# `check_headers_only` is the CMake build target.
cmake --build . --target check_headers_only

# `HeaderInclusionCheck` is the CTest name.
ctest --verbose
```

## Build on Android NDK (Alternative)

If cross-compilation from a build host is not available, the NDK can also be used directly. The above steps are the standard and recommended approach.

## Installation for Testing

### Option A: Using patchelf (MaterialBinLoader Approach)

1. Obtain the target Minecraft `.apk` file from the device:
   ```bash
   adb pull /data/app/*/base.apk minecraft.apk
   ```

2. Extract and modify:
   ```bash
   unzip -q minecraft.apk -d minecraft/
   cd minecraft/lib/arm64-v8a
   
   # Copy the bridge library
   cp /path/to/libpvr_minecraft_bridge.so .
   
   # Add dependency to the Minecraft native library
   patchelf --add-needed libpvr_minecraft_bridge.so libminecraftpe.so
   ```

3. Repackage and sign:
   ```bash
   cd /path/to/minecraft
   zip -r ../minecraft_modified.apk .
   # Sign with your debug key or a custom signing key
   jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 \
     -keystore ~/.android/debug.keystore \
     minecraft_modified.apk androiddebugkey
   ```

4. Install and test:
   ```bash
   adb install minecraft_modified.apk
   adb logcat | grep "PVR-Bridge"
   ```

### Option B: Using KafkaLauncher Approach (Future)

The bridge can also be loaded via a forwarder library that locates and patches the Minecraft native library at runtime. This is documented in EXP-002 and later experiments.

## Build Configuration Status

The repository contains an Android ARM64 build configuration, but this checkout
does not contain a verified build log or a generated shared-library artifact.
The following are source/configuration properties, not execution evidence:

- CMake target requests `arm64-v8a` when configured with the Android NDK.
- The bridge links against Android's `liblog.so` when `ANDROID` is enabled.
- The initialization marker is defined in source.
- No RenderDragon hooks, BGFX interception, or Minecraft proprietary code are present.

**Initial local verification record (2026-09-15):**

`ANDROID ARM64 BUILD NOT EXECUTED — Android NDK, CMake, and compiler are unavailable in the audit environment.`

That local limitation was later resolved by the reproducible GitHub Actions
workflow documented below; the local environment itself did not perform the
Android build.

## Verified Build Evidence

GitHub Actions run [34976852843](https://github.com/Maouuusamaaa/PVR-Minecraft-Research/actions/runs/34976852843)
completed successfully on 2026-09-15 for commit `3f5c64e`.

The workflow used the following environment:

- Runner: `ubuntu-24.04`
- OS: Ubuntu 24.04
- CMake: `3.22.1-g37088a8`
- Ninja: `1.10.2`
- Android NDK: `26.3.11579264`
- Android API: `android-33`
- ABI: `arm64-v8a`

The following stages passed:

1. Host configure, `check_headers_only` build, and `HeaderInclusionCheck` CTest.
2. Android ARM64 configure with `android.toolchain.cmake`.
3. Android ARM64 build of `pvr_minecraft_bridge`.
4. ELF, dependency, symbol, and artifact verification.

Verified artifact:

- Filename: `libpvr_minecraft_bridge.so`
- Size: `13024` bytes
- Type: `ELF 64-bit LSB shared object`
- Architecture: `ARM aarch64` / `AArch64`
- SONAME: `libpvr_minecraft_bridge.so`
- SHA-256: `8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73`

Dynamic dependencies reported by `llvm-readelf` were `liblog.so`, `libm.so`,
`libdl.so`, and `libc.so`; no Minecraft proprietary library dependency was
present. The dynamic symbol table contained `bridge_initialize` and the
bridge logging functions. The generated `.so` and text evidence are stored
in the GitHub Actions artifact `exp-001-android-arm64` for this run.

This evidence confirms the source-to-ARM64-ELF build path only. It does not
confirm that the library has been loaded into Minecraft. Runtime status remains
`NOT TESTED`.

## Evidence Required for TESTED

The following must be observed at runtime on the target device (itel P55 5G, Android 13, arm64-v8a) to classify EXP-001 as TESTED:

1. Minecraft Bedrock Edition version is recorded
2. The modified APK (with the bridge library) is installed on the target device
3. Minecraft is launched
4. `adb logcat` output contains:
   ```
   [PVR-Bridge] MARKER: PVR_BRIDGE_LOADED
   ```
5. The PID and TID context are logged:
   ```
   [PVR-Bridge] Process ID (PID): <actual_pid>
   [PVR-Bridge] Thread ID (TID): <actual_tid>
   ```
6. The log entries appear in the Minecraft process context (verified via `adb shell ps`)

**This is NOT claimed until the runtime test is performed.**

## Troubleshooting

### CMake: "Could not find Android NDK"

```bash
export ANDROID_NDK_ROOT=/path/to/android-ndk-r25c
```

### Build fails: "liblog.so not found"

Ensure the NDK platform is correct:
```bash
ls $ANDROID_NDK_ROOT/platforms/android-13/arch-arm64/usr/lib/
# Should contain: liblog.so, libm.so, etc.
```

### Binary is for wrong architecture

```bash
file libpvr_minecraft_bridge.so
# Check: "ARM aarch64" should appear
```

### APK modification fails at patchelf

Ensure Minecraft's `libminecraftpe.so` matches the architecture:
```bash
file libminecraftpe.so
# Should show: "ARM aarch64"
```

## Research Boundary

- **Included**: Source code, build configuration, logging infrastructure, documentation
- **NOT Included**: Minecraft APKs, Mojang native libraries, proprietary binaries, extracted copyrighted assets

## Next Steps

After EXP-001 TESTED (runtime observation on device):
1. Proceed to EXP-002 — Target Detection (identify render-related runtime targets)
2. Design EXP-003 — Frame/Render Reachability
3. Plan hook implementation based on observed targets

## References

- [Android NDK Documentation](https://developer.android.com/ndk/guides)
- [CMake Android Support](https://cmake.org/cmake/help/v3.22/manual/cmake-toolchains.7.html#cross-compiling-for-android)
- MaterialBinLoader: ARM/ARM64 dependency injection via patchelf
- KafkaLauncher: Runtime forwarder architecture

---

**Last Updated**: EXP-001 build configuration audit
**Status**: ANDROID ARM64 BUILD CONFIRMED via GitHub Actions; runtime NOT TESTED
