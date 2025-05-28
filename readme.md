# EFM32/EFR32 Neopixel LED Strip Driver

This project implements a driver for controlling WS2812B or SK6812 (Neopixel) LED strips using a Silicon Labs EFM32/EFR32 series microcontroller. It utilizes a Timer peripheral in PWM mode to generate the precise timing required by the Neopixels and leverages the LDMA (Linked Direct Memory Access) controller for efficient, CPU-independent data transfer to the LED strip.

## Features

*   Supports WS2812B (3-color components: G, R, B) and SK6812 (4-color components: G, R, B, W) Neopixel types (configurable in `neopixel.h`).
*   Uses PWM for generating the Neopixel data signal.
*   Employs LDMA for transferring pixel data to the PWM peripheral, minimizing CPU load.
*   Provides functions to:
    *   Initialize the Neopixel driver and associated hardware (GPIO, Timer, LDMA).
    *   Set the color of individual LEDs.
    *   Set all LEDs on a strip to a uniform color.
    *   Update the LED strip with the new color data.
*   Configurable number of LEDs and strips.

## Project Structure

*   `neopixel.c`/`neopixel.h`: Core logic for the Neopixel driver, including LED data management, PWM buffer preparation, and DMA setup.
*   `dma_config.c`/`dma_config.h`: Configuration and initialization for the LDMA controller and the specific DMA channel used for Neopixel data.
*   `main.c` (or `app.c` - *assumed*): Would contain the main application logic, initialization of peripherals (like CMU, GPIO, PWM through `sl_pwm` component), and calls to the Neopixel driver functions.

## Key Configuration Files

*   `neopixel.h`:
    *   `NEOPIXEL_TYPE`: Define as `NEOPIXEL_TYPE_WS2812B` or `NEOPIXEL_TYPE_SK6812`.
    *   `PWM_HI`, `PWM_LO`: Define the PWM duty cycle percentages for a Neopixel '1' bit and '0' bit, respectively. These values are relative to the `pwm_top_value` (Timer TOP value) and determine the mark-space ratio for the signal.
    *   `NUM_LEDS`: Number of LEDs in the strip.
    *   `NUM_STRIPS`: Number of LED strips (currently, the `neopixel_update` and `neopixel_init_dma_transfer` functions are hardcoded for a single strip at `strip_id = 0`).
*   `dma_config.h`:
    *   `DMA_CONFIG_DMA_CHANNEL`: Specifies the LDMA channel used for Neopixel data transfer.

## How it Works

1.  **Initialization (`neopixel_init`, `dma_init_controller`):**
    *   The `neopixel_t` data structure is cleared.
    *   The LDMA controller is initialized.
    *   (Implicitly, the PWM peripheral, GPIO for the data pin, and system clocks must be initialized by the main application, likely using Simplicity Studio's peripheral configuration tools).

2.  **Setting LED Colors (`neopixel_set_led`, `neopixel_set_all`):**
    *   The application calls these functions to define the color (and white level for SK6812) for each LED. This data is stored in the `neopixel_t` structure.

3.  **Updating the Strip (`neopixel_update`):**
    *   This function is called when the LED colors need to be sent to the strip.
    *   It iterates through each LED's color components (G, R, B, and optionally W).
    *   For each bit of each color component, it determines if it's a '1' or a '0'.
    *   Based on the bit value, it calculates a PWM duty cycle value using `PWM_HI` or `PWM_LO` percentages and the provided `pwm_top_value` (which should be the Timer's TOP value, determining the PWM period).
    *   These calculated duty cycle values are stored sequentially in the `wr_buf` (write buffer). Each value in this buffer corresponds to the PWM duty cycle required to represent one bit of the Neopixel data stream.

4.  **DMA Transfer (`neopixel_init_dma_transfer`):**
    *   This function configures an LDMA channel.
    *   The source of the DMA transfer is the `neopixel->wr_buf`.
    *   The destination is the Timer's Compare/Capture Value Buffer register (e.g., `TIMER0->CC[0].OCB` or `TIMER0->CC[0].CCVB` if double-buffering is used by the PWM peripheral).
    *   The LDMA is set up to trigger on a peripheral request from the Timer (e.g., `ldmaPeripheralSignal_TIMER0_CC0`, typically an underflow or compare match event that signals the PWM is ready for a new duty cycle value).
    *   Once started, the LDMA transfers one duty cycle value from `wr_buf` to the Timer's CCVB register for each PWM period, effectively serializing the Neopixel data bits.
    *   The PWM peripheral itself must be configured to generate a carrier frequency that allows the `PWM_HI` and `PWM_LO` duty cycles to create the correct bit timings for the Neopixels (e.g., ~0.4us for a '0' high time, ~0.8us for a '1' high time, with a total bit period of ~1.25us for an 800kHz data rate).

5.  **Transfer Completion (`neopixel_ldma_irq_handler`):**
    *   When the LDMA completes transferring all the data in `wr_buf`, an interrupt is generated.
    *   The `neopixel_ldma_irq_handler` (called from the main `LDMA_IRQHandler`) stops the PWM Timer to ensure the data line goes low, signaling the end of the Neopixel data transmission.

## Prerequisites & Usage

*   Silicon Labs EFM32/EFR32 Microcontroller.
*   Simplicity Studio IDE with appropriate SDK and GCC compiler.
*   A WS2812B or SK6812 LED strip.
*   Proper hardware connections:
    *   MCU data pin to the LED strip's Data In (DI) pin.
    *   Power (e.g., 5V) and Ground for the LED strip.
    *   Ensure the MCU's data pin is 5V tolerant or use a level shifter if the MCU is 3.3V and the strip requires 5V logic.

To use this driver:
1.  Initialize the necessary clocks (HFPERCLK, GPIO, TIMER, LDMA) in your main application.
2.  Initialize the GPIO pin used for Neopixel data output using Simplicity Studio's Pin Tool or direct register access.
3.  Initialize a Timer peripheral as a PWM output on the chosen GPIO pin (e.g., using `sl_pwm` component). Configure its frequency to be a multiple of the Neopixel bit rate (e.g., 2.4MHz to 3.2MHz for 800kHz Neopixels) to allow for fine-grained duty cycle control.
4.  Call `dma_init_controller()` to set up the LDMA.
5.  Create a `neopixel_t` instance.
6.  Call `neopixel_init()` with the instance.
7.  Set LED colors using `neopixel_set_led()` or `neopixel_set_all()`.
8.  Call `neopixel_update()` with your `neopixel_t` instance and the Timer's TOP value (e.g., `TIMER_TopGet(TIMER0)` or the equivalent `sl_pwm` API to get the max duty cycle value).
9.  Call `neopixel_init_dma_transfer()` to start sending data, providing the address of the Timer's compare channel buffer register (e.g., `&(SL_PWM_LED_STRIP_PWM_PERIPHERAL->CC[SL_PWM_LED_STRIP_PWM_CHANNEL].OCB)` or `&(SL_PWM_LED_STRIP_PWM_PERIPHERAL->CC[SL_PWM_LED_STRIP_PWM_CHANNEL].CCVB)`).
10. Ensure the `LDMA_IRQHandler` in `dma_config.c` is correctly placed and called, and that it routes the correct channel completion to `neopixel_ldma_irq_handler`.

## TODO / Improvements

*   Add support for multiple strips with independent DMA channels or a more complex buffer management scheme.
*   Make timer and DMA channel selection more configurable (e.g., via parameters in init functions rather than hardcoded defines).
*   Add more detailed error handling.
*   Provide examples for different EFM32/EFR32 series or specific boards.
*   Refine PWM frequency and HI/LO values for optimal compatibility with various Neopixel timing requirements.