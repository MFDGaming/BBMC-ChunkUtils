#ifndef BBMC_CHUNK_UTILS_WRITER_H
#define BBMC_CHUNK_UTILS_WRITER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *buffer;
    size_t size;
} block_storage_buffer_t;

block_storage_buffer_t write_block_storage(uint32_t *blocks, uint16_t blocks_size, uint32_t *palette, uint32_t palette_length);

#endif
