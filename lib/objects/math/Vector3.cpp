#include "Vector3.h"
#include "Facing.h"

Vector3 getSide(Vector3 vVector, int side, int step) {
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

Vector3 Add(Vector3 origin, Vector3 from) {
  return {origin.x + from.x, origin.y + from.y, origin.z + from.z};
}

Vector3 Subtract(Vector3 origin, Vector3 from) {
  return {origin.x - from.x, origin.y - from.y, origin.z - from.z};
}