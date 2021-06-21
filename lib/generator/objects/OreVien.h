#ifndef EXT_NOISELIB_LIB_GENERATOR_OBJECTS_OREVIEN_H_
#define EXT_NOISELIB_LIB_GENERATOR_OBJECTS_OREVIEN_H_

#include "TerrainObjects.h"
#include <lib/generator/populators/OrePopulator.h>

class OreVein : public TerrainObjects {
 public:
  explicit OreVein(OreType *oreType) : type(oreType) {}

  bool generate(SimpleChunkManager world,
                Random &random,
                int_fast32_t source_x,
                int_fast32_t source_y,
                int_fast32_t source_z) override;

 protected:
  /*
   * The square of the percentage of the radius that is the distance between the given block's
   * center and the center of an orthogonal ellipsoid. A block's center is inside the ellipsoid
   * if and only if its normalizedSquaredCoordinate values add up to less than 1.
   */
  static double normalizedSquaredCoordinate(double origin, double radius, int x) {
    double squared_normalized_x = (x + 0.5 - origin) / radius;
    squared_normalized_x *= squared_normalized_x;

    return squared_normalized_x;
  }

 private:
  OreType *type;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OBJECTS_OREVIEN_H_
