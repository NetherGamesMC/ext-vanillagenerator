#include "GroundGenerator.h"

#include <lib/biomes/BiomeClimate.h>
#include <lib/objects/math/Math.h>

void GroundGenerator::GenerateTerrainColumn(ChunkManager &world, Random &random, int_fast32_t x, int_fast32_t z, int biome, double surfaceNoise) {
  int seaLevel = 64;

  auto topMat = topMaterial->GetStateId();
  auto groundMat = groundMaterial->GetStateId();
  int groundMatId = groundMaterial->GetTypeId();

  int_fast32_t chunkX = x;
  int_fast32_t chunkZ = z;

  int surfaceHeight = Math::Max((int) (surfaceNoise / 3.0 + 3.0 + random.NextFloat() * 0.25), 1);
  int deep = -1;

  auto airBlock = MCBlock::GetBlockFromStateId(BlockIds::AIR);
  auto stoneBlock = MCBlock::GetBlockFromStateId(BlockIds::STONE);
  auto sandstoneBlock = MCBlock::GetBlockFromStateId(BlockIds::SANDSTONE);
  auto gravelBlock = MCBlock::GetBlockFromStateId(BlockIds::GRAVEL);
  auto bedrockBlock = MCBlock::GetBlockFromStateId(BlockIds::BEDROCK);
  auto iceBlock = MCBlock::GetBlockFromStateId(BlockIds::ICE);
  auto sandBlock = MCBlock::GetBlockFromStateId(BlockIds::SAND);
  auto stillWaterBlock = MCBlock::GetBlockIdAndMeta(BlockIds::WATER, 16);

  int air = airBlock->GetStateId();
  int stone = stoneBlock->GetStateId();
  int sandstone = sandstoneBlock->GetStateId();
  int gravel = gravelBlock->GetStateId();
  int bedrock = bedrockBlock->GetStateId();
  int ice = iceBlock->GetStateId();

  Chunk *chunk = world.GetChunk(x >> 4, z >> 4);
  auto blockX = static_cast<int_fast8_t>(x & 0x0f);
  auto blockZ = static_cast<int_fast8_t>(z & 0x0f);

  for (int_fast16_t y = 255; y >= 0; --y) {
    if (y <= random.NextInt(5)) {
      chunk->SetFullBlock(blockX, y, blockZ, bedrock);
    } else {
      const MCBlock *mat_id = MCBlock::GetBlockFromStateId(chunk->GetFullBlock(blockX, y, blockZ));
      if (mat_id == airBlock) {
        deep = -1;
      } else if (mat_id == stoneBlock) {
        if (deep == -1) {
          if (y >= seaLevel - 5 && y <= seaLevel) {
            topMat = topMaterial->GetStateId();
            groundMat = groundMaterial->GetStateId();
            groundMatId = groundMaterial->GetStateId();
          }

          deep = surfaceHeight;
          if (y >= seaLevel - 2) {
            chunk->SetFullBlock(blockX, y, blockZ, topMat);
          } else if (y < seaLevel - 8 - surfaceHeight) {
            topMat = air;
            groundMat = stone;
            groundMatId = stoneBlock->GetStateId();
            chunk->SetFullBlock(blockX, y, blockZ, gravel);
          } else {
            chunk->SetFullBlock(blockX, y, blockZ, groundMat);
          }
        } else if (deep > 0) {
          --deep;
          chunk->SetFullBlock(blockX, y, blockZ, groundMat);

          if (deep == 0 && groundMatId == sandBlock->GetStateId()) {
            deep = static_cast<int>(random.NextInt(4) + Math::Max(0, static_cast<int>(y - seaLevel - 1)));
            groundMat = sandstone;
            groundMatId = sandstoneBlock->GetStateId();
          }
        }
      } else if (mat_id == stillWaterBlock && y == seaLevel - 2 && BiomeClimate::IsCold(biome, chunkX, y, chunkZ)) {
        chunk->SetFullBlock(blockX, y, blockZ, ice);
      }
    }
  }
}