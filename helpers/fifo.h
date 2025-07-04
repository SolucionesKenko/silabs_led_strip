#ifndef __fifo_H
#define __fifo_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#ifndef MAX_FIFO_SIZE
#define MAX_FIFO_SIZE 128
#endif

/**
 * @brief FIFO buffer structure
 */
typedef struct {
    char buffer[MAX_FIFO_SIZE]; /**< The FIFO buffer */
    volatile uint32_t head; /**< Head index of the FIFO */
    volatile uint32_t tail; /**< Tail index of the FIFO */
    uint32_t size; /**< Size of the FIFO buffer */
} FIFO_HandleTypeDef;

/**
 * @brief Initializes the FIFO buffer.
 * 
 * @param fifo Pointer to the FIFO_HandleTypeDef structure.
 * @param size Size of the FIFO buffer.
 */
void fifo_init(FIFO_HandleTypeDef *fifo, uint16_t size);

/**
 * @brief Puts data into the FIFO buffer.
 * 
 * @param fifo Pointer to the FIFO_HandleTypeDef structure.
 * @param data Data to be put into the FIFO buffer.
 * @return 0 on success, 1 if FIFO is full.
 */
uint8_t fifo_put(FIFO_HandleTypeDef *fifo, char data);

/**
 * @brief Gets data from the FIFO buffer.
 * 
 * @param fifo Pointer to the FIFO_HandleTypeDef structure.
 * @param data Pointer to store the retrieved data.
 * @return 0 on success, 1 if FIFO is empty.
 */
uint8_t fifo_get(FIFO_HandleTypeDef *fifo, char *data);

/**
 * @brief Returns the number of bytes available in the FIFO buffer.
 * 
 * @param fifo Pointer to the FIFO_HandleTypeDef structure.
 * @return Number of bytes available in the FIFO buffer.
 */
uint32_t fifo_available(FIFO_HandleTypeDef *fifo);

/**
 * @brief Returns the number of free bytes in the FIFO buffer.
 * 
 * @param fifo Pointer to the FIFO_HandleTypeDef structure.
 * @return Number of free bytes in the FIFO buffer.
 */
uint32_t fifo_free(FIFO_HandleTypeDef *fifo);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_H */
