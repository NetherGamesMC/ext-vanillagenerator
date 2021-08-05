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
  if (random.nextFloat() < density_) {
    auto chunk = world.getChunk(chunkX, chunkZ);

    int_fast64_t sourceX = (chunkX << 4) + random.nextInt(16);
    int_fast64_t sourceZ = (chunkZ << 4) + random.nextInt(16);
    int_fast32_t sourceY = chunk->getHighestBlockAt(sourceX & 0x0f, sourceZ & 0x0f);

    sourceY = fixed_height_range_ ? sourceY : (int) random.nextInt(sourceY << 1);

    int_fast32_t y;
    int_fast64_t x, z;
    for (int i = 0; i < 64; i++) {
      x = sourceX + random.nextInt(8) - random.nextInt(8);
      z = sourceZ + random.nextInt(8) - random.nextInt(8);
      y = static_cast<int_fast32_t>(sourceY + random.nextInt(4) - random.nextInt(4));

      auto block = world.getBlockAt(x, y, z);
      auto blockBelow = world.getBlockAt(x, y - 1, z);
      if (y < 255 && block == AIR) {
        bool canPlaceShroom = false;
        switch (blockBelow.getId()) {
          case 243: // PODZOL
          case 110: // MYCELIUM
            canPlaceShroom = true;
            break;
          case 2: // GRASS
            canPlaceShroom = (GET_LIGHT_LEVEL(block.getId()) < 13);
            break;
          case 3: // DIRT
            if (blockBelow.getMeta() == 0x1) { // 0x1: DIRT_FLAG_COARSE
              canPlaceShroom = GET_LIGHT_LEVEL(block.getId()) < 13;
            } else {
              canPlaceShroom = false;
            }
            break;
        }

        if (canPlaceShroom) world.setBlockAt(x, y, z, block_);
      }
    }
  }
}
