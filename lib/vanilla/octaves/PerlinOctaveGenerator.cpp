#include "PerlinOctaveGenerator.h"
#include "lib/vanilla/Utils.h"

PerlinOctaveGenerator::PerlinOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv) {
  for (int i = 0; i < octavesNum; ++i) {
    auto perlin = PerlinNoise(random);

    perlinOctaves.push_back(perlin);
  }

  sizeX = size_xv;
  sizeY = size_yv;
  sizeZ = size_zv;
}

std::vector<double> PerlinOctaveGenerator::getFractalBrownianMotion(double x, double y, double z,
                                                                    double lacunarity,
                                                                    double persistence) {
  std::vector<double> noise(getArraySize(), 0.0);

  double freq = 1;
  double amp = 1;

  x = x * xScale;
  y = y * yScale;
  z = z * zScale;

  // fBm
  // the noise have to be periodic over x and z axis: otherwise it can go crazy with high
  // input, leading to strange oddities in terrain generation like the old minecraft farland
  // symptoms.
  for (PerlinNoise octave : perlinOctaves) {
    double dx = x * freq;
    double dz = z * freq;
    // compute integer part
    long lx = Utils::floorLong(dx);
    long lz = Utils::floorLong(dz);
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

    octave.getNoise(noise, dx, dy, dz, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, amp);

    freq *= lacunarity;
    amp *= persistence;
  }

  return noise;
}

int PerlinOctaveGenerator::getArraySize() const {
  return sizeX * sizeY * sizeZ;
}

int PerlinOctaveGenerator::getSizeY() const {
  return sizeY;
}