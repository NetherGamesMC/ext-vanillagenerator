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
  return static_cast<int>(random_.NextInt(max));
}

void MapLayer::SetCoordsSeed(int x, int z) {
  random_.SetSeed(seed_);
  random_.SetSeed(x * random_.NextInt() + z * random_.NextInt() ^ seed_);
}

MapLayerPair Initialize(int_fast64_t seed, bool isUHC) {
  int zoom = 2;

  std::shared_ptr<MapLayer> layer = std::shared_ptr<MapLayer>(new NoiseMapLayer(seed));

  layer = std::shared_ptr<MapLayer>(new WhittakerMapLayer(seed + 1, layer, WARM_WET));
  layer = std::shared_ptr<MapLayer>(new WhittakerMapLayer(seed + 1, layer, COLD_DRY));
  layer = std::shared_ptr<MapLayer>(new WhittakerMapLayer(seed + 1, layer, LARGER_BIOMES));

  for (int i = 0; i < 2; ++i) {
    layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 100 + i, layer, BLURRY));
  }

  for (int i = 0; i < 2; i++) {
    layer = std::shared_ptr<MapLayer>(new ErosionMapLayer(seed + 3 + i, layer));
  }

  if (!isUHC) {
    layer = std::shared_ptr<MapLayer>(new DeepOceanMapLayer(seed + 4, layer));
  }

  std::shared_ptr<MapLayer> layerMountains = std::shared_ptr<MapLayer>(
      new BiomeVariationMapLayer(seed + 200, layer, nullptr, isUHC));

  for (int i = 0; i < 2; i++) {
    layerMountains = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 200 + i, layerMountains));
  }

  layer = std::shared_ptr<MapLayer>(new BiomeMapLayer(seed + 5, layer, isUHC));
  for (int i = 0; i < 2; i++) {
    layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 200 + i, layer));
  }
  layer = std::shared_ptr<MapLayer>(new BiomeEdgeMapLayer(seed + 200, layer, isUHC));
  layer = std::shared_ptr<MapLayer>(new BiomeVariationMapLayer(seed + 200, layer, layerMountains, isUHC));
  layer = std::shared_ptr<MapLayer>(new RarePlainsMapLayer(seed + 201, layer));
  layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 300, layer));
  layer = std::shared_ptr<MapLayer>(new ErosionMapLayer(seed + 6, layer));
  layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 400, layer));
  layer = std::shared_ptr<MapLayer>(new BiomeThinEdgeMapLayer(seed + 400, layer, isUHC));
  layer = std::shared_ptr<MapLayer>(new ShoreMapLayer(seed + 7, layer, isUHC));
  for (int i = 0; i < zoom; i++) {
    layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 500 + i, layer));
  }

  std::shared_ptr<MapLayer> layerRiver = layerMountains;
  layerRiver = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 300, layerRiver));
  layerRiver = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 400, layerRiver));
  for (int i = 0; i < zoom; i++) {
    layerRiver = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 500 + i, layerRiver));
  }
  layerRiver = std::shared_ptr<MapLayer>(new RiverMapLayer(seed + 10, layerRiver, nullptr, isUHC));
  layer = std::shared_ptr<MapLayer>(new RiverMapLayer(seed + 1000, layerRiver, layer, isUHC));

  std::shared_ptr<MapLayer> layerLowerRes = layer;
  for (int i = 0; i < 2; i++) {
    layer = std::shared_ptr<MapLayer>(new ZoomMapLayer(seed + 2000 + i, layer));
  }

  layer = std::shared_ptr<MapLayer>(new SmoothMapLayer(seed + 1001, layer));

  return {layer, layerLowerRes};
}

}