#ifndef EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXNOISEGENERATOR_H_
#define EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXNOISEGENERATOR_H_

#include <lib/pocketmine/Random.h>

class BukkitSimplexNoiseGenerator {
 public:

  explicit BukkitSimplexNoiseGenerator(Random &random);

  double simplex2D(double xin, double yin);

 private:
  int8_t grad3[12][3] = {
      {1,  1,  0},
      {-1, 1,  0},
      {1,  -1, 0},
      {-1, -1, 0},
      {1,  0,  1},
      {-1, 0,  1},
      {1,  0,  -1},
      {-1, 0,  -1},
      {0,  1,  1},
      {0,  -1, 1},
      {0,  1,  -1},
      {0,  -1, -1}
  };

  uint8_t permutations[512] = {};

  double offsetX;
  double offsetY;
};


#endif //EXT_NOISELIB_LIB_VANILLA_BUKKITSIMPLEXNOISEGENERATOR_H_
