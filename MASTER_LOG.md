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
- TESTED: GitHub connector can currently create/update files on the default branch.
- CONFIRMED: `README.md` defines this repository as research, architecture, experiments and technical documentation for integrating PVR with Minecraft Bedrock RenderDragon on Android.
- TESTED: `MASTER_LOG.md` exists as the consolidated research state.
- TESTED: `EXPERIMENTS/EXP-001.md` records the controlled native-library-loading experiment and target/runtime evidence boundary.
- TESTED: `EXPERIMENTS/EXP-002.md` records the revised observation-first target-detection experiment.
- TESTED: `EXPERIMENTS/EXP-002-BASELINE-2026-09-15.md` records the frozen target baseline and current public-source review.
- TESTED: `EXPERIMENTS/EXP-001-AUDIT-2026-09-15.md` records the final EXP-001 boundary decision.
- TESTED: `EXPERIMENTS/EXP-001-DECISION-2026-09-15.md` records the formal closure and revised EXP-002 gate.

## RenderDragon / bgfx Research Status

- CONFIRMED: current public RenderDragon-adjacent projects expose and work with BGFX shader/material infrastructure.
- CONFIRMED: public Android RenderDragon shader projects document support for current 1.26-era Bedrock versions and Android material/shader workflows.
- CONFIRMED: Microsoft 1.26.0 documentation contains a RenderDragon for Creators Experiment section and records Android/PlayStation support for colored block lights in the Vibrant Visuals preview.
- CONFIRMED: BetterRenderDragon has an Android-related build/loading path and a shared-library target; its Android configuration is distinct from the Windows renderer-hooking path.
- CONFIRMED: BetterRenderDragon uses Kiero on its Windows build path, demonstrating a renderer-hooking approach in that project. This does not prove the Android interception point.
- CONFIRMED: a current public BetterRenderDragon Android configuration uses `preloader_android`, showing a distinct Android loading path rather than assuming the Windows hook stack applies unchanged.
- CONFIRMED: public RenderDragon shader repositories contain reconstructed BGFX SC and GLSL shader code generated from material.bin data.
- CONFIRMED: bgfx's public API records rendering commands such as `bgfx::submit()` and advances frame processing with `bgfx::frame()`; bgfx also supports multi-threaded command recording through `bgfx::Encoder`.
- CONFIRMED: MaterialBinLoader documents an Android ARM/ARM64 native-library loading approach by adding a dependency to `libminecraftpe.so` and rebuilding/signing the modified package.
- CONFIRMED: KafkaLauncher publicly documents an alternative Android forwarder architecture for a specific Minecraft 1.21.51.02 environment.
- PROBABLE: a useful research boundary exists around the RenderDragon frame-building / command-recording layer and its BGFX submission path.
- HYPOTHESIS: the production Minecraft Android binary exposes a stable, hookable function corresponding directly to BGFX `submit()` or a single backend submission function.
- UNKNOWN: exact production Android symbol/address/call chain for frame start, render builder, encoder/command recording, BGFX submit, backend execution and present.

## Main Blockers

1. Exact production RenderDragon Android frame/render submission path is not proven.
2. A minimal native bridge has **not** been demonstrated inside the Minecraft Android process. The controlled Android harness has demonstrated native loading and bridge initialization in its own process only.
3. PVR invocation from the Minecraft render/frame path is not proven.
4. PVR GPU work occurring inside Minecraft is not proven.
5. No partial renderer-subsystem takeover has been demonstrated.

## Experiments

### EXP-001 — Native Library Loading

Status: **CLOSED — CONTROLLED CHECKPOINT CONFIRMED; MINECRAFT TARGET BOUNDARY BLOCKED.**

Objective: prove that a controlled native bridge can load in the target Minecraft Android process.

The controlled objective was partially resolved in the researcher-controlled environment: the signed ARM64 Android harness loaded and initialized the bridge, and the controlled render target demonstrated a dedicated native render thread, runtime OpenGL ES/EGL identification, repeated BEGIN → RENDERED → PRESENT frames, and a renderer-side bridge call in the same process. CI run `34988519395` completed successfully after the Java `Surface` handoff correction.

This is valid runtime evidence for the controlled Android loading/render path only. It is not evidence of Minecraft loading.

Target Minecraft loading remains `BLOCKED / NOT TESTED` because no authorized, reproducible, reversible and observable target-side loading method has been established.

Runtime evidence records:
- `EXPERIMENTS/EXP-001-HARNESS-RUNTIME-2026-09-15.md`
- `EXPERIMENTS/EXP-001-RUNTIME-2026-09-15.md`
- `EXPERIMENTS/EXP-001-RUNTIME-TEST.md`
- `EXPERIMENTS/EXP-001-AUDIT-2026-09-15.md`
- `EXPERIMENTS/EXP-001-DECISION-2026-09-15.md`

### EXP-002 — Target Detection

Status: **IN PROGRESS — BASELINE CONFIRMED; RENDER TARGET UNKNOWN.**

Objective: identify and characterize the relevant native/runtime target associated with Minecraft rendering without assuming bridge execution, a particular renderer hook, or a public function name is identical to the production implementation.

Initial target baseline is frozen: Minecraft `1.26.45.1`, Android 13, `arm64-v8a`, package `com.mojang.minecraftpe`, observed process PID `11196`, and `libminecraftpe.so` present in the installed native-library directory. `/proc/11196/maps` and `/proc/11196/exe` were denied from the available shell context.

The public-source review has now been expanded with current 1.26-era RenderDragon/shader evidence and Android process/native-library documentation. This strengthens architecture context but does not identify a production hook point.

The revised experiment may use read-only target inventory, legitimate runtime observation, public-source architecture mapping, and comparison against the controlled render-target contract. It must not assume that PVR executes inside retail Minecraft.

Baseline record: `EXPERIMENTS/EXP-002-BASELINE-2026-09-15.md`.

### EXP-003 — Frame/Render Reachability

Status: HOLD.

Objective: prove that the bridge can execute on or observe a render/frame-related path, only after EXP-002 provides a defensible target boundary.

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

EXP-001 is formally closed at the Minecraft target boundary. Do not relabel the controlled result as Minecraft renderer integration.

EXP-002 is the active next step. The baseline and public-source review are complete for this checkpoint. The remaining work is direct, legitimate observation of the target runtime boundary. If that boundary remains unavailable from the permitted observation context, record it as `UNKNOWN/BLOCKED` rather than inventing a hook point.

Do not begin by importing the entire PVR renderer into Minecraft and do not use exploit-based or security-boundary-bypass instrumentation.

The minimum desired rendering model remains:

`Frame Start -> Render Builder / Render Pass Construction -> BGFX Command Recording -> BGFX Submit / Encoder -> Backend Execution -> GPU -> Present`

For every edge, record evidence level rather than assuming that a similarly named function is the real production path.

## Research Rule

One question, one controlled change, one measurable evidence set. Preserve negative results as FAILED records instead of silently discarding them.

## Legal / Repository Boundary

Do not commit proprietary Minecraft APKs, Mojang native libraries, extracted proprietary binaries, copyrighted assets, private device identifiers, or unrelated personal logs. Keep source, documentation, experiment notes, scripts, configuration and public references.

## Latest Research Finding

On 2026-09-15 the controlled Android environment reached a confirmed renderer-side checkpoint: the signed ARM64 test application loaded the PVR bridge in its own process, established a dedicated native render thread, identified its OpenGL ES/EGL backend at runtime, and produced repeated frame BEGIN → RENDERED → PRESENT sequences. The bridge's explicit initialization records PID/TID for direct thread correlation. CI run `34988519395` completed successfully after the `Surface` handoff correction. This closes the lower-level controlled rendering checkpoint.

The subsequent EXP-002 public-source review confirms that Android's native-library/process model is package-scoped, public Minecraft Android projects demonstrate version-specific controlled process architectures, and current 1.26-era RenderDragon projects expose shader/material evidence. None of these sources identifies the production native RenderDragon frame/submission call chain for Minecraft `1.26.45.1`.

Final state: **EXP-001 CLOSED AT TARGET BOUNDARY; EXP-002 IN PROGRESS WITH BASELINE CONFIRMED; RENDER TARGET UNKNOWN.**
