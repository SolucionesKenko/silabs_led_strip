#ifndef BG9X_HW_H
#define BG9X_HW_H

#include <stdint.h>
#include "sl_status.h"
#include "bg9x_at.h"
#include "pin_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> BG96_STA
// $[GPIO_BG96_STA]
#define BG96_STA_PORT                            MODEM_STA_PORT
#define BG96_STA_PIN                             MODEM_STA_PIN
//// [GPIO_BG96_STA]$
//
//// <gpio optional=true> BG96_PWK
//// $[GPIO_BG96_PWK]
#define BG96_PWK_PORT                            MODEM_PWKY_PORT
#define BG96_PWK_PIN                             MODEM_PWKY_PIN
// [GPIO_BG96_PWK]$

// <<< sl:end pin_tool >>>

/**************************************************************************//**
 * @brief
 *   BG96 module initialize function.
 *
 *****************************************************************************/
void bg96_hw_init();

/**************************************************************************//**
 * @brief
 *   BG96 is alive status getter function.
 *
 *****************************************************************************/
bool bg96_hw_is_alive(void);

/**************************************************************************//**
 * @brief
 *   BG96 module sleep function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER timer related error.
 *****************************************************************************/
sl_status_t bg96_hw_sleep();

/**************************************************************************//**
 * @brief
 *   BG96 module wake up function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER timer related error.
 *****************************************************************************/
sl_status_t bg96_hw_wake_up();

#ifdef __cplusplus
}
#endif

#endif // BG9X_HW_H 