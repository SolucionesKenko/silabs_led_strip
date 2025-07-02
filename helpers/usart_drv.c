#include "usart_drv.h"
#include "em_cmu.h"
#include "em_gpio.h"

// NOTE: Pin configuration must be set in app.c or board-specific code

void usart_drv_init_async(USART_TypeDef *usart, const usart_drv_config_t *cfg) {
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    init.baudrate = cfg->baudrate;
    // TODO: Map databits, parity, stopbits if needed
    USART_InitAsync(usart, &init);
    USART_Enable(usart, usartEnable);
}

void usart_drv_tx(USART_TypeDef *usart, uint8_t data) {
    // Wait for transmit buffer to be empty
    while (!(usart->STATUS & USART_STATUS_TXBL)) {}
    USART_Tx(usart, data);
}

void usart_drv_hello(USART_TypeDef *usart)
{
    usart_drv_tx(usart, 'H');
    usart_drv_tx(usart, 'i');
    usart_drv_tx(usart, '\r');
    usart_drv_tx(usart, '\n');
}

uint8_t usart_drv_rx(USART_TypeDef *usart) {
    // Wait for data to be available
    while (!(usart->STATUS & USART_STATUS_RXDATAV)) {}
    return (uint8_t)USART_Rx(usart);
}
