#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_


#include <utility>
#include "MapLayer.h"

enum ZoomType {
  NORMAL,
  BLURRY
};

using namespace GridBiome;

class ZoomMapLayer : public MapLayer {
 public:
  ZoomMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, ZoomType type = NORMAL)
      : MapLayer(seed), belowLayer_(std::move(belowLayer)), zoomType_(type) {}

  ~ZoomMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  int getNearest(int upperLeftVal, int upperRightVal, int lowerLeftVal, int lowerRightVal);

  std::shared_ptr<MapLayer> belowLayer_;
  ZoomType zoomType_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_ZOOMMAPLAYER_H_
