#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_


#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

struct Climate {
  int value;
  int finalValue;

  std::vector<int> crossTypes;
};

enum ClimateType { WARM_WET, COLD_DRY, LARGER_BIOMES };

class WhittakerMapLayer : public MapLayer {
 public:
  WhittakerMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, ClimateType type)
      : MapLayer(seed), belowLayer_(std::move(belowLayer)), type_(type) {}

  ~WhittakerMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;

  BiomeGrid SwapValues(int x, int z, int size_x, int size_z);

  BiomeGrid ModifyValues(int x, int z, int size_x, int size_z);
 private:
  ClimateType type_;
  std::shared_ptr<MapLayer> belowLayer_;

  const std::vector<Climate> climateArray_ = {
      {2, 4, {3, 1}},
      {3, 1, {2, 4}}
  };
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_WHITTAKERMAPLAYER_H_
