#pragma once
#ifndef EXT_VANILLAGENERATOR_LIB_OBJECTS_BLOCKS_BLOCK_INL_
#define EXT_VANILLAGENERATOR_LIB_OBJECTS_BLOCKS_BLOCK_INL_

#include <PhpPalettedBlockArrayObj.h>
#include <map>

namespace blocks {

const unsigned short INTERNAL_STATE_DATA_BITS = 11;
const unsigned short INTERNAL_STATE_DATA_MASK = ~(~0 << INTERNAL_STATE_DATA_BITS);

class MCBlock {
 protected:
  MCBlock(Block fullBlockStateId, int blockMetaId, int internalStateData);
 public:
  int GetTypeId() const;
  int GetStateId() const;
  int GetBlockMeta() const;

  bool operator==(const MCBlock *rhs) const;
  bool operator!=(const MCBlock *rhs) const;

  bool IsSolid() const;
  bool IsTransparent() const;
  bool IsFlowable() const;
  bool IsLiquid() const;

  int GetLightLevel() const;

  static const MCBlock *GetBlockIdAndMeta(unsigned int blockStateId, unsigned int meta);
  static const MCBlock *GetBlockFromStateId(unsigned int internalBlockStateId);

  static int WriteVineBlockFacingMeta(int facing);

  static void RegisterBlock(int id, int meta, int internalStateData);
 private:
  bool isSolid = false;
  bool isTransparent = false;
  bool isFlowable = false;
  bool isLiquid = false;
  int blockLightLevel = 0;

  int blockStateId;
  int blockMeta;
};

// As for PM5, all block ids are now known as block state ids. The legacy block id conversion
// is no longer possible and these block ids are the known internal block ids *and* is used
// by the generator.
class BlockIds {
 public:
  static const int AIR = 10000;
  static const int ACACIA_LEAVES = 10005;
  static const int ACACIA_LOG = 10006;
  static const int ACACIA_SAPLING = 10009;
  static const int ALLIUM = 10018;
  static const int ANDESITE = 10019;
  static const int AZURE_BLUET = 10024;
  static const int BEDROCK = 10032;
  static const int BIRCH_LEAVES = 10039;
  static const int BIRCH_LOG = 10040;
  static const int BLUE_ORCHID = 10054;
  static const int BROWN_MUSHROOM = 10064;
  static const int BROWN_MUSHROOM_BLOCK = 10065;
  static const int CACTUS = 10066;
  static const int CLAY = 10077;
  static const int COAL_ORE = 10079;
  static const int DANDELION = 10100;
  static const int DARK_OAK_LEAVES = 10105;
  static const int DARK_OAK_LOG = 10106;
  static const int DEAD_BUSH = 10120;
  static const int DIAMOND_ORE = 10123;
  static const int DIORITE = 10124;
  static const int DIRT = 10128;
  static const int DOUBLE_TALLGRASS = 10129;
  static const int FARMLAND = 10265;
  static const int FERN = 10266;
  static const int FIRE = 10267;
  static const int GLOWSTONE = 10275;
  static const int GOLD_ORE = 10277;
  static const int GRANITE = 10278;
  static const int GRASS = 10282;
  static const int GRAVEL = 10284;
  static const int HARDENED_CLAY = 10288;
  static const int ICE = 10293;
  static const int IRON_ORE = 10306;
  static const int JUNGLE_LEAVES = 10314;
  static const int JUNGLE_LOG = 10315;
  static const int LAPIS_LAZULI_ORE = 10329;
  static const int LARGE_FERN = 10330;
  static const int LAVA = 10331;
  static const int LILAC = 10337;
  static const int LILY_PAD = 10339;
  static const int MAGMA = 10344;
  static const int MELON = 10346;
  static const int MUSHROOM_STEM = 10358;
  static const int MYCELIUM = 10359;
  static const int NETHER_QUARTZ_ORE = 10366;
  static const int NETHERRACK = 10370;
  static const int OAK_LEAVES = 10376;
  static const int OAK_LOG = 10377;
  static const int ORANGE_TULIP = 10389;
  static const int OXEYE_DAISY = 10390;
  static const int PACKED_ICE = 10391;
  static const int PEONY = 10392;
  static const int PINK_TULIP = 10394;
  static const int PODZOL = 10395;
  static const int POPPY = 10405;
  static const int PUMPKIN = 10415;
  static const int RED_MUSHROOM = 10429;
  static const int RED_MUSHROOM_BLOCK = 10430;
  static const int RED_SAND = 10435;
  static const int RED_SANDSTONE = 10436;
  static const int RED_TULIP = 10441;
  static const int REDSTONE_ORE = 10445;
  static const int ROSE_BUSH = 10450;
  static const int SAND = 10451;
  static const int SANDSTONE = 10452;
  static const int SNOW = 10472;
  static const int SNOW_LAYER = 10473;
  static const int SOUL_SAND = 10474;
  static const int SPRUCE_LEAVES = 10480;
  static const int SPRUCE_LOG = 10481;
  static const int STAINED_CLAY = 10491;
  static const int STONE = 10496;
  static const int SUGARCANE = 10518;
  static const int SUNFLOWER = 10519;
  static const int TALL_GRASS = 10521;
  static const int VINES = 10528;
  static const int WATER = 10531;
  static const int WHITE_TULIP = 10536;

  inline static int FIRST_UNUSED_BLOCK_ID = 10706;
};

}

#endif // EXT_VANILLAGENERATOR_LIB_OBJECTS_BLOCKS_BLOCK_INL_
