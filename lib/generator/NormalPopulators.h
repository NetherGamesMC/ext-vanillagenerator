#ifndef EXT_NOISE_NORMAL_POPULATORS_H
#define EXT_NOISE_NORMAL_POPULATORS_H

#include "TerrainObjects.h"
#include "lib/pocketmine/Constants.h"
#include "lib/pocketmine/Random.h"

class Decorator {
public:
    virtual void decorate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) = 0;
};

class LakeDecorator : public Decorator {
public:
    LakeDecorator(MinecraftBlock mcBlock, int populatorRarity) : block(mcBlock), rarity(populatorRarity) {
        // NOOP
    }

    void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override {
        if (random.nextBoundedInt(rarity) == 0) {
            int source_x, source_z, source_y;
            source_x = (chunkX << 4) + random.nextBoundedInt(16);
            source_z = (chunkZ << 4) + random.nextBoundedInt(16);
            source_y = random.nextBoundedInt(world.getMaxY() - baseOffset) + baseOffset;
            if (block.getId() == 11 && (source_y >= 64 || random.nextBoundedInt(10) > 0)) {
                return;
            }

            while (world.getBlockAt(source_x, source_y, source_z).getId() == 0 && source_y > 5) {
                --source_y;
            }

            if (source_y >= 5) {
                Lake lake = Lake(block);

                lake.generate(world, random, source_x, source_y, source_z);
            }
        }
    }

private:
    MinecraftBlock block;
    int rarity;
    int baseOffset = 0;
};

#endif //EXT_NOISE_NORMAL_POPULATORS_H
