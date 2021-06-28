//
// Created by User on 25/6/2021.
//

#include "SimplexOctaveGenerator.h"

SimplexOctaveGenerator::SimplexOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv) {
  for (int i = 0; i < octavesNum; ++i) {
    auto simplex = SimplexNoise(random);

    simplexOctaves.push_back(simplex);
  }

  sizeX = size_xv;
  sizeY = size_yv;
  sizeZ = size_zv;
}

std::vector<double> SimplexOctaveGenerator::getFractalBrownianMotion(double x, double y, double z,
                                                                     double lacunarity,
                                                                     double persistence) {
  std::vector<double> noise(getArraySize(), 0.0);

  double freq = 1.0, amp = 1.0;

  for (SimplexNoise octave : simplexOctaves) {
    octave.getNoise(noise, x, y, z, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, 0.55 / amp);
    freq *= lacunarity;
    amp *= persistence;
  }

  return noise;
}

int SimplexOctaveGenerator::getArraySize() const {
  return sizeX * sizeY * sizeZ;
}

double SimplexOctaveGenerator::noise(double x, double y, double z, double frequency, double amplitude,
                                     bool normalized) {
  double result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

  x *= getXScale();
  y *= getYScale();
  z *= getZScale();

  for (SimplexNoise octave : simplexOctaves) {
    auto value = octave.noise3d(x * freq, y * freq, z * freq) * amp;

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

int SimplexOctaveGenerator::getSizeX() const {
  return sizeX;
}

int SimplexOctaveGenerator::getSizeZ() const {
  return sizeZ;
}
