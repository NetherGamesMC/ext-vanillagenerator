#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <array>
#include <PhpPalettedBlockArrayObj.h>

/**
 * An implementation of PocketMine-MP SubChunk class in C++
 * <p>
 * This SubChunk implementation is a bit different than the one in PocketMine-MP, we simply do not retrieve LightArray
 * because do not use them, this chunk supposedly place blocks in an adjacent coordinates.
 */
class SubChunk {
public:
    SubChunk(Block emptyBlockId, std::array<NormalBlockArrayContainer, 1> &blocks) : blockLayer(blocks), emptyBlockId(emptyBlockId) {
        // NOOP
    }

    Block getFullBlock(int x, int y, int z) {
        if (blockLayer.empty()) {
            return emptyBlockId;
        }

        return blockLayer[0].get(x, y, z);
    }

    void setFullBlock(int x, int y, int z, Block block) {
        if (blockLayer.empty()) {
            blockLayer.fill(NormalBlockArrayContainer(emptyBlockId));
        }

        blockLayer[0].set(x, y, z, block);
    }

    NormalBlockArrayContainer getContainer() {
        if (blockLayer.empty()) {
            return null;
        }

        return blockLayer[0];
    }

private:
    Block emptyBlockId;
    std::array<NormalBlockArrayContainer, 1> blockLayer;
};

#endif
