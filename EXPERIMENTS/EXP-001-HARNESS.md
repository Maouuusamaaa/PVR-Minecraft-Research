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
7. Builds the release APK with Gradle 8.6.
8. Verifies the APK contains both:
   - `lib/arm64-v8a/libpvr_minecraft_bridge.so`
   - `lib/arm64-v8a/libexp001_harness.so`
9. Uploads the APK and evidence as the `exp-001-android-harness` Actions artifact.

## APK Verification

The workflow verifies the actual APK archive, not only the source tree:

```bash
unzip -l exp-001-android-harness.apk
```

The required native entries are checked with exact `arm64-v8a` paths. The workflow also records the APK size, APK SHA-256, and bridge SHA-256 in `EXP-001-HARNESS-BUILD.txt`.

## Runtime Procedure

Runtime execution requires an authorized Android device or emulator with `adb`. The current environment has no available `adb` device or emulator, so no runtime result is claimed.

On a controlled device, after installing the harness APK from the Actions artifact:

```bash
adb install -r exp-001-android-harness.apk
adb logcat -c
adb shell monkey -p com.pvr.exp001.harness 1
adb shell pidof com.pvr.exp001.harness
adb logcat -v threadtime -d -s PVR-Bridge:* EXP-001-Harness:* '*:S'
```

The expected marker from a successful harness run is:

```text
PVR-Bridge: MARKER: PVR_BRIDGE_LOADED
```

The output should also include the bridge PID/TID lines and the harness process PID should be recorded separately. The exact device model, Android version, ABI, package, PID, timestamp, APK hash, and unedited logcat output must be preserved in the runtime record.

## Logcat Evidence

A marker string inside the `.so` or APK is not runtime evidence. Runtime evidence requires an actual launch and logcat capture showing:

1. The harness APK launched successfully.
2. The bridge constructor ran.
3. `PVR_BRIDGE_LOADED` appeared during that launch.
4. The bridge PID matches the harness PID.
5. The artifact and device details are recorded.

The workflow intentionally does not claim these facts because it only builds and packages the APK.

## Result

Current status:

```text
APK BUILD: CONFIRMED
ARM64 PACKAGING: CONFIRMED
ANDROID NATIVE LOADING: NOT TESTED
BRIDGE INITIALIZATION: NOT TESTED
MINECRAFT LOADING: NOT TESTED
```

GitHub Actions run [34979605711](https://github.com/Maouuusamaaa/PVR-Minecraft-Research/actions/runs/34979605711)
completed successfully for commit `860e13c4cb04dd183577fefc918bfffcf767aded`.

Actual APK evidence:

| Field | Value |
|---|---|
| Filename | `app-arm64-v8a-release-unsigned.apk` |
| Size | `40022` bytes |
| SHA-256 | `d9abde983df14c24ab314d0a2acccb5e2cc4aab80cd4a32abba0cca8ae133f61` |
| APK entries | `lib/arm64-v8a/libexp001_harness.so`, `lib/arm64-v8a/libpvr_minecraft_bridge.so` |
| Embedded bridge SHA-256 | `ab85db08a9ddf24fbc6aafa67d8b74987ddad314d93e2c08b15757a2a04b4228` |
| Runtime | **NOT TESTED** |

The embedded bridge hash differs from the earlier standalone build hash because
the harness workflow rebuilds the bridge from source in a separate job; the
workflow verifies the resulting artifact's packaging and source build, but does
not claim bit-for-bit reproducibility across independent native builds. The
earlier standalone artifact remains identified by
`8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73`.

These results confirm APK creation and ARM64 packaging only. They do not confirm
that Android loaded the library or that the bridge constructor emitted a marker.

## Limitations

- No Android device or emulator is available in the current environment.
- No `adb` runtime capture has been performed.
- The harness is not Minecraft and cannot prove loading into Minecraft.
- The bridge artifact is loaded through the normal Android application mechanism, not injected into another process.
- The harness does not validate RenderDragon, BGFX, renderer reachability, or GPU execution.

## Relation to Minecraft EXP-001

The harness provides a controlled lower-level checkpoint:

```text
Android package contains bridge
→ Android application loads bridge
→ bridge constructor emits marker
```

The separate Minecraft question remains:

```text
libpvr_minecraft_bridge.so
→ authorized loading mechanism
→ target Minecraft process
→ marker correlated with Minecraft PID
```

Only the second chain can establish Minecraft loading. Even a successful harness runtime test must leave:

```text
Minecraft loading = NOT TESTED
EXP-001 overall = NOT CONFIRMED
```

## Next Step

Run the harness on an authorized `arm64-v8a` Android device or emulator, collect real logcat and PID evidence, and record the result separately. Do not use the harness result as evidence of Minecraft loading and do not proceed to EXP-002 based only on this checkpoint.
