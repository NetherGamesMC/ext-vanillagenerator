#include <lib/generator/object/TallGrass.h>
#include "TallGrassDecorator.h"

using namespace blocks;

void TallGrassDecorator::SetFernDensity(double density) {
  fernDensity_ = density;
}

void TallGrassDecorator::Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t x, z;
  int_fast32_t y;

  x = random.NextInt(16);
  z = random.NextInt(16);
  int_fast32_t top_block = world.GetChunk(chunkX, chunkZ)->GetHighestBlockAt(x, z);
  if (top_block <= 0) return; // Nothing to do if this column is empty

  y = static_cast<int_fast32_t>(random.NextInt(abs(top_block << 1)));

  // the grass species can change on each decoration pass
  const MCBlock *species = MCBlock::GetBlockFromStateId(BlockIds::TALL_GRASS);
  if (fernDensity_ > 0 && random.NextFloat() < fernDensity_) {
    species = MCBlock::GetBlockFromStateId(BlockIds::FERN);
  }

  TallGrass(species).Generate(world, random, (chunkX << 4) + x, y, (chunkZ << 4) + z);
}
