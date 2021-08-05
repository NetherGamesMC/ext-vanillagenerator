#include "PerlinNoise.h"
#include "lib/objects/math/Math.h"

PerlinNoise::PerlinNoise(Random &random) {
  offsetX = random.NextFloat() * 256;
  offsetY = random.NextFloat() * 256;
  offsetZ = random.NextFloat() * 256;

  for (int i = 0; i < 256; i++) {
    permutations[i] = i;
  }

  for (int i = 0; i < 256; i++) {
    int pos = (int) random.NextInt(256 - i) + i;
    int old = permutations[i];
    permutations[i] = permutations[pos];
    permutations[pos] = old;
    permutations[i + 256] = permutations[i];
  }
}

void PerlinNoise::GetNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY,
                              int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
  if (sizeY == 1) {
    Get2dNoise(noise, x, z, sizeX, sizeZ, scaleX, scaleZ, amplitude);
  } else {
    Get3dNoise(noise, x, y, z, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, amplitude);
  }
}

void PerlinNoise::Get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeZ,
                                double scaleX, double scaleZ, double amplitude) {
  int index = 0;

  for (int i = 0; i < sizeX; i++) {
    double dx = x + offsetX + i * scaleX;
    int floorX = Math::Floor(dx);
    int ix = floorX & 255;
    dx -= floorX;
    double fx = Math::Fade(dx);
    for (int j = 0; j < sizeZ; j++) {
      double dz = z + offsetZ + j * scaleZ;
      int floorZ = Math::Floor(dz);
      int iz = floorZ & 255;
      dz -= floorZ;
      double fz = Math::Fade(dz);
      // Hash coordinates of the square corners
      int a = permutations[ix];
      int aa = permutations[a] + iz;
      int b = permutations[ix + 1];
      int ba = permutations[b] + iz;
      double x1 = Math::Lerp(fx, Math::Grad(permutations[aa], dx, 0, dz),
                              Math::Grad(permutations[ba], dx - 1, 0, dz));
      double x2 = Math::Lerp(fx, Math::Grad(permutations[aa + 1], dx, 0, dz - 1),
                              Math::Grad(permutations[ba + 1], dx - 1, 0, dz - 1));
      noise[index++] += Math::Lerp(fz, x1, x2) * amplitude;
    }
  }
}

void  PerlinNoise::Get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX,
                                int sizeY, int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
  int n = -1;
  double x1 = 0;
  double x2 = 0;
  double x3 = 0;
  double x4 = 0;
  int index = 0;
  for (int i = 0; i < sizeX; i++) {
    double dx = x + offsetX + (i * scaleX);
    int floorX = Math::Floor(dx);
    int ix = floorX & 255;
    dx -= floorX;
    double fx = Math::Fade(dx);
    for (int j = 0; j < sizeZ; j++) {
      double dz = z + offsetZ + j * scaleZ;
      int floorZ = Math::Floor(dz);
      int iz = floorZ & 255;
      dz -= floorZ;
      double fz = Math::Fade(dz);
      for (int k = 0; k < sizeY; k++) {
        double dy = y + offsetY + k * scaleY;
        int floorY = Math::Floor(dy);
        int iy = floorY & 255;
        dy -= floorY;
        double fy = Math::Fade(dy);
        if (k == 0 || iy != n) {
          n = iy;
          // Hash coordinates of the cube corners
          int a = permutations[ix] + iy;
          int aa = permutations[a] + iz;
          int ab = permutations[a + 1] + iz;
          int b = permutations[ix + 1] + iy;
          int ba = permutations[b] + iz;
          int bb = permutations[b + 1] + iz;
          x1 = Math::Lerp(fx, Math::Grad(permutations[aa], dx, dy, dz),
                           Math::Grad(permutations[ba], dx - 1, dy, dz));
          x2 = Math::Lerp(fx, Math::Grad(permutations[ab], dx, dy - 1, dz),
                           Math::Grad(permutations[bb], dx - 1, dy - 1, dz));
          x3 = Math::Lerp(fx, Math::Grad(permutations[aa + 1], dx, dy, dz - 1),
                           Math::Grad(permutations[ba + 1], dx - 1, dy, dz - 1));
          x4 = Math::Lerp(fx, Math::Grad(permutations[ab + 1], dx, dy - 1, dz - 1),
                           Math::Grad(permutations[bb + 1], dx - 1, dy - 1, dz - 1));
        }

        double y1 = Math::Lerp(fy, x1, x2);
        double y2 = Math::Lerp(fy, x3, x4);

        noise[index++] += Math::Lerp(fz, y1, y2) * amplitude;
      }
    }
  }
}