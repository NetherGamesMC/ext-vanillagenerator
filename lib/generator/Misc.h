#ifndef EXT_NOISELIB_MISC_H
#define EXT_NOISELIB_MISC_H

#include <lib/pocketmine/BlockList.h>
#include <lib/pocketmine/Random.h>
#include <lib/chunk/Chunk.h>

class OreType {
public:
    OreType(MinecraftBlock blockType, int minY, int maxY, int amountBlocks, int targetBlock, int value) : type(blockType), min_y(minY), max_y(maxY), amount(amountBlocks), target_type(targetBlock), total(value) {}

    MinecraftBlock getType() {
        return type;
    }

    int getMinY() const {
        return min_y;
    }

    int getMaxY() const {
        return max_y;
    }

    int getAmount() const {
        return amount;
    }

    int getTargetType() const {
        return target_type;
    }

    int getOreValues() const {
        return total;
    }

    /**
     * Generates a random height at which a vein of this ore can spawn.
     *
     * @param Random $random the PRNG to use
     *
     * @return int a random height for this ore
     */
    int getRandomHeight(Random &random) {
        return static_cast<int32_t>(min_y == max_y ? random.nextBoundedInt(min_y) + random.nextBoundedInt(min_y) : random.nextBoundedInt(max_y - min_y) + min_y);
    }

private:
    MinecraftBlock type;
    int min_y;
    int max_y;
    int amount;
    int total;
    int target_type = 1;
};

class DoublePlantDecoration {
public:
    DoublePlantDecoration(MinecraftBlock mcBlock, int _weight) : block(mcBlock), weight(_weight) {}

    MinecraftBlock getBlock() const {
        return block;
    }

    int getWeight() const {
        return weight;
    }

private:
    int weight;
    MinecraftBlock block;
};

typedef bool (*BlockValidators)(SimpleChunkManager &, int, int, int);

class BlockTransaction {
public:
    explicit BlockTransaction(SimpleChunkManager &manager) : world(manager) {
        addCallback(&verifyLocationIntegrity);
    }

    void addBlockAt(int x, int y, int z, MinecraftBlock *block) {
        blocks[morton3d_encode(x, y, z)] = block;
    }

    /**
     * @see BlockTransaction::fetchBlock()
     */
    MinecraftBlock fetchBlockAt(int x, int y, int z) {
        auto searchResult = blocks.find(morton3d_encode(x, y, z));
        if (searchResult == blocks.end()) {
            return world.getBlockAt(x, y, z);
        }

        return *searchResult->second;
    }

    /**
	 * Validates and attempts to apply the transaction to the given world. If any part of the transaction fails to
	 * validate, no changes will be made to the world.
	 *
	 * @return bool if the application was successful
	 */
    bool applyBlockChanges() {
        int64_t x, y, z;

        for (auto block : blocks) {
            morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

            for (BlockValidators validator : validators) {
                if (!validator(world, static_cast<int>(x), static_cast<int>(y), static_cast<int>(z))) {
                    return false;
                }
            }
        }

        for (auto block : blocks) {
            morton3d_decode(static_cast<int64_t>(block.first), x, y, z);

            world.setBlockAt(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), *block.second);
        }

        return true;
    }

    /**
	 * Add a validation predicate which will be used to validate every block.
	 * The callable signature should be the same as the below function.
	 */
    void addCallback(BlockValidators validator) {
        validators.emplace_back(validator);
    }

    void destroy() {
        for (auto block : blocks) {
            delete block.second;
        }

        blocks.clear();
    }

private:
    /**
     * This is a callback to verify the locations of the given coordinates. It does a
     * simple checks if the location is within the world radius.
     */
    static bool verifyLocationIntegrity(SimpleChunkManager &manager, int x, int y, int z) {
        return manager.isInWorld(x, y, z);
    }

    SimpleChunkManager world;

    std::vector<BlockValidators> validators;
    std::map<uint_fast64_t, MinecraftBlock *> blocks;
};

#endif // EXT_NOISELIB_MISC_H
