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
- TESTED: `MASTER_LOG.md` now exists as the consolidated research state.
- TESTED: `EXPERIMENTS/EXP-001.md` now records the controlled native-library-loading experiment design and current evidence boundary.

## RenderDragon / bgfx Research Status

- CONFIRMED: current public RenderDragon-adjacent projects expose and work with BGFX shader/material infrastructure.
- CONFIRMED: BetterRenderDragon has an Android-related build/loading path and a shared-library target; its Android configuration is distinct from the Windows renderer-hooking path.
- CONFIRMED: BetterRenderDragon uses Kiero on its Windows build path, demonstrating a renderer-hooking approach in that project. This does not prove the Android interception point.
- CONFIRMED: public RenderDragon shader repositories contain reconstructed BGFX SC and GLSL shader code generated from material.bin data.
- CONFIRMED: bgfx's public API records rendering commands such as `bgfx::submit()` and advances frame processing with `bgfx::frame()`; bgfx also supports multi-threaded command recording through `bgfx::Encoder`.
- CONFIRMED: MaterialBinLoader documents an Android ARM/ARM64 native-library loading approach by adding a dependency to `libminecraftpe.so` and rebuilding/signing the modified package.
- CONFIRMED: KafkaLauncher publicly documents an alternative Android forwarder architecture that locates the real Minecraft native library, resolves the native activity entry point, installs hooks, and forwards execution to the original entry point.
- PROBABLE: a useful research boundary exists around the RenderDragon frame-building / command-recording layer and its BGFX submission path.
- HYPOTHESIS: the production Minecraft Android binary exposes a stable, hookable function corresponding directly to BGFX `submit()` or a single backend submission function.
- UNKNOWN: exact production Android symbol/address/call chain for frame start, render builder, encoder/command recording, BGFX submit, backend execution and present.

## Main Blockers

1. Exact production RenderDragon Android frame/render submission path is not proven.
2. A minimal native bridge has not yet been demonstrated inside the Minecraft Android process.
3. PVR invocation from the Minecraft render/frame path is not proven.
4. PVR GPU work occurring inside Minecraft is not proven.
5. No partial renderer-subsystem takeover has been demonstrated.

## Experiments

### EXP-001 — Native Library Loading

Status: NOT STARTED — experiment design documented; runtime test still required.

Objective: prove that a controlled native bridge/library can load in the target Minecraft Android process.

Current evidence: public MaterialBinLoader and KafkaLauncher projects demonstrate native-loading/hooking architectures on Android, but neither proves compatibility with the user's current Minecraft version/device.

### EXP-002 — Target Detection

Status: NOT STARTED.

Objective: identify the relevant RenderDragon/native renderer module or stable runtime target without assuming symbol names.

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

The next runtime milestone is EXP-001. Before attempting renderer interception, prove the smallest possible fact: a controlled native bridge can initialize in the target Minecraft Android process.

After EXP-001 succeeds, map the relevant native/render module for EXP-002. The minimum desired rendering model remains:

`Frame Start -> Render Builder / Render Pass Construction -> BGFX Command Recording -> BGFX Submit / Encoder -> Backend Execution -> GPU -> Present`

For every edge, record evidence level rather than assuming that a similarly named function is the real production path.

## Research Rule

One question, one controlled change, one measurable evidence set. Preserve negative results as FAILED records instead of silently discarding them.

## Legal / Repository Boundary

Do not commit proprietary Minecraft APKs, Mojang native libraries, extracted proprietary binaries, or copyrighted binary assets. Keep source, documentation, experiment notes, scripts, configuration and public references.

## Latest Research Finding

The evidence base now includes two concrete public Android-native integration patterns: MaterialBinLoader documents adding a native ARM/ARM64 dependency to `libminecraftpe.so`, while KafkaLauncher documents a native forwarder that locates the real Minecraft library and installs hooks before forwarding to the original native activity entry point. These establish that native process-level integration patterns exist publicly, but they do not prove compatibility with the target Minecraft version or identify the RenderDragon production interception point. Therefore EXP-001 remains NOT STARTED at runtime, and the project must not yet claim Minecraft/PVR integration.
