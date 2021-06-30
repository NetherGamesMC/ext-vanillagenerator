#ifndef EXT_NOISELIB_LIB_VANILLA_SIMPLEXNOISE_H_
#define EXT_NOISELIB_LIB_VANILLA_SIMPLEXNOISE_H_

#include <lib/random/Random.h>
#include "PerlinNoise.h"

class SimplexNoise : public PerlinNoise {
 public:
  explicit SimplexNoise(Random &random);

  double noise3d(double d, double d1, double d2);

 protected:
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

  uint8_t permMod12[512] = {};

  void get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                     double scaleX, double scaleZ, double amplitude) override;

  void get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ, double scaleX,
                     double scaleY, double scaleZ, double amplitude) override;

 private:
  double simplex2D(double xin, double yin);

  double simplex3D(double xin, double yin, double zin);
};

#endif //EXT_NOISELIB_LIB_VANILLA_SIMPLEXNOISE_H_
