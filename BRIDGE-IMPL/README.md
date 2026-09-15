# PVR Minecraft Bridge — EXP-001

Minimal Android ARM64 native bridge for controlled research on Minecraft Bedrock RenderDragon.

## Quick Start

### Build

```bash
export ANDROID_NDK_ROOT=/path/to/android-ndk-r25c
mkdir build && cd build
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-13
cmake --build . --config Release
```

Output: `build/libpvr_minecraft_bridge.so`

### Verify Build

```bash
file libpvr_minecraft_bridge.so
```

Expected: `ELF 64-bit LSB shared object, ARM aarch64, ...`

### Run Tests

```bash
cmake --build . --target HeaderInclusionCheck
```

## What's Included

- ✅ Controlled initialization marker (logged to logcat)
- ✅ Android logging infrastructure
- ✅ CMake + Android NDK build system
- ✅ Build and architectural documentation
- ✅ No RenderDragon hooks, BGFX interception, or renderer code

## What's NOT Included

- ❌ Minecraft APKs or binaries
- ❌ Mojang native libraries
- ❌ RenderDragon hook implementations
- ❌ PVR renderer code
- ❌ Any proprietary or copyrighted binary assets

## Status

- **BUILDABLE/READY** ✅ (build-time verified)
- **TESTED** ⏳ (awaiting runtime observation on target device)

## Next Steps

1. Follow [BUILD.md](BUILD.md) for detailed build instructions
2. Install on target device (Android 13, ARM64, itel P55 5G)
3. Launch Minecraft and observe logcat:
   ```bash
   adb logcat | grep "PVR-Bridge"
   ```
4. Record results in `EXPERIMENTS/EXP-001.md`

## Documentation

- [BUILD.md](BUILD.md) — Detailed build guide, troubleshooting
- [ARCHITECTURE.md](ARCHITECTURE.md) — Design philosophy, strategy, extensibility

## References

See parent repository documentation:
- `EXPERIMENTS/EXP-001.md` — Experiment design
- `MASTER_LOG.md` — Project status
