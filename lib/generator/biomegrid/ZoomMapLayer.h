#ifndef EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
#define EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_

#include "MapLayer.h"

enum ZoomType {
  NORMAL,
  BLURRY
};

using namespace GridBiome;

class ZoomMapLayer : public MapLayer {
 public:
  ZoomMapLayer(int_fast64_t seed, MapLayer *below_layer, ZoomType type = NORMAL)
      : MapLayer(seed), below_layer_(below_layer), zoom_type_(type) {}

  BlockValues GenerateValues(int x, int z, int size_x, int size_z) override;

 private:
  int getNearest(int upperLeftVal, int upperRightVal, int lowerLeftVal, int lowerRightVal);

  MapLayer *below_layer_;
  ZoomType zoom_type_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
