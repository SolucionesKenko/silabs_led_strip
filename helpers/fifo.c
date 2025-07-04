#include "fifo.h"
#include <stdio.h>
#include <string.h>

void fifo_init(FIFO_HandleTypeDef *fifo, uint16_t size)
{
    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = MAX_FIFO_SIZE;
    memset(fifo->buffer, 0, sizeof(fifo->buffer));
}

uint8_t fifo_put(FIFO_HandleTypeDef *fifo, char data)
{
    // check if FIFO has room
    uint32_t next = (fifo->head + 1) % fifo->size;
    if (next == fifo->tail)
    {
        // FIFO full
        return 1;
    }
    fifo->buffer[fifo->head] = data;
    fifo->head = next;
    return 0; //success
}

uint8_t fifo_get(FIFO_HandleTypeDef *fifo, char *data)
{
    // check if FIFO has data
    if (fifo->head == fifo->tail)
    {
        return 1; // FIFO empty
    }
    *data = fifo->buffer[fifo->tail];
    fifo->tail = (fifo->tail + 1 ) % fifo->size;
    return 0; //success
}

uint32_t fifo_available(FIFO_HandleTypeDef *fifo)
{
    return (fifo->size + fifo->head - fifo->tail) % fifo->size;
}

uint32_t fifo_free(FIFO_HandleTypeDef *fifo)
{
    return (fifo->size - 1 - fifo_available(fifo));
}
