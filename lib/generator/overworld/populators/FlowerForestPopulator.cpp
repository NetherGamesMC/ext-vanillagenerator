#include <lib/generator/object/Flower.h>
#include <lib/objects/constants/BiomeList.h>
#include "FlowerForestPopulator.h"

const MinecraftBlock FlowerForestPopulator::flowers[10] = {
    POPPY,
    POPPY,
    DANDELION,
    ALLIUM,
    AZURE_BLUET,
    RED_TULIP,
    ORANGE_TULIP,
    WHITE_TULIP,
    PINK_TULIP,
    OXEYE_DAISY
};

void FlowerForestPopulator::InitPopulators() {
  ForestPopulator::InitPopulators();

  treeDecorator_.SetAmount(6);
  flowerDecorator_.SetAmount(0);

  doublePlantLoweringAmount = 1;

  noiseGen.SetScale((double) 1 / 48);
}

void FlowerForestPopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  ForestPopulator::OnGroundPopulation(world, random, chunkX, chunkZ);
  
  int_fast32_t sourceX = chunkX << 4;
  int_fast32_t sourceZ = chunkZ << 4;

  for (int_fast32_t i = 0; i < 100; i++) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t y = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(x, z) + 32));
    double noise = (noiseGen.Noise(x, z, 0.5, 0, 2.0, false) + 1.0) / 2.0;
    noise = noise < 0 ? 0 : noise > 0.9999 ? 0.9999 : noise;
    MinecraftBlock flower = flowers[static_cast<int_fast32_t>(noise * 9)];
    Flower(flower).Generate(world, random, x, y, z);
  }
}

std::vector<uint_fast8_t> FlowerForestPopulator::GetBiomes() const {
  return {FLOWER_FOREST};
}
