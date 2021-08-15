#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEVARIATIONMAPLAYER_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEVARIATIONMAPLAYER_H_

#include <lib/objects/constants/BiomeList.h>

#include <utility>
#include "MapLayer.h"

using namespace GridBiome;

class BiomeVariationMapLayer : public MapLayer {
 public:
  BiomeVariationMapLayer(int_fast64_t seed, std::shared_ptr<MapLayer> belowLayer, std::shared_ptr<MapLayer> variationLayer, bool isUHC)
      : MapLayer(seed),
        belowLayer_(std::move(belowLayer)),
        variationLayer_(std::move(variationLayer)),
        is_uhc_(isUHC) {
    if (isUHC) {
      variations_ = {
          {DESERT, {DESERT_HILLS}},
          {FOREST, {FOREST_HILLS}},
          {BIRCH_FOREST, {BIRCH_FOREST_HILLS}},
          {ROOFED_FOREST, {PLAINS}},
          {TAIGA, {TAIGA_HILLS}},
          {COLD_TAIGA, {COLD_TAIGA_HILLS}},
          {PLAINS, {FOREST, FOREST, FOREST_HILLS}},
          {ICE_PLAINS, {ICE_MOUNTAINS}},
          {EXTREME_HILLS, {EXTREME_HILLS_PLUS}},
          {SAVANNA, {SAVANNA_PLATEAU}},
          {MESA_PLATEAU_FOREST, {MESA}},
          {MESA_PLATEAU, {MESA}},
          {MESA, {MESA}}
      };
    }
  }

  ~BiomeVariationMapLayer();

  BiomeGrid GenerateValues(int x, int z, int sizeX, int sizeZ) override;

 private:
  /**
   * Generates a rectangle, replacing all the positive values in the previous layer with random
   * values from 2 to 31 while leaving zero and negative values unchanged.
   *
   * @param x the lowest x coordinate
   * @param z the lowest z coordinate
   * @param size_x the x coordinate range
   * @param size_z the z coordinate range
   * @return a flattened array of generated values
   */
  BiomeGrid GenerateRandomValues(int x, int z, int sizeX, int sizeZ);

  /**
   * Generates a rectangle using the previous layer and the variation layer.
   *
   * @param x the lowest x coordinate
   * @param z the lowest z coordinate
   * @param size_x the x coordinate range
   * @param size_z the z coordinate range
   * @return a flattened array of generated values
   */
  BiomeGrid MergeValues(int x, int z, int sizeX, int sizeZ);

  std::shared_ptr<MapLayer> belowLayer_;
  std::shared_ptr<MapLayer> variationLayer_;

  bool is_uhc_;

  const std::vector<int> islands_ = {PLAINS, FOREST};

  std::map<int, std::vector<int>> variations_ = {
      {DESERT, {DESERT_HILLS}},
      {FOREST, {FOREST_HILLS}},
      {BIRCH_FOREST, {BIRCH_FOREST_HILLS}},
      {ROOFED_FOREST, {PLAINS}},
      {TAIGA, {TAIGA_HILLS}},
      {MEGA_TAIGA, {MEGA_TAIGA_HILLS}},
      {COLD_TAIGA, {COLD_TAIGA_HILLS}},
      {PLAINS, {FOREST, FOREST, FOREST_HILLS}},
      {ICE_PLAINS, {ICE_MOUNTAINS}},
      {JUNGLE, {JUNGLE_HILLS}},
      {OCEAN, {DEEP_OCEAN}},
      {EXTREME_HILLS, {EXTREME_HILLS_PLUS}},
      {SAVANNA, {SAVANNA_PLATEAU}},
      {MESA_PLATEAU_FOREST, {MESA}},
      {MESA_PLATEAU, {MESA}},
      {MESA, {MESA}}
  };
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_BIOMEGRID_BIOMEVARIATIONMAPLAYER_H_
