#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_

#include "MapLayer.h"

using namespace GridBiome;

struct Climate {
  int value;
  int final_value;

  std::vector<int> cross_types;
};

enum ClimateType { WARM_WET, COLD_DRY, LARGER_BIOMES };

class WhittakerMapLayer : public MapLayer {
 public:
  WhittakerMapLayer(int_fast64_t seed, MapLayer *belowLayer, ClimateType type)
      : MapLayer(seed), below_layer_(belowLayer), type_(type) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;

  BlockValues SwapValues(int x, int z, int size_x, int size_z);

  BlockValues ModifyValues(int x, int z, int size_x, int size_z);
 private:
  ClimateType type_;
  MapLayer *below_layer_;

  const std::vector<Climate> climate_array_ = {
      {2, 4, {3, 1}},
      {3, 1, {2, 4}}
  };
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_
