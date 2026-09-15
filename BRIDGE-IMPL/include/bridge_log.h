/*
 * bridge_log.h
 *
 * PVR Minecraft Research — EXP-001 Native Bridge
 *
 * Minimal logging infrastructure for controlled observation of bridge state.
 * Uses Android's native logging (liblog) to record events to logcat.
 */

#ifndef PVR_BRIDGE_LOG_H
#define PVR_BRIDGE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bridge_log_init()
 *
 * Initialize logging infrastructure.
 * Sets up the logging tag and context.
 *
 * Must be called before any other logging calls.
 */
void bridge_log_init(void);

/**
 * bridge_log_info(const char *format, ...)
 *
 * Log an informational message.
 * Format follows standard printf conventions.
 *
 * Example:
 *   bridge_log_info("Bridge initialized in process %d, thread %d", pid, tid);
 */
void bridge_log_info(const char *format, ...);

/**
 * bridge_log_error(const char *format, ...)
 *
 * Log an error message.
 */
void bridge_log_error(const char *format, ...);

/**
 * bridge_log_marker(const char *marker_name)
 *
 * Log a distinct initialization marker.
 * Used to prove the bridge was loaded.
 *
 * Example:
 *   bridge_log_marker("PVR_BRIDGE_INIT");
 *   // Outputs to logcat: "[PVR-Bridge] MARKER: PVR_BRIDGE_INIT"
 */
void bridge_log_marker(const char *marker_name);

#ifdef __cplusplus
}
#endif

#endif // PVR_BRIDGE_LOG_H
