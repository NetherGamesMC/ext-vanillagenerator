#include "SimplexNoise.h"
#include "lib/objects/math/Math.h"

#define SQRT_3      (1.7320508075688772)
#define F2          (0.5 * (SQRT_3 - 1))
#define G2          (3 - SQRT_3) / 6
#define G22         (G2 * 2.0 - 1)
#define F3          (1.0 / 3.0)
#define G3          (1.0 / 6.0)
#define G32         (G3 * 2.0)
#define G33         (G3 * 3.0 - 1.0)

SimplexNoise::SimplexNoise(Random &random) : PerlinNoise(random) {
  for (int i = 0; i < 512; i++) {
    permMod12[i] = permutations[i] % 12;
  }
}

void SimplexNoise::Get2dNoise(std::vector<double> &noise, double x, double z, int sizeX, int sizeY,
                                 double scaleX, double scaleY, double amplitude) {
  int index = 0;
  for (int i = 0; i < sizeY; i++) {
    double zin = offsetY + (z + i) * scaleY;
    for (int j = 0; j < sizeX; j++) {
      double xin = offsetX + (x + j) * scaleX;
      noise[index++] += Simplex2d(xin, zin) * amplitude;
    }
  }
}

void SimplexNoise::Get3dNoise(std::vector<double> &noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                                 double scaleX, double scaleY, double scaleZ, double amplitude) {
  int index = 0;
  for (int i = 0; i < sizeZ; i++) {
    double zin = offsetZ + (z + i) * scaleZ;
    for (int j = 0; j < sizeX; j++) {
      double xin = offsetX + (x + j) * scaleX;
      for (int k = 0; k < sizeY; k++) {
        double yin = offsetY + (y + k) * scaleY;
        noise[index++] += Simplex3d(xin, yin, zin) * amplitude;
      }
    }
  }
}

double SimplexNoise::Simplex2d(double xin, double yin) {
  // Skew the input space to determine which simplex cell we're in
  double s = (xin + yin) * F2; // Hairy factor for 2D
  int i = Math::FloorSimplex(xin + s);
  int j = Math::FloorSimplex(yin + s);
  double t = (i + j) * G2;
  double dx0 = i - t; // Unskew the cell origin back to (x,y) space
  double dy0 = j - t;
  double x0 = xin - dx0; // The x,y distances from the cell origin
  double y0 = yin - dy0;

  // For the 2D case, the simplex shape is an equilateral triangle.

  // Determine which simplex we are in.
  int i1; // Offsets for second (middle) corner of simplex in (i,j) coords
  int j1;
  if (x0 > y0) {
    i1 = 1; // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    j1 = 0;
  } else {
    i1 = 0; // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    j1 = 1;
  }

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6

  double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  double y1 = y0 - j1 + G2;
  double x2 = x0 + G22; // Offsets for last corner in (x,y) unskewed coords
  double y2 = y0 + G22;

  // Work out the hashed gradient indices of the three simplex corners
  int ii = i & 255;
  int jj = j & 255;
  int gi0 = permMod12[ii + permutations[jj]];
  int gi1 = permMod12[ii + i1 + permutations[jj + j1]];
  int gi2 = permMod12[ii + 1 + permutations[jj + 1]];

  // Calculate the contribution from the three corners
  double t0 = 0.5 - x0 * x0 - y0 * y0;
  double n0;
  if (t0 < 0) {
    n0 = 0.0;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * Math::Dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
  }

  double t1 = 0.5 - x1 * x1 - y1 * y1;
  double n1;
  if (t1 < 0) {
    n1 = 0.0;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * Math::Dot(grad3[gi1], x1, y1);
  }

  double t2 = 0.5 - x2 * x2 - y2 * y2;
  double n2;
  if (t2 < 0) {
    n2 = 0.0;
  } else {
    t2 *= t2;
    n2 = t2 * t2 * Math::Dot(grad3[gi2], x2, y2);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70.0 * (n0 + n1 + n2);
}

double SimplexNoise::Simplex3d(double xin, double yin, double zin) {
// Skew the input space to determine which simplex cell we're in
  double s = (xin + yin + zin) * F3; // Very nice and simple skew factor for 3D
  int i = Math::FloorSimplex(xin + s);
  int j = Math::FloorSimplex(yin + s);
  int k = Math::FloorSimplex(zin + s);
  double t = (i + j + k) * G3;
  double dx0 = i - t; // Unskew the cell origin back to (x,y,z) space
  double dy0 = j - t;
  double dz0 = k - t;

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.

  int i1; // Offsets for second corner of simplex in (i,j,k) coords
  int j1;
  int k1;
  int i2; // Offsets for third corner of simplex in (i,j,k) coords
  int j2;
  int k2;

  double x0 = xin - dx0; // The x,y,z distances from the cell origin
  double y0 = yin - dy0;
  double z0 = zin - dz0;
  // Determine which simplex we are in
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1; // X Y Z order
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } else if (x0 >= z0) {
      i1 = 1; // X Z Y order
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } else {
      i1 = 0; // Z X Y order
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    }
  } else { // x0<y0
    if (y0 < z0) {
      i1 = 0; // Z Y X order
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } else if (x0 < z0) {
      i1 = 0; // Y Z X order
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } else {
      i1 = 0; // Y X Z order
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    }
  }

  // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
  // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
  // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
  // c = 1/6.
  double x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
  double y1 = y0 - j1 + G3;
  double z1 = z0 - k1 + G3;
  double x2 = x0 - i2 + G32; // Offsets for third corner in (x,y,z) coords
  double y2 = y0 - j2 + G32;
  double z2 = z0 - k2 + G32;

  // Work out the hashed gradient indices of the four simplex corners
  int ii = i & 255;
  int jj = j & 255;
  int kk = k & 255;
  int gi0 = permMod12[ii + permutations[jj + permutations[kk]]];
  int gi1 = permMod12[ii + i1 + permutations[jj + j1 + permutations[kk + k1]]];
  int gi2 = permMod12[ii + i2 + permutations[jj + j2 + permutations[kk + k2]]];
  int gi3 = permMod12[ii + 1 + permutations[jj + 1 + permutations[kk + 1]]];

  // Calculate the contribution from the four corners
  double t0 = 0.5 - x0 * x0 - y0 * y0 - z0 * z0;
  double n0; // Noise contributions from the four corners
  if (t0 < 0) {
    n0 = 0.0;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * Math::Dot(grad3[gi0], x0, y0, z0);
  }

  double t1 = 0.5 - x1 * x1 - y1 * y1 - z1 * z1;
  double n1;
  if (t1 < 0) {
    n1 = 0.0;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * Math::Dot(grad3[gi1], x1, y1, z1);
  }

  double t2 = 0.5 - x2 * x2 - y2 * y2 - z2 * z2;
  double n2;
  if (t2 < 0) {
    n2 = 0.0;
  } else {
    t2 *= t2;
    n2 = t2 * t2 * Math::Dot(grad3[gi2], x2, y2, z2);
  }

  double x3 = x0 + G33; // Offsets for last corner in (x,y,z) coords
  double y3 = y0 + G33;
  double z3 = z0 + G33;
  double t3 = 0.5 - x3 * x3 - y3 * y3 - z3 * z3;
  double n3;
  if (t3 < 0) {
    n3 = 0.0;
  } else {
    t3 *= t3;
    n3 = t3 * t3 * Math::Dot(grad3[gi3], x3, y3, z3);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to stay just inside [-1,1]
  return 32.0 * (n0 + n1 + n2 + n3);
}

double SimplexNoise::Noise3d(double xin, double yin, double zin) {
  xin += offsetX;
  yin += offsetZ;
  if (xin == 0.0) {
    return Simplex2d(xin, yin);
  }

  zin += offsetZ;
  return Simplex3d(xin, yin, zin);
}
