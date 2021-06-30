#include <lib/pocketmine/BlockList.h>
#include <lib/generator/objects/TallGrass.h>
#include "TallGrassDecorator.h"

void TallGrassDecorator::SetFernDensity(double density) {
  fern_density_ = density;
}

void TallGrassDecorator::Decorate(SimpleChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t x, z;
  int_fast32_t y;

  x = random.nextInt(16);
  z = random.nextInt(16);
  int_fast32_t top_block = world.getChunk(chunkX, chunkZ)->getHighestBlockAt(x, z);
  if (top_block <= 0) return; // Nothing to do if this column is empty

  y = static_cast<int_fast32_t>(random.nextInt(abs(top_block << 1)));

  // the grass species can change on each decoration pass
  MinecraftBlock species = TALL_GRASS;
  if (fern_density_ > 0 && random.nextFloat() < fern_density_) {
    species = FERN;
  }

  TallGrass(species).Generate(world, random, (chunkX << 4) + x, y, (chunkZ << 4) + z);
}
