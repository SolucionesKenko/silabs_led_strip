/*
 * dma_config.h
 *
 *  Created on: 28 may. 2025
 *      Author: snaiderbautista
 */

#ifndef DMA_CONFIG_H_
#define DMA_CONFIG_H_

#define DMA_CONFIG_DMA_CHANNEL 1
/**
 * @brief Initializes the LDMA controller.
 * This function sets up the LDMA controller with default parameters and
 * enables necessary interrupts at the NVIC level.
 */
void dma_init_controller(void);

#endif /* DMA_CONFIG_H_ */
