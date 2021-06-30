#ifndef EXT_NOISELIB_LIB_VANILLA_PERLINNOISE_H_
#define EXT_NOISELIB_LIB_VANILLA_PERLINNOISE_H_

#include <lib/random/Random.h>
#include <vector>

class PerlinNoise {
 public:
  explicit PerlinNoise(Random &random);

  void getNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                   double scaleX, double scaleY, double scaleZ, double amplitude);

 protected:
  uint8_t permutations[512] = {};

  double offsetX;
  double offsetY;
  double offsetZ;

  virtual void get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                             double scaleX, double scaleZ, double amplitude);

  virtual void get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                             double scaleX, double scaleY, double scaleZ, double amplitude);
};

#endif //EXT_NOISELIB_LIB_VANILLA_PERLINNOISE_H_
