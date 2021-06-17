#ifndef EXT_NOISELIB_MISC_H
#define EXT_NOISELIB_MISC_H

#include <lib/pocketmine/Random.h>
#include <lib/chunk/Chunk.h>

class OreType {
public:
    OreType(MinecraftBlock blockType, int minY, int maxY, int amountBlocks, int targetBlock, int value) : type(blockType), min_y(minY), max_y(maxY), amount(amountBlocks), target_type(targetBlock), total(value) {}

    MinecraftBlock getType() {
        return type;
    }

    int getMinY() {
        return min_y;
    }

    int getMaxY() {
        return max_y;
    }

    int getAmount() {
        return amount;
    }

    int getTargetType() {
        return target_type;
    }

    int getOreValues() {
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

#endif // EXT_NOISELIB_MISC_H
