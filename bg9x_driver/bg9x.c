#include "bg9x.h"
#include <string.h>
#include <stdlib.h>

// Internal state for network registration
typedef enum {
    BG9X_NETREG_IDLE = 0,
    BG9X_NETREG_START,
    BG9X_NETREG_CMD,
    BG9X_NETREG_DONE,
    BG9X_NETREG_ERROR
} bg9x_netreg_state_t;

struct bg9x_s {
    bg9x_at_t *at_drv;
    bg9x_at_config_t at_cfg;
    // Network registration state
    bg9x_netreg_state_t netreg_state;
    uint8_t netreg_cmd_idx;
    sl_status_t netreg_result;
    bool netreg_in_progress;
};

// Network registration AT command sequence
typedef struct {
    const char *cmd;
    const char *success_str;
    uint32_t timeout_ms;
} bg9x_cmd_step_t;

static const bg9x_cmd_step_t netreg_cmds[] = {
    { "AT+CFUN=0", "OK", 2000 },
    { "AT+QCFG=\"nwscanseq\",02,1", "OK", 2000 },
    { "AT+QCFG=\"band\",0,100002000000000F0E189F,10004200000000090E189F", "OK", 2000 },
    { "AT+QCFG=\"iotopmode\",0,1", "OK", 2000 },
    { "AT+QCFG=\"servicedomain\",1,1", "OK", 2000 },
    { "AT+CFUN=1", "OK", 2000 },
    { "AT+CREG=1", "OK", 2000 },
    { "AT+QICSGP=1,1,\"hologram\",\"\",\"\",1", "OK", 2000 },
};
#define NETREG_CMD_COUNT (sizeof(netreg_cmds)/sizeof(netreg_cmds[0]))

static void netreg_cmd_cb(bool success, const char *response, void *user_ctx)
{
    bg9x_t *bg = (bg9x_t *)user_ctx;
    if (!bg || !bg->netreg_in_progress) return;
    if (success) {
        bg->netreg_cmd_idx++;
        if (bg->netreg_cmd_idx >= NETREG_CMD_COUNT) {
            bg->netreg_state = BG9X_NETREG_DONE;
            bg->netreg_result = SL_STATUS_OK;
            bg->netreg_in_progress = false;
        } else {
            bg->netreg_state = BG9X_NETREG_CMD; // Next command
        }
    } else {
        bg->netreg_state = BG9X_NETREG_ERROR;
        bg->netreg_result = SL_STATUS_FAIL;
        bg->netreg_in_progress = false;
    }
}

bg9x_t *bg9x_create(const bg9x_at_config_t *at_cfg)
{
    if (!at_cfg) return NULL;
    bg9x_t *bg = (bg9x_t *)calloc(1, sizeof(bg9x_t));
    if (!bg) return NULL;
    bg->at_cfg = *at_cfg;
    bg->at_drv = bg9x_at_create(&bg->at_cfg);
    bg->netreg_state = BG9X_NETREG_IDLE;
    bg->netreg_cmd_idx = 0;
    bg->netreg_result = SL_STATUS_OK;
    bg->netreg_in_progress = false;
    return bg;
}

void bg9x_destroy(bg9x_t *bg)
{
    if (bg) {
        if (bg->at_drv) bg9x_at_destroy(bg->at_drv);
        free(bg);
    }
}

void bg9x_process(bg9x_t *bg)
{
    if (!bg) return;
    bg9x_at_process(bg->at_drv);
    // Network registration state machine
    if (bg->netreg_in_progress) {
        if (bg->netreg_state == BG9X_NETREG_CMD) {
            // Send next command
            const bg9x_cmd_step_t *step = &netreg_cmds[bg->netreg_cmd_idx];
            bg9x_at_send_cmd(bg->at_drv,
                             step->cmd,
                             step->success_str,
                             step->timeout_ms,
                             NULL, 0,
                             netreg_cmd_cb,
                             bg);
            bg->netreg_state = BG9X_NETREG_START; // Wait for callback
        }
    }
}

sl_status_t bg9x_network_register(bg9x_t *bg)
{
    if (!bg) return SL_STATUS_INVALID_PARAMETER;
    if (!bg->netreg_in_progress && bg->netreg_state != BG9X_NETREG_DONE) {
        // Start registration sequence
        bg->netreg_cmd_idx = 0;
        bg->netreg_state = BG9X_NETREG_CMD;
        bg->netreg_result = SL_STATUS_IN_PROGRESS;
        bg->netreg_in_progress = true;
    }
    if (bg->netreg_state == BG9X_NETREG_DONE) {
        return SL_STATUS_OK;
    } else if (bg->netreg_state == BG9X_NETREG_ERROR) {
        return SL_STATUS_FAIL;
    } else {
        return SL_STATUS_IN_PROGRESS;
    }
} 