#include "sl_sleeptimer.h"
#include "bg9x_hw.h"
#include "helpers/drv_digital_in.h"
#include "helpers/drv_digital_out.h"

#define BG96_GPIO_H_TIME 1000
#define BG96_TIMEOUT_MS  15000

static enum {
  bg96_ready = 0, bg96_wait_for_gpio, bg96_wait_for_device,
} bg96_state;

static bool required_state = false;
//static at_scheduler_status_t *global_output;

static digital_out_t pwk_pin;
static digital_in_t sta_pin;

static sl_sleeptimer_timer_handle_t bg96_timer_gpio_handle;
static sl_sleeptimer_timer_handle_t bg96_timer_process_handler;
static void bg96_hw_timer_gpio_callback(sl_sleeptimer_timer_handle_t *handle,
                                     void *data);
static void bg96_hw_timer_process_callback(sl_sleeptimer_timer_handle_t *handle,
                                        void *data);

/**************************************************************************//**
 * @brief
 *   BG96 module initialize function.
 *
 *****************************************************************************/
void bg96_hw_init()
{
//  at_parser_init(handle);

  digital_out_init(&pwk_pin, BG96_PWK_PORT, BG96_PWK_PIN);
  digital_in_pulldown_init(&sta_pin, BG96_STA_PORT, BG96_STA_PIN);

  sl_sleeptimer_start_periodic_timer_ms(&bg96_timer_process_handler,
                                        200,
                                        bg96_hw_timer_process_callback,
                                        NULL,
                                        0,
                                        0);
}

/**************************************************************************//**
 * @brief
 *   BG96 is alive status getter function.
 *
 *****************************************************************************/
bool bg96_hw_is_alive(void)
{
  return (bool) digital_in_read(&sta_pin);
}

/**************************************************************************//**
 * @brief
 *   BG96 module sleep function.
 *
 * @return
 *    SL_STATUS_OK if sleep initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_hw_sleep()
{


  if (bg96_ready != bg96_state) {
    return SL_STATUS_BUSY;
  }

  if (bg96_hw_is_alive()) {
    digital_out_high(&pwk_pin);
    sl_status_t sc = sl_sleeptimer_restart_timer_ms(&bg96_timer_gpio_handle,
                                                    BG96_GPIO_H_TIME,
                                                    bg96_hw_timer_gpio_callback,
                                                    (void *) NULL, 0, 0);

    if (SL_STATUS_OK != sc) {
      return sc;
    }

  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   BG96 module wake up function.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_hw_wake_up()
{

  if (bg96_ready != bg96_state) {
    return SL_STATUS_BUSY;
  }

  if (!bg96_hw_is_alive()) {
    digital_out_high(&pwk_pin);
    sl_status_t sc = sl_sleeptimer_restart_timer_ms(&bg96_timer_gpio_handle,
                                                    BG96_GPIO_H_TIME,
                                                    bg96_hw_timer_gpio_callback,
                                                    (void *) NULL, 0, 0);
    if (SL_STATUS_OK != sc) {
      return sc;
    }
    required_state = true;
    bg96_state = bg96_wait_for_gpio;

  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   Local callback function for bg96 timer.
 *
 *****************************************************************************/
static void bg96_hw_timer_gpio_callback(sl_sleeptimer_timer_handle_t *handle,
                                     void *data)
{
  (void)handle;
  (void) data;

  switch (bg96_state) {
    case bg96_wait_for_gpio:
      digital_out_low(&pwk_pin);
      bg96_state = bg96_wait_for_device;

      sl_sleeptimer_restart_timer_ms(&bg96_timer_gpio_handle,
                                     BG96_TIMEOUT_MS,
                                     bg96_hw_timer_gpio_callback,
                                     (void *) NULL,
                                     0,
                                     0);
      break;
    case bg96_wait_for_device:
      bg96_state = bg96_ready;
      break;
    default:
      bg96_state = bg96_ready;
  }
}

static void bg96_hw_timer_process_callback(sl_sleeptimer_timer_handle_t *handle,
                                        void *data)
{
  (void)handle;
  (void)data;

  switch (bg96_state) {
    case bg96_ready:
      break;
    case bg96_wait_for_gpio:
      break;
    case bg96_wait_for_device:
      if (bg96_hw_is_alive() == required_state) {
        sl_sleeptimer_stop_timer(&bg96_timer_gpio_handle);
        bg96_state = bg96_ready;
      }
      break;
  }
}
