# EXP-001 Loading Boundary Research — 2026-09-15

## Purpose

Determine whether the current Android architecture provides a normal, documented application-level mechanism that would allow the PVR bridge to be loaded into the installed retail Minecraft process without relying on cross-process injection or security-boundary bypasses.

## External Android Evidence

Android's official native-code documentation describes the normal application model: native libraries are compiled and packaged with an Android application, and application code loads them at runtime with mechanisms such as `System.loadLibrary()`. The documented flow is therefore application-owned native loading rather than one ordinary application loading native code into another application's process.

Android's security documentation explicitly warns against loading code from outside an application's APK because it increases the risk of code injection and code tampering. The Android platform also maintains application sandbox/process isolation; applications normally run under separate Unix-style UIDs/processes.

The official `<uses-native-library>` mechanism concerns making vendor-provided native shared libraries accessible to an application. It does not provide a general mechanism for one installed application to inject a new native library into another application's process.

These points support the repository's existing boundary: the fact that Minecraft exposes an extracted native-library directory does not constitute a supported cross-process loading interface.

## Target-Side Evidence

The 2026-09-15 device investigation established:

```text
Minecraft package: com.mojang.minecraftpe
Version: 1.26.45.1
PID: 11196
ABI: arm64-v8a
Main native library: libminecraftpe.so
```

The shell could inspect Package Manager metadata and the installed native-library directory, but direct access to `/proc/11196/maps` and `/proc/11196/exe` was denied.

The PVR bridge was successfully loaded in the separate controlled harness process PID `8936`. No equivalent `PVR_BRIDGE_LOADED` evidence exists for PID `11196`.

## Result

```text
Normal Android application-owned native loading = CONFIRMED by platform documentation
Cross-process bridge loading into retail Minecraft = NO SUPPORTED MECHANISM ESTABLISHED
Minecraft target bridge loading = BLOCKED / NOT TESTED
```

This is an architectural boundary conclusion, not a claim that native integration with Minecraft is impossible in every research environment. A specially controlled build, source-controlled application, authorized test harness, or other explicitly supported integration surface could change the boundary. None is currently established for the installed retail target.

## Decision

Do not attempt to work around Android's process isolation or permissions. Do not treat the readable native-library directory as permission to alter or inject into the Minecraft process.

EXP-001 should be considered **closed at the current target boundary** with the following final classification:

```text
EXP-001 controlled Android checkpoint = CONFIRMED
EXP-001 Minecraft target loading       = BLOCKED / NOT TESTED
EXP-001 overall target objective       = BLOCKED
```

## Consequence for EXP-002

The original EXP-002 objective assumes that native execution in the target Minecraft process has already been established. That prerequisite is not met. Therefore the project must not silently begin the original EXP-002 runtime procedure.

The next approved research direction should instead be a **controlled integration-path study**: identify an application/build/test environment where the researcher controls the native loading boundary and where renderer-target observations can be made without crossing another application's security boundary.

Only after such an environment is established should the original EXP-002 target-detection objective be resumed.

## Source Notes

Relevant official Android documentation consulted on 2026-09-15:

- Android Developers — Add C and C++ code to your project: normal native-library packaging/loading model.
- Android Developers — `<uses-native-library>`: application access to vendor native shared libraries.
- Android Developers — Security checklist: risks of dynamically loading code from outside the application APK.
- Android Developers — Android dynamic linker reference: native library loading APIs available to the calling application.
- Android Open Source Project — Android Compatibility Definition: application sandbox and process isolation requirements.

No proprietary Minecraft binary or extracted proprietary library was added to the repository.
