#pragma once
#ifndef FASTNOISE2_GEN_OCTAVES
#define FASTNOISE2_GEN_OCTAVES

#include <math/Random.h>
#include <noise/Noise.h>

namespace FastNoise2 {

template<typename T = NoiseBase>
class Octaves {
 public:
  /**
   * @brief Fractal/Fractional Brownian Motion (fBm) abstract summation of 2D/3D Simplex/Perlin noise.
   *
   * @param[in] random  The random class.
   * @param[in] octaves The number of octaves for the Fractal/Fractional Brownian Motion (fBm)
   * @param[in] param   A lambda with a parameter of (OctaveSettings &)
   */
  template<typename OctaveLambda>
  Octaves(Random &random, int octaves, OctaveLambda &&param) : fBm_array_(), octaves_() {
    size_t totalValues;
    while (--octaves > 0) {
      T noise(random);
      param(noise.GetOctaveSettings());

      totalValues = noise.GetOctaveSettings().getNoiseArray().size();

      octaves_.push_back(noise);
    }

    fBm_array_.resize(totalValues);
  }

  virtual NoiseArray &Fractal(float x, float z, float lacunarity, float persistence) = 0;
 protected:
  NoiseArray fBm_array_;
  std::vector<T> octaves_;
};

class SimplexOctaves : public Octaves<SimplexNoise> {
 public:
  template<typename OctaveLambda>
  SimplexOctaves(Random &random, int octaves, OctaveLambda &&param) : Octaves<SimplexNoise>(random, octaves, param) {};

  NoiseArray &Fractal(float x, float z, float lacunarity, float persistence) override;
};

}

#endif // FASTNOISE2_GEN_OCTAVES
