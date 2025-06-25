/*
 * neopixel.c
 *
 *  Created on: 28 may. 2025
 *      Author: snaiderbautista
 */


/*
 * neopixel.c
 *
 *  Created on: 24 abr. 2025
 *      Author: snaiderbautista
 */

#include "neopixel.h"
#include <string.h> // For memset
#include <stdint.h> // For standard integer types
#include "em_ldma.h" // For LDMA types and functions
#include "em_timer.h" // For TIMER_TypeDef, TIMER0
#include "em_gpio.h"
#include "em_cmu.h"
#include <stdbool.h>

#include "dma_config.h"

// Desired frequency in Hz for the PWM carrier for Neopixel bit encoding.
// This frequency needs to be high enough to construct Neopixel bit timings.
// Example: For an 800kHz Neopixel data rate, a common PWM frequency is 2.4MHz.
#define PWM_FREQ 800000 // Placeholder, adjust as needed for specific Neopixel type and timings

// Global LDMA descriptor for Neopixel data transfer (must persist)
LDMA_Descriptor_t neopixel_dma_desc_link;

/**************************************************************************//**
 * @brief GPIO initialization for Neopixel
 *****************************************************************************/
static void neopixel_hal_init_gpio(void)
{
  // Configure PA6 as Push-Pull (assuming PA8 is the Neopixel data pin)
  // Ensure this pin matches your hardware setup.
  GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull, 0);
}

/**************************************************************************//**
 * @brief CMU initialization for Neopixel
 *****************************************************************************/
static void neopixel_hal_init_cmu(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
}

/**************************************************************************//**
 * @brief TIMER initialization for Neopixel PWM
 *****************************************************************************/
static void neopixel_hal_init_timer(void)
{
  uint32_t timerFreq, topValue;
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

  timerInit.enable = false; // Don't start counter on initialization
  // timerInit.prescale = timerPrescale1; // Adjust prescaler if needed

  timerCCInit.mode = timerCCModePWM; // PWM mode
  timerCCInit.cmoa = timerOutputActionToggle;
  timerCCInit.edge = timerEdgeBoth;
  timerCCInit.outInvert = 0;

  TIMER_Init(TIMER0, &timerInit);

  // Route CC0 output to PA8 (Neopixel data pin)
  GPIO->TIMERROUTE[0].ROUTEEN  = GPIO_TIMER_ROUTEEN_CC0PEN;
  GPIO->TIMERROUTE[0].CC0ROUTE = (gpioPortA << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
                               | (8 << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);

  TIMER_InitCC(TIMER0, 0, &timerCCInit);

  // Set top value for the desired PWM_FREQ
  timerFreq = CMU_ClockFreqGet(cmuClock_TIMER0) / (timerInit.prescale + 1);
  topValue = (timerFreq / (uint32_t)PWM_FREQ);
  TIMER_TopSet(TIMER0, topValue);

   // Now start the TIMER
  TIMER_Enable(TIMER0, true);

  // Trigger DMA on compare event to set CCVB to update duty cycle on next period
  TIMER_IntEnable(TIMER0, TIMER_IEN_CC0);

  // TIMER is not enabled here; it will be enabled when data is ready to be sent.
}

/**
 * @brief LDMA IRQ Handler for Neopixel DMA completion.
 */
void neopixel_ldma_irq_handler(void)
{
  // This function is called from the main LDMA_IRQHandler in app.c
  // when a DONE0 interrupt for the Neopixel DMA channel occurs.

  // Stop the TIMER0 that generates the PWM signal for Neopixels.
  // This is important to ensure the line goes low after data transfer.
  TIMER_Enable(TIMER0, false);

  // Optionally, clear the GPIO pin state if needed to ensure it's low.
  GPIO_PinOutClear(gpioPortA, 8); // Example, if PA6 is data pin.
}

/**
 * @brief Initializes the Neopixel data structure and underlying hardware (GPIO, CMU, TIMER).
 *
 * @param neopixel Pointer to the neopixel_t structure.
 */
void neopixel_init(neopixel_t *neopixel) {
    // Initialize dependent hardware
    neopixel_hal_init_cmu();
    neopixel_hal_init_gpio();
    neopixel_hal_init_timer();

    // Clear all LED data and the write buffer
    memset(neopixel, 0, sizeof(neopixel_t));
}

/**
 * @brief Sets the color of a specific LED on a specific strip.
 *
 * @param neopixel Pointer to the neopixel_t structure.
 * @param strip_id The index of the strip (should be < NUM_STRIPS).
 * @param led_id The index of the LED on the strip (should be < NUM_LEDS).
 * @param led The neopixel_led_t color data to set.
 */
void neopixel_set_led(neopixel_t *neopixel, uint8_t strip_id, uint8_t led_id, neopixel_led_t led) {
    // Basic bounds check
    if (strip_id >= NUM_STRIPS || led_id >= NUM_LEDS || neopixel == NULL) {
        return; // Handle error appropriately (e.g., assert, log, return error code)
    }
    neopixel->strips[strip_id].leds[led_id] = led;
}


/**
 * @brief Sets all LEDs on a strip to the same RGBW color (for SK6812).
 *
 * @param neopixel Pointer to the neopixel_t structure.
 * @param strip_id The index of the strip.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param w White component.
 */
void neopixel_set_all(neopixel_t *neopixel, uint8_t strip_id, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    // Bounds check
    if (strip_id >= NUM_STRIPS || neopixel == NULL) {
        return;
    }

    for (uint16_t i = 0; i < NUM_LEDS; ++i) {
        // Create the color struct
        neopixel_led_t color = {
            .g = g,
            .r = r,
            .b = b
            // 'w' is only added if NEOPIXEL_TYPE is SK6812 due to struct definition
        };

#if (NEOPIXEL_TYPE == NEOPIXEL_TYPE_SK6812)
        // Set the white component only for SK6812
        color.w = w;
#else
        // For WS2812B, ensure 'w' parameter is marked as unused to avoid compiler warnings
        (void)w;
#endif

        // Assign the color to the LED in the struct
        neopixel->strips[strip_id].leds[i] = color;
    }
}

/**
 * @brief Populates the write buffer based on LED data and initiates the transfer.
 *        (DMA initiation part is conceptual).
 *
 * @param neopixel Pointer to the neopixel_t structure.
 * @param pwm_top_value The maximum value of the PWM register (TIMER_TopGet(TIMER0)).
 */
void neopixel_update(neopixel_t *neopixel, uint32_t pwm_top_value) {
    if (neopixel == NULL) {
        return; // Handle error
    }
    volatile uint8_t strip_id = 0; // Hardcoded to strip 0 for now
    volatile uint32_t wr_buf_idx = 0;

    uint32_t expected_buf_len = NUM_LEDS * NUM_BPP * NUM_BITS;
    if (WR_BUF_LEN < (expected_buf_len + 1)) {
        // Error: Buffer is too small.
        return;
    }

    for (uint16_t led_idx = 0; led_idx < NUM_LEDS; led_idx++) {
        volatile uint8_t color_components[NUM_BPP];

        color_components[0] = neopixel->strips[strip_id].leds[led_idx].g;
        color_components[1] = neopixel->strips[strip_id].leds[led_idx].r;
        color_components[2] = neopixel->strips[strip_id].leds[led_idx].b;
#if (NEOPIXEL_TYPE == NEOPIXEL_TYPE_SK6812)
        color_components[3] = neopixel->strips[strip_id].leds[led_idx].w;
#endif

        for (uint8_t comp_idx = 0; comp_idx < NUM_BPP; comp_idx++) {
            uint8_t current_byte = color_components[comp_idx];
            for (int bit_idx = 7; bit_idx >= 0; bit_idx--) {
                uint8_t bit_is_set = (current_byte >> bit_idx) & 0x01;
                uint32_t pwm_duty_raw = (uint32_t)(bit_is_set ? PWM_HI : PWM_LO);
                // Calculate actual duty cycle value for TIMER CCVB based on top value and desired percentage
                neopixel->wr_buf[wr_buf_idx++] = (pwm_top_value * pwm_duty_raw) / 100;
            }
        }
    }

    // Add final zero-duty-cycle pulse to ensure line goes low
    neopixel->wr_buf[wr_buf_idx] = 0;
}

/**
 * @brief Initializes the LDMA transfer for Neopixel data.
 *
 * @param neopixel Pointer to the neopixel_t structure.
 * @param timer_cc_ocb_reg Pointer to the timer compare channel output compare buffer register (e.g., &TIMER0->CC[0].OCB or &TIMER0->CC[0].CCVB).
 */
void neopixel_init_dma_transfer(neopixel_t *neopixel, void *timer_cc_reg) { // Renamed param for clarity
    if (neopixel == NULL || timer_cc_reg == NULL) return;
    TIMER_Enable(TIMER0, true);

    GPIO_PinOutSet(gpioPortA, 7);

    LDMA_TransferCfg_t periTransferTx = LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER0_CC0);

    // The descriptor macro handles subtracting 1 from the count, so we pass WR_BUF_LEN directly.
    LDMA_Descriptor_t xfer = LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(
                                        neopixel->wr_buf,    // Source: word-aligned buffer
                                        timer_cc_reg,        // Destination: word-aligned peripheral reg
                                        WR_BUF_LEN);         // Number of transfers

    neopixel_dma_desc_link = xfer;

    // The descriptor macro for BYTE transfers sets the size to byte, so we must override it to word.
    neopixel_dma_desc_link.xfer.size = ldmaCtrlSizeWord;
    neopixel_dma_desc_link.xfer.ignoreSrec = 0;
    neopixel_dma_desc_link.xfer.doneIfs = 1; // Interrupt when done

    // Channel 0 is assumed for Neopixel DMA.
    LDMA_IntEnable(LDMA_IEN_CHDONE_DEFAULT | LDMA_IEN_ERROR); // Enable DONE0 and ERROR interrupts for channel 0

    LDMA_StartTransfer(DMA_CONFIG_DMA_CHANNEL, (void*)&periTransferTx, (void*)&neopixel_dma_desc_link);
}
