#include "bg9x_at.h"
#include <string.h>
#include <stdlib.h>
#include "helpers/fifo.h"

// Internal state for a pending AT command
typedef struct {
    const char *cmd;
    const char *success_str;
    uint32_t timeout_ms;
    char *response_buf;
    uint16_t response_buf_size;
    uint16_t resp_idx;
    bg9x_at_cmd_cb_t cb;
    void *user_ctx;
    bool active;
    uint32_t start_tick;
    bool done;
    bool success;
} bg9x_at_cmd_state_t;

struct bg9x_at_s {
    USART_TypeDef *usart_hw;
    void (*usart_tx_fn)(USART_TypeDef *usart, uint8_t data);
    bg9x_at_fifo_if_t rx_fifo;
    bg9x_at_cmd_state_t cmd_state;
};

bg9x_at_t *bg9x_at_create(const bg9x_at_config_t *cfg) {
    if (!cfg || !cfg->usart_hw || !cfg->usart_tx_fn || !cfg->rx_fifo.put || !cfg->rx_fifo.get) {
        return NULL;
    }
    bg9x_at_t *drv = (bg9x_at_t *)calloc(1, sizeof(bg9x_at_t));
    if (!drv) return NULL;
    drv->usart_hw = cfg->usart_hw;
    drv->usart_tx_fn = cfg->usart_tx_fn;
    drv->rx_fifo = cfg->rx_fifo;
    memset(&drv->cmd_state, 0, sizeof(drv->cmd_state));
    return drv;
}

void bg9x_at_destroy(bg9x_at_t *drv) {
    if (drv) {
        free(drv);
    }
}

// To be called from USART RX handler
void bg9x_at_rx_byte(bg9x_at_t *drv, uint8_t byte) {
    if (!drv) return;
    drv->rx_fifo.put(drv->rx_fifo.fifo, (char)byte);
}

// Non-blocking send: start timer, send command, set up state
void bg9x_at_send_cmd(bg9x_at_t *drv,
                      const char *cmd,
                      const char *success_str,
                      uint32_t timeout_ms,
                      char *response_buf,
                      uint16_t response_buf_size,
                      bg9x_at_cmd_cb_t cb,
                      void *user_ctx) {
    if (!drv || !cmd || !success_str || drv->cmd_state.active) return;
    drv->cmd_state.cmd = cmd;
    drv->cmd_state.success_str = success_str;
    drv->cmd_state.timeout_ms = timeout_ms;
    drv->cmd_state.response_buf = response_buf;
    drv->cmd_state.response_buf_size = response_buf_size;
    drv->cmd_state.resp_idx = 0;
    drv->cmd_state.cb = cb;
    drv->cmd_state.user_ctx = user_ctx;
    drv->cmd_state.active = true;
    drv->cmd_state.done = false;
    drv->cmd_state.success = false;
    drv->cmd_state.start_tick = sl_sleeptimer_get_tick_count();
    // Flush response buffer
    if (response_buf && response_buf_size > 0) response_buf[0] = '\0';
    // Send command string (with CRLF)
    for (const char *p = cmd; *p; ++p) {
        drv->usart_tx_fn(drv->usart_hw, (uint8_t)*p);
    }
    drv->usart_tx_fn(drv->usart_hw, '\r');
    drv->usart_tx_fn(drv->usart_hw, '\n');
}

// To be called in main loop
typedef enum { BG9X_AT_IDLE, BG9X_AT_WAITING } bg9x_at_process_state_t;

void bg9x_at_process(bg9x_at_t *drv) {
    if (!drv || !drv->cmd_state.active || drv->cmd_state.done) return;
    bg9x_at_cmd_state_t *cs = &drv->cmd_state;
    // Check timeout
    uint32_t now_tick = sl_sleeptimer_get_tick_count();
    uint32_t timeout_tick = sl_sleeptimer_ms_to_tick((uint16_t)cs->timeout_ms);
    if ((now_tick - cs->start_tick) > timeout_tick) {
        cs->done = true;
        cs->success = false;
        if (cs->cb) cs->cb(false, cs->response_buf, cs->user_ctx);
        cs->active = false;
        return;
    }
    // Read available bytes from FIFO
    char b;
    while (drv->rx_fifo.get(drv->rx_fifo.fifo, &b) == 0) {
        if (cs->response_buf && cs->resp_idx < (cs->response_buf_size - 1)) {
            cs->response_buf[cs->resp_idx++] = b;
            cs->response_buf[cs->resp_idx] = '\0';
        }
        // Check for success string in buffer
        if (cs->response_buf && cs->resp_idx >= strlen(cs->success_str)) {
            if (strstr(cs->response_buf, cs->success_str)) {
                cs->done = true;
                cs->success = true;
                if (cs->cb) cs->cb(true, cs->response_buf, cs->user_ctx);
                cs->active = false;
                return;
            }
        } else if (!cs->response_buf) {
            // If not collecting response, use a small rolling window
            static char window[16];
            static uint8_t win_idx = 0;
            window[win_idx++ % sizeof(window)] = b;
            window[sizeof(window)-1] = '\0';
            if (strstr(window, cs->success_str)) {
                cs->done = true;
                cs->success = true;
                if (cs->cb) cs->cb(true, NULL, cs->user_ctx);
                cs->active = false;
                return;
            }
        }
    }
} 