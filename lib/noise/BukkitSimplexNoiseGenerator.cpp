#include "BukkitSimplexNoiseGenerator.h"
#include "lib/objects/math/Math.h"

#define SQRT_3      (1.7320508075688772)
#define F2          (0.5 * (SQRT_3 - 1))
#define G2          (3 - SQRT_3) / 6
#define G22         (G2 * 2.0 - 1)

BukkitSimplexNoiseGenerator::BukkitSimplexNoiseGenerator(Random &random) {
  offsetX = random.NextFloat() * 256;
  offsetY = random.NextFloat() * 256;
  random.NextSignedInt(); // offsetZ, generates the next random number

  for (int i = 0; i < 256; ++i) {
    permutations[i] = random.NextInt(256);
  }

  for (int i = 0; i < 256; ++i) {
    int pos = (int) random.NextInt(256 - i) + i;
    int old = permutations[i];

    permutations[i] = permutations[pos];
    permutations[pos] = old;
    permutations[i + 256] = permutations[i];
  }

  random.NextSignedInt(); // offsetW, generates the next random number
}

double BukkitSimplexNoiseGenerator::Simplex2d(double xin, double yin) {
  xin += offsetX;
  yin += offsetY;

  double n0, n1, n2; // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  double s = (xin + yin) * F2; // Hairy factor for 2D
  int i = Math::FloorSimplex(xin + s);
  int j = Math::FloorSimplex(yin + s);
  double t = (i + j) * G2;
  double X0 = i - t; // Unskew the cell origin back to (x,y) space
  double Y0 = j - t;
  double x0 = xin - X0; // The x,y distances from the cell origin
  double y0 = yin - Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.

  // Determine which simplex we are in.
  int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0) {
    i1 = 1;
    j1 = 0;
  } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
  else {
    i1 = 0;
    j1 = 1;
  } // upper triangle, YX order: (0,0)->(0,1)->(1,1)

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
  int gi0 = permutations[ii + permutations[jj]] % 12;
  int gi1 = permutations[ii + i1 + permutations[jj + j1]] % 12;
  int gi2 = permutations[ii + 1 + permutations[jj + 1]] % 12;

  // Calculate the contribution from the three corners
  double t0 = 0.5 - x0 * x0 - y0 * y0;
  if (t0 < 0) {
    n0 = 0.0;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * Math::Dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
  }

  double t1 = 0.5 - x1 * x1 - y1 * y1;
  if (t1 < 0) {
    n1 = 0.0;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * Math::Dot(grad3[gi1], x1, y1);
  }

  double t2 = 0.5 - x2 * x2 - y2 * y2;
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