/*
 * bridge_log.c
 *
 * PVR Minecraft Research — EXP-001 Native Bridge
 *
 * Implementation of the logging infrastructure.
 * Uses Android's native logging (liblog.so) to write to logcat.
 */

#include "bridge_log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef ANDROID
#include <android/log.h>
#else
#include <stdio.h>
#endif

#define LOG_TAG "PVR-Bridge"

/**
 * bridge_log_init()
 *
 * Initialize logging.
 * On Android, this is a no-op (liblog is always available).
 */
void bridge_log_init(void)
{
#ifdef ANDROID
    /* Android's liblog is ready after the constructor runs. */
#else
    /* On non-Android platforms, we'd use fprintf. */
#endif
}

/**
 * bridge_log_info()
 *
 * Log an informational message using Android's native logging.
 */
void bridge_log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);

#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, args);
#else
    fprintf(stderr, "[%s] INFO: ", LOG_TAG);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
#endif

    va_end(args);
}

/**
 * bridge_log_error()
 *
 * Log an error message using Android's native logging.
 */
void bridge_log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);

#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, args);
#else
    fprintf(stderr, "[%s] ERROR: ", LOG_TAG);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
#endif

    va_end(args);
}

/**
 * bridge_log_marker()
 *
 * Log a distinct initialization marker.
 * Used to prove the bridge was loaded.
 *
 * Format: "[PVR-Bridge] MARKER: <marker_name>"
 */
void bridge_log_marker(const char *marker_name)
{
    bridge_log_info("MARKER: %s", marker_name);
}
