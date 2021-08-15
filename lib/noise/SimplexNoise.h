#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXNOISE_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXNOISE_H_

#include <lib/objects/random/Random.h>
#include "PerlinNoise.h"

class SimplexNoise : public PerlinNoise {
 public:
  explicit SimplexNoise(Random &random);

  double Noise3d(double d, double d1, double d2);

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

  void Get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                     double scaleX, double scaleZ, double amplitude) override;

  void Get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ, double scaleX,
                     double scaleY, double scaleZ, double amplitude) override;

 private:
  double Simplex2d(double xin, double yin);

  double Simplex3d(double xin, double yin, double zin);
};

#endif //EXT_VANILLAGENERATOR_LIB_VANILLA_SIMPLEXNOISE_H_
