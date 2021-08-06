#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_


#include <utility>
#include "MapLayer.h"

enum ZoomType {
  NORMAL,
  BLURRY
};

using namespace GridBiome;

class ZoomMapLayer : public MapLayer {
 public:
  ZoomMapLayer(int_fast32_t seed, std::shared_ptr<MapLayer> below_layer, ZoomType type = NORMAL)
      : MapLayer(seed), below_layer_(std::move(below_layer)), zoom_type_(type) {}

  ~ZoomMapLayer();

  BiomeGrid GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  int getNearest(int upperLeftVal, int upperRightVal, int lowerLeftVal, int lowerRightVal);

  std::shared_ptr<MapLayer> below_layer_;
  ZoomType zoom_type_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
