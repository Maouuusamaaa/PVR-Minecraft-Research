# Controlled Render Target — Android Native Renderer Testbed

## Purpose

This testbed is the controlled renderer environment established during EXP-001. It provides an Android application owned by the research project so native loading, render-thread reachability, frame boundaries, and PVR bridge execution can be tested without attempting to modify or inject code into the retail Minecraft process.

The testbed is a lower-level calibration checkpoint for the research sequence; it does not advance the Minecraft-specific experiment state by itself.

## Research boundary

The testbed is **not Minecraft** and must never be presented as evidence that Minecraft/RenderDragon has been reached.

Its purpose is to establish a reproducible renderer-side baseline:

`Android app -> native process -> render thread -> graphics API -> frame boundary -> PVR bridge`

Only after this chain is experimentally demonstrated should Minecraft-specific observations be correlated with it.

## Implemented environment

- Android ARM64 application.
- Native C++ renderer component.
- Visible render surface.
- Dedicated native render thread with PID/TID logging.
- Per-frame begin/render/present markers.
- Runtime graphics-backend identification.
- PVR bridge loaded through the application's normal native-library loading path.
- No cross-process loading, injection, hooking, or bypass mechanism.
- No proprietary Minecraft binaries or extracted Minecraft libraries.

## Backend status

The current implementation uses **OpenGL ES 2 through EGL**. The renderer records the actual runtime API/vendor/renderer strings instead of treating library presence as proof of the active backend. Android's documentation specifically warns that engines can load both Vulkan and OpenGL ES while ultimately using only one. citeturn0search2

Vulkan remains a future controlled-backend candidate. Android documents Vulkan as its primary low-level graphics API for native game rendering and provides NDK/runtime support for it. citeturn0search0turn0search10

The choice of OpenGL ES for this checkpoint is deliberate: the current testbed already provides a working, observable EGL render loop. Switching the backend is a separate experiment and must not be conflated with the present PASS evidence.

## Instrumentation requirements

The renderer emits machine-readable markers for:

- renderer initialization;
- render thread start;
- frame begin;
- render work submitted;
- frame/present boundary;
- bridge initialization;
- bridge execution from the renderer path;
- render thread stop.

PID/TID are included on thread-sensitive markers. Frame markers include a frame counter. The bridge's explicit initialization call records its PID/TID so the renderer thread can be correlated with bridge execution.

## PASS criteria

The controlled target passes its first checkpoint only when all of the following are demonstrated on-device:

1. The APK launches normally.
2. The native renderer initializes.
3. A dedicated render path is active.
4. The render thread identity is observable.
5. Frames are repeatedly produced.
6. The PVR bridge initializes in the same controlled process.
7. A renderer-side call reaches the bridge on the expected render thread.
8. Logs provide enough evidence to correlate bridge execution with the render path.

The 2026-09-15 runtime evidence satisfies this controlled checkpoint: the renderer produced repeated BEGIN → RENDERED → PRESENT sequences, and the bridge was initialized from the renderer thread.

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

The controlled render target is a calibration environment for EXP-002, not a substitute for Minecraft-specific target detection. EXP-002 remains **HOLD** while EXP-001's Minecraft target boundary remains blocked/not tested.

## Source note

Android documents native Vulkan rendering as a supported Android game-engine path. Android also documents that logcat alone can be unreliable for identifying the active graphics API, which is why this testbed records the API directly from its graphics context. citeturn0search0turn0search2
