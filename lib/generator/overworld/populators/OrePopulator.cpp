#include <lib/objects/constants/BlockList.h>
#include <lib/generator/object/OreVein.h>

OrePopulator::OrePopulator() {
  AddOre(OreType{DIRT, 0, 256, 32, 10});
  AddOre(OreType{GRAVEL, 0, 256, 32, 8});
  AddOre(OreType{GRANITE, 0, 80, 32, 10});
  AddOre(OreType{DIORITE, 0, 80, 32, 10});
  AddOre(OreType{ANDESITE, 0, 80, 32, 10});
  AddOre(OreType{COAL_ORE, 0, 128, 16, 20});
  AddOre(OreType{IRON_ORE, 0, 64, 8, 20});
  AddOre(OreType{GOLD_ORE, 0, 32, 8, 2});
  AddOre(OreType{REDSTONE_ORE, 0, 16, 7, 8});
  AddOre(OreType{DIAMOND_ORE, 0, 16, 7, 1});
  AddOre(OreType{LAPIS_LAZULI_ORE, 16, 16, 6, 1});
}

void OrePopulator::Populate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) {
  int_fast32_t cx, cz, sourceX, sourceZ;
  int_fast32_t sourceY;

  cx = chunkX << 4;
  cz = chunkZ << 4;

  for (OreType oreType : ores) {
    for (int_fast32_t n = 0; n < oreType.total; ++n) {
      sourceX = cx + random.NextInt(16);
      sourceZ = cz + random.NextInt(16);
      sourceY = oreType.GetRandomHeight(random);

      OreVein(oreType).Generate(world, random, sourceX, sourceY, sourceZ);
    }
  }
}

void OrePopulator::AddOre(OreType ore) {
  ores.emplace_back(ore);
}

int_fast32_t OreType::GetRandomHeight(Random &random) const {
  return static_cast<int>(minY == maxY ? random.NextInt(minY) + random.NextInt(minY) : random.NextInt(maxY - minY) + minY);
}