#ifndef _DRV_DIGITAL_OUT_H_
#define _DRV_DIGITAL_OUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "em_gpio.h"
#include <stdint.h>

typedef enum {
  DIGITAL_OUT_SUCCESS = 0,
  DIGITAL_OUT_INVALID = -1
} digital_out_err_t;

typedef struct {
  GPIO_Port_TypeDef port;
  unsigned int pin;
} digital_out_t;

/**
 * @brief Initialize a GPIO pin as push-pull output (default LOW).
 */
digital_out_err_t digital_out_init(digital_out_t *out, GPIO_Port_TypeDef port, unsigned int pin);

/**
 * @brief Set the output pin HIGH.
 */
void digital_out_high(digital_out_t *out);

/**
 * @brief Set the output pin LOW.
 */
void digital_out_low(digital_out_t *out);

/**
 * @brief Toggle the output pin.
 */
void digital_out_toggle(digital_out_t *out);

/**
 * @brief Write a value (0 or 1) to the output pin.
 */
void digital_out_write(digital_out_t *out, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif // _DRV_DIGITAL_OUT_H_
