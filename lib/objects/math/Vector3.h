#ifndef EXT_VANILLAGENERATOR_LIB_OBJECTS_RANDOM_VECTOR3_H_
#define EXT_VANILLAGENERATOR_LIB_OBJECTS_RANDOM_VECTOR3_H_

#include <cstdint>

class Vector3 {
 public:
  template<typename Y>
  explicit Vector3(Y ix, Y iy, Y iz) : x(ix), y(iy), z(iz) {};

  Vector3 GetSide(int_fast32_t side, int_fast32_t step = 1);

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

#endif // EXT_VANILLAGENERATOR_LIB_OBJECTS_RANDOM_VECTOR3_H_
