#ifndef EXT_NOISELIB_LIB_VANILLA_UTILS_H_
#define EXT_NOISELIB_LIB_VANILLA_UTILS_H_

#include <cstdint>

class Math {
 public:
  static int Floor(double x);

  static double Lerp(double delta, double start, double end);

  static double Grad(int i, double d, double d1, double d2);

  static double Fade(double x);

  static int FloorSimplex(double x);

  static long FloorLong(double x);

  static double Dot(const int8_t g[3], double x, double y);

  static double Dot(const int8_t g[3], double x, double y, double z);
};


#endif // EXT_NOISELIB_LIB_VANILLA_UTILS_H_
