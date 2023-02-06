#pragma once
#ifndef FASTNOISE2_GEN_NOISE
#define FASTNOISE2_GEN_NOISE

#include <cmath>
#include <vector>
#include <memory>
#include <cstdint>
#include <numeric>

#include <math/Random.h>

namespace FastNoise2 {

typedef std::vector<float> NoiseArray;
typedef std::vector<uint_fast8_t> PermutationTable;

class NoiseBase;
class PerlinNoise;
class SimplexNoise;

enum class Pos {
  X, Y, Z,
  Count
};

template<typename T>
struct PerDimensionVariable {
  using Type = T;

  T varArray[(int) Pos::Count];

  template<typename U = T>
  explicit PerDimensionVariable(U value = 0) {
    for (T &element : varArray) {
      element = value;
    }
  }

  T &operator[](size_t i) {
    return varArray[i];
  }

  const T &operator[](size_t i) const {
    return varArray[i];
  }
};

struct OctaveSettings {
  template<Pos D>
  void SetSize(uint_fast32_t value) {
    size_[(int) D] = value;

    uint_fast32_t newSize = 1;
    if (size_[(int) Pos::X] > 0) {
      newSize *= size_[(int) Pos::X];
    }
    if (size_[(int) Pos::Z] > 0) {
      newSize *= size_[(int) Pos::Z];
    }
    if (size_[(int) Pos::Y] > 0) {
      newSize *= size_[(int) Pos::Y];
    }

    noise_array_.resize(newSize);
  }

  template<Pos D>
  void SetScale(float value) { scale_[(int) D] = value; }

  template<Pos D>
  void SetOffset(float value) { offset_[(int) D] = value; }

  NoiseArray &getNoiseArray() { return noise_array_; }

  PerDimensionVariable<uint_fast32_t> size_;
  PerDimensionVariable<float> scale_;
  PerDimensionVariable<float> offset_;

 private:
  NoiseArray noise_array_;
};

/**
 * Improved Noise abstraction class, method contains a variable that would house the noise
 * output without having to initialize to reinstate the object in the memory - as the object
 * will be passed by reference and reused later.
 */
class NoiseBase {
 public:
  explicit NoiseBase(Random &random) : permutations_(512) {
    settings_.SetOffset<Pos::X>(random.NextFloat() * 256);
    settings_.SetOffset<Pos::Y>(random.NextFloat() * 256);
    settings_.SetOffset<Pos::Z>(random.NextFloat() * 256);

    std::iota(permutations_.begin() + 1, permutations_.begin() + 256, 1);

    for (int_fast32_t i = 0; i < 256; i++) {
      int_fast32_t pos = (int) random.NextInt(256 - i) + i;
      int_fast32_t old = permutations_[i];
      permutations_[i] = permutations_[pos];
      permutations_[pos] = old;
      permutations_[i + 256] = permutations_[i];
    }
  }

  virtual void GetNoise2d(NoiseArray &noise_array, float x, float y, float frequency, float amplitude) = 0;

  virtual void GetNoise3d(NoiseArray &noise_array, float x, float y, float z, float frequency, float amplitude) = 0;

  OctaveSettings &GetOctaveSettings() { return settings_; }
 protected:
  OctaveSettings settings_;

  PermutationTable permutations_;
};

class PerlinNoise : public NoiseBase {
 public:
  PerlinNoise(Random &random);

  void GetNoise2d(NoiseArray &noise_array, float x, float y, float frequency, float amplitude) override;

  void GetNoise3d(NoiseArray &noise_array, float x, float y, float z, float frequency, float amplitude) override;
};

class SimplexNoise : public NoiseBase {
 public:
  SimplexNoise(Random &random);

  void GetNoise2d(NoiseArray &noise_array, float x, float y, float frequency, float amplitude) override;

  void GetNoise3d(NoiseArray &noise_array, float x, float y, float z, float frequency, float amplitude) override;
 private:
  float noise(float xin, float yin);

  float noise(float xin, float yin, float zin);

  PermutationTable perm_mod_12_;
};

static inline float lerp(float delta, float start, float end) {
  return (delta * (end - start)) + start;
}

static inline float fade(float x) {
  return x * x * x * (x * (x * 6 - 15) + 10);
}

/**
 * Computes the largest integer value not greater than the float one
 *
 * This method is faster than using (int_fast32_t)floor(fp).
 *
 * I measured it to be approximately twice as fast:
 *  float:  ~18.4ns instead of ~39.6ns on an AMD APU),
 *  double: ~20.6ns instead of ~36.6ns on an AMD APU),
 * Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
 *
 * @param[in] fp    float input value
 *
 * @return largest integer value not greater than fp
 */
static inline int_fast32_t fast_floor(float fp) {
  auto i = static_cast<int_fast32_t>(fp);
  return (fp < i) ? (i - 1) : (i);
}

/**
 * Helper functions to compute gradients-dot-residual vectors (2D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 *
 * @return gradient value
 */
static inline float grad(int_fast32_t hash, float x, float y) {
  const int_fast32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
  const float u = h < 4 ? x : y;  // into 8 simple gradient directions,
  const float v = h < 4 ? y : x;
  return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (x,y).
}

/**
 * Helper functions to compute gradients-dot-residual vectors (3D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 * @param[in] z     z coord of the distance to the corner
 *
 * @return gradient value
 */
static inline float grad(int_fast32_t hash, float x, float y, float z) {
  int_fast32_t h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
  float u = h < 8 ? x : y; // gradient directions, and compute dot product.
  float v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

}

#endif