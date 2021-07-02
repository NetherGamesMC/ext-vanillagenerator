#ifndef EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_

#include <lib/pocketmine/BlockList.h>
#include <lib/chunk/SimpleChunkManager.h>
#include <lib/random/Random.h>

class GroundGenerator {
 public:
  virtual void GenerateTerrainColumn(SimpleChunkManager &world,
                             Random &random,
                             int_fast64_t x,
                             int_fast64_t z,
                             int biome,
                             double surface_noise);

 protected:
  MinecraftBlock top_material = GRASS;
  MinecraftBlock ground_material = DIRT;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_GROUND_GROUNDGENERATOR_H_
