#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_

#include <lib/noise/octaves/SimplexOctaveGenerator.h>
#include "GroundGenerator.h"

typedef std::array<int, 64> ColourLayer;

enum MesaType {
  NORMAL,
  BRYCE,
  FOREST_TYPE,
};

class MesaGroundGenerator : public GroundGenerator {
 public:
  MesaGroundGenerator(MesaType type = NORMAL);

  void GenerateTerrainColumn(ChunkManager &world, Random &random, int_fast32_t x, int_fast32_t z, int biome, double surfaceNoise) override;

 private:
  void Initialize(int_fast64_t seed);

  void SetRandomLayerColor(Random &random, int minLayerCount, int minLayerHeight, int color);

  void InitializeColorLayers(Random &random);

  static void SetColoredGroundLayer(ChunkManager &man, int_fast32_t x, int_fast32_t y, int_fast32_t z, int color);

  int_fast64_t seed_ = 0;

  SimplexOctaveGenerator colorNoise_ = {random_, 1, 0, 0, 0};
  SimplexOctaveGenerator canyonHeightNoise_ = {random_, 4, 0, 0, 0};
  SimplexOctaveGenerator canyonScaleNoise_ = {random_, 1, 0, 0, 0};
  Random random_ = {seed_};

  ColourLayer colourLayer_ = {};
  MesaType type_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_
