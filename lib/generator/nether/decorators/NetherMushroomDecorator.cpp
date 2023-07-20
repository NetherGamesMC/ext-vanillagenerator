
#include "NetherMushroomDecorator.h"

void NetherMushroomDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  using namespace blocks;

  int_fast32_t height = world.GetMaxY();

  int_fast32_t sourceX = (chunkX << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  int_fast32_t sourceZ = (chunkZ << 4) + static_cast<int_fast32_t>(random.NextInt(16));
  auto sourceY = static_cast<int_fast16_t>(random.NextInt(height));

  auto airBlock = MCBlock::GetBlockFromStateId(BlockIds::AIR);
  auto netherrackBlock = MCBlock::GetBlockFromStateId(BlockIds::NETHERRACK);
  auto netherQuartzOre = MCBlock::GetBlockFromStateId(BlockIds::NETHER_QUARTZ_ORE);
  auto soulSand = MCBlock::GetBlockFromStateId(BlockIds::SOUL_SAND);
  auto gravel = MCBlock::GetBlockFromStateId(BlockIds::GRAVEL);

  for (int_fast32_t i = 0; i < 64; ++i) {
    int_fast32_t x = sourceX + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    int_fast32_t z = sourceZ + static_cast<int_fast32_t>(random.NextInt(8) - random.NextInt(8));
    auto y = static_cast<int_fast16_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    auto block = world.GetBlockAt(x, y, z);
    auto blockBelow = world.GetBlockAt(x, y - 1, z);
    if (y < height && block == airBlock) {
      bool found = false;
      for (auto mcBlock : {netherrackBlock, netherQuartzOre, soulSand, gravel}) {
        if (mcBlock == blockBelow) {
          found = true;
          break;
        }
      }

      if (found) {
        world.SetBlockAt(x, y, z, type);
      }
    }
  }
}