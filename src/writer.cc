#include <chunk_utils/bit_array.h>
#include <chunk_utils/writer.h>
#include <math.h>
#include <stdlib.h>

void write_varint32(uint8_t *data, size_t *offset, uint32_t input) {
    uint32_t value = input;
    for (uint8_t i = 0; i < 5; ++i) {
        uint8_t to_write = (uint8_t)(value & 0x7f);
        value >>= 7;
        if (value != 0) {
            data[(*offset)++] = to_write | 0x80;
        } else {
            data[(*offset)++] = to_write;
            break;
        }
    }
}

void write_zigzag32(uint8_t *data, size_t *offset, int32_t input) {
    uint32_t raw = (input >= 0) ? ((*((uint32_t *)&input)) << 1) : (((~(*((uint32_t *) &input))) << 1) | 1);
    write_varint32(data, offset, raw);
}

block_storage_buffer_t write_block_storage(uint32_t *blocks, uint16_t blocks_size, uint32_t *palette, uint32_t palette_length) {
    float bits_per_block = ceilf(log2f((float)palette_length));
    bit_array_version_t version;
    if (bits_per_block == 0.0f || bits_per_block == 1.0f) {
        version = bit_array_v1;
    } else if (bits_per_block == 2.0f) {
        version = bit_array_v2;
    } else if (bits_per_block == 3.0f) {
        version = bit_array_v3;
    } else if (bits_per_block == 4.0f) {
        version = bit_array_v4;
    } else if (bits_per_block == 5.0f) {
        version = bit_array_v5;
    } else if (bits_per_block == 6.0f) {
        version = bit_array_v6;
    } else if (bits_per_block == 7.0f || bits_per_block == 8.0f) {
        version = bit_array_v8;
    } else {
        version = bit_array_v16;
    }
    // header (uint8_t) + words (uint32_t[]) + pallete_size (zigzag32) + palette (SignedVarInt[])
    size_t buffer_length = 1 + (version.words_per_chunk * 4) + 4 + (palette_length * 4);
    uint8_t *buffer = (uint8_t *)malloc(buffer_length);

    buffer[0] = version.header;

    size_t write_offset = 1;

    uint16_t position = 0;
    for (uint16_t w = 0; w < version.words_per_chunk; ++w) {
        uint32_t word = 0;
        for (uint8_t block = 0; block < version.blocks_per_word; ++block) {
            if (position >= 4096) {
                break;
            }
            uint32_t state = blocks[position++];
            word |= state << (version.bits_per_block * block);
        }
        // I hope Big Endian never existed so that i could just cast the pointer
        buffer[write_offset++] = (uint8_t)(word & 0xff);
        buffer[write_offset++] = (uint8_t)((word >> 8) & 0xff);
        buffer[write_offset++] = (uint8_t)((word >> 16) & 0xff);
        buffer[write_offset++] = (uint8_t)((word >> 24) & 0xff);
    }
    write_zigzag32(buffer, &write_offset, palette_length);
    for (uint32_t i = 0; i < palette_length; ++i) {
        write_zigzag32(buffer, &write_offset, palette[i]);
    }
    block_storage_buffer_t block_storage_buffer = {buffer, write_offset};
    return block_storage_buffer;
}
