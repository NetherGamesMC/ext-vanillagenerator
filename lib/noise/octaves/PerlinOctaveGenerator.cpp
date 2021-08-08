#include <lib/objects/math/Math.h>
#include "PerlinOctaveGenerator.h"

PerlinOctaveGenerator::PerlinOctaveGenerator(Random &random, int octavesNum, int sizeXv, int sizeYv, int sizeZv) {
  for (int i = 0; i < octavesNum; ++i) {
    auto perlin = PerlinNoise(random);

    perlinOctaves.push_back(perlin);
  }

  sizeX = sizeXv;
  sizeY = sizeYv;
  sizeZ = sizeZv;
}

std::vector<double> PerlinOctaveGenerator::GetFractalBrownianMotion(double x, double y, double z,
                                                                    double lacunarity,
                                                                    double persistence) {
  std::vector<double> noise(GetArraySize(), 0.0);

  double freq = 1;
  double amp = 1;

  x = x * xScale;
  y = y * yScale;
  z = z * zScale;

  // fBm
  // the noise have to be periodic over x and z Axis: otherwise it can go crazy with high
  // input, leading to strange oddities in terrain generation like the old minecraft farland
  // symptoms.
  for (PerlinNoise octave : perlinOctaves) {
    double dx = x * freq;
    double dz = z * freq;
    // compute integer part
    long lx = Math::FloorLong(dx);
    long lz = Math::FloorLong(dz);
    // compute fractional part
    dx -= lx;
    dz -= lz;
    // wrap integer part to 0..16777216
    lx %= 16777216;
    lz %= 16777216;
    // add to fractional part
    dx += lx;
    dz += lz;

    double dy = y * freq;

    octave.GetNoise(noise, dx, dy, dz, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, amp);

    freq *= lacunarity;
    amp *= persistence;
  }

  return noise;
}

int PerlinOctaveGenerator::GetArraySize() const {
  return sizeX * sizeY * sizeZ;
}

int PerlinOctaveGenerator::GetSizeY() const {
  return sizeY;
}