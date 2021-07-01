#ifndef EXT_NOISELIB_LIB_GENERATOR_VANILLABIOMEGRID_H_
#define EXT_NOISELIB_LIB_GENERATOR_VANILLABIOMEGRID_H_

#include <lib/generator/biomegrid/MapLayer.h>

using namespace GridBiome;

class VanillaBiomeGrid {
 public:
  explicit VanillaBiomeGrid(BiomeGrid biome) : biomes_(std::move(biome)) {}

  int getBiome(int x, int z);

  void setBiome(int x, int z, int biome_id);

  void setBiomes(BiomeGrid grid);
 private:
  BiomeGrid biomes_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_VANILLABIOMEGRID_H_
