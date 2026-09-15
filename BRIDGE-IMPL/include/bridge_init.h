/*
 * bridge_init.h
 *
 * PVR Minecraft Research — EXP-001 Native Bridge
 *
 * Minimal initialization marker for controlled research.
 * This header declares the initialization entry point that will be
 * called when the bridge is first loaded into the Minecraft process.
 *
 * No RenderDragon hooks, BGFX interception, or renderer code here.
 * Research-only usage.
 */

#ifndef PVR_BRIDGE_INIT_H
#define PVR_BRIDGE_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bridge_initialize()
 *
 * Called automatically by the Android runtime during .so loading.
 * This is the initialization marker for EXP-001.
 *
 * On success: logs a controlled initialization event that can be observed
 * at runtime to prove the bridge was loaded.
 *
 * Does NOT:
 * - Hook any RenderDragon or BGFX functions
 * - Attempt to interact with Minecraft rendering
 * - Require any Minecraft libraries or symbols
 * - Cause any rendering modifications
 *
 * Expected behavior:
 * - Initialize logging infrastructure
 * - Record process/thread context
 * - Write initialization marker to logcat (observable via 'adb logcat')
 * - Return successfully
 *
 * Return: 0 on success
 */
int bridge_initialize(void);

#ifdef __cplusplus
}
#endif

#endif // PVR_BRIDGE_INIT_H
