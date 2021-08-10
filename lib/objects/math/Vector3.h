#ifndef EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
#define EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_

#include <cstdint>

class Vector3 {
 public:
  Vector3(double x, double y, double z);
  Vector3(int x, int y, int z);

  Vector3 GetSide(int side, int step = 1);

  [[nodiscard]] int_fast32_t GetFloorX() const;
  [[nodiscard]] int_fast32_t GetFloorY() const;
  [[nodiscard]] int_fast32_t GetFloorZ() const;

  [[nodiscard]] Vector3 Subtract(double ix, double iy, double iz) const;
  [[nodiscard]] Vector3 SubtractVector(Vector3 from) const;
  [[nodiscard]] Vector3 Add(double ix, double iy, double iz) const;
  [[nodiscard]] Vector3 AddVector(Vector3 from) const;

  double x;
  double y;
  double z;
};

#endif // EXT_NOISELIB_LIB_OBJECTS_RANDOM_VECTOR3_H_
