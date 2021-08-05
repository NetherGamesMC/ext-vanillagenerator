#include "GroundGenerator.h"

#include <lib/biomes/BiomeClimate.h>

void GroundGenerator::GenerateTerrainColumn(ChunkManager &world,
                                            Random &random,
                                            int_fast64_t x,
                                            int_fast64_t z,
                                            int biome,
                                            double surface_noise) {

  int sea_level = 64;

  auto top_mat = top_material.getFullId();
  auto ground_mat = ground_material.getFullId();
  int ground_mat_id = ground_material.getId();

  int_fast64_t chunk_x = x;
  int_fast64_t chunk_z = z;

  int surface_height = FuncMax((int) (surface_noise / 3.0 + 3.0 + random.nextFloat() * 0.25), 1);
  int deep = -1;

  int air = AIR.getFullId();
  int stone = STONE.getFullId();
  int sandstone = SANDSTONE.getFullId();
  int gravel = GRAVEL.getFullId();
  int bedrock = BEDROCK.getFullId();
  int ice = ICE.getFullId();

  Chunk *chunk = world.getChunk(x >> 4, z >> 4);
  auto block_x = static_cast<int_fast8_t>(x & 0x0f);
  auto block_z = static_cast<int_fast8_t>(z & 0x0f);

  for (int_fast16_t y = 255; y >= 0; --y) {
    if (y <= random.nextInt(5)) {
      chunk->setFullBlock(block_x, y, block_z, bedrock);
    } else {
      int mat_id = MinecraftBlock(chunk->getFullBlock(block_x, y, block_z)).getId();
      if (mat_id == AIR.getId()) {
        deep = -1;
      } else if (mat_id == STONE.getId()) {
        if (deep == -1) {
          if (y >= sea_level - 5 && y <= sea_level) {
            top_mat = top_material.getFullId();
            ground_mat = ground_material.getFullId();
            ground_mat_id = ground_material.getId();
          }

          deep = surface_height;
          if (y >= sea_level - 2) {
            chunk->setFullBlock(block_x, y, block_z, top_mat);
          } else if (y < sea_level - 8 - surface_height) {
            top_mat = air;
            ground_mat = stone;
            ground_mat_id = STONE.getId();
            chunk->setFullBlock(block_x, y, block_z, gravel);
          } else {
            chunk->setFullBlock(block_x, y, block_z, ground_mat);
          }
        } else if (deep > 0) {
          --deep;
          chunk->setFullBlock(block_x, y, block_z, ground_mat);

          if (deep == 0 && ground_mat_id == SAND.getId()) {
            deep = static_cast<int>(random.nextInt(4) + FuncMax(0, static_cast<int>(y - sea_level - 1)));
            ground_mat = sandstone;
            ground_mat_id = SANDSTONE.getId();
          }
        }
      } else if (mat_id == STILL_WATER.getId() && y == sea_level - 2 && Biome::isCold(biome, chunk_x, y, chunk_z)) {
        chunk->setFullBlock(block_x, y, block_z, ice);
      }
    }
  }
}