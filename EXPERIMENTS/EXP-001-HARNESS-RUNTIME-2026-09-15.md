# EXP-001 Harness Runtime Evidence — 2026-09-15

## Scope

This is the controlled Android harness checkpoint for EXP-001. It tests normal Android native-library loading and bridge initialization in the researcher-controlled harness process.

It is **not** Minecraft runtime evidence and does not establish RenderDragon access.

## Device / Environment

- Android version: Android 13 (SDK 33)
- Target ABI: `arm64-v8a`
- Harness package: `com.pvr.exp001.harness`
- APK source: GitHub Actions artifact `exp-001-android-harness-signed`
- Harness build commit: `9595a2f154b8f5319ecb0da3fbe3d5dd4f009f6f`
- APK SHA-256 verified on device: `fdbbd7df546e323e766aac3c81c04efee29e129f4762220c7ae8de363757e38d`
- APK size reported by build evidence: `44115` bytes
- Signing: Android Debug certificate, APK Signature Scheme v2 verified

## Installation Evidence

The APK was copied to `/data/local/tmp/exp-001-android-harness-signed.apk` using the authorized `rish`/Shizuku shell path and installed with Android's package manager.

Command result:

```text
~/rish -c 'pm install -r /data/local/tmp/exp-001-android-harness-signed.apk'
Success
```

Package manager subsequently reported:

```text
package:com.pvr.exp001.harness
```

Therefore APK installation and package registration are **CONFIRMED**.

## Runtime Procedure Executed

The harness was stopped, logcat was cleared, the package was launched through Android's activity manager interface, and the `PVR-Bridge` log stream was collected.

Launch command:

```text
~/rish -c 'monkey -p com.pvr.exp001.harness 1'
```

Result included:

```text
data="com.pvr.exp001.harness"
Events injected: 1
```

## Unedited Relevant Logcat Evidence

```text
09-15 21:44:26.606  8936  8936 I PVR-Bridge: =========================================
09-15 21:44:26.606  8936  8936 I PVR-Bridge: PVR Minecraft Bridge — EXP-001
09-15 21:44:26.606  8936  8936 I PVR-Bridge: =========================================
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Bridge constructor running
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Process ID (PID): 8936
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Thread ID (TID): 8936
09-15 21:44:26.607  8936  8936 I PVR-Bridge: MARKER: PVR_BRIDGE_LOADED
09-15 21:44:26.607  8936  8936 I PVR-Bridge: Bridge initialization complete
09-15 21:44:26.607  8936  8936 I PVR-Bridge: =========================================
09-15 21:44:26.620  8936  8936 I PVR-Bridge: bridge_initialize() called explicitly
```

## Result

### Controlled Android native loading

**CONFIRMED.**

The bridge constructor executed and emitted `PVR_BRIDGE_LOADED`. The explicit `bridge_initialize()` call also completed. The Android process ID reported by the bridge was PID `8936`.

This demonstrates the complete controlled-harness chain:

```text
Signed APK
-> Android package installation
-> harness process launch
-> native bridge loading
-> bridge constructor execution
-> PVR_BRIDGE_LOADED
-> bridge_initialize() execution
```

### Minecraft loading

**NOT TESTED.**

The observed process was the controlled harness package `com.pvr.exp001.harness`, not Minecraft. No Minecraft PID or Minecraft logcat evidence was used for this result.

### RenderDragon / BGFX

**NOT TESTED.**

This checkpoint does not interact with RenderDragon, BGFX, the Minecraft renderer, or the GPU renderer path.

## Classification

```text
Android APK installation = CONFIRMED
Android native library loading in controlled harness = CONFIRMED
Bridge constructor execution = CONFIRMED
PVR_BRIDGE_LOADED = CONFIRMED
bridge_initialize() = CONFIRMED
Minecraft process loading = NOT TESTED
RenderDragon reachability = NOT TESTED
EXP-001 overall = NOT CONFIRMED
```

The controlled harness result is a completed lower-level checkpoint, not completion of the target Minecraft experiment.

## Next Step

Update EXP-001 documentation with this controlled-runtime evidence. Do not proceed to EXP-002 until the repository's EXP-001 completion criterion for the target Minecraft process is either demonstrated or explicitly classified as BLOCKED/NOT TESTED with a documented loading boundary.
