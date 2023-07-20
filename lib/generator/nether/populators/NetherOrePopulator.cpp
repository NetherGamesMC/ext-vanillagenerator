#include "NetherOrePopulator.h"

NetherOrePopulator::NetherOrePopulator(int_fast32_t worldHeight) {
  using namespace blocks;

  auto netherrackBlock = MCBlock::GetBlockFromStateId(BlockIds::NETHERRACK);
  auto netherQuartzOre = MCBlock::GetBlockFromStateId(BlockIds::NETHER_QUARTZ_ORE);
  auto magmaBlock = MCBlock::GetBlockFromStateId(BlockIds::MAGMA);

  AddOre(OreType{netherQuartzOre, 10, worldHeight - (10 * (worldHeight >> 7)), 13, 0, 16});
  AddOre(OreType{magmaBlock, 26, 32 + (5 * (worldHeight >> 7)), 32, 0, 16});
}