#ifndef EXT_VANILLAGENERATOR_RANDOM_H
#define EXT_VANILLAGENERATOR_RANDOM_H

#include <cstdint>

class Random {
 public:
  Random(int_fast32_t seed);

  void SetSeed(int_fast32_t mSeed);

  auto NextInt() -> int_fast32_t;
  auto NextInt(int_fast32_t bound) -> int_fast32_t;

  auto NextSignedInt() -> int_fast32_t;

  /**
   * Returns a float between 0.0 and 1.0 (inclusive)
   */
  auto NextFloat() -> float;

  /**
   * Returns a float between -1.0 and 1.0 (inclusive)
   */
  auto NextSignedFloat() -> float;

  /**
   * Returns a random boolean
   */
  auto NextBoolean() -> bool;

  auto NextLong() -> int_fast32_t;

  /**
   * Returns a random integer between start and end
   *
   * @param int start default 0
   * @param int end default 0x7fffffff
   */
  auto NextRange(int_fast32_t start = 0, int_fast32_t end = 0x7fffffff) -> int_fast32_t;

  [[nodiscard]] auto GetSeed() const -> int_fast32_t;

 private:
  int_fast32_t x = 0;
  int_fast32_t y = 0;
  int_fast32_t z = 0;
  int_fast32_t w = 0;

  int_fast32_t seed;
};

#endif

