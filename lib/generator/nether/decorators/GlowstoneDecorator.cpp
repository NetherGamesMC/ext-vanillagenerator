#include <lib/objects/constants/BlockList.h>
#include <lib/objects/math/Facing.h>
#include <lib/objects/math/Vector3.h>
#include "GlowstoneDecorator.h"

void GlowstoneDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t amount = variableAmount ? 1 + random.NextInt(1 + random.NextInt(10)) : 10;

  int_fast32_t height = world.GetMaxY();
  int_fast32_t sourceYMargin = 8 * (height >> 7);

  for (int_fast32_t i = 0; i < amount; ++i) {
    int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
    int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
    int_fast32_t sourceY = 4 + random.NextInt(height - sourceYMargin);

    int_fast32_t block = world.GetBlockAt(sourceX, sourceY, sourceZ).GetId();
    if (block != AIR.GetId() || world.GetBlockAt(sourceX, sourceY + 1, sourceZ).GetId() != NETHERRACK.GetId()) {
      continue;
    }

    world.SetBlockAt(sourceX, sourceY, sourceZ, GLOWSTONE);

    for (int_fast32_t j = 0; j < 1500; ++j) {
      int_fast32_t x = sourceX + random.NextInt(8) - random.NextInt(8);
      int_fast32_t z = sourceZ + random.NextInt(8) - random.NextInt(8);
      int_fast32_t y = sourceY - random.NextInt(12);
      int_fast32_t block = world.GetBlockAt(x, y, z).GetId();
      if (block != AIR.GetId()) {
        continue;
      }

      int_fast32_t glowstoneBlockCount = 0;
      Vector3 vector = { x, y, z };
      for (int face : {Facing::EAST, Facing::WEST, Facing::DOWN, Facing::UP, Facing::SOUTH, Facing::NORTH}) {
        auto pos = GetSide(vector, face, 1);
        if (world.GetBlockAt(pos.x, pos.y, pos.z).GetId() == GLOWSTONE.GetId()) {
          ++glowstoneBlockCount;
        }
      }

      if (glowstoneBlockCount == 1) {
        world.SetBlockAt(x, y, z, GLOWSTONE);
      }
    }
  }
}
