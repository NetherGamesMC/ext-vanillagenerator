#include "OreVein.h"

#define M_PI 3.14159265358979323846

bool OreVein::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) const {
  double amount = oreType_.amount;
  float angle = random.NextFloat() * (float) M_PI;
  double dx1 = static_cast<double>(sourceX) + (sin(angle) * amount / 8.0F);
  double dx2 = static_cast<double>(sourceX) - (sin(angle) * amount / 8.0F);
  double dz1 = static_cast<double>(sourceZ) + (cos(angle) * amount / 8.0F);
  double dz2 = static_cast<double>(sourceZ) - (cos(angle) * amount / 8.0F);
  double dy1 = sourceY + static_cast<double>(random.NextInt(3)) - 2;
  double dy2 = sourceY + static_cast<double>(random.NextInt(3)) - 2;

  bool succeeded = false;

  for (int_fast32_t i = 0; i < amount; i++) {
    double originX = dx1 + (dx2 - dx1) * i / amount;
    double originY = dy1 + (dy2 - dy1) * i / amount;
    double originZ = dz1 + (dz2 - dz1) * i / amount;
    double q = random.NextFloat() * amount / 16.0;
    double radiusH = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
    double radiusV = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
    for (int_fast32_t x = (int) (originX - radiusH); x <= (int) (originX + radiusH); x++) {
      // scale the center of x to the range [-1, 1] within the circle
      double squaredNormalizedX = NormalizedSquaredCoordinate(originX, radiusH, x);

      if (squaredNormalizedX >= 1) continue;

      for (int_fast32_t y = (int) (originY - radiusV); y <= (int) (originY + radiusV); y++) {
        double squaredNormalizedY = NormalizedSquaredCoordinate(originY, radiusV, y);

        if (squaredNormalizedX + squaredNormalizedY >= 1) continue;

        for (int_fast32_t z = (int) (originZ - radiusH); z <= (int) (originZ + radiusH); z++) {
          double squaredNormalizedZ = NormalizedSquaredCoordinate(originZ, radiusH, z);
          double normalized = squaredNormalizedX + squaredNormalizedY + squaredNormalizedZ;

          if (normalized < 1 && world.GetBlockAt(x, y, z).GetId() == oreType_.targetType) {
            world.SetBlockAt(x, y, z, oreType_.blockType);
            succeeded = true;
          }
        }
      }
    }
  }

  return succeeded;
}

double OreVein::NormalizedSquaredCoordinate(double origin, double radius, int_fast32_t x) {
  double squared_normalized_x = (x + 0.5 - origin) / radius;
  squared_normalized_x *= squared_normalized_x;

  return squared_normalized_x;
}

OreVein::OreVein(OreType oreType) : oreType_(oreType) {}