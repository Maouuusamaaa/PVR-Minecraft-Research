/*
 * check_headers.c
 *
 * Minimal compile-time test to verify headers are well-formed.
 */

#include "bridge_init.h"
#include "bridge_log.h"

int main(void)
{
    /* Just verify headers compile and symbols are accessible */
    bridge_log_init();
    bridge_log_info("Header check passed");
    bridge_initialize();
    return 0;
}
