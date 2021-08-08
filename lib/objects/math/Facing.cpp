#include "Facing.h"

#include <stdexcept>
#include <string>
#include <algorithm>

const int Facing::ALL[6] = {
    Facing::DOWN,
    Facing::UP,
    Facing::NORTH,
    Facing::SOUTH,
    Facing::WEST,
    Facing::EAST,
};

const int Facing::HORIZONTAL[4] = {
    Facing::NORTH,
    Facing::SOUTH,
    Facing::WEST,
    Facing::EAST,
};

void Facing::Validate(int facing) {
  if (std::find(std::begin(ALL), std::end(ALL), facing) != std::end(ALL)) {
    throw std::invalid_argument("Invalid direction " + std::to_string(facing));
  }
}

int Facing::clockwise(int axis, int direction) {
  switch (axis) {
    case AXIS_Y:
      switch (direction) {
        case NORTH: return EAST;
        case EAST: return SOUTH;
        case SOUTH: return WEST;
        case WEST: return NORTH;
        default: throw std::invalid_argument("Cannot Rotate direction " + std::to_string(direction) + " around Axis " + std::to_string(axis));
      }
    case AXIS_Z:
      switch (direction) {
        case UP: return EAST;
        case EAST: return DOWN;
        case DOWN: return WEST;
        case WEST: return UP;
        default: throw std::invalid_argument("Cannot Rotate direction " + std::to_string(direction) + " around Axis " + std::to_string(axis));
      }
    case AXIS_X:
      switch (direction) {
        case UP: return NORTH;
        case NORTH: return DOWN;
        case DOWN: return SOUTH;
        case SOUTH: return UP;
        default: throw std::invalid_argument("Cannot Rotate direction " + std::to_string(direction) + " around Axis " + std::to_string(axis));
      }
    default:throw std::invalid_argument("Invalid Axis " + std::to_string(axis));
  }
}