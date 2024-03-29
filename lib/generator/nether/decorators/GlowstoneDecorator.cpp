#include <lib/objects/math/Facing.h>
#include <lib/objects/math/Vector3.h>
#include "GlowstoneDecorator.h"

void GlowstoneDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  using namespace blocks;

  int_fast32_t amount = variableAmount ? 1 + static_cast<int_fast32_t>(random.NextInt(1 + random.NextInt(10))) : 10;

  int_fast32_t height = world.GetMaxY();
  int_fast32_t sourceYMargin = 8 * (height >> 7);

  for (int_fast32_t i = 0; i < amount; ++i) {
    int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
    int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));

    auto sourceY = static_cast<int_fast16_t>(4 + random.NextInt(height - sourceYMargin));

    auto airBlock = MCBlock::GetBlockFromStateId(BlockIds::AIR);
    auto block = world.GetBlockAt(sourceX, sourceY, sourceZ);
    if (block != airBlock || world.GetBlockAt(sourceX, sourceY + 1, sourceZ)->GetStateId() != BlockIds::NETHERRACK) {
      continue;
    }

    auto glowstone = MCBlock::GetBlockFromStateId(BlockIds::GLOWSTONE);
    world.SetBlockAt(sourceX, sourceY, sourceZ, glowstone);

    for (int_fast32_t j = 0; j < 1500; ++j) {
      int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
      int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
      int_fast32_t y = static_cast<int_fast16_t>(sourceY - random.NextInt(12));

      if (world.GetBlockAt(x, y, z) != airBlock) {
        continue;
      }

      int_fast32_t glowstoneBlockCount = 0;
      Vector3 vector = Vector3(x, y, z);
      for (int_fast32_t face : {Facing::EAST, Facing::WEST, Facing::DOWN, Facing::UP, Facing::SOUTH, Facing::NORTH}) {
        auto pos = vector.GetSide(face, 1);
        if (world.GetBlockAt(pos.GetFloorX(), static_cast<int_fast16_t>(pos.GetFloorY()), pos.GetFloorZ()) == glowstone) {
          ++glowstoneBlockCount;
        }
      }

      if (glowstoneBlockCount == 1) {
        world.SetBlockAt(x, y, z, glowstone);
      }
    }
  }
}
