#include <lib/pocketmine/BlockList.h>
#include <lib/generator/objects/OreVien.h>
#include "OrePopulator.h"

OrePopulator::OrePopulator() {
  addOre(OreType{DIRT, 0, 256, 32, 1, 10});
  addOre(OreType{DIRT, 0, 256, 32, 1, 10});
  addOre(OreType{GRAVEL, 0, 256, 32, 1, 8});
  addOre(OreType{GRANITE, 0, 80, 32, 1, 10});
  addOre(OreType{DIORITE, 0, 80, 32, 1, 10});
  addOre(OreType{ANDESITE, 0, 80, 32, 1, 10});
  addOre(OreType{COAL_ORE, 0, 128, 16, 1, 20});
  addOre(OreType{IRON_ORE, 0, 64, 8, 1, 20});
  addOre(OreType{GOLD_ORE, 0, 32, 8, 1, 2});
  addOre(OreType{REDSTONE_ORE, 0, 16, 7, 1, 8});
  addOre(OreType{DIAMOND_ORE, 0, 16, 7, 1, 1});
  addOre(OreType{LAPIS_LAZULI_ORE, 16, 16, 6, 1, 1});
}

void OrePopulator::populate(SimpleChunkManager &world, Random &random, int chunkX, int chunkZ) {
  int cx, cz, source_x, source_y, source_z;
  cx = chunkX << 4;
  cz = chunkZ << 4;

  for (OreType oreType : ores) {
    for (int n = 0; n < oreType.total; ++n) {
      source_x = cx + random.nextBoundedInt(16);
      source_z = cz + random.nextBoundedInt(16);
      source_y = oreType.getRandomHeight(random);

      OreVein(&oreType).generate(world, random, source_x, source_y, source_z);
    }
  }
}

void OrePopulator::addOre(const OreType &ore) {
  ores.emplace_back(ore);
}

int OreType::getRandomHeight(Random &random) const {
  return static_cast<int32_t>(min_y == max_y ? random.nextBoundedInt(min_y) + random.nextBoundedInt(min_y) :
                              random.nextBoundedInt(max_y - min_y) + min_y);
}