#ifndef EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_OREVEIN_H_
#define EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_OREVEIN_H_

#include "TerrainObjects.h"
#include <lib/generator/overworld/populators/OrePopulator.h>

class OreVein {
 public:
  explicit OreVein(OreType oreType);

  bool Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) const;
 protected:
  /*
   * The square of the percentage of the radius that is the distance between the given block's
   * center and the center of an orthogonal ellipsoid. A block's center is inside the ellipsoid
   * if and only if its normalizedSquaredCoordinate values add up to less than 1.
   */
  static double NormalizedSquaredCoordinate(double origin, double radius, int_fast32_t x);

 private:
  OreType oreType_;
};

#endif //EXT_VANILLAGENERATOR_LIB_GENERATOR_OBJECTS_OREVEIN_H_
