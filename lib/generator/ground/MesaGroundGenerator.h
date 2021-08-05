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
    colour_layer_.fill(-1); // Hardened clay

    top_material = RED_SAND;
    ground_material = MinecraftBlock(STAINED_CLAY.getId(), 1); // Orange block
  }

  ~MesaGroundGenerator();

  void GenerateTerrainColumn(ChunkManager &world,
                             Random &random,
                             int_fast64_t x,
                             int_fast64_t z,
                             int biome,
                             double surface_noise) override;

 private:
  void Initialize(int_fast64_t seed);

  void SetRandomLayerColor(Random &random, int min_layer_count, int min_layer_height, int color);

  void InitializeColorLayers(Random &random);

  static void SetColoredGroundLayer(ChunkManager &man, int_fast64_t x, int_fast32_t y, int_fast64_t z, int color);

  int_fast64_t seed_ = 0;

  SimplexOctaveGenerator *color_noise_ = nullptr;
  SimplexOctaveGenerator *canyon_height_noise_ = nullptr;
  SimplexOctaveGenerator *canyon_scale_noise_ = nullptr;
  Random *random_ = nullptr;

  ColourLayer colour_layer_ = {};
  MesaType type_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_GROUND_MESAGROUNDGENERATOR_H_
