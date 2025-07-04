#ifndef BG9X_AT_H
#define BG9X_AT_H

#include <stdint.h>
#include <stdbool.h>
#include "em_usart.h"
#include "sl_sleeptimer.h"
#include "helpers/fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handle for BG9x driver
typedef struct bg9x_at_s bg9x_at_t;

// Callback for AT command completion (non-blocking)
typedef void (*bg9x_at_cmd_cb_t)(bool success, const char *response, void *user_ctx);

// Abstract FIFO interface for DI/IoC
// User must provide implementation for these functions
typedef struct {
    uint8_t (*put)(FIFO_HandleTypeDef *fifo, char data);
    uint8_t (*get)(FIFO_HandleTypeDef *fifo, char *data);
    FIFO_HandleTypeDef *fifo;
} bg9x_at_fifo_if_t;

// BG9x driver configuration for DI/IoC
typedef struct {
    USART_TypeDef *usart_hw; // USART peripheral (already initialized by app)
    bg9x_at_fifo_if_t rx_fifo; // RX FIFO interface
    void (*usart_tx_fn)(USART_TypeDef *usart, uint8_t data); // TX function (provided by app)
} bg9x_at_config_t;

// Create/init BG9x AT driver instance
// Returns pointer to driver handle, or NULL on error
bg9x_at_t *bg9x_at_create(const bg9x_at_config_t *cfg);

// Destroy/free BG9x AT driver instance
void bg9x_at_destroy(bg9x_at_t *drv);

// To be called from USART RX handler: pushes RX byte to queue
void bg9x_at_rx_byte(bg9x_at_t *drv, uint8_t byte);

// Send AT command (non-blocking): starts timer, sends command, returns immediately
// Calls cb when response is received or timeout occurs
void bg9x_at_send_cmd(bg9x_at_t *drv,
                      const char *cmd,
                      const char *success_str,
                      uint32_t timeout_ms,
                      char *response_buf,
                      uint16_t response_buf_size,
                      bg9x_at_cmd_cb_t cb,
                      void *user_ctx);

// To be called in main loop: processes RX queue, parses response, checks timeout, calls cb if done
void bg9x_at_process(bg9x_at_t *drv);

#ifdef __cplusplus
}
#endif

#endif // BG9X_AT_H 
