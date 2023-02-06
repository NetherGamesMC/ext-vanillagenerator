#include <noise/Noise.h>

namespace FastNoise2 {

PerlinNoise::PerlinNoise(Random &random) : NoiseBase(random) {

}

void PerlinNoise::GetNoise2d(NoiseArray &noise_array, float x, float y, float frequency, float amplitude) {
  const float scaleX = settings_.scale_[(int) Pos::X];
  const float scaleY = settings_.scale_[(int) Pos::Y];

  int_fast32_t index = 0;
  for (int_fast32_t i = 0; i < settings_.size_[(int) Pos::X]; i++) {
    auto dx = x + settings_.offset_[(int) Pos::X] + i * scaleX;

    auto floorX = fast_floor(dx);
    auto ix = floorX & 255;

    dx -= floorX;

    const auto fx = fade(dx);
    for (int_fast32_t j = 0; j < settings_.size_[(int) Pos::Y]; j++) {
      auto dz = y +  settings_.offset_[(int) Pos::Y] + j * scaleY;

      auto floorZ = fast_floor(dz);
      auto iz = floorZ & 255;

      dz -= floorZ;

      const auto fz = fade(dz);

      // Hash coordinates of the square corners
      int_fast32_t ab = permutations_.at(permutations_.at(ix)) + iz;
      int_fast32_t bb = permutations_.at(permutations_.at(ix + 1)) + iz;

      const auto x1 = lerp(fx,
                           grad(permutations_.at(ab), dx, 0, dz),
                           grad(permutations_.at(bb), dx - 1, 0, dz));
      const auto x2 = lerp(fx,
                           grad(permutations_.at(ab + 1), dx, 0, dz - 1),
                           grad(permutations_.at(bb + 1), dx - 1, 0, dz - 1));

      noise_array.at(index++) += lerp(fz, x1, x2) * amplitude;
    }
  }
}

void PerlinNoise::GetNoise3d(NoiseArray &noise_array, float x, float y, float z, float frequency, float amplitude) {
  const int_fast32_t sizeX = settings_.size_[(int) Pos::X];
  const int_fast32_t sizeY = settings_.size_[(int) Pos::Y];
  const int_fast32_t sizeZ = settings_.size_[(int) Pos::Z];

  const float scaleX = settings_.scale_[(int) Pos::X] * frequency;
  const float scaleY = settings_.scale_[(int) Pos::Y] * frequency;
  const float scaleZ = settings_.scale_[(int) Pos::Z] * frequency;

  const float offsetX = settings_.offset_[(int) Pos::X];
  const float offsetY = settings_.offset_[(int) Pos::Y];
  const float offsetZ = settings_.offset_[(int) Pos::Z];

  int_fast32_t n = -1, index = 0;

  float x1 = 0;
  float x2 = 0;
  float x3 = 0;
  float x4 = 0;

  for (int_fast32_t i = 0; i < sizeX; i++) {
    auto dx = x + offsetX + i * scaleX;

    auto floorX = fast_floor(dx);
    auto ix = floorX & 255;

    dx -= floorX;

    const auto fx = fade(dx);
    for (int_fast32_t j = 0; j < sizeZ; j++) {
      auto dz = z + offsetZ + j * scaleZ;

      auto floorZ = fast_floor(dz);
      auto iz = floorZ & 255;

      dz -= floorZ;

      const auto fz = fade(dz);
      for (int_fast32_t k = 0; k < sizeY; k++) {
        auto dy = y + offsetY + k * scaleY;

        auto floorY = fast_floor(dy);
        auto iy = floorY & 255;

        dy -= floorY;

        const auto fy = fade(dy);
        if (k == 0 || iy != n) {
          n = iy;

          // Hash coordinates of the cube corners
          int_fast32_t aa = permutations_.at(ix) + iy;
          int_fast32_t ab = permutations_.at(aa) + iz;
          int_fast32_t ac = permutations_.at(aa + 1) + iz;
          int_fast32_t ba = permutations_.at(ix + 1) + iy;
          int_fast32_t bb = permutations_.at(ba) + iz;
          int_fast32_t bc = permutations_.at(ba + 1) + iz;

          x1 = lerp(fx,
                    grad(permutations_.at(ab), dx, dy, dz),
                    grad(permutations_.at(bb), dx - 1, dy, dz));
          x2 = lerp(fx,
                    grad(permutations_.at(ac), dx, dy - 1, dz),
                    grad(permutations_.at(bc), dx - 1, dy - 1, dz));
          x3 = lerp(fx,
                    grad(permutations_.at(ab + 1), dx, dy, dz - 1),
                    grad(permutations_.at(bb + 1), dx - 1, dy, dz - 1));
          x4 = lerp(fx,
                    grad(permutations_.at(ac + 1), dx, dy - 1, dz - 1),
                    grad(permutations_.at(bc + 1), dx - 1, dy - 1, dz - 1));
        }

        auto y1 = lerp(fy, x1, x2);
        auto y2 = lerp(fy, x3, x4);

        noise_array.at(index++) += lerp(fz, y1, y2) * amplitude;
      }
    }
  }
}

}