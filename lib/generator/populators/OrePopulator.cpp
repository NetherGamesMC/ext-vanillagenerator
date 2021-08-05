#include <lib/objects/constants/BlockList.h>
#include <lib/generator/objects/OreVein.h>

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

void OrePopulator::Populate(ChunkManager &world, Random &random, int_fast64_t chunkX, int_fast64_t chunkZ) {
  int_fast64_t cx, cz, source_x, source_z;
  int_fast32_t source_y;

  cx = chunkX << 4;
  cz = chunkZ << 4;

  for (OreType oreType : ores) {
    for (int n = 0; n < oreType.total; ++n) {
      source_x = cx + random.nextInt(16);
      source_z = cz + random.nextInt(16);
      source_y = oreType.getRandomHeight(random);

      OreVein(&oreType).Generate(world, random, source_x, source_y, source_z);
    }
  }
}

void OrePopulator::addOre(const OreType &ore) {
  ores.emplace_back(ore);
}

int OreType::getRandomHeight(Random &random) const {
  return static_cast<int>(min_y == max_y ?
                          random.nextInt(min_y) + random.nextInt(min_y) :
                          random.nextInt(max_y - min_y) + min_y);
}