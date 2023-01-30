#ifndef BBMC_CHUNK_UTILS_BIT_ARRAY_H
#define BBMC_CHUNK_UTILS_BIT_ARRAY_H

#include <stdint.h>

typedef struct {
    uint8_t bits_per_block;
    uint8_t blocks_per_word;
    uint16_t words_per_chunk;
    uint8_t padding_size;
    uint8_t header;
} bit_array_version_t;

extern bit_array_version_t bit_array_v1;
extern bit_array_version_t bit_array_v2;
extern bit_array_version_t bit_array_v3;
extern bit_array_version_t bit_array_v4;
extern bit_array_version_t bit_array_v5;
extern bit_array_version_t bit_array_v6;
extern bit_array_version_t bit_array_v8;
extern bit_array_version_t bit_array_v16;

#endif