# Controlled Render Target — Android Native Renderer Testbed

## Purpose

This testbed is the next controlled environment after EXP-001. It provides an Android application owned by the research project so native loading, render-thread reachability, frame boundaries, and PVR execution can be tested without attempting to modify or inject code into the retail Minecraft process.

## Research boundary

The testbed is **not Minecraft** and must never be presented as evidence that Minecraft/RenderDragon has been reached.

Its purpose is to establish a reproducible renderer-side baseline:

`Android app -> native process -> render thread -> graphics API -> frame boundary -> PVR bridge`

Only after this chain is experimentally demonstrated should Minecraft-specific observations be correlated with it.

## Proposed implementation

- Android ARM64 application.
- Native C/C++ renderer component.
- A visible render surface.
- Explicit render-thread identification and logging.
- Per-frame begin/end markers.
- Graphics backend identification at runtime.
- PVR bridge loaded through the application's normal native-library loading path.
- No cross-process loading, injection, hooking, or bypass mechanism.
- No proprietary Minecraft binaries or extracted Minecraft libraries.

## Backend strategy

The first backend should use the graphics API that is actually available and observable on the test device. Vulkan is the preferred first candidate because Android documents Vulkan as its primary low-level graphics API and provides NDK support for native applications. OpenGL ES remains a useful fallback/test path where device support requires it. citeturn0search0turn0search8

The implementation must record the selected backend rather than assuming it from library presence alone. Android's own guidance notes that engines can load both Vulkan and OpenGL ES while ultimately using only one, so library-loading evidence alone is insufficient. citeturn0search7

## Instrumentation requirements

The renderer must emit machine-readable markers for:

- renderer initialization;
- render thread start;
- frame begin;
- render work submitted;
- frame/present boundary;
- frame end;
- bridge initialization;
- bridge execution from the renderer path.

Each marker should include PID, TID, monotonic timestamp, and frame counter where applicable.

## PASS criteria

The controlled target passes its first checkpoint only when all of the following are demonstrated on-device:

1. The APK launches normally.
2. The native renderer initializes.
3. A dedicated render path is active.
4. The render thread identity is observable.
5. Frames are repeatedly produced.
6. The PVR bridge initializes in the same controlled process.
7. A renderer-side call reaches the bridge on the expected render thread.
8. Logs provide enough evidence to correlate bridge execution with a specific frame.

## NON-PASS conditions

- Bridge initializes but no renderer path is reached.
- Renderer runs but bridge execution occurs only on an unrelated thread.
- Logs cannot distinguish frame events from startup events.
- The graphics backend cannot be identified reliably.
- Results depend on an uncontrolled external process.

## Interpretation boundary

A PASS proves only that the research bridge can reach a controlled native rendering path on Android.

It does **not** prove:

- Minecraft loading;
- RenderDragon loading;
- BGFX usage by the target Minecraft version;
- compatibility with `libminecraftpe.so`;
- compatibility with the production Minecraft renderer;
- renderer replacement feasibility.

Those remain separate research questions.

## Relation to EXP-002

This testbed is a preparation and calibration environment for EXP-002. It does not replace the Minecraft-specific target-detection objective. EXP-002 remains HOLD until an appropriate controlled observation path for the target renderer is established.

## Source note

Android documents native Vulkan rendering as a supported Android game-engine path and documents the Vulkan loader/runtime model. Android also documents GLES layers as a controlled debugging mechanism for debuggable applications, reinforcing the distinction between supported application-owned instrumentation and modifying an unrelated production process. citeturn0search0turn0search4
