/*
 * neopixel.h
 *
 *  Created on: 24 abr. 2025
 *      Author: snaiderbautista
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include <stdint.h> // For standard integer types (uint8_t, etc.)
#include "em_ldma.h"

#define NEOPIXEL_TYPE_WS2812B (0)
#define NEOPIXEL_TYPE_SK6812 (1)
#define NEOPIXEL_TYPE (NEOPIXEL_TYPE_WS2812B)

#define PWM_HI (64)
#define PWM_LO (32)

// LED parameters
#if (NEOPIXEL_TYPE == NEOPIXEL_TYPE_WS2812B)
    #define NUM_BPP (3) // WS2812B
#elif (NEOPIXEL_TYPE == NEOPIXEL_TYPE_SK6812)
    #define NUM_BPP (4) // SK6812
#endif
#define NUM_LEDS (10)
#define NUM_BITS (8)
#define NUM_BYTES (NUM_BPP)
#define NUM_STRIPS (1)
#define WR_BUF_LEN ((NUM_BPP * NUM_LEDS * NUM_BITS) + 1) // +1 for a final zero-duty-cycle pulse

enum neopixel_strip_id {
    NEOPIXEL_STRIP_ID_0,
    NEOPIXEL_STRIP_ID_1,
    NEOPIXEL_STRIP_ID_COUNT
};

typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
#if (NEOPIXEL_TYPE == NEOPIXEL_TYPE_SK6812)
    uint8_t w;
#endif
} neopixel_led_t;

extern LDMA_Descriptor_t neopixel_dma_desc_link; // Make descriptor externally visible if needed by IRQ handler directly

typedef struct {
    neopixel_led_t leds[NUM_LEDS];
} neopixel_strip_t;

typedef struct {
    neopixel_strip_t strips[NUM_STRIPS];
    uint32_t wr_buf[WR_BUF_LEN];
} neopixel_t;

void neopixel_init(neopixel_t *neopixel);
void neopixel_set_led(neopixel_t *neopixel, uint8_t strip_id, uint8_t led_id, neopixel_led_t led);
void neopixel_set_all(neopixel_t *neopixel, uint8_t strip_id, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void neopixel_update(neopixel_t *neopixel, uint32_t pwm_top_value);
void neopixel_init_dma_transfer(neopixel_t *neopixel, void *timer_cc_ocb_reg); // timer_cc_ocb_reg is e.g. &TIMER0->CC[0].OCB
void neopixel_ldma_irq_handler(void);

#endif /* NEOPIXEL_H_ */
