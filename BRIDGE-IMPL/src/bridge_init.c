/*
 * bridge_init.c
 *
 * PVR Minecraft Research — EXP-001 Native Bridge
 *
 * Implementation of the initialization marker for the native bridge.
 * This file is called automatically when the .so is loaded by Android.
 */

#include "bridge_init.h"
#include "bridge_log.h"

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

/**
 * __attribute__((constructor)) — Automatic initialization
 *
 * This function is called by the dynamic linker automatically when
 * the .so is loaded into the process, before JNI_OnLoad or main().
 *
 * Semantics:
 * - Runs in the thread that triggered the load
 * - Runs before any user code in the application
 * - If it crashes or returns non-zero, the .so load fails
 *
 * For EXP-001, this is the initialization marker.
 */
__attribute__((constructor))
static void pvr_bridge_constructor(void)
{
    /* Initialize logging first */
    bridge_log_init();

    /* Record the initialization context */
    pid_t pid = getpid();
    pid_t tid = gettid();

    bridge_log_info("=========================================");
    bridge_log_info("PVR Minecraft Bridge — EXP-001");
    bridge_log_info("=========================================");
    bridge_log_info("Bridge constructor running");
    bridge_log_info("Process ID (PID): %d", (int)pid);
    bridge_log_info("Thread ID (TID): %d", (int)tid);

    /* Log the initialization marker — this is observable at runtime */
    bridge_log_marker("PVR_BRIDGE_LOADED");

    bridge_log_info("Bridge initialization complete");
    bridge_log_info("=========================================");
}

/**
 * bridge_initialize()
 *
 * Explicit initialization function.
 * Can be called from Java/JNI if needed, but the constructor above
 * should have already run.
 */
int bridge_initialize(void)
{
    bridge_log_info("bridge_initialize() called explicitly");
    return 0;
}
