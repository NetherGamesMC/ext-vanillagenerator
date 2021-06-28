#include "OreVein.h"

#define M_PI 3.14159265358979323846

bool OreVein::generate(SimpleChunkManager world, Random &random, int sourceX, int sourceY, int sourceZ) {
  double amount = type->amount;
  float angle = random.nextFloat() * (float) M_PI;
  double dx1 = sourceX + sin(angle) * amount / 8.0F;
  double dx2 = sourceX - sin(angle) * amount / 8.0F;
  double dz1 = sourceZ + cos(angle) * amount / 8.0F;
  double dz2 = sourceZ - cos(angle) * amount / 8.0F;
  double dy1 = sourceY + static_cast<double>(random.nextBoundedInt(3)) - 2;
  double dy2 = sourceY + static_cast<double>(random.nextBoundedInt(3)) - 2;
  bool succeeded = false;
  for (int i = 0; i < amount; i++) {
    double originX = dx1 + (dx2 - dx1) * i / amount;
    double originY = dy1 + (dy2 - dy1) * i / amount;
    double originZ = dz1 + (dz2 - dz1) * i / amount;
    double q = random.nextFloat() * amount / 16.0;
    double radiusH = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
    double radiusV = (sin(static_cast<double>(i) * (float) M_PI / amount) + 1 * q + 1) / 2.0;
    for (int x = (int) (originX - radiusH); x <= (int) (originX + radiusH); x++) {
      // scale the center of x to the range [-1, 1] within the circle
      double squaredNormalizedX = normalizedSquaredCoordinate(originX, radiusH, x);

      if (squaredNormalizedX >= 1) continue;

      for (int y = (int) (originY - radiusV); y <= (int) (originY + radiusV); y++) {
        double squaredNormalizedY = normalizedSquaredCoordinate(originY, radiusV, y);

        if (squaredNormalizedX + squaredNormalizedY >= 1) continue;

        for (int z = (int) (originZ - radiusH); z <= (int) (originZ + radiusH); z++) {
          double squaredNormalizedZ = normalizedSquaredCoordinate(originZ, radiusH, z);

          if ((squaredNormalizedX + squaredNormalizedY + squaredNormalizedZ) < 1
              && world.getBlockAt(x, y, z).getId() == type->target_type) {
            world.setBlockAt(x, y, z, type->block_type);
            succeeded = true;
          }
        }
      }
    }
  }

  return succeeded;
}