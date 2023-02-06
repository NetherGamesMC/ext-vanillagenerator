#include <cstdint>
#include <noise/Noise.h>

/**
 * @file    SimplexNoise.cpp
 * @brief   A Perlin Simplex Noise C++ Implementation (2D, 3D).
 *
 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * This C++ implementation is based on the speed-improved Java version 2012-03-09
 * by Stefan Gustavson (original Java source code in the public domain).
 * http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java:
 * - Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * - Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * - Better rank ordering method by Stefan Gustavson in 2012.
 *
 * This implementation is "Simplex Noise" as presented by
 * Ken Perlin at a relatively obscure and not often cited course
 * session "Real-Time Shading" at Siggraph 2001 (before real
 * time shading actually took on), under the title "hardware noise".
 * The 3D function is numerically equivalent to his Java reference
 * code available in the PDF course notes, although I re-implemented
 * it from scratch to get more readable code. The 2D and 4D cases
 * were implemented from scratch by me from Ken Perlin's text.
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace FastNoise2 {

static inline int8_t grad3[12][3] = {
    {1,  1,  0},
    {-1, 1,  0},
    {1,  -1, 0},
    {-1, -1, 0},
    {1,  0,  1},
    {-1, 0,  1},
    {1,  0,  -1},
    {-1, 0,  -1},
    {0,  1,  1},
    {0,  -1, 1},
    {0,  1,  -1},
    {0,  -1, -1}
};

static inline float Dot(const int8_t g[3], float x, float y) {
  return g[0] * x + g[1] * y;
}

static inline float Dot(const int8_t g[3], float x, float y, float z) {
  return g[0] * x + g[1] * y + g[2] * z;
}

SimplexNoise::SimplexNoise(Random &random) : NoiseBase(random), perm_mod_12_(512) {
  for (int i = 0; i < 512; i++) {
    perm_mod_12_.at(i) = permutations_.at(i) % 12;
  }
}

void SimplexNoise::GetNoise2d(NoiseArray &noise_array, float x, float y, float frequency, float amplitude) {
  const size_t sizeX = settings_.size_[(int) Pos::X];
  const size_t sizeY = settings_.size_[(int) Pos::Y];

  const float scaleX = settings_.scale_[(int) Pos::X] * frequency;
  const float scaleY = settings_.scale_[(int) Pos::Y] * frequency;

  const float offsetX = settings_.offset_[(int) Pos::X];
  const float offsetY = settings_.offset_[(int) Pos::Y];

  int index = 0;
  for (size_t i = 0; i < sizeY; i++) {
    auto zin = offsetY + static_cast<float>(y + i) * scaleY;

    for (size_t j = 0; j < sizeX; j++) {
      float xin = offsetX + static_cast<float>(x + j) * scaleX;

      noise_array[index++] += noise(xin, zin) * amplitude;
    }
  }
}

void SimplexNoise::GetNoise3d(NoiseArray &noise_array, float x, float y, float z, float frequency, float amplitude) {
  const size_t sizeX = settings_.size_[(int) Pos::X];
  const size_t sizeY = settings_.size_[(int) Pos::Y];
  const size_t sizeZ = settings_.size_[(int) Pos::Z];

  const float scaleX = settings_.scale_[(int) Pos::X] * frequency;
  const float scaleY = settings_.scale_[(int) Pos::Y] * frequency;
  const float scaleZ = settings_.scale_[(int) Pos::Z] * frequency;

  const float offsetX = settings_.offset_[(int) Pos::X];
  const float offsetY = settings_.offset_[(int) Pos::Y];
  const float offsetZ = settings_.offset_[(int) Pos::Z];

  int index = 0;
  for (size_t i = 0; i < sizeZ; i++) {
    auto zin = offsetZ + (z + i) * scaleZ;

    for (size_t j = 0; j < sizeX; j++) {
      float xin = offsetX + (x + j) * scaleX;

      for (size_t k = 0; k < sizeY; k++) {
        float yin = offsetY + (y + k) * scaleY;

        noise_array[index++] += noise(xin, yin, zin) * amplitude;
      }
    }
  }
}

#define SQRT_3      (1.7320508075688772)
#define F2          (0.5 * (SQRT_3 - 1))
#define G2          ((3 - SQRT_3) / 6)
#define G22         (G2 * 2.0 - 1)
#define F3          (1.0 / 3.0)
#define G3          (1.0 / 6.0)
#define G32         (G3 * 2.0)
#define G33         (G3 * 3.0 - 1.0)

/**
 * 2D Perlin simplex noise
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x, float y) {
  float n0, n1, n2;   // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  const float s = (x + y) * F2;  // Hairy factor for 2D
  const int32_t i = fast_floor(x + s);
  const int32_t j = fast_floor(y + s);

  // Unskew the cell origin back to (x,y) space
  const float t = static_cast<float>(i + j) * G2;
  const float X0 = i - t;
  const float Y0 = j - t;
  const float x0 = x - X0;  // The x,y distances from the cell origin
  const float y0 = y - Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    i1 = 1;
    j1 = 0;
  } else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    i1 = 0;
    j1 = 1;
  }

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6

  const float x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
  const float y1 = y0 - j1 + G2;
  const float x2 = x0 + G22;   // Offsets for last corner in (x,y) unskewed coords
  const float y2 = y0 + G22;

  // Work out the hashed gradient indices of the three simplex corners
  const int ii = i & 255;
  const int jj = j & 255;
  const int gi0 = perm_mod_12_.at(ii + permutations_.at(jj));
  const int gi1 = perm_mod_12_.at(ii + i1 + permutations_.at(jj + j1));
  const int gi2 = perm_mod_12_.at(ii + 1 + permutations_.at(jj + 1));

  // Calculate the contribution from the first corner
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  if (t0 < 0.0f) {
    n0 = 0.0f;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * Dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
  }

  // Calculate the contribution from the second corner
  float t1 = 0.5f - x1 * x1 - y1 * y1;
  if (t1 < 0.0f) {
    n1 = 0.0f;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * Dot(grad3[gi1], x1, y1);
  }

  // Calculate the contribution from the third corner
  float t2 = 0.5f - x2 * x2 - y2 * y2;
  if (t2 < 0.0f) {
    n2 = 0.0f;
  } else {
    t2 *= t2;
    n2 = t2 * t2 * Dot(grad3[gi2], x2, y2);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70.0f * (n0 + n1 + n2);
}

/**
 * 3D Perlin simplex noise
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 * @param[in] z float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x, float y, float z) {
  float n0, n1, n2, n3; // Noise contributions from the four corners

  // Skew the input space to determine which simplex cell we're in
  float s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
  int i = fast_floor(x + s);
  int j = fast_floor(y + s);
  int k = fast_floor(z + s);
  float t = (i + j + k) * G3;
  float X0 = i - t; // Unskew the cell origin back to (x,y,z) space
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = x - X0; // The x,y,z distances from the cell origin
  float y0 = y - Y0;
  float z0 = z - Z0;

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
  // Determine which simplex we are in.
  int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
  int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // X Y Z order
    } else if (x0 >= z0) {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; // X Z Y order
    } else {
      i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; // Z X Y order
    }
  } else { // x0<y0
    if (y0 < z0) {
      i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; // Z Y X order
    } else if (x0 < z0) {
      i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; // Y Z X order
    } else {
      i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // Y X Z order
    }
  }

  // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
  // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
  // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
  // c = 1/6.
  float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords
  float y2 = y0 - j2 + 2.0f * G3;
  float z2 = z0 - k2 + 2.0f * G3;
  float x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords
  float y3 = y0 - 1.0f + 3.0f * G3;
  float z3 = z0 - 1.0f + 3.0f * G3;

  // Work out the hashed gradient indices of the four simplex corners
  const int ii = i & 255;
  const int jj = j & 255;
  const int kk = k & 255;
  int gi0 = permutations_.at(ii + permutations_.at(jj + permutations_.at(kk)));
  int gi1 = permutations_.at(ii + i1 + permutations_.at(jj + j1 + permutations_.at(kk + k1)));
  int gi2 = permutations_.at(ii + i2 + permutations_.at(jj + j2 + permutations_.at(kk + k2)));
  int gi3 = permutations_.at(ii + 1 + permutations_.at(jj + 1 + permutations_.at(kk + 1)));

  // Calculate the contribution from the four corners
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  if (t0 < 0) {
    n0 = 0.0;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * grad(gi0, x0, y0, z0);
  }
  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  if (t1 < 0) {
    n1 = 0.0;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * grad(gi1, x1, y1, z1);
  }
  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  if (t2 < 0) {
    n2 = 0.0;
  } else {
    t2 *= t2;
    n2 = t2 * t2 * grad(gi2, x2, y2, z2);
  }
  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  if (t3 < 0) {
    n3 = 0.0;
  } else {
    t3 *= t3;
    n3 = t3 * t3 * grad(gi3, x3, y3, z3);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to stay just inside [-1,1]
  return 32.0f * (n0 + n1 + n2 + n3);
}

}