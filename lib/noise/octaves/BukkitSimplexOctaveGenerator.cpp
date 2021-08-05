#include "BukkitSimplexOctaveGenerator.h"

BukkitSimplexOctaveGenerator::BukkitSimplexOctaveGenerator(Random &random, int octaves) {
  for (int i = 0; i < octaves; i++) {
    auto simplexNoise = BukkitSimplexNoiseGenerator(random);

    simplexOctaves.push_back(simplexNoise);
  }
}

double BukkitSimplexOctaveGenerator::Noise(double x, double y, double frequency, double amplitude, bool normalized) {
  double result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

  for (BukkitSimplexNoiseGenerator octave : simplexOctaves) {
    auto value = octave.Simplex2d(x * freq, y * freq) * amp;

    result += value;
    max += amp;
    freq *= frequency;
    amp *= amplitude;
  }

  if (normalized) {
    result /= max;
  }

  return result;
}