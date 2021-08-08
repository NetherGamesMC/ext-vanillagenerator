#ifndef EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
#define EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_

#include <cstdint>

struct Vector3 {
  int_fast32_t x;
  int_fast32_t y;
  int_fast32_t z;
};

Vector3 GetSide(Vector3 vVector, int side, int step);

Vector3 Subtract(Vector3 origin, Vector3 from);

#endif // EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
