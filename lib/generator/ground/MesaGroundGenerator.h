#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_

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
  explicit MesaGroundGenerator(MesaType type = NORMAL) : type_(type) {
    colourLayer_.fill(-1); // Hardened clay

    topMaterial = RED_SAND;
    groundMaterial = MinecraftBlock(STAINED_CLAY.GetId(), 1); // Orange block
  }

  ~MesaGroundGenerator();

  void GenerateTerrainColumn(ChunkManager &world,
                             Random &random,
                             int_fast32_t x,
                             int_fast32_t z,
                             int biome,
                             double surfaceNoise) override;

 private:
  void Initialize(int_fast32_t seed);

  void SetRandomLayerColor(Random &random, int minLayerCount, int minLayerHeight, int color);

  void InitializeColorLayers(Random &random);

  static void SetColoredGroundLayer(ChunkManager &man, int_fast32_t x, int_fast32_t y, int_fast32_t z, int color);

  int_fast32_t seed_ = 0;

  SimplexOctaveGenerator *colorNoise_ = nullptr;
  SimplexOctaveGenerator *canyonHeightNoise_ = nullptr;
  SimplexOctaveGenerator *canyonScaleNoise_ = nullptr;
  Random *random_ = nullptr;

  ColourLayer colourLayer_ = {};
  MesaType type_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_
