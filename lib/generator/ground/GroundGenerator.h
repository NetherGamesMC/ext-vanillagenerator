#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_

#include <lib/objects/constants/BlockList.h>
#include <lib/chunk/ChunkManager.h>
#include <lib/objects/random/Random.h>

class GroundGenerator {
 public:
  virtual void GenerateTerrainColumn(ChunkManager &world,
                                     Random &random,
                                     int_fast64_t x,
                                     int_fast64_t z,
                                     int biome,
                                     double surface_noise);
 protected:

  template<typename Y>
  Y FuncMin(const Y &num1, const Y &num2) {
    return num1 <= num2 ? num1 : num2;
  }

  template<typename Y>
  Y FuncMax(const Y &num1, const Y &num2) {
    return num1 >= num2 ? num1 : num2;
  }

  MinecraftBlock top_material = GRASS;
  MinecraftBlock ground_material = DIRT;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
