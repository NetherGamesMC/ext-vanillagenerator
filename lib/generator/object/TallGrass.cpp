#include <lib/objects/constants/BlockList.h>
#include "TallGrass.h"

bool TallGrass::Generate(ChunkManager &world,
                         Random &random,
                         int_fast32_t sourceX,
                         int_fast32_t sourceY,
                         int_fast32_t sourceZ) {

  uint_fast32_t currentBlockId;
  do {
    currentBlockId = world.GetBlockAt(sourceX, sourceY, sourceZ).GetId();
    --sourceY;
  } while ((currentBlockId == AIR.GetId() || currentBlockId == 18) && sourceY > 0);
  ++sourceY;

  int_fast32_t x, z;
  int_fast32_t y;

  bool succeeded = false;
  int_fast32_t height = world.GetMaxY();
  for (int_fast32_t i = 0; i < 128; ++i) {
    x = sourceX + random.NextInt(8) - random.NextInt(8);
    z = sourceZ + random.NextInt(8) - random.NextInt(8);
    y = static_cast<int_fast32_t>(sourceY + random.NextInt(4) - random.NextInt(4));

    MinecraftBlock blockType = world.GetBlockAt(x, y, z);
    MinecraftBlock blockTypeBelow = world.GetBlockAt(x, y - 1, z);
    if (y < height && blockType == AIR && (blockTypeBelow == GRASS || blockTypeBelow == DIRT)) {
      world.SetBlockAt(x, y, z, grassType_);
      succeeded = true;
    }
  }

  return succeeded;
}