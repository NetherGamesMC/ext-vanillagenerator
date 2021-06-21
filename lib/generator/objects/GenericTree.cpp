#include <lib/pocketmine/Logic.h>
#include "GenericTree.h"

bool GenericTree::generate(SimpleChunkManager world, Random &random, int source_x, int source_y, int source_z) {
  if (cannotGenerateAt(source_x, source_y, source_z, world)) {
    return false;
  }

  // generate the leaves
  for (int y = source_y + height - 3; y <= source_y + height; ++y) {
    int n = y - (source_y + height);
    int radius = (int) (1 - n / 2);
    for (int x = source_x - radius; x <= source_x + radius; ++x) {
      for (int z = source_z - radius; z <= source_z + radius; ++z) {
        if (abs(x - source_x) != radius || abs(z - source_z) != radius || (random.nextBoolean() && n != 0)) {
          replaceIfAirOrLeaves(x, y, z, leavesTypes, world);
        }
      }
    }
  }

  // generate the trunk
  for (int y = 0; y < height; ++y) {
    replaceIfAirOrLeaves(source_x, source_y + y, source_z, logType, world);
  }

  // block below trunk is always dirt
  transaction.addBlockAt(source_x, source_y - 1, source_z, new MinecraftBlock(3, 0));

  return true;
}

bool GenericTree::canHeightFit(int base_height) const {
  return base_height >= 1 && base_height + height + 1 < Y_MAX;
}

bool GenericTree::canPlace(int base_x, int base_y, int base_z, SimpleChunkManager world) {
  for (int y = base_y; y <= base_y + 1 + height; ++y) {
    // Space requirement
    int radius = 1; // default radius if above first block
    if (y == base_y) {
      radius = 0; // radius at source block y is 0 (only trunk)
    } else if (y >= base_y + 1 + height - 2) {
      radius = 2; // max radius starting at leaves bottom
    }

    // check for block collision on horizontal slices
    for (int x = base_x - radius; x <= base_x + radius; ++x) {
      for (int z = base_z - radius; z <= base_z + radius; ++z) {
        if (y >= 0 && y < world.getMaxY()) {
          // we can overlap some blocks around
          if (std::find(overrides.begin(), overrides.end(), world.getBlockAt(x, y, z).getId()) == overrides.end()) {
            return false;
          }
        } else { // height out of range
          return false;
        }
      }
    }
  }

  return true;
}

void GenericTree::replaceIfAirOrLeaves(int x, int y, int z, MinecraftBlock newBlock, SimpleChunkManager world) {
  auto oldBlock = world.getBlockAt(x, y, z).getId();
  if (oldBlock == 0 || oldBlock == 18) {
    transaction.addBlockAt(x, y, z, new MinecraftBlock(newBlock.getId(), newBlock.getMeta()));
  }
}

bool GenericTree::canPlaceOn(MinecraftBlock soil) {
  uint_fast16_t type = soil.getId();

  return type == GRASS.getId() || type == DIRT.getId() || type == FARMLAND.getId();
}

bool GenericTree::cannotGenerateAt(int base_x, int base_y, int base_z, SimpleChunkManager world) {
  return !canHeightFit(base_y)
      || !canPlaceOn(world.getBlockAt(base_x, base_y - 1, base_z))
      || !canPlace(base_x, base_y, base_z, world);
}