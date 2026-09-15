# EXP-001 Runtime Loading Test

## Objective

Determine whether the controlled native bridge can be loaded and initialized inside the target Minecraft Bedrock Android process. The success marker is `PVR_BRIDGE_LOADED`, and the evidence must demonstrate that the marker came from the target Minecraft process rather than merely from a standalone Android application.

This experiment does not attempt RenderDragon hooking, BGFX interception, renderer replacement, GPU modification, or gameplay integration.

## Current Artifact

The current build artifact was produced by GitHub Actions run [34976852843](https://github.com/Maouuusamaaa/PVR-Minecraft-Research/actions/runs/34976852843).

| Field | Value |
|---|---|
| Filename | `libpvr_minecraft_bridge.so` |
| ABI | `arm64-v8a` |
| Android API target | `android-33` |
| Size | `13024` bytes |
| SHA-256 | `8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73` |
| ELF | 64-bit AArch64 shared object |
| Runtime status | **NOT TESTED** |

The artifact is available from the GitHub Actions artifact named `exp-001-android-arm64`. The artifact must be downloaded and hashed again before a device test; a copied filename or embedded marker string is not runtime evidence.

## Preconditions

The experiment requires an authorized, user-controlled Android test environment. The following values must be recorded before the test and must not be guessed:

| Field | Value to record |
|---|---|
| Device | `<actual device model>` |
| Android version | `<actual Android version/build>` |
| Minecraft Bedrock version | `<actual installed version>` |
| ABI | `<actual process ABI; expected arm64-v8a>` |
| Package name | `<actual package name>` |
| Minecraft PID | `<record after launch>` |
| Loading method | `<authorized method actually used>` |
| Library path | `<actual path used by the loader>` |
| Test timestamp | `<UTC timestamp>` |
| Log source | `<adb logcat capture path/identifier>` |

Required tools include `adb`, the Android SDK platform tools, the verified artifact, and a method that is legitimately available in the controlled test environment. No exploit, credential theft, security bypass, stealth persistence, or unauthorized process injection is part of this procedure.

## Target Device

No target device is connected to the current audit environment. The following are therefore currently unknown:

- Device model and build.
- Android version.
- Minecraft package name and version.
- Target process PID.
- Whether an authorized loading method is available for that installation.

## Target Minecraft

The target must be a user-controlled test installation for which the loading method is authorized and reversible. The exact Minecraft version and package name must be captured from the device rather than inferred from public projects or documentation.

The repository must not receive a proprietary Minecraft APK, `libminecraftpe.so`, extracted native library, copyrighted asset, or modified production package.

## Loading Method

### Controlled Android test harness

A minimal Android test harness that loads `libpvr_minecraft_bridge.so` is a valid optional control experiment. It can demonstrate that the Android dynamic linker loads the bridge and that the constructor emits `PVR_BRIDGE_LOADED` in a process controlled by the researcher.

It does **not** demonstrate Minecraft loading. A harness result must be classified as:

```text
Android native loading = TESTED
Minecraft loading = NOT TESTED
```

### Minecraft process loading

The repository currently contains no legitimate, reproducible, target-device loader that places the bridge into the installed Minecraft process without relying on a separately authorized user-controlled setup. Public documentation about dependency injection or launcher architectures is architectural reference only; it is not evidence for the installed target version.

Therefore the Minecraft loading method is currently:

```text
MANUAL / BLOCKED pending an authorized controlled loading method
```

The test must not modify or redistribute proprietary Minecraft binaries as part of this repository task. If the user has an authorized lab setup that can load the bridge into the target process, the exact method, version, and reversibility steps must be recorded here before execution.

## Exact Procedure

### 1. Verify the artifact

Download the GitHub Actions artifact and verify its hash:

```bash
sha256sum libpvr_minecraft_bridge.so
```

Expected SHA-256 for the current artifact:

```text
8a986f71e7a670f8f0141c07a8330174b3e1fb6e6d7eddf47b91bbd639826a73
```

Record the actual output and artifact source. Do not continue if the hash differs without recording a new build identity.

### 2. Prepare log capture

Clear or mark the log boundary, then start a capture that preserves timestamps and process identity:

```bash
adb logcat -c
adb logcat -v threadtime -s PVR-Bridge:* '*:S' > exp-001-logcat.txt
```

Keep the capture running during the loading and launch steps.

### 3. Identify the target process

Launch the authorized test target and record the exact package and PID:

```bash
adb shell pm list packages | grep -i minecraft
adb shell pidof <actual.package.name>
adb shell getprop ro.build.version.release
adb shell getprop ro.product.model
adb shell getprop ro.product.cpu.abilist
```

The PID must be recorded after launch and before interpreting any marker.

### 4. Perform the authorized loading action

Use only the authorized, reversible loading method recorded in the `Loading method` field. Do not use exploit chains, security bypasses, credential access, stealth techniques, or unapproved injection into a process that is not controlled by the researcher.

If no such method is available, stop and classify the Minecraft runtime experiment as `BLOCKED`/`NOT TESTED`. Do not substitute a test harness result for Minecraft evidence.

### 5. Launch or restart Minecraft

Launch the target using the documented test procedure, then obtain the PID again:

```bash
adb shell monkey -p <actual.package.name> 1
adb shell pidof <actual.package.name>
```

Record the timestamp and PID from the same test window as the log capture.

### 6. Collect and filter evidence

After the process has had time to initialize, stop the capture and inspect it:

```bash
grep -F 'PVR_BRIDGE_LOADED' exp-001-logcat.txt
grep -F 'Process ID (PID):' exp-001-logcat.txt
grep -F 'Thread ID (TID):' exp-001-logcat.txt
```

The accepted evidence must show, in the same controlled test window:

1. `PVR_BRIDGE_LOADED` appears.
2. The PID/TID lines are present.
3. The reported PID matches the actual Minecraft PID.
4. The package/process identity matches the target Minecraft installation.
5. The artifact hash, device, Minecraft version, Android version, ABI, loading method, and timestamp are recorded.

A marker from a standalone harness, a string found in the `.so`, or a log line without process correlation is insufficient.

## Evidence Collection

A completed runtime record should include:

- Artifact path and SHA-256.
- Exact GitHub Actions run and source commit.
- Device model and Android build.
- Minecraft package and exact version.
- ABI and process name.
- PID and TID.
- Authorized loading method and library path.
- Timestamp with timezone or UTC.
- Unedited logcat excerpt containing the marker.
- Command output used to identify the process.
- Failure output if initialization did not occur.

Do not commit private device identifiers, account information, proprietary APKs, native libraries, or unrelated personal logs.

## Success Criteria

The result can be classified as `TESTED` and `CONFIRMED` for runtime loading only when a controlled, reproducible run demonstrates that:

```text
PVR_BRIDGE_LOADED
```

was emitted by the bridge constructor in the target Minecraft process, and the PID/process identity correlation is preserved in the evidence.

This success criterion proves native bridge initialization in the target process only. It does not prove RenderDragon reachability, BGFX interception, renderer modification, PVR execution, or GPU work.

## Failure Criteria

Classify the run as `FAILED` if an authorized loading attempt was actually performed and the bridge failed to load or initialize with reproducible error evidence.

Classify it as `BLOCKED` if the test cannot be performed because the required device, exact target information, or authorized loading mechanism is unavailable.

Classify it as `NOT TESTED` when no target-device attempt has been executed.

Do not classify a build-only result as runtime success.

## Result

```text
NOT TESTED
```

No target Android device is connected to the audit environment, no Minecraft PID is available, and no Minecraft logcat evidence has been collected.

## Evidence

Current evidence is limited to the verified Android ARM64 build artifact and GitHub Actions build logs. There is currently no valid evidence of `PVR_BRIDGE_LOADED` originating from a Minecraft process.

## Limitations

- Android device access is not available in the current environment.
- `adb` is not available in the current environment.
- Minecraft version, package name, PID, and process ABI are unknown.
- An authorized, reproducible loading method for the target installation has not been established in this repository.
- The bridge constructor demonstrates intended behavior after a library is loaded; it does not itself cause the library to be loaded into Minecraft.
- A controlled Android test harness, if added later, will validate Android native loading only and will not complete the Minecraft loading experiment.

## Next Step

Obtain an authorized user-controlled Android test environment and document the exact Minecraft package/version and reversible loading method before execution. If no such method is available, preserve the result as `BLOCKED`/`NOT TESTED`; do not proceed to EXP-002 or claim EXP-001 completion.
