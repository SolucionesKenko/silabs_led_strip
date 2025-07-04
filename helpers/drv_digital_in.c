#include "drv_digital_in.h"
#include <stddef.h>

digital_in_err_t digital_in_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin) {
  if (in == NULL) {
    return DIGITAL_IN_INVALID;
  }

  in->port = port;
  in->pin = pin;

  // Configure pin as input, no pull resistor
  GPIO_PinModeSet(port, pin, gpioModeInput, 0);

  return DIGITAL_IN_SUCCESS;
}

digital_in_err_t digital_in_pullup_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin) {
  if (in == NULL) {
    return DIGITAL_IN_INVALID;
  }

  in->port = port;
  in->pin = pin;

  // Configure pin as input with pull-up (1)
  GPIO_PinModeSet(port, pin, gpioModeInputPull, 1);

  return DIGITAL_IN_SUCCESS;
}

digital_in_err_t digital_in_pulldown_init(digital_in_t *in, GPIO_Port_TypeDef port, unsigned int pin) {
  if (in == NULL) {
    return DIGITAL_IN_INVALID;
  }

  in->port = port;
  in->pin = pin;

  // Configure pin as input with pull-down (0)
  GPIO_PinModeSet(port, pin, gpioModeInputPull, 0);

  return DIGITAL_IN_SUCCESS;
}

uint8_t digital_in_read(const digital_in_t *in) {
  return GPIO_PinInGet(in->port, in->pin) ? 1 : 0;
}
