#include "BrownMushroomTree.h"

void BrownMushroomTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetOverrides({0, 18, 161});
  SetHeight(static_cast<int_fast32_t>(random.NextInt(3)) + 4);
}

bool BrownMushroomTree::CanPlaceOn(MinecraftBlock soil) {
  return soil == GRASS || soil == DIRT || soil == MYCELIUM;
}

bool BrownMushroomTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (CannotGenerateAt(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  // generate the stem
  for (int_fast32_t y = 0; y < height; y++) {
    transaction->AddBlockAt(sourceX, sourceY + y, sourceZ, MUSHROOM_STEM); // stem texture
  }

  // TODO: 1.13, replace with MultipleFacing BlockData
  // get the mushroom's cap Y start
  int_fast32_t capY = sourceY + height; // for brown mushroom it starts on top directly
  if (type == RED_MUSHROOM_BLOCK) {
    capY = sourceY + height - 3; // for red mushroom, cap's thickness is 4 blocks
  }

  // generate mushroom's cap
  for (int_fast32_t y = capY; y <= sourceY + height; y++) { // from bottom to top of mushroom
    int_fast32_t radius = 1; // radius for the top of red mushroom
    if (y < sourceY + height) {
      radius = 2; // radius for red mushroom cap is 2
    }
    if (type == BROWN_MUSHROOM_BLOCK) {
      radius = 3; // radius always 3 for a brown mushroom
    }

    // loop over horizontal slice
    for (int_fast32_t x = sourceX - radius; x <= sourceX + radius; x++) {
      for (int_fast32_t z = sourceZ - radius; z <= sourceZ + radius; z++) {
        uint_fast8_t data = 5; // cap texture on top
        // cap's borders/corners treatment
        if (x == sourceX - radius) {
          data = 4; // cap texture on top and west
        } else if (x == sourceX + radius) {
          data = 6; // cap texture on top and east
        }
        if (z == sourceZ - radius) {
          data -= 3;
        } else if (z == sourceZ + radius) {
          data += 3;
        }

        // corners shrink treatment
        // if it's a brown mushroom we need it always
        // it's a red mushroom, it's only applied below the top
        if (type == BROWN_MUSHROOM_BLOCK || y < sourceY + height) {

          // excludes the real corners of the cap structure
          if ((x == sourceX - radius || x == sourceX + radius)
          && (z == sourceZ - radius || z == sourceZ + radius)) {
            continue;
          }

          // mushroom's cap corners treatment
          if (x == sourceX - (radius - 1)
          && z == sourceZ - radius) {
            data = 1; // cap texture on top, west and north
          } else if (x == sourceX - radius
          && z == sourceZ - (radius - 1)) {
            data = 1; // cap texture on top, west and north
          } else if (x == sourceX + radius - 1
          && z == sourceZ - radius) {
            data = 3; // cap texture on top, north and east
          } else if (x == sourceX + radius
          && z == sourceZ - (radius - 1)) {
            data = 3; // cap texture on top, north and east
          } else if (x == sourceX - (radius - 1)
          && z == sourceZ + radius) {
            data = 7; // cap texture on top, south and west
          } else if (x == sourceX - radius
          && z == sourceZ + radius - 1) {
            data = 7; // cap texture on top, south and west
          } else if (x == sourceX + radius - 1
          && z == sourceZ + radius) {
            data = 9; // cap texture on top, east and south
          } else if (x == sourceX + radius
          && z == sourceZ + radius - 1) {
            data = 9; // cap texture on top, east and south
          }
        }

        // a data of 5 below the top layer means air
        if (data != 5 || y >= sourceY + height) {
          // TODO: 1.13, set BlockData
          transaction->AddBlockAt(x, y, z, {type.GetId(), data});
        }
      }
    }
  }

  return true;
}

bool BrownMushroomTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  for (int_fast32_t y = baseY; y <= baseY + 1 + height; y++) {
    // Space requirement is 7x7 blocks, so brown mushroom's cap
    // can be directly touching a mushroom next to it.
    // Since red mushrooms fits in 5x5 blocks it will never
    // touch another huge mushroom.
    int_fast32_t radius = 3;
    if (y <= baseY + 3) {
      radius = 0; // radius is 0 below 4 blocks tall (only the stem to take in account)
    }

    // check for block collision on horizontal slices
    for (int_fast32_t x = baseX - radius; x <= baseX + radius; x++) {
      for (int_fast32_t z = baseZ - radius; z <= baseZ + radius; z++) {
        if (y < 0 || y >= 256) { // height out of range
          return false;
        }
        // skip source block check
        if (y != baseY || x != baseX || z != baseZ) {
          // we can overlap leaves around
          const MinecraftBlock &blockType = world.GetBlockAt(x, y, z);
          if (std::find(overrides.begin(), overrides.end(), blockType.GetId()) == overrides.end()) {
            return false;
          }
        }
      }
    }
  }
  return true;
}


