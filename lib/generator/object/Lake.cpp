#include <lib/objects/constants/BiomeList.h>
#include <lib/objects/constants/BlockList.h>
#include <lib/biomes/BiomeClimate.h>
#include <lib/objects/constants/Logic.h>
#include "Lake.h"

const int_fast32_t Lake::LAKE_MAX_HEIGHT = 8;
const int_fast32_t Lake::LAKE_MAX_DIAMETER = 16;

bool Lake::Generate(ChunkManager &world, Random &random, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  double sizeX, sizeY, sizeZ, dx, dy, dz;
  bool succeeded = false;
  sourceY -= 4;

  std::vector<int_fast64_t> lakeMap;
  for (int_fast32_t n = 0; n < random.NextInt(4) + 4; ++n) {
    sizeX = random.NextFloat() * 6.0 + 3;
    sizeY = random.NextFloat() * 4.0 + 2;
    sizeZ = random.NextFloat() * 6.0 + 3;
    dx = random.NextFloat() * (LAKE_MAX_DIAMETER - sizeX - 2) + 1 + sizeX / 2.0;
    dy = random.NextFloat() * (LAKE_MAX_HEIGHT - sizeY - 4) + 2 + sizeY / 2.0;
    dz = random.NextFloat() * (LAKE_MAX_DIAMETER - sizeZ - 2) + 1 + sizeZ / 2.0;

    for (int_fast32_t x = 1; x < LAKE_MAX_DIAMETER - 1; ++x) {
      for (int_fast32_t z = 1; z < LAKE_MAX_DIAMETER - 1; ++z) {
        for (int_fast32_t y = 1; y < LAKE_MAX_HEIGHT - 1; ++y) {
          double nx = (x - dx) / (sizeX / 2.0);
          nx *= nx;
          double ny = (y - dy) / (sizeY / 2.0);
          ny *= ny;
          double nz = (z - dz) / (sizeZ / 2.0);
          nz *= nz;

          if ((nx + ny + nz) < 1.0) {
            SetLakeBlock(lakeMap, x, y, z);
            succeeded = true;
          }
        }
      }
    }
  }

  if (!CanPlace(lakeMap, world, sourceX, sourceY, sourceZ)) return false;

  auto chunk = world.GetChunk(sourceX >> 4, sourceZ >> 4);
  auto biomeArray = chunk->GetBiomeArray();

  int_fast32_t biome = biomeArray.Get((sourceX + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f, (sourceZ + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f);
  bool mycel_biome = biome == MUSHROOM_SHORE;

  for (int_fast32_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast32_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = 0; y < LAKE_MAX_DIAMETER; ++y) {
        if (!IsLakeBlock(lakeMap, x, y, z)) {
          continue;
        }

        MinecraftBlock replaceType = this->type_;
        MinecraftBlock block = world.GetBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        MinecraftBlock blockAbove = world.GetBlockAt(sourceX + x, sourceY + y + 1, sourceZ + z);
        uint8_t blockType = block.GetId();
        uint8_t blockAboveType = blockAbove.GetId();
        if ((blockType == 3 && (blockAboveType == 17 || blockAboveType == 162)) || blockType == 17 || blockType == 162) {
          continue;
        }

        if (y >= (LAKE_MAX_HEIGHT / 2)) {
          replaceType = AIR;
          if (KillWeakBlocksAbove(world, sourceX + x, sourceY + y, sourceZ + z)) {
            break;
          }

          if ((blockType == 79 || blockType == 174) && type_.GetId() == 9) {
            replaceType = block;
          }
        } else if (y == (LAKE_MAX_HEIGHT / 2 - 1)) {
          biome = biomeArray.Get(x & 0x0f, z & 0x0f);
          if (type_.GetId() == 9 && BiomeClimate::IsCold(biome, sourceX + x, y, sourceZ + z)) {
            type_ = ICE;
          }
        }

        transaction_.AddBlockAt(sourceX + x, sourceY + y, sourceZ + z, replaceType);
      }
    }
  }

  for (int_fast32_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast32_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = LAKE_MAX_HEIGHT / 2; y < LAKE_MAX_HEIGHT; ++y) {
        if (!IsLakeBlock(lakeMap, x, y, z)) {
          continue;
        }

        MinecraftBlock block = world.GetBlockAt(sourceX + x, sourceY + y - 1, sourceZ + z);
        MinecraftBlock blockAbove = world.GetBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        if (block.GetId() == 3 && IS_TRANSPARENT(blockAbove.GetId()) && GET_LIGHT_LEVEL(blockAbove.GetId()) > 0) {
          transaction_.AddBlockAt(sourceX + x, sourceY + y - 1, sourceZ + z, mycel_biome ? MYCELIUM : GRASS);
        }
      }
    }
  }

  return succeeded;
}

bool Lake::IsLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  return std::find(lakeMap.begin(), lakeMap.end(), (x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y) != lakeMap.end();
}

void Lake::SetLakeBlock(std::vector<int_fast64_t> &lakeMap, int_fast32_t x, int_fast32_t y, int_fast32_t z) {
  lakeMap.emplace_back((x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y);
}

bool Lake::CanPlace(std::vector<int_fast64_t> &lakeMap, ChunkManager &world, int_fast32_t sourceX, int_fast32_t sourceY, int_fast32_t sourceZ) {
  for (int_fast32_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast32_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = 0; y < LAKE_MAX_HEIGHT; ++y) {

        if (IsLakeBlock(lakeMap, x, y, z)
            || (((x >= (LAKE_MAX_DIAMETER - 1)) || !IsLakeBlock(lakeMap, x + 1, y, z))
                && ((x <= 0) || !IsLakeBlock(lakeMap, x - 1, y, z))
                && ((z >= (LAKE_MAX_DIAMETER - 1)) || !IsLakeBlock(lakeMap, x, y, z + 1))
                && ((z <= 0) || !IsLakeBlock(lakeMap, x, y, z - 1))
                && ((z >= (LAKE_MAX_HEIGHT - 1)) || !IsLakeBlock(lakeMap, x, y + 1, z))
                && ((z <= 0) || !IsLakeBlock(lakeMap, x, y - 1, z)))) {
          continue;
        }

        MinecraftBlock block = world.GetBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        if (y >= LAKE_MAX_HEIGHT / 2 && (IS_LIQUID(block.GetId()) || block.GetId() == 79)) {
          return false; // there's already some liquids above
        }

        if ((y < LAKE_MAX_HEIGHT / 2) && !(IS_SOLID(block.GetId())) && block.GetId() != this->type_.GetId()) {
          return false; // bottom must be solid and do not overlap with another liquid type
        }
      }
    }
  }

  return true;
}

Lake::Lake(MinecraftBlock block, BlockTransaction &transaction) : type_(block), transaction_(transaction) {}