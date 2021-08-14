#include <lib/objects/math/Math.h>
#include "BigOakTree.h"

void BigOakTree::Initialize(Random &random, BlockTransaction &txn) {
  GenericTree::Initialize(random, txn);

  SetHeight(static_cast<int_fast32_t>(random.NextInt(12)) + 5);
}

void BigOakTree::SetMaxLeafDistance(int_fast32_t distance) {
  maxLeafDistance = distance;
}

bool BigOakTree::CanPlace(int_fast32_t baseX, int_fast32_t baseY, int_fast32_t baseZ, ChunkManager &world) {
  Vector3 from = Vector3(baseX, baseY, baseZ);
  Vector3 to = Vector3(baseX, baseY + height - 1, baseZ);
  int_fast32_t blocks = CountAvailableBlocks(from, to, world);
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
    for (int_fast32_t y = 0; y < maxLeafDistance; y++) {
      double size = y > 0 && y < maxLeafDistance - 1.0 ? 3.0 : 2.0;
      int_fast32_t nodeDistance = (int) (0.618 + size);
      for (int_fast32_t x = -nodeDistance; x <= nodeDistance; x++) {
        for (int_fast32_t z = -nodeDistance; z <= nodeDistance; z++) {
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
  for (int_fast32_t y = 0; y < trunkHeight; y++) {
    transaction->AddBlockAt(sourceX, sourceY + y, sourceZ, logType);
  }

  // generate the branches
  for (LeafNode node : leafNodes) {
    if ((node.branchY - sourceY) >= (height * 0.2)) {
      Vector3 base = Vector3(sourceX, node.branchY, sourceZ);
      Vector3 leafNode = Vector3(node.x, node.y, node.z);
      Vector3 branch = leafNode.SubtractVector(base);

      int_fast32_t maxDistance = Math::Max(abs(branch.GetFloorY()), Math::Max(abs(branch.GetFloorX()), abs(branch.GetFloorZ())));
      if (maxDistance > 0) {
        double dx = branch.x / maxDistance;
        double dy = branch.y / maxDistance;
        double dz = branch.z / maxDistance;
        for (int_fast32_t i = 0; i <= maxDistance; i++) {
          Vector3 newBranch = base.Add(0.5 + i * dx, 0.5 + i * dy, 0.5 + i * dz);
          int_fast32_t x = abs(newBranch.GetFloorX() - newBranch.GetFloorX());
          int_fast32_t z = abs(newBranch.GetFloorZ() - newBranch.GetFloorZ());
          int_fast32_t max = Math::Max(x, z);

          uint_fast8_t direction = max > 0 ? max == x ? 4 : 8 : 0; // EAST / SOUTH

          transaction->AddBlockAt(newBranch.GetFloorX(), newBranch.GetFloorY(), newBranch.GetFloorZ(), {logType.GetId(), static_cast<uint_fast8_t>(logType.GetMeta() | direction)});
        }
      }
    }
  }

  return true;
}

int_fast32_t BigOakTree::CountAvailableBlocks(Vector3 from, Vector3 to, ChunkManager &world) {
  int_fast32_t n = 0;
  Vector3 target = to.SubtractVector(from);
  int_fast32_t maxDistance = Math::Max(abs(target.GetFloorY()), Math::Max(abs(target.GetFloorX()), abs(target.GetFloorZ())));
  double dx = target.x / maxDistance;
  double dy = target.y / maxDistance;
  double dz = target.z / maxDistance;
  for (double i = 0; i <= maxDistance; i++, n++) {
    target = from.Add((double) (0.5F + i * dx), 0.5F + i * dy, 0.5F + i * dz);
    if (target.GetFloorY() < 0 || target.GetFloorY() > 255
        || std::find(overrides.begin(), overrides.end(), world.GetBlockAt(target.GetFloorX(), target.GetFloorY(), target.GetFloorZ()).GetId()) == overrides.end()) {
      return n;
    }
  }
  return -1;
}

std::vector<LeafNode> BigOakTree::GenerateLeafNodes(int_fast32_t blockX, int_fast32_t blockY, int_fast32_t blockZ, ChunkManager &world, Random &random) {
  std::vector<LeafNode> leafNodes;
  int_fast32_t y = blockY + height - maxLeafDistance;
  int_fast32_t trunkTopY = blockY + trunkHeight;
  leafNodes.push_back({blockX, y, blockZ, trunkTopY});

  int_fast32_t nodeCount = (int) (1.382 + pow(LEAF_DENSITY * (double) height / 13.0, 2.0));
  nodeCount = nodeCount < 1 ? 1 : nodeCount;

  for (int_fast32_t l = --y - blockY; l >= 0; l--, y--) {
    double h = height / 2.0;
    double v = h - l;
    double f = l < height * 0.3 ? -1.0 : v == h ? h * 0.5 : h <= abs(v) ? 0.0 : sqrt(h * h - v * v) * 0.5;
    if (f >= 0.0F) {
      for (int_fast32_t i = 0; i < nodeCount; i++) {
        double d1 = f * (random.NextFloat() + 0.328);
        double d2 = random.NextFloat() * M_PI * 2.0;
        int_fast32_t x = lround(d1 * sin(d2) + blockX + 0.5);
        int_fast32_t z = lround(d1 * cos(d2) + blockZ + 0.5);
        if (CountAvailableBlocks(Vector3(x, y, z), Vector3(x, y + maxLeafDistance, z), world) == -1) {
          int_fast32_t offX = blockX - x;
          int_fast32_t offZ = blockZ - z;
          double distance = 0.381 * hypot(offX, offZ);
          int_fast32_t branchBaseY = Math::Min(trunkTopY, static_cast<int_fast32_t>(y - distance));
          if (CountAvailableBlocks(Vector3(x, branchBaseY, z), Vector3(x, y, z), world) == -1) {
            leafNodes.push_back({x, y, z, branchBaseY});
          }
        }
      }
    }
  }

  return leafNodes;
}