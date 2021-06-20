#ifndef EXT_NOISE_NORMAL_POPULATORS_H
#define EXT_NOISE_NORMAL_POPULATORS_H

#include "Misc.h"
#include "TerrainObjects.h"

#include <lib/pocketmine/BlockList.h>
#include <lib/pocketmine/Constants.h>
#include <lib/pocketmine/Random.h>
#include <lib/vanilla/Biome.h>

class Populator;
class Decorator;
class OrePopulator;
class LakeDecorator;
class DoublePlantDecorator;
class TreeDecorator;

class Populator {
public:
    virtual void populate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) = 0;
};

class BiomePopulator : public Populator {
public:
    BiomePopulator();

    void populate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) override {
        for (Populator *populator : inGroundPopulators) {
            populator->populate(chunk, random, chunkX, chunkZ);
        }

        for (Populator *populator : onGroundPopulators) {
            populator->populate(chunk, random, chunkX, chunkZ);
        }
    }

    virtual void initPopulators();

    void clean();

private:
    std::vector<Populator *> inGroundPopulators;
    std::vector<Populator *> onGroundPopulators;

    LakeDecorator *waterLakeDecorator;
    LakeDecorator *lavaLakeDecorator;
    OrePopulator *orePopulator;

    DoublePlantDecorator *doublePlantDecorator;
    TreeDecorator *treeDecorator;
};

class PlainsPopulator : public BiomePopulator {

};

class OrePopulator : public Populator {
public:
    OrePopulator();

    void populate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) override;

protected:
    void addOre(const OreType ore) {
        ores.emplace_back(ore);
    }

private:
    std::vector<OreType> ores;
};

// Decorator for BiomePopulator of inGroundPopulators

class Decorator : public Populator {
public:
    void setAmount(int amountVal) {
        amount = amountVal;
    }

    virtual void decorate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) = 0;

    void populate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) override {
        for (int i = 0; i < amount; ++i) {
            decorate(chunk, random, chunkX, chunkZ);
        }
    }

protected:
    int amount = 0;
};

class LakeDecorator : public Decorator {
public:
    LakeDecorator(MinecraftBlock mcBlock, int populatorRarity, int offset = 0) : block(mcBlock), rarity(populatorRarity), baseOffset(offset) {
        // NOOP
    }

    void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;

private:
    MinecraftBlock block;

    int rarity;
    int baseOffset = 0;
};

class DoublePlantDecorator : public Decorator {
public:
    void setDoublePlants(std::vector<DoublePlantDecoration> doublePlants);

    void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
private:
    MinecraftBlock getRandomDoublePlant(Random random);

    std::vector<DoublePlantDecoration> decorations;
};

class TreeDecorator : public Decorator {
public:
    void setTrees(std::vector<TreeDecoration> trees);

    void populate(SimpleChunkManager &chunk, Random &random, int chunkX, int chunkZ) override;

    void decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) override;
private:
    /**
     * Returns a callback to generate a GenericTree.
     */
    TreeObject getRandomTree(Random random);

    std::vector<TreeDecoration> decorations;
};
#endif //EXT_NOISE_NORMAL_POPULATORS_H
