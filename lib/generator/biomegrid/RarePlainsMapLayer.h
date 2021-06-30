#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_

#include <lib/pocketmine/BiomeList.h>
#include "MapLayer.h"

using namespace GridBiome;

class RarePlainsMapLayer : public MapLayer {
 public:
  RarePlainsMapLayer(int_fast64_t seed, MapLayer &below_layer) : MapLayer(seed), below_layer_(below_layer) {}

  BlockValues GenerateValues(int x, int z, int sizeX, int sizeZ) override;
 private:
  MapLayer &below_layer_;

  const std::map<int, int> RARE_PLAINS = {{PLAINS, SUNFLOWER_PLAINS}};
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_RAREPLAINSMAPLAYER_H_
