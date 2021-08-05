#include <lib/objects/constants/BiomeList.h>
#include <lib/objects/constants/BlockList.h>
#include <lib/biomes/BiomeClimate.h>
#include <lib/objects/constants/Logic.h>
#include "Lake.h"

bool Lake::Generate(ChunkManager world,
                    Random &random,
                    int_fast64_t sourceX,
                    int_fast32_t sourceY,
                    int_fast64_t sourceZ) {

  double size_x, size_y, size_z, dx, dy, dz;
  bool succeeded = false;
  sourceY -= 4;

  std::vector<int_fast64_t> lake_map;
  for (int n = 0; n < random.nextInt(4) + 4; ++n) {
    size_x = random.nextFloat() * 6.0 + 3;
    size_y = random.nextFloat() * 4.0 + 2;
    size_z = random.nextFloat() * 6.0 + 3;
    dx = random.nextFloat() * (LAKE_MAX_DIAMETER - size_x - 2) + 1 + size_x / 2.0;
    dy = random.nextFloat() * (LAKE_MAX_HEIGHT - size_y - 4) + 2 + size_y / 2.0;
    dz = random.nextFloat() * (LAKE_MAX_DIAMETER - size_z - 2) + 1 + size_z / 2.0;

    for (int x = 1; x < LAKE_MAX_DIAMETER - 1; ++x) {
      for (int z = 1; z < LAKE_MAX_DIAMETER - 1; ++z) {
        for (int y = 1; y < LAKE_MAX_HEIGHT - 1; ++y) {
          double nx = (x - dx) / (size_x / 2.0);
          nx *= nx;
          double ny = (y - dy) / (size_y / 2.0);
          ny *= ny;
          double nz = (z - dz) / (size_z / 2.0);
          nz *= nz;

          if ((nx + ny + nz) < 1.0) {
            SetLakeBlock(lake_map, x, y, z);
            succeeded = true;
          }
        }
      }
    }
  }

  if (!CanPlace(lake_map, world, sourceX, sourceY, sourceZ))return false;

  auto chunk = world.getChunk(sourceX >> 4, sourceZ >> 4);
  auto biomeArray = chunk->getBiomeArray();

  int biome = biomeArray->get((sourceX + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f,
                              (sourceZ + 8 + LAKE_MAX_DIAMETER / 2) & 0x0f);

  bool mycel_biome = biome == MUSHROOM_SHORE;

  for (int_fast64_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast64_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = 0; y < LAKE_MAX_DIAMETER; ++y) {
        if (!IsLakeBlock(lake_map, x, y, z)) {
          continue;
        }

        MinecraftBlock replaceType = this->type_;
        MinecraftBlock block = world.getBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        MinecraftBlock block_above = world.getBlockAt(sourceX + x, sourceY + y + 1, sourceZ + z);
        uint8_t block_type = block.getId();
        uint8_t block_above_type = block_above.getId();
        if ((block_type == 3 && (block_above_type == 17 || block_above_type == 162)) || block_type == 17 ||
            block_type == 162) {
          continue;
        }

        if (y >= (LAKE_MAX_HEIGHT / 2)) {
          replaceType = AIR;
          if (killWeakBlocksAbove(world, sourceX + x, sourceY + y, sourceZ + z)) {
            break;
          }

          if ((block_type == 79 || block_type == 174) && this->type_.getId() == 9) {
            replaceType = block;
          }
        } else if (y == (LAKE_MAX_HEIGHT / 2 - 1)) {
          biome = biomeArray->get(x & 0x0f, z & 0x0f);
          if (type_.getId() == 9 && Biome::isCold(biome, sourceX + x, y, sourceZ + z)) {
            type_ = ICE;
          }
        }

        transaction_.AddBlockAt(sourceX + x, sourceY + y, sourceZ + z, replaceType);
      }
    }
  }

  for (int_fast64_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast64_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = LAKE_MAX_HEIGHT / 2; y < LAKE_MAX_HEIGHT; ++y) {
        if (!IsLakeBlock(lake_map, x, y, z)) {
          continue;
        }

        MinecraftBlock block = world.getBlockAt(sourceX + x, sourceY + y - 1, sourceZ + z);
        MinecraftBlock block_above = world.getBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        if (block.getId() == 3 && IS_TRANSPARENT(block_above.getId()) && GET_LIGHT_LEVEL(block_above.getId()) > 0) {
          transaction_.AddBlockAt(sourceX + x, sourceY + y - 1, sourceZ + z, mycel_biome ? MYCELIUM : GRASS);
        }
      }
    }
  }

  return succeeded;
}

bool Lake::IsLakeBlock(std::vector<int_fast64_t> &lake_map, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  return std::find(lake_map.begin(), lake_map.end(), (x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y)
      != lake_map.end();
}

void Lake::SetLakeBlock(std::vector<int_fast64_t> &lake_map, int_fast64_t x, int_fast32_t y, int_fast64_t z) {
  lake_map.emplace_back((x * LAKE_MAX_DIAMETER + z) * LAKE_MAX_HEIGHT + y);
}

bool Lake::CanPlace(std::vector<int_fast64_t> &lake_map,
                    ChunkManager world,
                    int_fast64_t sourceX,
                    int_fast32_t sourceY,
                    int_fast64_t sourceZ) {

  for (int_fast64_t x = 0; x < LAKE_MAX_DIAMETER; ++x) {
    for (int_fast64_t z = 0; z < LAKE_MAX_DIAMETER; ++z) {
      for (int_fast32_t y = 0; y < LAKE_MAX_HEIGHT; ++y) {

        if (IsLakeBlock(lake_map, x, y, z)
            || (((x >= (LAKE_MAX_DIAMETER - 1)) || !IsLakeBlock(lake_map, x + 1, y, z))
                && ((x <= 0) || !IsLakeBlock(lake_map, x - 1, y, z))
                && ((z >= (LAKE_MAX_DIAMETER - 1)) || !IsLakeBlock(lake_map, x, y, z + 1))
                && ((z <= 0) || !IsLakeBlock(lake_map, x, y, z - 1))
                && ((z >= (LAKE_MAX_HEIGHT - 1)) || !IsLakeBlock(lake_map, x, y + 1, z))
                && ((z <= 0) || !IsLakeBlock(lake_map, x, y - 1, z)))) {
          continue;
        }

        MinecraftBlock block = world.getBlockAt(sourceX + x, sourceY + y, sourceZ + z);
        if (y >= LAKE_MAX_HEIGHT / 2 && (IS_LIQUID(block.getId()) || block.getId() == 79)) {
          return false; // there's already some liquids above
        }

        if ((y < LAKE_MAX_HEIGHT / 2) && !(IS_SOLID(block.getId())) && block.getId() != this->type_.getId()) {
          return false; // bottom must be solid and do not overlap with another liquid type
        }
      }
    }
  }

  return true;
}