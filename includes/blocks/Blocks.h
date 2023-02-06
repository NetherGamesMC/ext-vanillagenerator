#ifndef VANILLAGENERATOR_SRC_BLOCKS_BLOCKS_H_
#define VANILLAGENERATOR_SRC_BLOCKS_BLOCKS_H_

namespace block {

const unsigned short INTERNAL_METADATA_BITS = 4;
const unsigned short INTERNAL_METADATA_MASK = ~(~0 << INTERNAL_METADATA_BITS);

/**
 * Minecraft block representation, the underlying block array needs to be initialized_ before it is being used
 * with the function:
 *
 * @code Block::RegisterAllBlocks() @endcode
 *
 * The function will register all possible blocks_ into memory to allow faster block allocation and references.
 */
class Block {
 protected:
  // Keep it protected, we need to keep the block class encapsulated from another use-case.
  Block(unsigned short block, unsigned char block_meta);
 public:
  /**
   * Returns minecraft full block id for the given block.
   */
  [[nodiscard]] auto GetFullId() const -> unsigned int;

  /**
   * Return the id of the block.
   */
  [[nodiscard]] auto GetId() const -> unsigned short;

  /**
   * Return the meta_ of the block
   */
  [[nodiscard]] auto GetMeta() const -> unsigned char;

  /**
   * Return true if the block is an air, false if otherwise.
   */
  [[nodiscard]] auto IsObjectNull() const -> bool;

  /**
   * Comparison operator with another Block to the RHS.
   */
  bool operator==(const Block &rhs) const;

  /**
   * @brief Comparison operator with another Block to the RHS.
   */
  bool operator!=(const Block &rhs) const;

  /**
   * @brief Register all id-meta_ based blocks_ into memory.
   */
  static void RegisterAllBlocks();

  /**
   * @brief Return an object for the id and meta_, it will return null if the
   * existing index for the id and meta_ does not exists in blocks_ array.
   */
  static const Block *GetBlockIdAndMeta(unsigned short id, unsigned char meta);
  static const Block *GetBlockFromFullId(unsigned int fullBlockId);

  static unsigned char WriteLegacyHorizontalFacing(int facing);
  static unsigned char WriteVineBlockFacingMeta(int facing);
 private:
  inline static bool initialized_ = false;
  inline static Block *blocks_[1024 << INTERNAL_METADATA_BITS];

  unsigned short block_id_;
  unsigned char meta_;

  /**
   * Register a block into the blocks_ array.
   */
  static void RegisterBlock(int id, int meta);
};

}

// All the possible block ids in minecraft bedrock edition.
namespace BlockIds {

const int AIR = 0;
const int STONE = 1;
const int GRASS = 2;
const int DIRT = 3;
const int COBBLESTONE = 4;
const int PLANKS = 5;
const int SAPLING = 6;
const int BEDROCK = 7;
const int FLOWING_WATER = 8;
const int STILL_WATER = 9, WATER = 9;
const int FLOWING_LAVA = 10;
const int LAVA = 11, STILL_LAVA = 11;
const int SAND = 12;
const int GRAVEL = 13;
const int GOLD_ORE = 14;
const int IRON_ORE = 15;
const int COAL_ORE = 16;
const int LOG = 17;
const int LEAVES = 18;
const int SPONGE = 19;
const int GLASS = 20;
const int LAPIS_ORE = 21;
const int LAPIS_BLOCK = 22;
const int DISPENSER = 23;
const int SANDSTONE = 24;
const int NOTEBLOCK = 25, NOTE_BLOCK = 25;
const int BED_BLOCK = 26;
const int GOLDEN_RAIL = 27, POWERED_RAIL = 27;
const int DETECTOR_RAIL = 28;
const int STICKY_PISTON = 29;
const int COBWEB = 30, WEB = 30;
const int TALLGRASS = 31, TALL_GRASS = 31;
const int DEADBUSH = 32, DEAD_BUSH = 32;
const int PISTON = 33;
const int PISTONARMCOLLISION = 34, PISTON_ARM_COLLISION = 34;
const int WOOL = 35;
const int ELEMENT_0 = 36;
const int DANDELION = 37, YELLOW_FLOWER = 37;
const int POPPY = 38, RED_FLOWER = 38;
const int BROWN_MUSHROOM = 39;
const int RED_MUSHROOM = 40;
const int GOLD_BLOCK = 41;
const int IRON_BLOCK = 42;
const int DOUBLE_STONE_SLAB = 43;
const int STONE_SLAB = 44;
const int BRICK_BLOCK = 45;
const int TNT = 46;
const int BOOKSHELF = 47;
const int MOSSY_COBBLESTONE = 48, MOSS_STONE = 48;
const int OBSIDIAN = 49;
const int TORCH = 50;
const int FIRE = 51;
const int MOB_SPAWNER = 52, MONSTER_SPAWNER = 52;
const int OAK_STAIRS = 53, WOODEN_STAIRS = 53;
const int CHEST = 54;
const int REDSTONE_WIRE = 55;
const int DIAMOND_ORE = 56;
const int DIAMOND_BLOCK = 57;
const int CRAFTING_TABLE = 58, WORKBENCH = 58;
const int WHEAT_BLOCK = 59;
const int FARMLAND = 60;
const int FURNACE = 61;
const int BURNING_FURNACE = 62, LIT_FURNACE = 62;
const int SIGN_POST = 63, STANDING_SIGN = 63;
const int OAK_DOOR_BLOCK = 64, WOODEN_DOOR_BLOCK = 64;
const int LADDER = 65;
const int RAIL = 66;
const int COBBLESTONE_STAIRS = 67, STONE_STAIRS = 67;
const int WALL_SIGN = 68;
const int LEVER = 69;
const int STONE_PRESSURE_PLATE = 70;
const int IRON_DOOR_BLOCK = 71;
const int WOODEN_PRESSURE_PLATE = 72;
const int REDSTONE_ORE = 73;
const int GLOWING_REDSTONE_ORE = 74, LIT_REDSTONE_ORE = 74;
const int UNLIT_REDSTONE_TORCH = 75;
const int LIT_REDSTONE_TORCH = 76, REDSTONE_TORCH = 76;
const int STONE_BUTTON = 77;
const int SNOW_LAYER = 78;
const int ICE = 79;
const int SNOW = 80, SNOW_BLOCK = 80;
const int CACTUS = 81;
const int CLAY_BLOCK = 82;
const int REEDS_BLOCK = 83, SUGARCANE_BLOCK = 83;
const int JUKEBOX = 84;
const int FENCE = 85;
const int PUMPKIN = 86;
const int NETHERRACK = 87;
const int SOUL_SAND = 88;
const int GLOWSTONE = 89;
const int PORTAL = 90;
const int JACK_O_LANTERN = 91, LIT_PUMPKIN = 91;
const int CAKE_BLOCK = 92;
const int REPEATER_BLOCK = 93, UNPOWERED_REPEATER = 93;
const int POWERED_REPEATER = 94;
const int INVISIBLEBEDROCK = 95, INVISIBLE_BEDROCK = 95;
const int TRAPDOOR = 96, WOODEN_TRAPDOOR = 96;
const int MONSTER_EGG = 97;
const int STONEBRICK = 98, STONE_BRICK = 98, STONE_BRICKS = 98;
const int BROWN_MUSHROOM_BLOCK = 99;
const int RED_MUSHROOM_BLOCK = 100;
const int IRON_BARS = 101;
const int GLASS_PANE = 102;
const int MELON_BLOCK = 103;
const int PUMPKIN_STEM = 104;
const int MELON_STEM = 105;
const int VINE = 106, VINES = 106;
const int FENCE_GATE = 107, OAK_FENCE_GATE = 107;
const int BRICK_STAIRS = 108;
const int STONE_BRICK_STAIRS = 109;
const int MYCELIUM = 110;
const int LILY_PAD = 111, WATERLILY = 111, WATER_LILY = 111;
const int NETHER_BRICK_BLOCK = 112;
const int NETHER_BRICK_FENCE = 113;
const int NETHER_BRICK_STAIRS = 114;
const int NETHER_WART_PLANT = 115;
const int ENCHANTING_TABLE = 116, ENCHANTMENT_TABLE = 116;
const int BREWING_STAND_BLOCK = 117;
const int CAULDRON_BLOCK = 118;
const int END_PORTAL = 119;
const int END_PORTAL_FRAME = 120;
const int END_STONE = 121;
const int DRAGON_EGG = 122;
const int REDSTONE_LAMP = 123;
const int LIT_REDSTONE_LAMP = 124;
const int DROPPER = 125;
const int ACTIVATOR_RAIL = 126;
const int COCOA = 127, COCOA_BLOCK = 127;
const int SANDSTONE_STAIRS = 128;
const int EMERALD_ORE = 129;
const int ENDER_CHEST = 130;
const int TRIPWIRE_HOOK = 131;
const int TRIPWIRE = 132, TRIP_WIRE = 132;
const int EMERALD_BLOCK = 133;
const int SPRUCE_STAIRS = 134;
const int BIRCH_STAIRS = 135;
const int JUNGLE_STAIRS = 136;
const int COMMAND_BLOCK = 137;
const int BEACON = 138;
const int COBBLESTONE_WALL = 139, STONE_WALL = 139;
const int FLOWER_POT_BLOCK = 140;
const int CARROTS = 141, CARROT_BLOCK = 141;
const int POTATOES = 142, POTATO_BLOCK = 142;
const int WOODEN_BUTTON = 143;
const int MOB_HEAD_BLOCK = 144, SKULL_BLOCK = 144;
const int ANVIL = 145;
const int TRAPPED_CHEST = 146;
const int LIGHT_WEIGHTED_PRESSURE_PLATE = 147;
const int HEAVY_WEIGHTED_PRESSURE_PLATE = 148;
const int COMPARATOR_BLOCK = 149, UNPOWERED_COMPARATOR = 149;
const int POWERED_COMPARATOR = 150;
const int DAYLIGHT_DETECTOR = 151, DAYLIGHT_SENSOR = 151;
const int REDSTONE_BLOCK = 152;
const int NETHER_QUARTZ_ORE = 153, QUARTZ_ORE = 153;
const int HOPPER_BLOCK = 154;
const int QUARTZ_BLOCK = 155;
const int QUARTZ_STAIRS = 156;
const int DOUBLE_WOODEN_SLAB = 157;
const int WOODEN_SLAB = 158;
const int STAINED_CLAY = 159, STAINED_HARDENED_CLAY = 159, TERRACOTTA = 159;
const int STAINED_GLASS_PANE = 160;
const int LEAVES2 = 161;
const int LOG2 = 162;
const int ACACIA_STAIRS = 163;
const int DARK_OAK_STAIRS = 164;
const int SLIME = 165, SLIME_BLOCK = 165;
const int GLOW_STICK = 166;
const int IRON_TRAPDOOR = 167;
const int PRISMARINE = 168;
const int SEALANTERN = 169, SEA_LANTERN = 169;
const int HAY_BALE = 170, HAY_BLOCK = 170;
const int CARPET = 171;
const int HARDENED_CLAY = 172;
const int COAL_BLOCK = 173;
const int PACKED_ICE = 174;
const int DOUBLE_PLANT = 175;
const int STANDING_BANNER = 176;
const int WALL_BANNER = 177;
const int DAYLIGHT_DETECTOR_INVERTED = 178, DAYLIGHT_SENSOR_INVERTED = 178;
const int RED_SANDSTONE = 179;
const int RED_SANDSTONE_STAIRS = 180;
const int DOUBLE_STONE_SLAB2 = 181;
const int STONE_SLAB2 = 182;
const int SPRUCE_FENCE_GATE = 183;
const int BIRCH_FENCE_GATE = 184;
const int JUNGLE_FENCE_GATE = 185;
const int DARK_OAK_FENCE_GATE = 186;
const int ACACIA_FENCE_GATE = 187;
const int REPEATING_COMMAND_BLOCK = 188;
const int CHAIN_COMMAND_BLOCK = 189;
const int HARD_GLASS_PANE = 190;
const int HARD_STAINED_GLASS_PANE = 191;
const int CHEMICAL_HEAT = 192;
const int SPRUCE_DOOR_BLOCK = 193;
const int BIRCH_DOOR_BLOCK = 194;
const int JUNGLE_DOOR_BLOCK = 195;
const int ACACIA_DOOR_BLOCK = 196;
const int DARK_OAK_DOOR_BLOCK = 197;
const int GRASS_PATH = 198;
const int FRAME_BLOCK = 199, ITEM_FRAME_BLOCK = 199;
const int CHORUS_FLOWER = 200;
const int PURPUR_BLOCK = 201;
const int COLORED_TORCH_RG = 202;
const int PURPUR_STAIRS = 203;
const int COLORED_TORCH_BP = 204;
const int UNDYED_SHULKER_BOX = 205;
const int END_BRICKS = 206;
const int FROSTED_ICE = 207;
const int END_ROD = 208;
const int END_GATEWAY = 209;

const int MAGMA = 213;
const int NETHER_WART_BLOCK = 214;
const int RED_NETHER_BRICK = 215;
const int BONE_BLOCK = 216;

const int SHULKER_BOX = 218;
const int PURPLE_GLAZED_TERRACOTTA = 219;
const int WHITE_GLAZED_TERRACOTTA = 220;
const int ORANGE_GLAZED_TERRACOTTA = 221;
const int MAGENTA_GLAZED_TERRACOTTA = 222;
const int LIGHT_BLUE_GLAZED_TERRACOTTA = 223;
const int YELLOW_GLAZED_TERRACOTTA = 224;
const int LIME_GLAZED_TERRACOTTA = 225;
const int PINK_GLAZED_TERRACOTTA = 226;
const int GRAY_GLAZED_TERRACOTTA = 227;
const int SILVER_GLAZED_TERRACOTTA = 228;
const int CYAN_GLAZED_TERRACOTTA = 229;

const int BLUE_GLAZED_TERRACOTTA = 231;
const int BROWN_GLAZED_TERRACOTTA = 232;
const int GREEN_GLAZED_TERRACOTTA = 233;
const int RED_GLAZED_TERRACOTTA = 234;
const int BLACK_GLAZED_TERRACOTTA = 235;
const int CONCRETE = 236;
const int CONCRETEPOWDER = 237, CONCRETE_POWDER = 237;
const int CHEMISTRY_TABLE = 238;
const int UNDERWATER_TORCH = 239;
const int CHORUS_PLANT = 240;
const int STAINED_GLASS = 241;

const int PODZOL = 243;
const int BEETROOT_BLOCK = 244;
const int STONECUTTER = 245;
const int GLOWINGOBSIDIAN = 246, GLOWING_OBSIDIAN = 246;
const int NETHERREACTOR = 247, NETHER_REACTOR = 247;
const int INFO_UPDATE = 248;
const int INFO_UPDATE2 = 249;
const int MOVINGBLOCK = 250, MOVING_BLOCK = 250;
const int OBSERVER = 251;
const int STRUCTURE_BLOCK = 252;
const int HARD_GLASS = 253;
const int HARD_STAINED_GLASS = 254;
const int RESERVED6 = 255;

const int PRISMARINE_STAIRS = 257;
const int DARK_PRISMARINE_STAIRS = 258;
const int PRISMARINE_BRICKS_STAIRS = 259;
const int STRIPPED_SPRUCE_LOG = 260;
const int STRIPPED_BIRCH_LOG = 261;
const int STRIPPED_JUNGLE_LOG = 262;
const int STRIPPED_ACACIA_LOG = 263;
const int STRIPPED_DARK_OAK_LOG = 264;
const int STRIPPED_OAK_LOG = 265;
const int BLUE_ICE = 266;
const int ELEMENT_1 = 267;
const int ELEMENT_2 = 268;
const int ELEMENT_3 = 269;
const int ELEMENT_4 = 270;
const int ELEMENT_5 = 271;
const int ELEMENT_6 = 272;
const int ELEMENT_7 = 273;
const int ELEMENT_8 = 274;
const int ELEMENT_9 = 275;
const int ELEMENT_10 = 276;
const int ELEMENT_11 = 277;
const int ELEMENT_12 = 278;
const int ELEMENT_13 = 279;
const int ELEMENT_14 = 280;
const int ELEMENT_15 = 281;
const int ELEMENT_16 = 282;
const int ELEMENT_17 = 283;
const int ELEMENT_18 = 284;
const int ELEMENT_19 = 285;
const int ELEMENT_20 = 286;
const int ELEMENT_21 = 287;
const int ELEMENT_22 = 288;
const int ELEMENT_23 = 289;
const int ELEMENT_24 = 290;
const int ELEMENT_25 = 291;
const int ELEMENT_26 = 292;
const int ELEMENT_27 = 293;
const int ELEMENT_28 = 294;
const int ELEMENT_29 = 295;
const int ELEMENT_30 = 296;
const int ELEMENT_31 = 297;
const int ELEMENT_32 = 298;
const int ELEMENT_33 = 299;
const int ELEMENT_34 = 300;
const int ELEMENT_35 = 301;
const int ELEMENT_36 = 302;
const int ELEMENT_37 = 303;
const int ELEMENT_38 = 304;
const int ELEMENT_39 = 305;
const int ELEMENT_40 = 306;
const int ELEMENT_41 = 307;
const int ELEMENT_42 = 308;
const int ELEMENT_43 = 309;
const int ELEMENT_44 = 310;
const int ELEMENT_45 = 311;
const int ELEMENT_46 = 312;
const int ELEMENT_47 = 313;
const int ELEMENT_48 = 314;
const int ELEMENT_49 = 315;
const int ELEMENT_50 = 316;
const int ELEMENT_51 = 317;
const int ELEMENT_52 = 318;
const int ELEMENT_53 = 319;
const int ELEMENT_54 = 320;
const int ELEMENT_55 = 321;
const int ELEMENT_56 = 322;
const int ELEMENT_57 = 323;
const int ELEMENT_58 = 324;
const int ELEMENT_59 = 325;
const int ELEMENT_60 = 326;
const int ELEMENT_61 = 327;
const int ELEMENT_62 = 328;
const int ELEMENT_63 = 329;
const int ELEMENT_64 = 330;
const int ELEMENT_65 = 331;
const int ELEMENT_66 = 332;
const int ELEMENT_67 = 333;
const int ELEMENT_68 = 334;
const int ELEMENT_69 = 335;
const int ELEMENT_70 = 336;
const int ELEMENT_71 = 337;
const int ELEMENT_72 = 338;
const int ELEMENT_73 = 339;
const int ELEMENT_74 = 340;
const int ELEMENT_75 = 341;
const int ELEMENT_76 = 342;
const int ELEMENT_77 = 343;
const int ELEMENT_78 = 344;
const int ELEMENT_79 = 345;
const int ELEMENT_80 = 346;
const int ELEMENT_81 = 347;
const int ELEMENT_82 = 348;
const int ELEMENT_83 = 349;
const int ELEMENT_84 = 350;
const int ELEMENT_85 = 351;
const int ELEMENT_86 = 352;
const int ELEMENT_87 = 353;
const int ELEMENT_88 = 354;
const int ELEMENT_89 = 355;
const int ELEMENT_90 = 356;
const int ELEMENT_91 = 357;
const int ELEMENT_92 = 358;
const int ELEMENT_93 = 359;
const int ELEMENT_94 = 360;
const int ELEMENT_95 = 361;
const int ELEMENT_96 = 362;
const int ELEMENT_97 = 363;
const int ELEMENT_98 = 364;
const int ELEMENT_99 = 365;
const int ELEMENT_100 = 366;
const int ELEMENT_101 = 367;
const int ELEMENT_102 = 368;
const int ELEMENT_103 = 369;
const int ELEMENT_104 = 370;
const int ELEMENT_105 = 371;
const int ELEMENT_106 = 372;
const int ELEMENT_107 = 373;
const int ELEMENT_108 = 374;
const int ELEMENT_109 = 375;
const int ELEMENT_110 = 376;
const int ELEMENT_111 = 377;
const int ELEMENT_112 = 378;
const int ELEMENT_113 = 379;
const int ELEMENT_114 = 380;
const int ELEMENT_115 = 381;
const int ELEMENT_116 = 382;
const int ELEMENT_117 = 383;
const int ELEMENT_118 = 384;
const int SEAGRASS = 385;
const int CORAL = 386;
const int CORAL_BLOCK = 387;
const int CORAL_FAN = 388;
const int CORAL_FAN_DEAD = 389;
const int CORAL_FAN_HANG = 390;
const int CORAL_FAN_HANG2 = 391;
const int CORAL_FAN_HANG3 = 392;
const int KELP = 393;
const int DRIED_KELP_BLOCK = 394;
const int ACACIA_BUTTON = 395;
const int BIRCH_BUTTON = 396;
const int DARK_OAK_BUTTON = 397;
const int JUNGLE_BUTTON = 398;
const int SPRUCE_BUTTON = 399;
const int ACACIA_TRAPDOOR = 400;
const int BIRCH_TRAPDOOR = 401;
const int DARK_OAK_TRAPDOOR = 402;
const int JUNGLE_TRAPDOOR = 403;
const int SPRUCE_TRAPDOOR = 404;
const int ACACIA_PRESSURE_PLATE = 405;
const int BIRCH_PRESSURE_PLATE = 406;
const int DARK_OAK_PRESSURE_PLATE = 407;
const int JUNGLE_PRESSURE_PLATE = 408;
const int SPRUCE_PRESSURE_PLATE = 409;
const int CARVED_PUMPKIN = 410;
const int SEA_PICKLE = 411;
const int CONDUIT = 412;

const int TURTLE_EGG = 414;
const int BUBBLE_COLUMN = 415;
const int BARRIER = 416;
const int STONE_SLAB3 = 417;
const int BAMBOO = 418;
const int BAMBOO_SAPLING = 419;
const int SCAFFOLDING = 420;
const int STONE_SLAB4 = 421;
const int DOUBLE_STONE_SLAB3 = 422;
const int DOUBLE_STONE_SLAB4 = 423;
const int GRANITE_STAIRS = 424;
const int DIORITE_STAIRS = 425;
const int ANDESITE_STAIRS = 426;
const int POLISHED_GRANITE_STAIRS = 427;
const int POLISHED_DIORITE_STAIRS = 428;
const int POLISHED_ANDESITE_STAIRS = 429;
const int MOSSY_STONE_BRICK_STAIRS = 430;
const int SMOOTH_RED_SANDSTONE_STAIRS = 431;
const int SMOOTH_SANDSTONE_STAIRS = 432;
const int END_BRICK_STAIRS = 433;
const int MOSSY_COBBLESTONE_STAIRS = 434;
const int NORMAL_STONE_STAIRS = 435;
const int SPRUCE_STANDING_SIGN = 436;
const int SPRUCE_WALL_SIGN = 437;
const int SMOOTH_STONE = 438;
const int RED_NETHER_BRICK_STAIRS = 439;
const int SMOOTH_QUARTZ_STAIRS = 440;
const int BIRCH_STANDING_SIGN = 441;
const int BIRCH_WALL_SIGN = 442;
const int JUNGLE_STANDING_SIGN = 443;
const int JUNGLE_WALL_SIGN = 444;
const int ACACIA_STANDING_SIGN = 445;
const int ACACIA_WALL_SIGN = 446;
const int DARKOAK_STANDING_SIGN = 447;
const int DARKOAK_WALL_SIGN = 448;
const int LECTERN = 449;
const int GRINDSTONE = 450;
const int BLAST_FURNACE = 451;
const int STONECUTTER_BLOCK = 452;
const int SMOKER = 453;
const int LIT_SMOKER = 454;
const int CARTOGRAPHY_TABLE = 455;
const int FLETCHING_TABLE = 456;
const int SMITHING_TABLE = 457;
const int BARREL = 458;
const int LOOM = 459;

const int BELL = 461;
const int SWEET_BERRY_BUSH = 462;
const int LANTERN = 463;
const int CAMPFIRE = 464;
const int LAVA_CAULDRON = 465;
const int JIGSAW = 466;
const int WOOD = 467;
const int COMPOSTER = 468;
const int LIT_BLAST_FURNACE = 469;

}

#endif //VANILLAGENERATOR_SRC_BLOCKS_BLOCKS_H_