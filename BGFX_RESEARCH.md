# BGFX / RenderDragon Research

## Purpose

Establish what can be proven about the relationship between Minecraft Bedrock RenderDragon and BGFX, and isolate what still requires Android runtime experiments.

## Evidence Levels

### CONFIRMED — BGFX command/frame model

Public BGFX documentation and source examples show the application-facing flow of setting render state/resources, calling `bgfx::submit(view, program)`, and then calling `bgfx::frame()` to advance the frame. BGFX also exposes `bgfx::Encoder` for recording rendering commands from multiple threads.

Important implication: a call to `bgfx::submit()` is not automatically equivalent to an immediate low-level GPU API draw call. The application records rendering intent; BGFX processes it through its rendering architecture.

### CONFIRMED — RenderDragon-adjacent BGFX shader/material ecosystem

Public repositories contain reconstructed vanilla RenderDragon GLSL and BGFX SC shader code. MaterialBinTool documentation also describes `.material.bin` as RenderDragon material data and BGFX SC as BGFX's shader-compiler GLSL flavor.

This establishes strong evidence for BGFX's relevance to RenderDragon's shader/material pipeline.

### CONFIRMED — BetterRenderDragon has separate Android and Windows build paths

BetterRenderDragon's current public build configuration defines a shared `BetterRenderDragon` library. The Android branch uses an Android preloader dependency and removes Windows-specific memory, patch, GUI and DLL entry-point sources. The Windows branch links Windows graphics libraries and uses Kiero-related files.

This is useful architectural evidence, but it does NOT prove a particular production Android RenderDragon hook.

### CONFIRMED — Kiero evidence is primarily Windows-side in this reference

The public build configuration includes Kiero source and D3D11/D3D12 support in the Windows configuration. The Android configuration removes Windows-specific sources. Therefore Kiero's Windows renderer-hooking path must not be treated as proof of an Android RenderDragon interception point.

## Working Render Path Model

The current research model is:

`Frame Start`
`  -> Render/Pass Construction`
`  -> BGFX command recording`
`  -> BGFX submit / Encoder`
`  -> BGFX frame boundary`
`  -> Renderer backend`
`  -> GPU execution`
`  -> Present`

Every arrow is a research target. The model is not a claim that Minecraft Android exposes each stage as a separately hookable function.

## What We Can Infer Safely

1. RenderDragon shader/material work can be studied through public shader/material projects.
2. BGFX's public architecture provides a useful conceptual model for separating command recording from backend execution.
3. A renderer modification can theoretically target an earlier command-generation stage, a BGFX-level boundary, or a lower backend boundary, but the safest target cannot be selected until the actual Android call chain is observed.
4. A PVR integration should initially avoid replacing the complete renderer. A small subsystem such as visibility/culling or render budgeting provides a much smaller experimental surface.

## What We Must NOT Claim Yet

- The exact production Android `bgfx::submit()` call site inside Minecraft.
- The exact production Android function that corresponds to BGFX's internal backend submission.
- That a stable exported symbol exists.
- That an Android native bridge can currently load into the target process.
- That PVR can currently execute inside Minecraft.
- That PVR can currently submit GPU work through RenderDragon.
- That PVR can replace RenderDragon.

## Next Evidence Required

### A. Public-source call-chain mapping

Trace RenderDragon-adjacent public projects for:

- native initialization
- renderer object discovery
- frame lifecycle
- render builder / render pass construction
- BGFX context/encoder usage
- shader/material loading
- backend selection
- submission and presentation boundaries

### B. Target runtime observation

On the controlled Android test device, establish:

- Minecraft Bedrock version
- process/module inventory relevant to rendering
- GPU/driver information
- native library load behavior
- whether a controlled bridge can execute
- whether execution can be correlated with a render/frame event

### C. Minimal PVR proof

Only after A/B succeed:

1. load a minimal PVR bridge;
2. call a harmless PVR function;
3. record logs/timing;
4. prove execution on the intended runtime path;
5. only then test GPU-side work.

## Research Conclusion

Current evidence supports the statement: **BGFX is an important RenderDragon-adjacent rendering abstraction and public tooling exposes its shader/material ecosystem.**

Current evidence does NOT support the stronger statement: **we know the exact Android production RenderDragon draw-submission interception point.**

That stronger statement remains UNKNOWN/HYPOTHESIS and is the next major research target.
