/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "sl_pwm.h"
#include "sl_pwm_instances.h"

#include "em_ldma.h"
#include "em_timer.h"

#include "neopixel.h"
#include "dma_config.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "helpers/usart_drv.h"
#include "sl_sleeptimer.h"

// Example config for both UARTs
static usart_drv_config_t uart_cfg = {
    .baudrate = 115200,
    .databits = 8,
    .parity = 0,
    .stopbits = 1
};

// === USART0 (VCOM) Pin/Port Definitions ===
#define USART0_TX_PORT   gpioPortA
#define USART0_TX_PIN    5
#define USART0_RX_PORT   gpioPortA
#define USART0_RX_PIN    6

// === USART1 (App) Pin/Port Definitions ===
#define USART1_TX_PORT   gpioPortB
#define USART1_TX_PIN    0
#define USART1_RX_PORT   gpioPortB
#define USART1_RX_PIN    1

static void uart_setup(void)
{
    // Enable clocks for USART0 and USART1
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_USART1, true);

    // USART0 (VCOM):
    GPIO_PinModeSet(USART0_TX_PORT, USART0_TX_PIN, gpioModePushPull, 1); // TX
    GPIO_PinModeSet(USART0_RX_PORT, USART0_RX_PIN, gpioModeInput,    0); // RX
    GPIO->USARTROUTE[0].TXROUTE = (USART0_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT) | (USART0_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].RXROUTE = (USART0_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT) | (USART0_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN | GPIO_USART_ROUTEEN_RXPEN;

    // USART1 (App):
    GPIO_PinModeSet(USART1_TX_PORT, USART1_TX_PIN, gpioModePushPull, 1); // TX
    GPIO_PinModeSet(USART1_RX_PORT, USART1_RX_PIN, gpioModeInput,    0); // RX
    GPIO->USARTROUTE[1].TXROUTE = (USART1_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT) | (USART1_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[1].RXROUTE = (USART1_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT) | (USART1_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[1].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN | GPIO_USART_ROUTEEN_RXPEN;

    // Initialize both UARTs
    usart_drv_init_async(USART0, &uart_cfg);
    usart_drv_init_async(USART1, &uart_cfg);
}

static void uart_run(void)
{
    // Example: send a test byte on USART1
    static bool sent = false;
    if (!sent) {
        usart_drv_hello(USART0);
        usart_drv_hello(USART1);
        sent = true;
    }
    // Print received data from USART1 to USART0 (debug)
    if (USART1->STATUS & USART_STATUS_RXDATAV) {
      uint8_t data = usart_drv_rx(USART1);
      usart_drv_tx(USART0, data); // Print to debug port
    }
}

neopixel_t strips;



static void dma_transfer_syn()
{
//  delay_ms(10);
  neopixel_update(&strips, TIMER_TopGet(TIMER0));
//  delay_ms(10);
  neopixel_init_dma_transfer(&strips, (void*)&TIMER0->CC[0].OCB);
}

uint8_t led_state = 0;
static void patern_0(void) {
    for (int i = 0; i < 8; i++)
    {
       sl_sleeptimer_delay_millisecond(64);
       neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 0, 255, 0);
       dma_transfer_syn();
       sl_sleeptimer_delay_millisecond(64);
       neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 0, 0, 0);
       dma_transfer_syn();
    }
}

static void patern_1(void) {
    for (int i = 0; i < 8; i++)
    {
        sl_sleeptimer_delay_millisecond(64);
        neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 255, 0, 0);
        dma_transfer_syn();
        sl_sleeptimer_delay_millisecond(64);
        neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 0, 0, 0);
        dma_transfer_syn();
    }
}

static void patern_2(void) {
    for (int i = 0; i < 8; i++)
    {
        sl_sleeptimer_delay_millisecond(64);
        neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 255, 0, 0, 0);
        dma_transfer_syn();
        sl_sleeptimer_delay_millisecond(64);
        neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 0, 0, 0);
        dma_transfer_syn();
    }
}

static void run_led()
{
  patern_0();
  sl_sleeptimer_delay_millisecond(1000);
  patern_1();
  sl_sleeptimer_delay_millisecond(1000);
  patern_2();
  sl_sleeptimer_delay_millisecond(1000);
}

void app_init(void)
{
  dma_init_controller();
  uart_setup();
  neopixel_init(&strips);
  sl_sleeptimer_delay_millisecond(10);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  while (1)
  {
    uart_run();
//    run_led();
  }
}

