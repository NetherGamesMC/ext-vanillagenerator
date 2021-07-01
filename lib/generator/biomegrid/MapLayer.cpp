#include "MapLayer.h"
#include "NoiseMapLayer.h"
#include "WhittakerMapLayer.h"
#include "ZoomMapLayer.h"
#include "ErosionMapLayer.h"
#include "DeepOceanMapLayer.h"
#include "BiomeVariationMapLayer.h"
#include "BiomeMapLayer.h"
#include "BiomeEdgeMapLayer.h"
#include "RarePlainsMapLayer.h"
#include "BiomeThinEdgeMapLayer.h"
#include "ShoreMapLayer.h"
#include "RiverMapLayer.h"
#include "SmoothMapLayer.h"

namespace GridBiome {

int MapLayer::NextInt(int max) {
  return static_cast<int>(random_->nextInt(max));
}

void MapLayer::SetCoordsSeed(int x, int z) {
  random_->setSeed(seed_);
  random_->setSeed(x * random_->nextInt() + z * random_->nextInt() ^ seed_);
}

MapLayerPair initialize(int_fast64_t seed) {
  int zoom = 2;

  MapLayer *layer = new NoiseMapLayer(seed);

  layer = new WhittakerMapLayer(seed + 1, layer, WARM_WET);
  layer = new WhittakerMapLayer(seed + 1, layer, COLD_DRY);
  layer = new WhittakerMapLayer(seed + 1, layer, LARGER_BIOMES);

  for (int i = 0; i < 2; ++i) {
    layer = new ZoomMapLayer(seed + 100 + i, layer, BLURRY);
  }

  for (int i = 0; i < 2; i++) {
    layer = new ErosionMapLayer(seed + 3 + i, layer);
  }

  layer = new DeepOceanMapLayer(seed + 4, layer);

  MapLayer *layerMountains = new BiomeVariationMapLayer(seed + 200, layer, nullptr);
  for (int i = 0; i < 2; i++) {
    layerMountains = new ZoomMapLayer(seed + 200 + i, layerMountains);
  }

  layer = new BiomeMapLayer(seed + 5, layer);
  for (int i = 0; i < 2; i++) {
    layer = new ZoomMapLayer(seed + 200 + i, layer);
  }
  layer = new BiomeEdgeMapLayer(seed + 200, layer);
  layer = new BiomeVariationMapLayer(seed + 200, layer, layerMountains);
  layer = new RarePlainsMapLayer(seed + 201, layer);
  layer = new ZoomMapLayer(seed + 300, layer);
  layer = new ErosionMapLayer(seed + 6, layer);
  layer = new ZoomMapLayer(seed + 400, layer);
  layer = new BiomeThinEdgeMapLayer(seed + 400, layer);
  layer = new ShoreMapLayer(seed + 7, layer);
  for (int i = 0; i < zoom; i++) {
    layer = new ZoomMapLayer(seed + 500 + i, layer);
  }

  MapLayer *layerRiver = layerMountains;
  layerRiver = new ZoomMapLayer(seed + 300, layerRiver);
  layerRiver = new ZoomMapLayer(seed + 400, layerRiver);
  for (int i = 0; i < zoom; i++) {
    layerRiver = new ZoomMapLayer(seed + 500 + i, layerRiver);
  }
  layerRiver = new RiverMapLayer(seed + 10, layerRiver, nullptr);
  layer = new RiverMapLayer(seed + 1000, layerRiver, layer);

  MapLayer *layerLowerRes = layer;
  for (int i = 0; i < 2; i++) {
    layer = new ZoomMapLayer(seed + 2000 + i, layer);
  }

  layer = new SmoothMapLayer(seed + 1001, layer);

  return {layer, layerLowerRes};
}

}