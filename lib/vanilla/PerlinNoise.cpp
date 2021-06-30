#include "PerlinNoise.h"
#include "Utils.h"

PerlinNoise::PerlinNoise(Random &random) {
  offsetX = random.nextFloat() * 256;
  offsetY = random.nextFloat() * 256;
  offsetZ = random.nextFloat() * 256;

  for (int i = 0; i < 256; i++) {
    permutations[i] = i;
  }

  for (int i = 0; i < 256; i++) {
    int pos = (int) random.nextInt(256 - i) + i;
    int old = permutations[i];
    permutations[i] = permutations[pos];
    permutations[pos] = old;
    permutations[i + 256] = permutations[i];
  }
}

void PerlinNoise::getNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY,
                              int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
  if (sizeY == 1) {
    get2dNoise(noise, x, z, sizeX, sizeZ, scaleX, scaleZ, amplitude);
  } else {
    get3dNoise(noise, x, y, z, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, amplitude);
  }
}

void PerlinNoise::get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                                double scaleX, double scaleZ, double amplitude) {
  int index = 0;

  for (int i = 0; i < sizeX; i++) {
    double dx = x + offsetX + i * scaleX;
    int floorX = Utils::floor(dx);
    int ix = floorX & 255;
    dx -= floorX;
    double fx = Utils::fade(dx);
    for (int j = 0; j < sizeZ; j++) {
      double dz = z + offsetZ + j * scaleZ;
      int floorZ = Utils::floor(dz);
      int iz = floorZ & 255;
      dz -= floorZ;
      double fz = Utils::fade(dz);
      // Hash coordinates of the square corners
      int a = permutations[ix];
      int aa = permutations[a] + iz;
      int b = permutations[ix + 1];
      int ba = permutations[b] + iz;
      double x1 = Utils::lerp(fx, Utils::grad(permutations[aa], dx, 0, dz),
                              Utils::grad(permutations[ba], dx - 1, 0, dz));
      double x2 = Utils::lerp(fx, Utils::grad(permutations[aa + 1], dx, 0, dz - 1),
                              Utils::grad(permutations[ba + 1], dx - 1, 0, dz - 1));
      noise[index++] += Utils::lerp(fz, x1, x2) * amplitude;
    }
  }
}

void  PerlinNoise::get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX,
                                int sizeY, int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
  int n = -1;
  double x1 = 0;
  double x2 = 0;
  double x3 = 0;
  double x4 = 0;
  int index = 0;
  for (int i = 0; i < sizeX; i++) {
    double dx = x + offsetX + (i * scaleX);
    int floorX = Utils::floor(dx);
    int ix = floorX & 255;
    dx -= floorX;
    double fx = Utils::fade(dx);
    for (int j = 0; j < sizeZ; j++) {
      double dz = z + offsetZ + j * scaleZ;
      int floorZ = Utils::floor(dz);
      int iz = floorZ & 255;
      dz -= floorZ;
      double fz = Utils::fade(dz);
      for (int k = 0; k < sizeY; k++) {
        double dy = y + offsetY + k * scaleY;
        int floorY = Utils::floor(dy);
        int iy = floorY & 255;
        dy -= floorY;
        double fy = Utils::fade(dy);
        if (k == 0 || iy != n) {
          n = iy;
          // Hash coordinates of the cube corners
          int a = permutations[ix] + iy;
          int aa = permutations[a] + iz;
          int ab = permutations[a + 1] + iz;
          int b = permutations[ix + 1] + iy;
          int ba = permutations[b] + iz;
          int bb = permutations[b + 1] + iz;
          x1 = Utils::lerp(fx, Utils::grad(permutations[aa], dx, dy, dz),
                           Utils::grad(permutations[ba], dx - 1, dy, dz));
          x2 = Utils::lerp(fx, Utils::grad(permutations[ab], dx, dy - 1, dz),
                           Utils::grad(permutations[bb], dx - 1, dy - 1, dz));
          x3 = Utils::lerp(fx, Utils::grad(permutations[aa + 1], dx, dy, dz - 1),
                           Utils::grad(permutations[ba + 1], dx - 1, dy, dz - 1));
          x4 = Utils::lerp(fx, Utils::grad(permutations[ab + 1], dx, dy - 1, dz - 1),
                           Utils::grad(permutations[bb + 1], dx - 1, dy - 1, dz - 1));
        }

        double y1 = Utils::lerp(fy, x1, x2);
        double y2 = Utils::lerp(fy, x3, x4);

        noise[index++] += Utils::lerp(fz, y1, y2) * amplitude;
      }
    }
  }
}