#ifndef EXT_NOISELIB_CHUNK_H
#define EXT_NOISELIB_CHUNK_H

#include <new>
#include <array>
#include <cmath>
#include <map>

#include <lib/MortonHelper.h>
#include <PhpPalettedBlockArrayObj.h>

class BiomeArray {
public:
    static const size_t DATA_SIZE = 256;

    BiomeArray(const gsl::span<const uint8_t, DATA_SIZE> &values) {
        memcpy(mValues.data(), values.data(), sizeof(mValues));
    }

    int get(const uint8_t x, const uint8_t z) const {
        unsigned int offset;
        index(x, z, offset);

        return mValues[offset];
    }

    void set(const uint8_t x, const uint8_t z, const int value) {
        unsigned int offset;
        index(x, z, offset);

        mValues[offset] = value;
    }

private:
    std::array<uint8_t, DATA_SIZE> mValues;

    static inline void index(const uint8_t x, const uint8_t z, unsigned int &offset) {
        offset = (z << 4) | x;
    }

};

class MinecraftBlock {
public:
    explicit MinecraftBlock(Block block) {
        blockId = static_cast<int>(block) >> 4;
        meta = block & 0xf;
    }

    MinecraftBlock(int block, int blockMeta) : blockId(block), meta(blockMeta) {
        // NOOP
    }

    unsigned int getFullId() const {
        return (blockId << 4) | meta;
    }

    int getId() const {
        return blockId;
    }

    int getMeta() const {
        return meta;
    }

private:
    int blockId;
    uint8_t meta;
};

class Chunk {
public:
    Chunk(int64_t a, std::array<NormalBlockArrayContainer *, 16> &b, BiomeArray c) : biomeArray(c) {
        std::copy(std::begin(b), std::end(b), std::begin(blockLayer));

        morton2d_decode(a, chunkX, chunkZ);
    }

    NormalBlockArrayContainer *getSubChunk(int y) {
        if (y < 0 || y >= blockLayer.size()) {
            return nullptr;
        }

        return blockLayer[y];
    }

    void setFullBlock(int x, int y, int z, Block block) {
        BlockArrayContainer<Block> *subChunk;
        if((subChunk = getSubChunk(y >> 4)) == nullptr){
            throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " were not found");
        }

        subChunk->set(x, y & 0xf, z, block);
    }

    Block getFullBlock(int x, int y, int z) {
        BlockArrayContainer<Block> *subChunk;
        if((subChunk = getSubChunk(y >> 4)) == nullptr){
            throw std::invalid_argument("Subchunk y=" + std::to_string(y >> 4) + " were not found");
        }

        return subChunk->get(x, y & 0x0f, z);
    }

    BiomeArray getBiomeArray() const {
        return biomeArray;
    }

    int_fast64_t getX() const {
        return chunkX;
    }

    int_fast64_t getZ() const {
        return chunkZ;
    }

private:
    std::array<NormalBlockArrayContainer *, 16> blockLayer;
    BiomeArray biomeArray;

    int_fast64_t chunkX;
    int_fast64_t chunkZ;
};

class SimpleChunkManager {
public:
    SimpleChunkManager(const int_fast16_t minHeight, const int_fast16_t maxHeight) : minY(minHeight), maxY(maxHeight) {
        // NOOP
    }

    void setChunk(int64_t chunkX, int64_t chunkZ, Chunk *chunk) {
        chunks.insert({morton2d_encode(chunkX, chunkZ), chunk});
    }

    Chunk *getChunk(int64_t chunkX, int64_t chunkZ) {
        uint_fast64_t location = morton2d_encode(chunkX, chunkZ);

        auto searchResult = chunks.find(location);
        if (searchResult == chunks.end()) {
            return nullptr;
        }

        return searchResult->second;
    }

    MinecraftBlock getBlockAt(int x, int y, int z) {
        Chunk *chunk;
        if (isInWorld(x, y, z) && (chunk = getChunk(x >> 4, z >> 4)) != nullptr) {
            return MinecraftBlock(chunk->getFullBlock(x & 0xf, y, z & 0xf));
        }

        return MinecraftBlock((Block) 0);
    }

    void setBlockAt(int x, int y, int z, MinecraftBlock block) {
        Chunk *chunk;

        if ((chunk = getChunk(x >> 4, z >> 4)) != nullptr) {
            chunk->setFullBlock(x & 0xf, y, z & 0xf, block.getFullId());
        } else {
            throw std::invalid_argument("Cannot set block at coordinates x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z) + ", terrain is not loaded or out of bounds");
        }
    }

    bool isInWorld(int x, int y, int z) const {
        return x <= INT32_MAX && x >= INT32_MIN && y < maxY && y >= minY && z <= INT32_MAX && z >= INT32_MIN;
    }

    int_fast16_t getMinY() const {
        return minY;
    }

    int_fast16_t getMaxY() const {
        return maxY;
    }

    void clean() {
        for (auto data : chunks) {
            delete data.second;
        }

        chunks.clear();
    }

private:
    std::map<uint_fast64_t, Chunk *> chunks;

    int_fast16_t minY;
    int_fast16_t maxY;
};

#endif
