#!/system/bin/sh
# EXP-002 — Observation-only Android target inventory
# Run from Termux. Requires the existing Shizuku rish helper at ~/rish.
# This script does not modify Minecraft, inject code, attach a debugger, or bypass permissions.

set -u

PKG="com.mojang.minecraftpe"
OUT="${HOME}/pvr-exp-002-observation-$(date +%Y%m%d-%H%M%S).txt"
RISH="${HOME}/rish"

if [ ! -x "$RISH" ]; then
    echo "ERROR: $RISH not found or not executable."
    exit 1
fi

run_rish() {
    "$RISH" -c "$1"
}

{
    echo "EXP-002 OBSERVATION-ONLY TARGET INVENTORY"
    echo "timestamp=$(date -Iseconds 2>/dev/null || date)"
    echo "package=$PKG"
    echo

    echo "=== PACKAGE / APK PATH ==="
    run_rish "pm path $PKG"
    echo

    echo "=== PACKAGE INFO (SELECTED) ==="
    run_rish "dumpsys package $PKG | grep -E 'versionName=|versionCode=|targetSdk=|primaryCpuAbi=|secondaryCpuAbi=|codePath=|resourcePath='"
    echo

    echo "=== PROCESS IDENTITY ==="
    # pidof can be unavailable/inconsistent through the available rish shell context.
    # Resolve the package PID from the same read-only process table used below.
    PID="$(run_rish "ps -A -o USER,PID,PPID,NAME 2>/dev/null | grep -E '[[:space:]]${PKG}$' | head -n 1" 2>/dev/null | sed 's/^ *//' | cut -d ' ' -f 2 | tr -d '\r')"
    echo "pid=${PID:-NOT_RUNNING}"
    if [ -n "${PID:-}" ]; then
        run_rish "ps -A -o USER,PID,PPID,NAME | grep -E '(^| )${PID} '"
        echo
        echo "--- /proc status ---"
        run_rish "cat /proc/${PID}/status 2>&1 | grep -E '^(Name|State|Pid|PPid|Uid|Gid|Threads):'"
        echo
        echo "--- /proc cmdline ---"
        run_rish "cat /proc/${PID}/cmdline 2>&1 | tr '\0' ' '; echo"
        echo
        echo "--- /proc maps (read-only probe) ---"
        run_rish "cat /proc/${PID}/maps 2>&1 | head -n 40"
        echo
        echo "--- /proc exe (read-only probe) ---"
        run_rish "readlink /proc/${PID}/exe 2>&1"
    fi
    echo

    echo "=== NATIVE LIBRARY DIRECTORY ==="
    NATIVE_DIR="$(run_rish "pm path --user 0 $PKG" 2>/dev/null | sed -n 's/^package:\(.*\)/\1/p' | head -n 1)"
    echo "base_apk=${NATIVE_DIR:-UNKNOWN}"
    run_rish "cmd package path $PKG 2>&1"
    echo

    echo "=== ABI / CPU ==="
    run_rish "getprop ro.product.cpu.abilist"
    run_rish "getprop ro.product.cpu.abi"
    run_rish "getprop ro.product.cpu.abilist64"
    echo

    echo "=== GPU / RENDERER PROPERTIES ==="
    run_rish "getprop | grep -Ei 'gpu|gles|vulkan|renderer|egl'"
    echo

    echo "=== SURFACEFLINGER / DISPLAY OBSERVATION ==="
    run_rish "dumpsys SurfaceFlinger 2>&1 | grep -Ei 'GLES|Vulkan|EGL|GPU|RenderEngine|Display|refresh-rate' | head -n 120"
    echo

    echo "=== RUNNING MINECRAFT-RELATED PROCESSES ==="
    run_rish "ps -A -o USER,PID,PPID,NAME 2>&1 | grep -Ei 'minecraft|mojang|com.mojang' | head -n 80"
    echo

    echo "=== EXP-002 CLASSIFICATION ==="
    echo "Direct /proc maps or exe access may be denied by Android process isolation."
    echo "Do not interpret a module name alone as a RenderDragon submission point."
    echo "No target modification or security-boundary bypass was performed by this script."
} | tee "$OUT"

echo
echo "Observation saved to: $OUT"
