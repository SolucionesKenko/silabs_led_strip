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

void app_init(void)
{

  dma_init_controller();
  neopixel_init(&strips);

  neopixel_set_all(&strips, NEOPIXEL_STRIP_ID_0, 0, 255, 0, 0);

  neopixel_update(&strips, TIMER_TopGet(sl_pwm_led_strip_pwm.timer));

  neopixel_init_dma_transfer(&strips, (void*)&TIMER0->CC[0].OCB);
  sl_pwm_start(&sl_pwm_led_strip_pwm);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
