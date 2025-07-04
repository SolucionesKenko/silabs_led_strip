#include "drv_digital_out.h"
#include <stddef.h>
digital_out_err_t digital_out_init(digital_out_t *out, GPIO_Port_TypeDef port, unsigned int pin) {
  if (out == NULL) {
    return DIGITAL_OUT_INVALID;
  }

  out->port = port;
  out->pin = pin;

  // Configure pin as push-pull output, default LOW
  GPIO_PinModeSet(out->port, out->pin, gpioModePushPull, 0);

  return DIGITAL_OUT_SUCCESS;
}

void digital_out_high(digital_out_t *out) {
  GPIO_PinOutSet(out->port, out->pin);
}

void digital_out_low(digital_out_t *out) {
  GPIO_PinOutClear(out->port, out->pin);
}

void digital_out_toggle(digital_out_t *out) {
  GPIO_PinOutToggle(out->port, out->pin);
}

void digital_out_write(digital_out_t *out, uint8_t value) {
  if (value) {
    GPIO_PinOutSet(out->port, out->pin);
  } else {
    GPIO_PinOutClear(out->port, out->pin);
  }
}
