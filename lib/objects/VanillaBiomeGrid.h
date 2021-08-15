#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_VANILLABIOMEGRID_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_VANILLABIOMEGRID_H_

#include <lib/generator/biomegrid/MapLayer.h>

using namespace GridBiome;

class VanillaBiomeGrid {
 public:
  explicit VanillaBiomeGrid(BiomeGrid biome) : biomes_(std::move(biome)) {}

  [[nodiscard]] int GetBiome(int x, int z) const;

  void SetBiome(int x, int z, int biomeId);

  void SetBiomes(BiomeGrid grid);
 private:
  BiomeGrid biomes_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_VANILLABIOMEGRID_H_
