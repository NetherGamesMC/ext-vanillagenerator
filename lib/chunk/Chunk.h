#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <new>
#include <array>
#include <PhpPalettedBlockArrayObj.h>

/**
 * An implementation of PocketMine-MP SimpleChunkManager class in C++
 * <p>
 * This SubChunk implementation is a bit different than the one in PocketMine-MP, we simply do not retrieve LightArray
 * because do not use them, this chunk supposedly place blocks in an adjacent coordinates.
 */
class Chunk {
public:
    Chunk(std::vector<NormalBlockArrayContainer *> &blocks) : blockLayer(blocks) {
        // NOOP
    }

    NormalBlockArrayContainer *getSubChunk(int y) {
        if (y < 0 || y >= blockLayer.size()) {
            return nullptr;
        }

        return blockLayer[y];
    }

    void setFullBlock(int x, int y, int z, Block block) {
        getSubChunk(y >> 4)->set(x, y & 0xf, z, block);
    }

    Block getFullBlock(int x, int y, int z) {
        return getSubChunk(y >> 4)->get(x, y & 0x0f, z);
    }

private:
    std::vector<NormalBlockArrayContainer *> &blockLayer;
};

#endif
