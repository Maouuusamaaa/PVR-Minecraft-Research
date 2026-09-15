# PVR Minecraft Bridge — EXP-001 Architecture

## Project Structure

```
BRIDGE-IMPL/
├── CMakeLists.txt              # Build configuration for Android NDK
├── BUILD.md                    # Build instructions and troubleshooting
├── ARCHITECTURE.md             # This file
├── README.md                   # Quick start
├── include/
│   ├── bridge_init.h          # Initialization marker
│   └── bridge_log.h           # Logging infrastructure
├── src/
│   ├── bridge_init.c          # Constructor-based initialization
│   └── bridge_log.c           # Android logcat logging
└── test/
    └── check_headers.c        # Header inclusion test
```

## Design Philosophy

The bridge is designed around a **single, minimal, measurable objective**: prove that a controlled native library can be loaded into the Minecraft Android process.

### Principles

1. **Isolation**: The bridge is self-contained and does not depend on Minecraft internals
2. **Reversibility**: Can be unloaded/removed without affecting Minecraft's core function
3. **Observability**: Initialization is logged to logcat and can be verified from the command line
4. **Research-Only**: No production features, no renderer integration, no claim of compatibility

## Initialization Strategy

### Constructor-Based Initialization (`bridge_init.c`)

```c
__attribute__((constructor))
static void pvr_bridge_constructor(void)
{
    /* Runs automatically when the .so is loaded by the dynamic linker */
    bridge_log_init();
    bridge_log_info("Bridge constructor running");
    bridge_log_marker("PVR_BRIDGE_LOADED");
}
```

**Why this approach?**

- **Automatic**: The dynamic linker calls this before any application code runs
- **No JNI required**: Works without Java/JNI glue code
- **Minimal contract**: Only requires the .so to be in the library load path
- **Observable**: The log output is immediately visible in logcat

### Logging Infrastructure (`bridge_log.c`)

- **On Android**: Uses `__android_log_*()` to write to logcat with tag `[PVR-Bridge]`
- **On other platforms**: Falls back to `fprintf(stderr, ...)`
- **Thread-safe**: Android's liblog is thread-safe

**Log output example:**
```
[PVR-Bridge] =========================================
[PVR-Bridge] PVR Minecraft Bridge — EXP-001
[PVR-Bridge] =========================================
[PVR-Bridge] Bridge constructor running
[PVR-Bridge] Process ID (PID): 12345
[PVR-Bridge] Thread ID (TID): 12367
[PVR-Bridge] MARKER: PVR_BRIDGE_LOADED
[PVR-Bridge] Bridge initialization complete
[PVR-Bridge] =========================================
```

## What This Bridge Does NOT Do

| Feature | Status | Reason |
|---------|--------|--------|
| Hook RenderDragon | ❌ Not implemented | Deferred to EXP-003 |
| Intercept BGFX | ❌ Not implemented | Deferred to EXP-003 |
| Load PVR renderer | ❌ Not implemented | Deferred to EXP-004 |
| Modify Minecraft rendering | ❌ Not implemented | Deferred to EXP-005/006 |
| Depend on Minecraft libs | ❌ Not implemented | Bridge is independent |
| Claim full integration | ❌ Not done | Only proves loading |

## Build Artifacts

### Primary Artifact

**`libpvr_minecraft_bridge.so`** is the expected output (ARM64 shared library).
Its existence and architecture must be verified from an actual build; the
repository does not commit a generated binary.

### Verification

```bash
# Check format
file libpvr_minecraft_bridge.so
# Output: ELF 64-bit LSB shared object, ARM aarch64, ...

# Check dependencies
readelf -d libpvr_minecraft_bridge.so | grep NEEDED
# Output: liblog.so, libc.so, libm.so (all Android standard)

# Check symbols (should be clean)
nm libpvr_minecraft_bridge.so | grep -i render
# Output: (should be empty)
```

## Runtime Behavior

### Stage 1: Library Load (Automatic)

When Minecraft's Java process loads the bridge via dlopen or patchelf:

1. Android dynamic linker resolves dependencies (liblog.so)
2. Dynamic linker calls all `__attribute__((constructor))` functions
3. Our `pvr_bridge_constructor()` runs
4. Logging is initialized
5. Context (PID, TID) is recorded
6. Marker is logged to logcat

### Stage 2: Optional Explicit Call

```c
/* If called from JNI or another context */
int result = bridge_initialize();
```

This is optional; the constructor runs automatically.

## Evidence Collection Strategy

### BUILD CONFIGURATION (Source Review)

The source and CMake configuration define:
- a shared-library target named `pvr_minecraft_bridge`;
- Android `liblog.so` linkage;
- constructor-based initialization and logging;
- no RenderDragon/BGFX hooks or Minecraft proprietary dependencies.

These are source/configuration findings only. They do not prove that CMake,
the Android NDK, or the linker has successfully produced an ARM64 artifact.

### TESTED (Runtime Test on Device)

⏳ Requires runtime execution on target device:
1. Minecraft Bedrock is installed and recorded
2. Modified APK (with bridge .so) is prepared
3. Modified APK is installed on device
4. Minecraft is launched
5. `adb logcat | grep "PVR-Bridge"` returns the initialization marker
6. Context (PID, TID) confirms the bridge ran in Minecraft's process
7. All logs are recorded in the experiment log

## Dependency Analysis

### Build-Time Dependencies

- CMake 3.22+ (build tool)
- Android NDK r25+ (toolchain)
- liblog.so (Android standard library, included in NDK)

### Runtime Dependencies

- Android 13+ (or compatible)
- liblog.so (provided by Android)
- Minecraft Bedrock Edition (target process)

### No Dependencies On

- PVR (not loaded yet)
- RenderDragon code (not accessed)
- BGFX (not hooked)
- Minecraft's native libraries (not linked statically)

## Thread Safety

The bridge initialization runs in the thread that loads the .so (typically the dynamic linker thread). Logging calls use Android's thread-safe liblog API.

```c
bridge_log_info("Thread ID (TID): %d", (int)gettid());
```

Each thread gets its own logcat entry with proper synchronization.

## Memory Safety

The code follows standard C best practices:

- No buffer overflows (format strings are controlled)
- No use-after-free (logging outlives initialization)
- No uninitialized variables (all initialized before use)
- No global mutable state (logging uses Android's thread-local storage)

## Extensibility

The architecture is designed to be extended without breaking EXP-001:

- **EXP-002**: Add target detection (inspect process maps, resolve symbols)
- **EXP-003**: Add frame-observation hooks (non-intrusive)
- **EXP-004**: Add PVR component loading (separate from bridge)
- **EXP-005+**: Add rendering integration (incremental)

Each experiment builds on EXP-001's proven bridge loading without modifying the core initialization.

## Reversibility

To remove the bridge:

1. If using patchelf: rebuild the APK without the patchelf step
2. If using a forwarder (KafkaLauncher): remove the forwarder library
3. Original Minecraft APK is restored; no system-level changes

## References

- [Android Native Debugging](https://developer.android.com/ndk/guides/native-debugging)
- [ELF and Program Loading](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)
- [liblog API Documentation](https://developer.android.com/ndk/reference/group/logging)

---

**Architecture Version**: 1.0
**EXP-001 Phase**: Implementation
**Status**: BUILD CONFIGURATION FIXED — BUILD NOT VERIFIED
