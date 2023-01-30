#include <napi.h>
#include <chunk_utils/writer.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

Napi::Buffer<uint8_t> writeBlockStorage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    Napi::Buffer<uint8_t> buffer;

    napi_status status;

    size_t argc = 2;
    napi_value args[2];
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    assert(status == napi_ok);

    if (argc < 2) {
        napi_throw_type_error(env, NULL, "invalid argument count");
        return buffer;
    }

    bool is_array = 0;
    status = napi_is_array(env, args[0], &is_array);
    assert(status == napi_ok);
    if (!is_array) {
        napi_throw_type_error(env, NULL, "blocks must be an array");
        return buffer;
    }

    status = napi_is_array(env, args[1], &is_array);
    assert(status == napi_ok);
    if (!is_array) {
        napi_throw_type_error(env, NULL, "palette must be an array");
        return buffer;
    }

    napi_value js_blocks = args[0];

    if (!js_blocks) {
        puts("block must not be null");
        return buffer;
    }

    napi_value js_palette = args[1];

    if (!js_palette) {
        puts("palette must not be null");
        return buffer;
    }

    uint32_t blocks_length;
    status = napi_get_array_length(env, js_blocks, &blocks_length);
    assert(status == napi_ok);

    uint32_t palette_length;
    status = napi_get_array_length(env, js_palette, &palette_length);
    assert(status == napi_ok);

    uint32_t *blocks = (uint32_t *)malloc(blocks_length * sizeof(uint32_t));

    uint32_t *palette = (uint32_t *)malloc(palette_length * sizeof(uint32_t));

    for (uint32_t i = 0; i < blocks_length; ++i) {
        napi_value ret;
        int32_t value;

        napi_get_element(env, js_blocks, i, &ret);
        napi_get_value_int32(env, ret, &value);

        blocks[i] = value;
    }

    for (uint32_t i = 0; i < palette_length; ++i) {
        napi_value ret;
        int32_t value;

        napi_get_element(env, js_palette, i, &ret);
        napi_get_value_int32(env, ret, &value);

        palette[i] = value;
    }

    block_storage_buffer_t block_storage_buffer = write_block_storage(blocks, blocks_length, palette, palette_length);

    buffer = Napi::Buffer<uint8_t>::Copy(env, block_storage_buffer.buffer, block_storage_buffer.size);

    free(blocks);
    free(palette);
    free(block_storage_buffer.buffer);

    return buffer;
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "writeBlockStorage"), Napi::Function::New(env, writeBlockStorage));
    return exports;
};

NODE_API_MODULE(chunk_utils, init);