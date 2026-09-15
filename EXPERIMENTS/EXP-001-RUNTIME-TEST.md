# EXP-001 Runtime Loading Test

## Objective

Determine whether the controlled native bridge can be loaded and initialized inside the target Minecraft Bedrock Android process. The success marker is `PVR_BRIDGE_LOADED`, and the evidence must demonstrate that the marker came from the target Minecraft process rather than merely from a standalone Android application.

This experiment does not attempt RenderDragon hooking, BGFX interception, renderer replacement, GPU modification, or gameplay integration.

## Current Runtime Checkpoint

The target device was inspected directly on 2026-09-15 through the user's authorized `rish`/Shizuku shell path. Minecraft is installed and running, and its process identity and native-library layout are known.

The remaining blocker is the loading boundary: no authorized, reproducible, reversible method has been established for loading the PVR bridge into the installed retail Minecraft process. Android also denied direct shell access to `/proc/11196/maps` and `/proc/11196/exe`.

Therefore the Minecraft target portion remains **BLOCKED / NOT TESTED**. The controlled harness remains a separate confirmed Android loading checkpoint.

## Current Artifact

The current bridge build artifact was produced by GitHub Actions run [34976852843](https://github.com/Maouuusamaaa/PVR-Minecraft-Research/actions/runs/34976852843).

| Field | Value |
|---|---|
| Filename | `libpvr_minecraft_bridge.so` |
| ABI | `arm64-v8a` |
| Android API target | `android-33` |
| Size | `13024` bytes |
| SHA-256 | `8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73` |
| ELF | 64-bit AArch64 shared object |

## Target Device Evidence — 2026-09-15

### Device and process

```text
Android version: 13
Device model: itel P661N
ABI list: arm64-v8a, armeabi-v7a, armeabi
Minecraft package: com.mojang.minecraftpe
Minecraft version: 1.26.45.1
Minecraft versionCode: 972604501
minSdk: 32
targetSdk: 36
Minecraft PID: 11196
Minecraft UID: 10267
Required ABI: arm64-v8a
Instruction set: arm64
Activity: com.mojang.minecraftpe/.MainActivity
Application class: com.pairip.application.Application
```

The process was independently identified using `ps` and `pidof`, both returning PID `11196` for `com.mojang.minecraftpe`.

### APK and native-library layout

Package Manager reported these package paths:

```text
package:/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/base.apk
package:/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/split_config.arm64_v8a.apk
package:/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/split_config.en.apk
package:/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/split_config.xhdpi.apk
package:/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/split_install_pack.apk
```

Package dump reported:

```text
legacyNativeLibraryDir=/data/app/~~VTcSKTNd05Rguzn6xVYk5g==/com.mojang.minecraftpe-zsnavqdOUWJNnjzupyw3sw==/lib
extractNativeLibs=true
splits=[base, config.arm64_v8a, config.en, config.xhdpi, install_pack]
```

The native directory contained:

```text
libHttpClient.Android.so       326K
libMediaDecoders_Android.so    1.4M
libPlayFabMultiplayer.so       4.1M
libc++_shared.so               1.1M
libconscrypt_jni.so             2.0M
libfmod.so                      1.1M
libmaesdk.so                    7.0M
libminecraftpe.so              299M
libpairipcore.so                611K
```

`libminecraftpe.so` was further identified as:

```text
ELF shared object, 64-bit LSB arm64
for Android 26
built by NDK r28c (13676358)
BuildID=868e275cb295e9a275bb29d2258edc2f7dc48761
stripped
```

ELF header inspection confirmed `ELF64`, little-endian, type `DYN`, machine `arm64`, 13 program headers and 28 section headers. The ELF entry point field is `0x0`.

### Process inspection boundary

The following read-only checks were attempted against PID `11196`:

```text
/proc/11196/maps -> Permission denied
/proc/11196/exe  -> Permission denied
```

`dumpsys meminfo 11196` did expose aggregate process information, including:

```text
Native Heap PSS: 84601 KB
.so mmap PSS:     8955 KB
GL mtrack PSS:    1796 KB
TOTAL PSS:      332371 KB
```

These aggregate values do not identify individual loaded `.so` files and therefore are not accepted as proof of bridge loading.

## Controlled Android Harness Evidence

The signed harness test is separately confirmed. It used package `com.pvr.exp001.harness` and produced `PVR_BRIDGE_LOADED` in harness PID `8936`.

The harness evidence is recorded in `EXP-001-HARNESS-RUNTIME-2026-09-15.md`.

Classification:

```text
Android native library loading in controlled harness = CONFIRMED
Minecraft native bridge loading = NOT TESTED
```

## Loading Method Boundary

The repository currently contains no legitimate, reproducible, target-device loading mechanism that places `libpvr_minecraft_bridge.so` into the installed retail Minecraft process without relying on a separately authorized user-controlled test setup.

The native-library directory being readable does not itself provide a process-loading mechanism. Likewise, identifying PID `11196` does not establish that a library can or should be inserted into that process.

No exploit, security bypass, stealth technique, credential access, or unauthorized process injection is part of this experiment.

## Result

```text
CONTROLLED ANDROID CHECKPOINT = CONFIRMED
MINECRAFT TARGET LOADING       = BLOCKED / NOT TESTED
EXP-001 OVERALL                = NOT CONFIRMED
```

The Minecraft portion must not be promoted to `TESTED` or `CONFIRMED` merely because the standalone harness succeeded.

## Completion Criteria

EXP-001 can be completed for its defined target objective only when an authorized, reproducible and reversible run demonstrates:

```text
PVR_BRIDGE_LOADED
```

from the bridge constructor inside the target Minecraft process, with PID/process identity correlation preserved.

If no legitimate loading method becomes available, the final research record should preserve the controlled Android checkpoint and explicitly classify the Minecraft target boundary as `BLOCKED`.

## Research Boundary

No proprietary Minecraft APKs, `libminecraftpe.so`, extracted native libraries, copyrighted assets, private device identifiers, or unrelated personal logs are committed to the repository.
