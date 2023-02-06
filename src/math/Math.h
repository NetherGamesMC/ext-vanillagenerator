#ifndef VANILLAGENERATOR_SRC_MATH_MATH_H_
#define VANILLAGENERATOR_SRC_MATH_MATH_H_

#include <cstdint>

// Compatibility reason, some applications other than PHP will not have this type definition.
#ifndef ZEND_LONG_DEFINITION
#define ZEND_LONG_DEFINITION
typedef int32_t zend_long;
#endif

namespace math {

// Math related classes
class Math {
 public:
  static uint_fast64_t morton2d_encode(int64_t x, int64_t y);
  static void morton2d_decode(int64_t chunkCord, int64_t &chunkX, int64_t &chunkZ);

  static uint_fast64_t morton3d_encode(int_fast64_t x, int_fast32_t y, int_fast64_t z);
  static void morton3d_decode(uint_fast64_t chunkCord, int_fast32_t &rx, int_fast32_t &ry, int_fast32_t &rz);
};

}

#endif //VANILLAGENERATOR_SRC_MATH_MATH_H_
