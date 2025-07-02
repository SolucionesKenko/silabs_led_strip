#ifndef USART_DRV_H
#define USART_DRV_H

#include "em_usart.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Config struct for USART
typedef struct {
    uint32_t baudrate;
    uint32_t databits; // 8 or 9
    uint32_t parity;   // 0: none, 1: odd, 2: even
    uint32_t stopbits; // 1 or 2
} usart_drv_config_t;

// Initialize a USART in async (UART) mode
void usart_drv_init_async(USART_TypeDef *usart, const usart_drv_config_t *cfg);

// Transmit a single byte (blocking)
void usart_drv_tx(USART_TypeDef *usart, uint8_t data);

// Transmit 'HI' message
void usart_drv_hello(USART_TypeDef *usart);

// Receive a single byte (blocking)
uint8_t usart_drv_rx(USART_TypeDef *usart);

// Example: Add more functions for string transmit, etc. if needed

#ifdef __cplusplus
}
#endif

#endif // USART_DRV_H 
