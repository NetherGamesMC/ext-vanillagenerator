#include <lib/objects/math/Vector3.h>
#include "Cactus.h"

using namespace blocks;

bool Cactus::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast16_t sourceY, int_fast32_t sourceZ) {
  if (world.GetBlockAt(sourceX, sourceY, sourceZ)->GetTypeId() == BlockIds::AIR) {
    auto height = random.NextInt(random.NextInt(2) + 1) + 1;

    for (int_fast32_t n = sourceY; n <= sourceY + height; ++n) {
      auto belowBlock = world.GetBlockAt(sourceX, static_cast<int_fast16_t>(n - 1), sourceZ)->GetTypeId();
      if (belowBlock == BlockIds::SAND || (belowBlock == BlockIds::CACTUS && world.GetBlockAt(sourceX, static_cast<int_fast16_t>(n + 1), sourceZ)->GetTypeId() == BlockIds::AIR)) {
        Vector3 iVec = Vector3(sourceX, n, sourceZ);

        for (int_fast32_t facing : Facing::HORIZONTAL) {
          auto face = iVec.GetSide(facing, 1);
          if (world.GetBlockAt(face.x, face.y, face.z)->IsSolid()) {
            return n > sourceY;
          }
        }

        world.SetBlockAt(sourceX, n, sourceZ, MCBlock::GetBlockFromStateId(BlockIds::CACTUS));
      }
    }
  }

  return true;
}