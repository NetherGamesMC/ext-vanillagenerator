#include <lib/objects/constants/BiomeList.h>
#include "TaigaPopulator.h"

void TaigaPopulator::InitPopulators() {
  doublePlantDecorator_.SetAmount(7);
  doublePlantDecorator_.SetDoublePlants({{1, LARGE_FERN}});
  treeDecorator_.SetAmount(10);
  treeDecorator_.SetTrees({{2, redwoodTree}, {1, tallRedwoodTree}});
  tallGrassDecorator_.SetFernDensity(0.8);
  deadBushDecorator_.SetAmount(1);
  taigaBrownMushroomDecorator.SetAmount(1);
  taigaBrownMushroomDecorator.SetUseFixedHeightRange();
  taigaBrownMushroomDecorator.SetDensity(0.25);
  taigaRedMushroomDecorator.SetAmount(1);
  taigaRedMushroomDecorator.SetDensity(0.125);
}

std::vector<uint_fast8_t> TaigaPopulator::GetBiomes() const {
  return {TAIGA, TAIGA_HILLS, TAIGA_MOUNTAINS, COLD_TAIGA, COLD_TAIGA_HILLS, COLD_TAIGA_MOUNTAINS};
}

void TaigaPopulator::OnGroundPopulation(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  BiomePopulator::OnGroundPopulation(world, random, chunkX, chunkZ);

  taigaBrownMushroomDecorator.Decorate(world, random, chunkX, chunkZ);
  taigaRedMushroomDecorator.Decorate(world, random, chunkX, chunkZ);
}
