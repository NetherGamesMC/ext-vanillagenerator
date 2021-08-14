#include <cmath>
#include "Vector3.h"
#include "Facing.h"

Vector3 Vector3::GetSide(int_fast32_t side, int_fast32_t step) {
  switch (side) {
    case Facing::DOWN: return Vector3(x, y - step, z);
    case Facing::UP: return Vector3(x, y + step, z);
    case Facing::NORTH: return Vector3(x, y, z - step);
    case Facing::SOUTH: return Vector3(x, y, z + step);
    case Facing::WEST: return Vector3(x - step, y, z);
    case Facing::EAST: return Vector3(x + step, y, z);
    default: return *this;
  }
}

Vector3 Vector3::Subtract(double ix, double iy, double iz) const {
  return Add(-ix, -iy, -iz);
}

Vector3 Vector3::SubtractVector(Vector3 from) const {
  return Add(-from.x, -from.y, -from.z);
}

Vector3 Vector3::Add(double ix, double iy, double iz) const {
  return Vector3(x + ix, y + iy, z + iz);
}

Vector3 Vector3::AddVector(Vector3 from) const {
  return Add(from.x, from.y, from.z);
}

int_fast32_t Vector3::GetFloorX() const {
  return (int) std::floor(x);
}

int_fast32_t Vector3::GetFloorY() const {
  return (int) std::floor(y);
}

int_fast32_t Vector3::GetFloorZ() const {
  return (int) std::floor(z);
}