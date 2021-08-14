#include <lib/objects/constants/BiomeList.h>
#include <lib/generator/object/DoubleTallPlant.h>
#include <lib/generator/object/Flower.h>
#include <lib/generator/object/TallGrass.h>
#include "PlainsPopulator.h"

const MinecraftBlock PlainsPopulator::flowers[3] = {
    POPPY,
    AZURE_BLUET,
    OXEYE_DAISY,
};

const MinecraftBlock PlainsPopulator::tulips[4] = {
    RED_TULIP,
    ORANGE_TULIP,
    WHITE_TULIP,
    PINK_TULIP,
};

void PlainsPopulator::InitPopulators() {
  BiomePopulator::InitPopulators();
}

std::vector<uint_fast8_t> PlainsPopulator::GetBiomes() const {
  return {PLAINS};
}

void PlainsPopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t sourceX = chunkX << 4;
  int_fast32_t sourceZ = chunkZ << 4;

  int_fast32_t flowerAmount = 15;
  int_fast32_t tallGrassAmount = 5;
  if (noiseGen.Noise(sourceX + 8, sourceZ + 8, 0, 0.5, 2.0, false) >= -0.8) {
    flowerAmount = 4;
    tallGrassAmount = 10;
    for (int_fast32_t i = 0; i < 7; i++) {
      int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(16));
      int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(16));
      int_fast32_t y = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(x, z) + 32));
      DoubleTallPlant(DOUBLE_TALLGRASS).Generate(world, random, x, y, z);
    }
  }

  MinecraftBlock flower = DANDELION;
  if (noiseGen.Noise(sourceX + 8, sourceZ + 8, 0, 0.5, 2.0, false) < -0.8) {
    flower = tulips[random.NextInt(2)];
  } else if (random.NextInt(3) > 0) {
    flower = flowers[random.NextInt(3)];
  }

  for (int_fast32_t i = 0; i < flowerAmount; i++) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t y = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(x, z) + 32));
    Flower(flower).Generate(world, random, x, y, z);
  }

  for (int_fast32_t i = 0; i < tallGrassAmount; i++) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t y = static_cast<int_fast32_t>(random.NextInt(world.GetHighestElevationAt(x, z) << 1));
    TallGrass(TALL_GRASS).Generate(world, random, x, y, z);
  }

  BiomePopulator::OnGroundPopulation(world, random, chunkX, chunkZ);
}

