#include "VanillaBiomeGrid.h"

int VanillaBiomeGrid::GetBiome(int x, int z) const {
  auto result = biomes_.find(x | z << 4);
  if (result == biomes_.end()) {
    return -1;
  }

  return result->second & 0xFF;
}

void VanillaBiomeGrid::SetBiome(int x, int z, int biomeId) {
  biomes_[x | z << 4] = biomeId;
}

void VanillaBiomeGrid::SetBiomes(BiomeGrid grid) {
  biomes_ = std::move(grid);
}