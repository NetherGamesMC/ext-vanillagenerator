#include <cmath>
#include "Vector3.h"
#include "Facing.h"

Vector3::Vector3(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}
Vector3::Vector3(int ix, int iy, int iz) : x(ix), y(iy), z(iz) {}

Vector3 Vector3::GetSide(int side, int step) {
  switch (side) {
    case Facing::DOWN: return {x, y - step, z};
    case Facing::UP: return {x, y + step, z};
    case Facing::NORTH: return {x, y, z - step};
    case Facing::SOUTH: return {x, y, z + step};
    case Facing::WEST: return {x - step, y, z};
    case Facing::EAST: return {x + step, y, z};
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
  return {x + ix, y + iy, z + iz};
}

Vector3 Vector3::AddVector(Vector3 from) const {
  return Add(from.x, from.y, from.z);
}

int_fast32_t Vector3::getFloorX() const {
  return (int) std::floor(x);
}

int_fast32_t Vector3::getFloorY() const {
  return (int) std::floor(y);
}

int_fast32_t Vector3::getFloorZ() const {
  return (int) std::floor(z);
}