#include "Vector3.h"
#include "Facing.h"

Vector3 GetSide(Vector3 vVector, int side, int step) {
  switch (side) {
    case Facing::DOWN: return {vVector.x, vVector.y - step, vVector.z};
    case Facing::UP: return {vVector.x, vVector.y + step, vVector.z};
    case Facing::NORTH: return {vVector.x, vVector.y, vVector.z - step};
    case Facing::SOUTH: return {vVector.x, vVector.y, vVector.z + step};
    case Facing::WEST: return {vVector.x - step, vVector.y, vVector.z};
    case Facing::EAST: return {vVector.x + step, vVector.y, vVector.z};
    default: return vVector;
  }
}