#ifndef EXT_NOISELIB_LIB_VANILLA_UTILS_H_
#define EXT_NOISELIB_LIB_VANILLA_UTILS_H_

#include <cstdint>

class Math {
 public:
  static int floor(double x);

  static double lerp(double delta, double start, double end);

  static double grad(int i, double d, double d1, double d2);

  static double fade(double x);

  static int floorSimplex(double x);

  static long floorLong(double x);

  static double dot(const int8_t g[3], double x, double y);

  static double dot(const int8_t g[3], double x, double y, double z);
};


#endif // EXT_NOISELIB_LIB_VANILLA_UTILS_H_
