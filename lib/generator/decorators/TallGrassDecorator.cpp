#include <lib/pocketmine/BlockList.h>
#include <lib/generator/objects/TallGrass.h>
#include "TallGrassDecorator.h"

void TallGrassDecorator::setFernDensity(double fernIntensity) {
  fernIntensity_ = fernIntensity;
}

void TallGrassDecorator::decorate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  int x, y, z;

  x = random.nextBoundedInt(16);
  z = random.nextBoundedInt(16);
  int_fast16_t top_block = world.getChunk(chunkX, chunkZ)->getHighestBlockAt(x, z);
  if (top_block <= 0) return; // Nothing to do if this column is empty

  y = random.nextBoundedInt(abs(top_block << 1));

  // the grass species can change on each decoration pass
  MinecraftBlock species = TALL_GRASS;
  if (fernIntensity_ > 0 && random.nextFloat() < fernIntensity_) {
    species = FERN;
  }

  TallGrass(species).generate(world, random, (chunkX << 4) + x, y, (chunkZ << 4) + z);
}
