#include <noise/octaves/Octaves.h>
#include <iostream>
#include <chrono>
#include <algorithm>

namespace FastNoise2 {

NoiseArray &SimplexOctaves::Fractal(float x, float z, float lacunarity, float persistence) {
  std::fill(fBm_array_.begin(), fBm_array_.end(), 0.0f);

  float freq = 1.0, amp = 1.0;
  for (SimplexNoise octave : octaves_) {
    octave.GetNoise2d(fBm_array_, x, z, freq, 0.55f / amp);

    freq *= lacunarity;
    amp *= persistence;
  }

  return fBm_array_;
}

}