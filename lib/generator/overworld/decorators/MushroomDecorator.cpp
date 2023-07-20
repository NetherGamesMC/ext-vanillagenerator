#include "MushroomDecorator.h"

void MushroomDecorator::SetUseFixedHeightRange() {
  fixedHeightRange_ = true;
}

void MushroomDecorator::SetDensity(double density) {
  density_ = density;
}

void MushroomDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  if (random.NextFloat() < density_) {
    auto chunk = world.GetChunk(chunkX, chunkZ);

    int_fast32_t sourceX = (chunkX << 4) + random.NextInt(16);
    int_fast32_t sourceZ = (chunkZ << 4) + random.NextInt(16);
    int_fast32_t sourceY = chunk->GetHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);

    sourceY = fixedHeightRange_ ? sourceY : (int) random.NextInt(sourceY << 1);

    int_fast32_t y;
    int_fast32_t x, z;
    for (int_fast32_t i = 0; i < 64; i++) {
      x = sourceX + random.NextInt(8) - random.NextInt(8);
      z = sourceZ + random.NextInt(8) - random.NextInt(8);
      y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

      auto block = world.GetBlockAt(x, y, z);
      auto blockBelow = world.GetBlockAt(x, y - 1, z);
      if (y < 255 && block->GetTypeId() == BlockIds::AIR) {
        bool canPlaceShroom = false;
        switch (blockBelow->GetTypeId()) {
          case BlockIds::PODZOL:
          case BlockIds::MYCELIUM:
            canPlaceShroom = true;
            break;
          case BlockIds::GRASS:
            canPlaceShroom = block->GetLightLevel() < 13;
            break;
          case BlockIds::DIRT:
            if (blockBelow->GetBlockMeta() == 0x0) { // 0x0: DIRT_FLAG_COARSE
              canPlaceShroom = block->GetLightLevel() < 13;
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
