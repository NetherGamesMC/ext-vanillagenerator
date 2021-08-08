#ifndef EXT_POCKETMINE_MATH_FACING_H
#define EXT_POCKETMINE_MATH_FACING_H

class Facing {
 private:
  static int clockwise(int axis, int direction);
 public:
  const static int AXIS_Y = 0;
  const static int AXIS_Z = 1;
  const static int AXIS_X = 2;

  const static int FLAG_AXIS_POSITIVE = 1;

  const static int DOWN = AXIS_Y << 1;
  const static int UP = (AXIS_Y << 1) | FLAG_AXIS_POSITIVE;
  const static int NORTH = AXIS_Z << 1;
  const static int SOUTH = (AXIS_Z << 1) | FLAG_AXIS_POSITIVE;
  const static int WEST = AXIS_X << 1;
  const static int EAST = (AXIS_X << 1) | FLAG_AXIS_POSITIVE;

  const static int ALL[6];

  const static int HORIZONTAL[4];

  /**
   * Returns the Axis of the given direction.
   *
   * @param direction
   *
   * @return
   */
  static int Axis(int direction) {
    return direction >> 1; // shift off positive/negative bit
  }

  /**
   * Returns whether the direction is facing the positive of its Axis.
   *
   * @param direction
   *
   * @return
   */
  static bool IsPositive(int direction) {
    return (direction & FLAG_AXIS_POSITIVE) == FLAG_AXIS_POSITIVE;
  }

  /**
   * Returns the Opposite Facing of the specified one.
   *
   * @param direction 0-5 one of the Facing::* constants
   *
   * @return
   */
  static int Opposite(int direction) {
    return direction ^ FLAG_AXIS_POSITIVE;
  }

  /**
   * Rotates the given direction around the Axis.
   *
   * @param direction
   * @param axis
   * @param clockwise
   *
   * @return
   *
   * @throws invalid_argument if not possible to Rotate direction around Axis
   */
  static int Rotate(int direction, int axis, bool clockwise) {
    int rotated = Facing::clockwise(axis, direction);
    return clockwise ? rotated : Opposite(rotated);
  }

  /**
   * Validates the given integer as a Facing direction.
   *
   * @param facing
   */
  static void Validate(int facing);
};

#endif //EXT_POCKETMINE_MATH_FACING_H