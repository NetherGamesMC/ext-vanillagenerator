//
// Created by User on 25/6/2021.
//

#include "SimplexOctaveGenerator.h"

SimplexOctaveGenerator::SimplexOctaveGenerator(Random &random, int octavesNum, int sizeXv, int sizeYv, int sizeZv) {
  for (int i = 0; i < octavesNum; ++i) {
    auto simplex = SimplexNoise(random);

    simplexOctaves.push_back(simplex);
  }

  sizeX = sizeXv;
  sizeY = sizeYv;
  sizeZ = sizeZv;
}

std::vector<double> SimplexOctaveGenerator::GetFractalBrownianMotion(double x, double y, double z,
                                                                     double lacunarity,
                                                                     double persistence) {
  std::vector<double> noise(GetArraySize(), 0.0);

  double freq = 1.0, amp = 1.0;

  for (SimplexNoise octave : simplexOctaves) {
    octave.GetNoise(noise, x, y, z, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, 0.55 / amp);
    freq *= lacunarity;
    amp *= persistence;
  }

  return noise;
}

int SimplexOctaveGenerator::GetArraySize() const {
  return sizeX * sizeY * sizeZ;
}

double SimplexOctaveGenerator::Noise(double x, double y, double z, double frequency, double amplitude,
                                     bool normalized) {
  double result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

  x *= GetXScale();
  y *= GetYScale();
  z *= GetZScale();

  for (SimplexNoise octave : simplexOctaves) {
    auto value = octave.Noise3d(x * freq, y * freq, z * freq) * amp;

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

int SimplexOctaveGenerator::GetSizeX() const {
  return sizeX;
}

int SimplexOctaveGenerator::GetSizeZ() const {
  return sizeZ;
}
