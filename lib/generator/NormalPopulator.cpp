#include <utility>

#include "NormalPopulators.h"

BiomePopulator::BiomePopulator() {
    waterLakeDecorator = new LakeDecorator(WATER, 4);
    lavaLakeDecorator = new LakeDecorator(LAVA, 8, 8);
    orePopulator = new OrePopulator();

    doublePlantDecorator = new DoublePlantDecorator();
    treeDecorator = new TreeDecorator();

    inGroundPopulators.emplace_back(waterLakeDecorator);
    inGroundPopulators.emplace_back(lavaLakeDecorator);
    //inGroundPopulators.emplace_back(surfaceCaveDecorator);
    //inGroundPopulators.emplace_back(dungeonPopulator);
    inGroundPopulators.emplace_back(orePopulator);
    //inGroundPopulators.add(sandPatchDecorator);
    //inGroundPopulators.add(clayPatchDecorator);
    //inGroundPopulators.add(gravelPatchDecorator);

    onGroundPopulators.emplace_back(doublePlantDecorator);
    onGroundPopulators.emplace_back(treeDecorator);
    //onGroundPopulators.add(desertWellDecorator);
    //onGroundPopulators.add(flowerDecorator);
    //onGroundPopulators.add(tallGrassDecorator);
    //onGroundPopulators.add(deadBushDecorator);
    //onGroundPopulators.add(brownMushroomDecorator);
    //onGroundPopulators.add(redMushroomDecorator);
    //onGroundPopulators.add(sugarCaneDecorator);
    //onGroundPopulators.add(pumpkinDecorator);
    //onGroundPopulators.add(cactusDecorator);
    //onGroundPopulators.add(flowingWaterDecorator);
    //onGroundPopulators.add(flowingLavaDecorator);
}

void BiomePopulator::clean() {
    inGroundPopulators.clear();
    onGroundPopulators.clear();

    delete waterLakeDecorator;
    delete lavaLakeDecorator;
    delete orePopulator;
    delete doublePlantDecorator;
    delete treeDecorator;
}

void BiomePopulator::initPopulators() {
    waterLakeDecorator->setAmount(1);
    lavaLakeDecorator->setAmount(1);

    // Follows GlowstoneMC's populators object values.
    doublePlantDecorator->setAmount(0);
    treeDecorator->setAmount(INT32_MIN);
    treeDecorator->setTrees({});
}

std::vector<uint_fast8_t> BiomePopulator::getBiomes() {
    return ALL_BIOMES;
}

void BiomePopulator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    for (Populator *populator : inGroundPopulators) {
        populator->populate(world, random, chunkX, chunkZ);
    }

    for (Populator *populator : onGroundPopulators) {
        populator->populate(world, random, chunkX, chunkZ);
    }
}

void LakeDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    if (random.nextBoundedInt(rarity) == 0) {
        int source_x, source_z, source_y;
        source_x = static_cast<int32_t>((chunkX << 4) + random.nextBoundedInt(16));
        source_z = static_cast<int32_t>((chunkZ << 4) + random.nextBoundedInt(16));
        source_y = static_cast<int32_t>(random.nextBoundedInt(world.getMaxY() - baseOffset) + baseOffset);
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

OrePopulator::OrePopulator() {
    addOre(OreType(DIRT, 0, 256, 32, 1, 10));
    addOre(OreType(DIRT, 0, 256, 32, 1, 10));
    addOre(OreType(GRAVEL, 0, 256, 32, 1, 8));
    addOre(OreType(GRANITE, 0, 80, 32, 1, 10));
    addOre(OreType(DIORITE, 0, 80, 32, 1, 10));
    addOre(OreType(ANDESITE, 0, 80, 32, 1, 10));
    addOre(OreType(COAL_ORE, 0, 128, 16, 1, 20));
    addOre(OreType(IRON_ORE, 0, 64, 8, 1, 20));
    addOre(OreType(GOLD_ORE, 0, 32, 8, 1, 2));
    addOre(OreType(REDSTONE_ORE, 0, 16, 7, 1, 8));
    addOre(OreType(DIAMOND_ORE, 0, 16, 7, 1, 1));
    addOre(OreType(LAPIS_LAZULI_ORE, 16, 16, 6, 1, 1));
}

void OrePopulator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    int cx, cz, source_x, source_y, source_z;
    cx = chunkX << 4;
    cz = chunkZ << 4;

    for (OreType oreType : ores) {
        for (int n = 0; n < oreType.getOreValues(); ++n) {
            source_x = cx + random.nextBoundedInt(16);
            source_z = cz + random.nextBoundedInt(16);
            source_y = oreType.getRandomHeight(random);

            auto oreData = OreVein(&oreType);
            oreData.generate(world, random, source_x, source_y, source_z);
        }
    }
}

void DoublePlantDecorator::setDoublePlants(std::vector<DoublePlantDecoration> doublePlants) {
    decorations = std::move(doublePlants);
}

void DoublePlantDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    auto chunk = world.getChunk(chunkX, chunkZ);

    int x = static_cast<int>(random.nextBoundedInt(16));
    int z = static_cast<int>(random.nextBoundedInt(16));
    int source_y = static_cast<int>(random.nextBoundedInt(chunk->getHighestBlockAt(x, z) + 32));

    MinecraftBlock species = getRandomDoublePlant(random);
    if (species.isObjectNull()){
        return;
    }

    DoubleTallPlant(species).generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z);
}

MinecraftBlock DoublePlantDecorator::getRandomDoublePlant(Random random) {
    int totalWeight = 0;
    for (auto deco : decorations) {
        totalWeight += deco.getWeight();
    }

    int weight = static_cast<int>(random.nextBoundedInt(totalWeight));
    for (auto deco : decorations) {
        weight -= deco.getWeight();
        if (weight < 0) {
            return deco.getBlock();
        }
    }

    return AIR;
}

void TreeDecorator::setTrees(std::vector<TreeDecoration> trees) {
    decorations = std::move(trees);
}

TreeObject TreeDecorator::getRandomTree(Random random) {
    int totalWeight = 0;
    for (auto deco : decorations) {
        totalWeight += deco.getWeight();
    }

    int weight = static_cast<int>(random.nextBoundedInt(totalWeight));
    for (auto deco : decorations) {
        weight -= deco.getWeight();
        if (weight < 0) {
            return deco.getBlock();
        }
    }

    return nullptr;
}

void TreeDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    auto chunk = world.getChunk(chunkX, chunkZ);

    int x = random.nextBoundedInt(16);
    int z = random.nextBoundedInt(16);
    int source_y = chunk->getHighestBlockAt(x, z);

    TreeObject treeObject = getRandomTree(random);
    if (treeObject != nullptr) {
        BlockTransaction txn = BlockTransaction(world);
        auto tree = treeObject(random, txn);

        if (tree.generate(world, random, (chunkX << 4) + x, source_y, (chunkZ << 4) + z)) {
            txn.applyBlockChanges();
        }

        txn.destroy();
    }
}

void TreeDecorator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
    int treeAmount = amount;
    if (random.nextBoundedInt(10) == 0) {
        ++treeAmount;
    }

    for (int i = 0; i < treeAmount; ++i) {
        decorate(world, random, chunkX, chunkZ);
    }
}
