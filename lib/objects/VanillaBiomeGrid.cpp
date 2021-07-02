#include "VanillaBiomeGrid.h"

int VanillaBiomeGrid::getBiome(int x, int z) const {
  auto result = biomes_.find(x | z << 4);
  if (result == biomes_.end()) {
    return -1;
  }

  return result->second & 0xFF;
}

void VanillaBiomeGrid::setBiome(int x, int z, int biome_id) {
  biomes_[x | z << 4] = biome_id;
}

void VanillaBiomeGrid::setBiomes(BiomeGrid grid) {
  biomes_ = std::move(grid);
}