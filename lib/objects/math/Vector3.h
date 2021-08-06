#ifndef EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
#define EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_

#include <cstdint>

struct Vector3 {
  int_fast32_t x;
  int_fast32_t y;
  int_fast32_t z;
};

Vector3 getSide(Vector3 vVector, int side, int step);

#endif // EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
