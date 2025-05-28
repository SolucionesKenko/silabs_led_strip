/*
 * dma_confiig.c
 *
 *  Created on: 28 may. 2025
 *      Author: snaiderbautista
 */

#include "dma_config.h"
#include "em_ldma.h" // For LDMA_Init_t, LDMA_Init, etc.
#include "em_cmu.h"  // For clock enables if necessary (assuming LDMA clock is enabled elsewhere or by default)
#include "neopixel.h"
// If LDMA_INIT_DEFAULT requires specific clock enabling, it should be done here or ensured it's done before this call.

void LDMA_IRQHandler(void)
{
  uint32_t pending;
  pending = LDMA_IntGet();
  LDMA_IntClear(pending);

  if(pending & LDMA_IF_ERROR)
  {
      //TIMER_Enable(TIMER0, false);
  }
  if(pending & LDMA_IF_DONE1)
  {
     neopixel_ldma_irq_handler();
  }
}

/**
 * @brief Initializes the LDMA controller.
 */
void dma_init_controller(void)
{
  // Ensure LDMA clock is enabled (usually done in main CMU init, but good practice here too if it's a self-contained module)
  // CMU_ClockEnable(cmuClock_LDMA, true); // This is now in app.c's initCMU

  // Initialize the LDMA controller with default settings
  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  LDMA_Init(&ldmaInit);

  // Enable LDMA IRQ in NVIC. Note: Specific channel interrupts (like DONE0 or ERROR)
  // should be enabled when a transfer is configured and started.
  // LDMA_IntEnable(LDMA_IEN_CHDONE_DEFAULT | LDMA_IEN_ERROR); // This might be too broad for a general init
  NVIC_ClearPendingIRQ(LDMA_IRQn);
  NVIC_EnableIRQ(LDMA_IRQn);
}
