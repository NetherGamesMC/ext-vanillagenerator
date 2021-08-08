#include <lib/objects/math/Math.h>
#include "BigOakTree.h"

void BigOakTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(12)) + 5);
}

void BigOakTree::SetMaxLeafDistance(int distance) {
  maxLeafDistance = distance;
}

bool BigOakTree::CanPlaceOn(MinecraftBlock soil) {
  Vector from = new Vector(baseX, baseY, baseZ);
  Vector to = new Vector(baseX, baseY + height - 1, baseZ);
  int blocks = countAvailableBlocks(from, to, world);
  if (blocks == -1) {
    return true;
  } else if (blocks > 5) {
    height = blocks;
    return true;
  }
  return false;
}

int BigOakTree::CountAvailableBlocks(Vector3 from, Vector3 to, ChunkManager &world) {
  int n = 0;
  Vector3 target = Subtract(to, from);
  int maxDistance = Math::Max(abs(target.y), Math::Max(abs(target.x), abs(target.z)));
  float dx = (float) target.x / maxDistance;
  float dy = (float) target.y / maxDistance;
  float dz = (float) target.z / maxDistance;
  for (int i = 0; i <= maxDistance; i++, n++) {
    target = from.clone()
        .add(new Vector((double) (0.5F + i * dx), 0.5F + i * dy, 0.5F + i * dz));
    if (target.getBlockY() < 0 || target.getBlockY() > 255
    || !overridables.contains(blockTypeAt(
        target.getBlockX(), target.getBlockY(), target.getBlockZ(), world))) {
      return n;
    }
  }
  return -1;
}
