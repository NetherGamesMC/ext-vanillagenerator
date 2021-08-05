#include "GroundGenerator.h"

#include <lib/biomes/BiomeClimate.h>

void GroundGenerator::GenerateTerrainColumn(ChunkManager &world,
                                            Random &random,
                                            int_fast64_t x,
                                            int_fast64_t z,
                                            int biome,
                                            double surface_noise) {

  int sea_level = 64;

  auto top_mat = top_material.GetFullId();
  auto ground_mat = ground_material.GetFullId();
  int ground_mat_id = ground_material.GetId();

  int_fast64_t chunk_x = x;
  int_fast64_t chunk_z = z;

  int surface_height = FuncMax((int) (surface_noise / 3.0 + 3.0 + random.NextFloat() * 0.25), 1);
  int deep = -1;

  int air = AIR.GetFullId();
  int stone = STONE.GetFullId();
  int sandstone = SANDSTONE.GetFullId();
  int gravel = GRAVEL.GetFullId();
  int bedrock = BEDROCK.GetFullId();
  int ice = ICE.GetFullId();

  Chunk *chunk = world.GetChunk(x >> 4, z >> 4);
  auto block_x = static_cast<int_fast8_t>(x & 0x0f);
  auto block_z = static_cast<int_fast8_t>(z & 0x0f);

  for (int_fast16_t y = 255; y >= 0; --y) {
    if (y <= random.NextInt(5)) {
      chunk->SetFullBlock(block_x, y, block_z, bedrock);
    } else {
      int mat_id = MinecraftBlock(chunk->GetFullBlock(block_x, y, block_z)).GetId();
      if (mat_id == AIR.GetId()) {
        deep = -1;
      } else if (mat_id == STONE.GetId()) {
        if (deep == -1) {
          if (y >= sea_level - 5 && y <= sea_level) {
            top_mat = top_material.GetFullId();
            ground_mat = ground_material.GetFullId();
            ground_mat_id = ground_material.GetId();
          }

          deep = surface_height;
          if (y >= sea_level - 2) {
            chunk->SetFullBlock(block_x, y, block_z, top_mat);
          } else if (y < sea_level - 8 - surface_height) {
            top_mat = air;
            ground_mat = stone;
            ground_mat_id = STONE.GetId();
            chunk->SetFullBlock(block_x, y, block_z, gravel);
          } else {
            chunk->SetFullBlock(block_x, y, block_z, ground_mat);
          }
        } else if (deep > 0) {
          --deep;
          chunk->SetFullBlock(block_x, y, block_z, ground_mat);

          if (deep == 0 && ground_mat_id == SAND.GetId()) {
            deep = static_cast<int>(random.NextInt(4) + FuncMax(0, static_cast<int>(y - sea_level - 1)));
            ground_mat = sandstone;
            ground_mat_id = SANDSTONE.GetId();
          }
        }
      } else if (mat_id == STILL_WATER.GetId() && y == sea_level - 2 && Biome::IsCold(biome, chunk_x, y, chunk_z)) {
        chunk->SetFullBlock(block_x, y, block_z, ice);
      }
    }
  }
}