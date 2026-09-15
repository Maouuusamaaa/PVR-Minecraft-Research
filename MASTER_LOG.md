# PVR-Minecraft-Research — MASTER LOG

## Project Identity

Repository: `Maouuusamaaa/PVR-Minecraft-Research`

Research target: determine, by evidence, whether Perceptual Voxel Renderer (PVR) can contribute to Minecraft Bedrock Edition RenderDragon rendering on Android ARM64.

Primary progression:
`RenderDragon/bgfx understanding -> minimal native bridge -> PVR execution -> GPU work -> partial renderer subsystem integration -> broader renderer integration -> renderer-takeover feasibility`

The project must not claim full integration or renderer replacement until experimentally demonstrated.

## Status Vocabulary

- CONFIRMED: supported by strong source/evidence.
- TESTED: actually executed or observed.
- PROBABLE: technically plausible but not directly proven.
- HYPOTHESIS: conjecture requiring experiment.
- BLOCKED: concrete blocker prevents the intended test.
- FAILED: attempted and unsuccessful.
- UNKNOWN: unresolved.

## Current State

### PVR

- CONFIRMED: PVR is a native renderer project with World Truth, representation/streaming, greedy meshing, GPU scene, hierarchical/temporal visibility, perceptual culling, render budgeting, GPU-culling contracts and Vulkan backend infrastructure.
- CONFIRMED: Android ARM64 build infrastructure exists in the PVR repository.
- TESTED: a previous fresh CMake build/CTest run reported 15/15 tests passing. This result is historical and must be re-run before treating it as current build evidence.
- UNKNOWN: end-to-end physical-GPU execution in the Minecraft process.
- UNKNOWN: actual RenderDragon integration.

### Research Repository

- TESTED: GitHub connector can currently read this repository.
- TESTED: GitHub connector can currently create files on the default branch. A harmless connector test created `github-connector-test.txt` with commit `525682550f357d3162ead0585af966bd9f91356b`. This is current-chat evidence; older 403 failures remain historical failures and are not erased.
- CONFIRMED: `README.md` defines this repository as research, architecture, experiments and technical documentation for integrating PVR with Minecraft Bedrock RenderDragon on Android.
- TESTED: `MASTER_LOG.md` exists as the consolidated research state.
- TESTED: `EXPERIMENTS/EXP-001.md` records the controlled native-library-loading experiment and target/runtime evidence boundary.
- TESTED: `EXPERIMENTS/EXP-002.md` records the target-detection experiment design and evidence boundary.

## RenderDragon / bgfx Research Status

- CONFIRMED: current public RenderDragon-adjacent projects expose and work with BGFX shader/material infrastructure.
- CONFIRMED: BetterRenderDragon has an Android-related build/loading path and a shared-library target; its Android configuration is distinct from the Windows renderer-hooking path.
- CONFIRMED: BetterRenderDragon uses Kiero on its Windows build path, demonstrating a renderer-hooking approach in that project. This does not prove the Android interception point.
- CONFIRMED: a current public BetterRenderDragon Android configuration uses `preloader_android`, showing a distinct Android loading path rather than assuming the Windows hook stack applies unchanged.
- CONFIRMED: public RenderDragon shader repositories contain reconstructed BGFX SC and GLSL shader code generated from material.bin data.
- CONFIRMED: bgfx's public API records rendering commands such as `bgfx::submit()` and advances frame processing with `bgfx::frame()`; bgfx also supports multi-threaded command recording through `bgfx::Encoder`.
- CONFIRMED: MaterialBinLoader documents an Android ARM/ARM64 native-library loading approach by adding a dependency to `libminecraftpe.so` and rebuilding/signing the modified package.
- CONFIRMED: KafkaLauncher publicly documents an alternative Android forwarder architecture that locates the real Minecraft native library, resolves the native activity entry point, installs hooks, and forwards execution to the original entry point.
- PROBABLE: a useful research boundary exists around the RenderDragon frame-building / command-recording layer and its BGFX submission path.
- HYPOTHESIS: the production Minecraft Android binary exposes a stable, hookable function corresponding directly to BGFX `submit()` or a single backend submission function.
- UNKNOWN: exact production Android symbol/address/call chain for frame start, render builder, encoder/command recording, BGFX submit, backend execution and present.

## Main Blockers

1. Exact production RenderDragon Android frame/render submission path is not proven.
2. A minimal native bridge has **not yet been demonstrated inside the Minecraft Android process**. The controlled Android harness has now demonstrated native loading and bridge initialization in its own process only.
3. PVR invocation from the Minecraft render/frame path is not proven.
4. PVR GPU work occurring inside Minecraft is not proven.
5. No partial renderer-subsystem takeover has been demonstrated.

## Experiments

### EXP-001 — Native Library Loading

Status: **CONTROLLED ANDROID RUNTIME CHECKPOINT CONFIRMED; MINECRAFT TARGET NOT TESTED.**

Objective: prove that a controlled native bridge can load in the target Minecraft Android process.

Current confirmed checkpoint: the signed ARM64 Android harness was installed and launched on the user's Android 13 device. The bridge constructor emitted `PVR_BRIDGE_LOADED`, reported PID/TID `8936`, completed bridge initialization, and `bridge_initialize()` was called explicitly.

This is valid runtime evidence for the controlled Android loading path only. It is not evidence of Minecraft loading.

Runtime evidence record: `EXPERIMENTS/EXP-001-HARNESS-RUNTIME-2026-09-15.md`.

Target Minecraft loading remains `NOT TESTED` because no legitimate, reproducible and reversible target-side loading method has yet been established in the repository.

### EXP-002 — Target Detection

Status: NOT STARTED — research/design phase documented; runtime observation still required.

Objective: identify the relevant native/runtime target associated with Minecraft rendering without assuming that public function names or hook points are identical to the production implementation.

New design record: `EXPERIMENTS/EXP-002.md`.

Required evidence includes the exact Minecraft version, Android/ABI/device/GPU context, native module boundary, candidate renderer-related runtime target, lifecycle/thread context where observable, and a confidence/status classification. Finding a native module or symbol is explicitly not treated as proof of the RenderDragon BGFX submission point.

### EXP-003 — Frame/Render Reachability

Status: NOT STARTED.

Objective: prove that the bridge can execute on or observe a render/frame-related path.

### EXP-004 — PVR Execution

Status: NOT STARTED.

Objective: call a minimal PVR component from the target process and record evidence.

### EXP-005 — GPU Work

Status: NOT STARTED.

Objective: prove that PVR causes controlled GPU work in the Minecraft process.

### EXP-006 — Minimal Visual/Renderer Effect

Status: NOT STARTED.

Objective: produce one controlled, reversible rendering effect attributable to the bridge/PVR path.

### EXP-007 — Partial Subsystem Integration

Status: NOT STARTED.

Candidates: visibility/culling, render budgeting, GPU culling, mesh processing or another narrowly bounded subsystem.

### EXP-008 — Broader Renderer Integration

Status: NOT STARTED.

### EXP-009 — Partial Renderer Takeover

Status: NOT STARTED.

### EXP-010 — Full Renderer Replacement Feasibility

Status: NOT STARTED.

## Immediate Next Step

Do not begin by importing the entire PVR renderer into Minecraft.

The controlled Android native-loading checkpoint for EXP-001 is now complete. The remaining target-side milestone is to determine whether the bridge can be loaded into the user's authorized Minecraft test installation through a legitimate, reproducible and reversible mechanism.

If that target-side boundary is established and tested successfully, complete EXP-001 and then use EXP-002 to map the relevant native/render target from target-side evidence. If no legitimate loading method is available, record the Minecraft portion as BLOCKED/NOT TESTED rather than substituting harness evidence.

The minimum desired rendering model remains:

`Frame Start -> Render Builder / Render Pass Construction -> BGFX Command Recording -> BGFX Submit / Encoder -> Backend Execution -> GPU -> Present`

For every edge, record evidence level rather than assuming that a similarly named function is the real production path.

## Research Rule

One question, one controlled change, one measurable evidence set. Preserve negative results as FAILED records instead of silently discarding them.

## Legal / Repository Boundary

Do not commit proprietary Minecraft APKs, Mojang native libraries, extracted proprietary binaries, or copyrighted binary assets. Keep source, documentation, experiment notes, scripts, configuration and public references.

## Latest Research Finding

On 2026-09-15 the controlled Android harness reached a confirmed runtime checkpoint on the user's Android device: the signed `com.pvr.exp001.harness` APK installed successfully, launched, loaded the bridge, emitted `PVR_BRIDGE_LOADED`, and executed `bridge_initialize()` in PID `8936`. This closes the lower-level Android native-loading checkpoint. It does not establish Minecraft loading, RenderDragon reachability, BGFX interception, PVR execution inside Minecraft, or GPU work. The project therefore remains blocked at the Minecraft loading boundary until an authorized reproducible target-side loading method is available.
