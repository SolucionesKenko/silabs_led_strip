#ifndef BG9X_H
#define BG9X_H

#include <stdint.h>
#include "sl_status.h"
#include "bg9x_at.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bg9x_s bg9x_t;

// Create/init BG9x high-level driver
bg9x_t *bg9x_create(const bg9x_at_config_t *at_cfg);

// Destroy/free BG9x driver
void bg9x_destroy(bg9x_t *bg);

// To be called in main loop to process BG9x state machines
void bg9x_process(bg9x_t *bg);

// Non-blocking network registration (returns IN_PROGRESS, OK, or error)
sl_status_t bg9x_network_register(bg9x_t *bg);

#ifdef __cplusplus
}
#endif

#endif // BG9X_H 