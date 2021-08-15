#ifndef EXT_VANILLAGENERATOR_LIB_VANILLA_UTILS_H_
#define EXT_VANILLAGENERATOR_LIB_VANILLA_UTILS_H_

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

  template<typename Y>
  static Y Min(const Y &num1, const Y &num2) {
    return num1 <= num2 ? num1 : num2;
  }

  template<typename Y>
  static Y Max(const Y &num1, const Y &num2) {
    return num1 >= num2 ? num1 : num2;
  }
};

#endif // EXT_VANILLAGENERATOR_LIB_VANILLA_UTILS_H_
