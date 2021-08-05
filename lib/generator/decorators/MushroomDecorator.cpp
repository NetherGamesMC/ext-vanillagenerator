#include <lib/objects/constants/BlockList.h>
#include <lib/objects/constants/Logic.h>
#include "MushroomDecorator.h"

void MushroomDecorator::SetUseFixedHeightRange() {
  fixed_height_range_ = true;
}

void MushroomDecorator::SetDensity(double density) {
  density_ = density;
}

void MushroomDecorator::Decorate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  if (random.NextFloat() < density_) {
    auto chunk = world.GetChunk(chunkX, chunkZ);

    int_fast64_t sourceX = (chunkX << 4) + random.NextInt(16);
    int_fast64_t sourceZ = (chunkZ << 4) + random.NextInt(16);
    int_fast32_t sourceY = chunk->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);

    sourceY = fixed_height_range_ ? sourceY : (int) random.NextInt(sourceY << 1);

    int_fast32_t y;
    int_fast64_t x, z;
    for (int i = 0; i < 64; i++) {
      x = sourceX + random.NextInt(8) - random.NextInt(8);
      z = sourceZ + random.NextInt(8) - random.NextInt(8);
      y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

      auto block = world.GetBlockAt(x, y, z);
      auto blockBelow = world.GetBlockAt(x, y - 1, z);
      if (y < 255 && block == AIR) {
        bool canPlaceShroom = false;
        switch (blockBelow.GetId()) {
          case 243: // PODZOL
          case 110: // MYCELIUM
            canPlaceShroom = true;
            break;
          case 2: // GRASS
            canPlaceShroom = (GET_LIGHT_LEVEL(block.GetId()) < 13);
            break;
          case 3: // DIRT
            if (blockBelow.GetMeta() == 0x1) { // 0x1: DIRT_FLAG_COARSE
              canPlaceShroom = GET_LIGHT_LEVEL(block.GetId()) < 13;
            } else {
              canPlaceShroom = false;
            }
            break;
        }

        if (canPlaceShroom) world.SetBlockAt(x, y, z, block_);
      }
    }
  }
}
