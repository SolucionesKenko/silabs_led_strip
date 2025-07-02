#include "usart_drv.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_core.h"

// NOTE: Pin configuration must be set in app.c or board-specific code

// Static callback pointers for each USART instance (support USART0 and USART1)
static usart_drv_rx_cb_t usart0_rx_cb = 0;
static usart_drv_rx_cb_t usart1_rx_cb = 0;

void usart_drv_init_async(USART_TypeDef *usart, const usart_drv_config_t *cfg) {
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    init.baudrate = cfg->baudrate;
    // TODO: Map databits, parity, stopbits if needed
    USART_InitAsync(usart, &init);
    USART_Enable(usart, usartEnable);
}

void usart_drv_enable_rx_interrupt(USART_TypeDef *usart, usart_drv_rx_cb_t cb) {
    if (usart == USART0) {
        usart0_rx_cb = cb;
        USART_IntClear(USART0, _USART_IF_MASK);
        USART_IntEnable(USART0, USART_IEN_RXDATAV);
        NVIC_ClearPendingIRQ(USART0_RX_IRQn);
        NVIC_EnableIRQ(USART0_RX_IRQn);
    } else if (usart == USART1) {
        usart1_rx_cb = cb;
        USART_IntClear(USART1, _USART_IF_MASK);
        USART_IntEnable(USART1, USART_IEN_RXDATAV);
        NVIC_ClearPendingIRQ(USART1_RX_IRQn);
        NVIC_EnableIRQ(USART1_RX_IRQn);
    }
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

// Interrupt handlers for USART0 and USART1
void USART0_RX_IRQHandler(void) {
    uint32_t flags = USART_IntGet(USART0);
    USART_IntClear(USART0, flags);
    if ((flags & USART_IF_RXDATAV) && usart0_rx_cb) {
        uint8_t data = (uint8_t)USART_Rx(USART0);
        usart0_rx_cb(data);
    }
}

void USART1_RX_IRQHandler(void) {
    uint32_t flags = USART_IntGet(USART1);
    USART_IntClear(USART1, flags);
    if ((flags & USART_IF_RXDATAV) && usart1_rx_cb) {
        uint8_t data = (uint8_t)USART_Rx(USART1);
        usart1_rx_cb(data);
    }
}
