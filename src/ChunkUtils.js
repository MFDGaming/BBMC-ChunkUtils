const chunk_utils = require('bindings')('chunk_utils')

class ChunkUtils {
    /**
     * Converts the blocks and palette to a buffer
     * @param {Array} blocks 
     * @param {Array} palette 
     */
    static writeBlockStorage(blocks, palette) {
        return chunk_utils.writeBlockStorage(blocks, palette);
    }
}

module.exports = ChunkUtils;
