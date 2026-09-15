# EXP-001 Android Native Loading Harness

## Purpose

This harness is a minimal Android application controlled by the researcher. Its purpose is to validate the normal Android native loading path for `libpvr_minecraft_bridge.so` on `arm64-v8a` and Android API 33.

The harness is a control experiment only:

> **Android harness success does not equal Minecraft loading success.**

The harness does not access, modify, inject into, or depend on a Minecraft process.

## Scope

Included:

- A minimal Android Activity.
- Normal `System.loadLibrary("pvr_minecraft_bridge")` loading.
- A JNI wrapper that calls the existing `bridge_initialize()` API.
- Packaging for `arm64-v8a`.
- Logcat observation of the bridge constructor marker.

Excluded:

- RenderDragon hooks.
- BGFX hooks.
- Minecraft libraries or APKs.
- Proprietary assets.
- Process injection.
- Security bypasses or exploit techniques.
- Minecraft runtime testing.

## Artifact

The verified bridge artifact is produced by the existing EXP-001 GitHub Actions build:

| Field | Value |
|---|---|
| Filename | `libpvr_minecraft_bridge.so` |
| ABI | `arm64-v8a` |
| Android API | `android-33` |
| SHA-256 | `8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73` |
| Source build run | [34976852843](https://github.com/Maouuusamaaa/PVR-Minecraft-Research/actions/runs/34976852843) |

The harness workflow rebuilds this artifact from the repository source before packaging it. The artifact is copied into the APK's `lib/arm64-v8a/` directory as a prebuilt native dependency; the harness does not reimplement the bridge.

## Android Configuration

| Setting | Value |
|---|---|
| Runner | `ubuntu-24.04` |
| Android API | `android-33` |
| ABI | `arm64-v8a` |
| Android NDK | `26.3.11579264` |
| CMake | `3.22.1` |
| Gradle | `8.6` |
| Package | `com.pvr.exp001.harness` |

## Project Structure

```text
EXPERIMENTS/EXP-001-HARNESS/
├── app/
│   ├── build.gradle
│   └── src/main/
│       ├── AndroidManifest.xml
│       ├── java/com/pvr/exp001/harness/MainActivity.java
│       ├── cpp/CMakeLists.txt
│       └── cpp/harness_jni.cpp
├── build.gradle
└── settings.gradle
```

The workflow creates the following generated path before Gradle packaging:

```text
app/src/main/jniLibs/arm64-v8a/libpvr_minecraft_bridge.so
```

Generated `.so` and APK files are not committed to the repository.

## Build Procedure

The reproducible workflow is:

```text
.github/workflows/exp-001-android-harness.yml
```

It performs the following operations:

1. Checks out the repository.
2. Sets up Java 17.
3. Uses the runner's Android SDK command-line tools.
4. Installs Android API 33, NDK `26.3.11579264`, and CMake `3.22.1`.
5. Cross-builds `BRIDGE-IMPL` for `arm64-v8a`.
6. Copies the resulting bridge into `app/src/main/jniLibs/arm64-v8a/`.
7. Builds the debug APK with Gradle 8.6 using the standard Gradle/Android debug
   keystore; no private keystore or password is stored in the repository.
8. Verifies the APK signature, package metadata, zip alignment, and that the
   APK contains both:
   - `lib/arm64-v8a/libpvr_minecraft_bridge.so`
   - `lib/arm64-v8a/libexp001_harness.so`
9. Uploads the signed APK and evidence as the
   `exp-001-android-harness-signed` Actions artifact.

## APK Verification

The workflow verifies the actual APK archive, not only the source tree. It records the APK size, APK SHA-256, and bridge SHA-256 in `EXP-001-HARNESS-BUILD.txt`.

The signed APK is checked with the Android Build Tools `apksigner`, `aapt2`, and `zipalign`. These checks are package/signing checks only; they do not execute the APK.

## Runtime Result — 2026-09-15

The signed APK was downloaded to the user's Android 13 device, independently hashed, copied to `/data/local/tmp`, and installed with Android's package manager.

Device-side APK SHA-256:

```text
fdbbd7df546e323e766aac3c81c04efee29e129f4762220c7ae8de363757e38d
```

This exactly matches the SHA-256 recorded in the downloaded `EXP-001-HARNESS-BUILD.txt` evidence.

Installation result:

```text
~/rish -c 'pm install -r /data/local/tmp/exp-001-android-harness-signed.apk'
Success
```

Package registration:

```text
package:com.pvr.exp001.harness
```

The harness was then launched after clearing logcat. The `PVR-Bridge` stream produced:

```text
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Bridge constructor running
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Process ID (PID): 8936
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Thread ID (TID): 8936
09-15 21:44:26.607  8936  8936 I PVR-Bridge: MARKER: PVR_BRIDGE_LOADED
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Bridge initialization complete
09-15 21:44:26.620  8936  8936 I PVR-Bridge: bridge_initialize() called explicitly
```

Full device evidence is preserved in `EXP-001-HARNESS-RUNTIME-2026-09-15.md`.

## Runtime Classification

```text
APK BUILD: CONFIRMED
ARM64 PACKAGING: CONFIRMED
APK INSTALLATION: CONFIRMED
ANDROID NATIVE LOADING IN CONTROLLED HARNESS: CONFIRMED
BRIDGE CONSTRUCTOR EXECUTION: CONFIRMED
PVR_BRIDGE_LOADED: CONFIRMED
BRIDGE INITIALIZATION: CONFIRMED
MINECRAFT LOADING: NOT TESTED
```

The observed bridge PID was `8936`. The marker and initialization messages occurred in the same PID/TID, establishing that the bridge constructor and explicit initialization executed in the controlled harness process.

## Important Boundary

The successful harness result does not prove Minecraft loading. The harness uses the normal Android application loading mechanism and is deliberately isolated from Minecraft.

The separate Minecraft question remains:

```text
libpvr_minecraft_bridge.so
→ authorized loading mechanism
→ target Minecraft process
→ marker correlated with Minecraft PID
```

Only that second chain can establish Minecraft loading.

## Limitations

- The harness is not Minecraft.
- The harness does not validate RenderDragon, BGFX, renderer reachability, or GPU execution.
- No Minecraft process was targeted by this runtime test.
- No proprietary Minecraft APK or native library was used or committed.

## Next Step

The controlled Android runtime checkpoint is complete. Before EXP-002, the remaining EXP-001 target requirement is a legitimate, reproducible and reversible loading boundary for the user's Minecraft installation. If that cannot be established, keep Minecraft loading classified as `BLOCKED`/`NOT TESTED` and do not infer it from the harness result.
