#ifndef _DRV_DIGITAL_IN_H_
#define _DRV_DIGITAL_IN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "em_gpio.h"
#include <stdint.h>

typedef enum {
  DIGITAL_IN_SUCCESS = 0,
  DIGITAL_IN_INVALID = -1
} digital_in_err_t;

typedef struct {
  GPIO_Port_TypeDef port;
  unsigned int pin;
} digital_in_t;

/**
 * @brief Initialize a GPIO pin as input (floating).
 */
digital_in_err_t digital_in_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin);

/**
 * @brief Initialize a GPIO pin as input with pull-up resistor.
 */
digital_in_err_t digital_in_pullup_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin);

/**
 * @brief Initialize a GPIO pin as input with pull-down resistor.
 */
digital_in_err_t digital_in_pulldown_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin);

/**
 * @brief Read the digital state of the input pin (0 or 1).
 */
uint8_t digital_in_read(const digital_in_t *in);

#ifdef __cplusplus
}
#endif

#endif // _DRV_DIGITAL_IN_H_
