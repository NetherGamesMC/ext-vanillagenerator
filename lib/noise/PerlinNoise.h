#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINNOISE_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINNOISE_H_

#include <lib/objects/random/Random.h>
#include <vector>

class PerlinNoise {
 public:
  explicit PerlinNoise(Random &random);

  void GetNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                   double scaleX, double scaleY, double scaleZ, double amplitude);

 protected:
  uint8_t permutations[512] = {};

  double offsetX;
  double offsetY;
  double offsetZ;

  virtual void Get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                             double scaleX, double scaleZ, double amplitude);

  virtual void Get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                             double scaleX, double scaleY, double scaleZ, double amplitude);
};

#endif //EXT_VANILLAGENERATOR_LIB_VANILLA_PERLINNOISE_H_
