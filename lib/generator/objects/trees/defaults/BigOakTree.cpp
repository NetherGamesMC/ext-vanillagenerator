#include <lib/objects/math/Math.h>
#include "BigOakTree.h"

void BigOakTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(12)) + 5);
}

void BigOakTree::SetMaxLeafDistance(int distance) {
  maxLeafDistance = distance;
}

bool BigOakTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  Vector3 from = {baseX, baseY, baseZ};
  Vector3 to = {baseX, baseY + height - 1, baseZ};
  int blocks = CountAvailableBlocks(from, to, world);
  if (blocks == -1) {
    return true;
  } else if (blocks > 5) {
    height = blocks;
    return true;
  }
  return false;
}

bool BigOakTree::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  if (!CanPlaceOn(world.GetBlockAt(sourceX, sourceY - 1, sourceZ)) || !CanPlace(sourceX, sourceY, sourceZ, world)) {
    return false;
  }

  trunkHeight = (int) (height * 0.618);
  if (trunkHeight >= height) {
    trunkHeight = height - 1;
  }

  std::vector<LeafNode> leafNodes = GenerateLeafNodes(sourceX, sourceY, sourceZ, world, random);

  for (LeafNode node : leafNodes) {
    for (int y = 0; y < maxLeafDistance; y++) {
      double size = y > 0 && y < maxLeafDistance - 1.0 ? 3.0 : 2.0;
      int nodeDistance = (int) (0.618 + size);
      for (int x = -nodeDistance; x <= nodeDistance; x++) {
        for (int z = -nodeDistance; z <= nodeDistance; z++) {
          double sizeX = abs(x) + 0.5;
          double sizeZ = abs(z) + 0.5;
          if (sizeX * sizeX + sizeZ * sizeZ <= size * size && std::find(overrides.begin(), overrides.end(), world.GetBlockAt(node.x, node.y, node.z).GetId()) != overrides.end()) {
            transaction->AddBlockAt(node.x + x, node.y + y, node.z + z, leavesTypes);
          }
        }
      }
    }
  }

  // generate the trunk
  for (int y = 0; y < trunkHeight; y++) {
    transaction->AddBlockAt(sourceX, sourceY + y, sourceZ, logType);
  }

  // generate the branches
  for (LeafNode node : leafNodes) {
    if ((node.branchY - sourceY) >= (height * 0.2)) {
      Vector3 base = {sourceX, node.branchY, sourceZ};
      Vector3 leafNode = {node.x, node.y, node.z};
      Vector3 branch = leafNode.SubtractVector(base);

      int maxDistance = Math::Max(abs(branch.getFloorY()), Math::Max(abs(branch.getFloorX()), abs(branch.getFloorZ())));
      if (maxDistance > 0) {
        double dx = branch.x / maxDistance;
        double dy = branch.y / maxDistance;
        double dz = branch.z / maxDistance;
        for (int i = 0; i <= maxDistance; i++) {
          Vector3 newBranch = base.Add(0.5 + i * dx, 0.5 + i * dy, 0.5 + i * dz);
          int x = abs(newBranch.getFloorX() - newBranch.getFloorX());
          int z = abs(newBranch.getFloorZ() - newBranch.getFloorZ());
          int max = Math::Max(x, z);

          uint_fast8_t direction = max > 0 ? max == x ? 4 : 8 : 0; // EAST / SOUTH

          transaction->AddBlockAt(newBranch.getFloorX(), newBranch.getFloorY(), newBranch.getFloorZ(), {logType.GetId(), static_cast<uint_fast8_t>(logType.GetMeta() | direction)});
        }
      }
    }
  }

  return true;
}

int BigOakTree::CountAvailableBlocks(Vector3 from, Vector3 to, ChunkManager &world) {
  int n = 0;
  Vector3 target = to.SubtractVector(from);
  int maxDistance = Math::Max(abs(target.getFloorY()), Math::Max(abs(target.getFloorX()), abs(target.getFloorZ())));
  double dx = target.x / maxDistance;
  double dy = target.y / maxDistance;
  double dz = target.z / maxDistance;
  for (double i = 0; i <= maxDistance; i++, n++) {
    target = from.Add((double) (0.5F + i * dx), 0.5F + i * dy, 0.5F + i * dz);
    if (target.getFloorY() < 0 || target.getFloorY() > 255
        || std::find(overrides.begin(), overrides.end(), world.GetBlockAt(target.getFloorX(), target.getFloorY(), target.getFloorZ()).GetId()) == overrides.end()) {
      return n;
    }
  }
  return -1;
}

std::vector<LeafNode> BigOakTree::GenerateLeafNodes(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ, ChunkManager &world, Random &random) {
  std::vector<LeafNode> leafNodes;
  int y = blockY + height - maxLeafDistance;
  int trunkTopY = blockY + trunkHeight;
  leafNodes.push_back({blockX, y, blockZ, trunkTopY});

  int nodeCount = (int) (1.382 + pow(LEAF_DENSITY * (double) height / 13.0, 2.0));
  nodeCount = nodeCount < 1 ? 1 : nodeCount;

  for (int l = --y - blockY; l >= 0; l--, y--) {
    double h = height / 2.0;
    double v = h - l;
    double f = l < height * 0.3 ? -1.0 : v == h ? h * 0.5 : h <= abs(v) ? 0.0 : sqrt(h * h - v * v) * 0.5;
    if (f >= 0.0F) {
      for (int i = 0; i < nodeCount; i++) {
        double d1 = f * (random.NextFloat() + 0.328);
        double d2 = random.NextFloat() * M_PI * 2.0;
        int x = lround(d1 * sin(d2) + blockX + 0.5);
        int z = lround(d1 * cos(d2) + blockZ + 0.5);
        if (CountAvailableBlocks({x, y, z}, {x, y + maxLeafDistance, z}, world) == -1) {
          int offX = blockX - x;
          int offZ = blockZ - z;
          double distance = 0.381 * hypot(offX, offZ);
          int branchBaseY = Math::Min(trunkTopY, (int) (y - distance));
          if (CountAvailableBlocks({x, branchBaseY, z}, {x, y, z}, world) == -1) {
            leafNodes.push_back({x, y, z, branchBaseY});
          }
        }
      }
    }
  }

  return leafNodes;
}