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

neopixel_t strips;

void delay_ms(uint32_t ms)
{
  // for (uint32_t i = 0; i < 1000000; i++)
  //   {
  //     __NOP();
  //     __NOP();
  //     __NOP();
  //     __NOP();
  //     __NOP();
  //     __NOP();
  //     __NOP();
  //   }
  sl_sleeptimer_delay_millisecond(64);
}

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
//       while(LDMA_TransferDone(DMA_CONFIG_DMA_CHANNEL) != 1)
//              {
//
//              }
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

void app_init(void)
{

  dma_init_controller();
  neopixel_init(&strips);

  delay_ms(10);

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  while (1)
    {
      patern_0();
      sl_sleeptimer_delay_millisecond(1000);
      patern_1();
      sl_sleeptimer_delay_millisecond(1000);
      patern_2();
      sl_sleeptimer_delay_millisecond(1000);
    }
}

