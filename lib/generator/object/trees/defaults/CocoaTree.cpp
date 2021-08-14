#include <lib/objects/math/Facing.h>
#include <lib/objects/math/Vector3.h>
#include "CocoaTree.h"

const int_fast32_t CocoaTree::COCOA_FACES[4] = {Facing::NORTH, Facing::EAST, Facing::SOUTH, Facing::WEST};
const int_fast32_t CocoaTree::COCOA_SIZE[3] = {SIZE_SMALL, SIZE_MEDIUM, SIZE_LARGE};

bool CocoaTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (!JungleTree::Generate(world, random, sourceX, sourceY, sourceZ)) {
    return false;
  }

  // places some vines on the trunk
  AddVinesOnTrunk(sourceX, sourceY, sourceZ, world, random);
  // search for air around leaves to grow hanging vines
  AddVinesOnLeaves(sourceX, sourceY, sourceZ, world, random);
  // and maybe place some cocoa
  AddCocoa(sourceX, sourceY, sourceZ, world, random);

  return true;
}

void CocoaTree::AddVinesOnTrunk(int_fast32_t trunkX, int_fast32_t trunkY, int_fast32_t trunkZ, ChunkManager &world, Random &random) {
  for (int_fast32_t y = 1; y < height; y++) {
    if (random.NextInt(3) != 0 && world.GetBlockAt(trunkX - 1, trunkY + y, trunkZ) == AIR) {
      transaction->AddBlockAt(trunkX - 1, trunkY + y, trunkZ, {1704});
    }
    if (random.NextInt(3) != 0 && world.GetBlockAt(trunkX + 1, trunkY + y, trunkZ) == AIR) {
      transaction->AddBlockAt(trunkX + 1, trunkY + y, trunkZ, {1698});
    }
    if (random.NextInt(3) != 0 && world.GetBlockAt(trunkX, trunkY + y, trunkZ - 1) == AIR) {
      transaction->AddBlockAt(trunkX, trunkY + y, trunkZ - 1, {1697});
    }
    if (random.NextInt(3) != 0 && world.GetBlockAt(trunkX, trunkY + y, trunkZ + 1) == AIR) {
      transaction->AddBlockAt(trunkX, trunkY + y, trunkZ + 1, {1700});
    }
  }
}

void CocoaTree::AddHangingVine(int_fast32_t x, int_fast32_t y, int_fast32_t z, int_fast32_t facing, ChunkManager &world) {
  for (int_fast32_t i = 0; i < 5; i++) {
    if (world.GetBlockAt(x, y - i, z) != AIR) {
      break;
    }

    transaction->AddBlockAt(x, y - i, z, {VINES.GetId(), MinecraftBlock::writeVineBlockFacingMeta(facing)});
  }
}

void CocoaTree::AddVinesOnLeaves(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world, Random &random) {
  for (int_fast32_t y = baseY - 3 + height; y <= baseY + height; y++) {
    int_fast32_t ny = y - (baseY + height);
    int_fast32_t radius = 2 - ny / 2;
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; x++) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; z++) {
        if (world.GetBlockAt(x, y, z).GetId() == 18) {
          if (random.NextInt(4) == 0 && world.GetBlockAt(x - 1, y, z) == AIR) {
            AddHangingVine(x - 1, y, z, Facing::EAST, world);
          }
          if (random.NextInt(4) == 0 && world.GetBlockAt(x + 1, y, z) == AIR) {
            AddHangingVine(x + 1, y, z, Facing::WEST, world);
          }
          if (random.NextInt(4) == 0 && world.GetBlockAt(x, y, z - 1) == AIR) {
            AddHangingVine(x, y, z - 1, Facing::SOUTH, world);
          }
          if (random.NextInt(4) == 0 && world.GetBlockAt(x, y, z + 1) == AIR) {
            AddHangingVine(x, y, z + 1, Facing::NORTH, world);
          }
        }
      }
    }
  }
}

void CocoaTree::AddCocoa(int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ, ChunkManager &world, Random &random) {
  if (height > 5 && random.NextInt(5) == 0) {
    for (int_fast32_t y = 0; y < 2; y++) {
      for (int_fast32_t cocoaFace : COCOA_FACES) { // rotate the 4 trunk faces
        if (random.NextInt(4 - y) == 0) { // higher it is, more chances there is
          int_fast32_t size = COCOA_SIZE[random.NextInt(2)];
          const Vector3 &block = Vector3(sourceX, sourceY, sourceZ).GetSide(cocoaFace);

          if (world.GetBlockAt(block.GetFloorX(), block.GetFloorY(), block.GetFloorZ()) != AIR) {
            continue;
          }

          uint_fast8_t meta = MinecraftBlock::writeLegacyHorizontalFacing(Facing::Opposite(cocoaFace)) | (size << 2);
          transaction->AddBlockAt(block.GetFloorX(), block.GetFloorY(), block.GetFloorZ(), {COCOA_POD.GetId(), meta});
        }
      }
    }
  }
}
