#include "bg9x_app.h"
#include <string.h>
#include <stdlib.h>

struct bg9x_app_s {
    bg9x_t *bg9x;
    // Network registration state
    enum {
        BG9X_APP_NETREG_IDLE = 0,
        BG9X_APP_NETREG_START,
        BG9X_APP_NETREG_CMD,
        BG9X_APP_NETREG_DONE,
        BG9X_APP_NETREG_ERROR
    } netreg_state;
    bool netreg_started;
    bool netreg_reported;
};

// Network registration AT command sequence
typedef struct {
    const char *cmd;
    const char *success_str;
    uint32_t timeout_ms;
} bg9x_app_cmd_step_t;




bg9x_app_t *bg9x_app_create(const bg9x_at_config_t *at_cfg)
{
    if (!at_cfg) return NULL;
    bg9x_app_t *app = (bg9x_app_t *)calloc(1, sizeof(bg9x_app_t));
    if (!app) return NULL;
    app->bg9x = bg9x_create(at_cfg);
    app->netreg_state = BG9X_APP_NETREG_IDLE;
    app->netreg_started = false;
    app->netreg_reported = false;
    return app;
}

void bg9x_app_destroy(bg9x_app_t *app)
{
    if (app) {
        if (app->bg9x) bg9x_destroy(app->bg9x);
        free(app);
    }
}

void bg9x_app_process(bg9x_app_t *app)
{
    if (!app || !app->bg9x) return;
    bg9x_process(app->bg9x);
}

sl_status_t bg9x_app_network_register(bg9x_app_t *app)
{
    if (!app || !app->bg9x) return SL_STATUS_INVALID_PARAMETER;
    return bg9x_network_register(app->bg9x);
} 
