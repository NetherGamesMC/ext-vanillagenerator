#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TERRAINOBJECTS_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TERRAINOBJECTS_H_

#include <lib/chunk/SimpleChunkManager.h>
#include <lib/random/Random.h>

class TerrainObjects {
 public:
  /**
   * Removes weak blocks like grass, shrub, flower or mushroom directly above the given block, if present.
   * Does not drop an item.
   *
   * @return bool whether a block was removed; false if none was present
   */
  static bool killWeakBlocksAbove(SimpleChunkManager &world, int_fast32_t x, int_fast32_t y, int_fast32_t z);

  virtual bool generate(SimpleChunkManager world,
                        Random &random,
                        int_fast32_t source_x,
                        int_fast32_t source_y,
                        int_fast32_t source_z) = 0;
};

#endif // EXT_NOISELIB_LIB_GENERATOR_OBJECTS_TERRAINOBJECTS_H_
