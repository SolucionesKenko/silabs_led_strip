#ifndef BG9X_APP_H
#define BG9X_APP_H

#include <stdint.h>
#include "sl_status.h"
#include "bg9x.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bg9x_app_s bg9x_app_t;

// Create/init BG9x app module (wraps bg9x)
bg9x_app_t *bg9x_app_create(const bg9x_at_config_t *at_cfg);

// Destroy/free BG9x app module
void bg9x_app_destroy(bg9x_app_t *app);

// To be called in main loop to process BG9x app state machines
void bg9x_app_process(bg9x_app_t *app);

// Non-blocking network registration (returns IN_PROGRESS, OK, or error)
sl_status_t bg9x_app_network_register(bg9x_app_t *app);

// (future) Add more APIs: GNSS, TCP, etc.

#ifdef __cplusplus
}
#endif

#endif // BG9X_APP_H
