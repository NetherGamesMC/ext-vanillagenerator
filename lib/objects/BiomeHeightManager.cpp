#include <map>
#include <cstdint>
#include <vector>
#include <lib/pocketmine/BiomeList.h>
#include "BiomeHeightManager.h"

namespace BiomeHeightManager {

std::map<uint_fast8_t, BiomeHeight> *heights = nullptr;
BiomeHeight *defaultHeight = nullptr;

void registerBiome(const BiomeHeight climate, const std::vector<uint_fast8_t> &biomeIds) {
  for (uint_fast8_t biomeId : biomeIds) {
    heights->insert({biomeId, climate});
  }
}

BiomeHeight get(int biome) {
  if (heights->find(biome) != heights->end()) {
    return (*heights)[biome];
  }

  return (*defaultHeight);
}

void init() {
  if (heights != nullptr || defaultHeight != nullptr) {
    return;
  }

  heights = new std::map<uint_fast8_t, BiomeHeight>;
  defaultHeight = new BiomeHeight{0.1, 0.2};

  registerBiome({-1.0, 0.1}, {OCEAN, FROZEN_OCEAN});
  registerBiome({-1.8, 0.1}, {DEEP_OCEAN});
  registerBiome({-0.5, 0.0}, {RIVER, FROZEN_RIVER});
  registerBiome({0.0, 0.025}, {BEACH, COLD_BEACH, MUSHROOM_SHORE});
  registerBiome({0.1, 0.8}, {STONE_BEACH});
  registerBiome({0.125, 0.05}, {DESERT, ICE_PLAINS, SAVANNA});

  registerBiome({1.0, 0.5}, {EXTREME_HILLS, EXTREME_HILLS_PLUS, EXTREME_HILLS_MOUNTAINS, EXTREME_HILLS_PLUS_MOUNTAINS});

  registerBiome({0.2, 0.2}, {TAIGA, COLD_TAIGA, MEGA_TAIGA});
  registerBiome({-0.2, 0.1}, {SWAMPLAND});
  registerBiome({0.2, 0.3}, {MUSHROOM_ISLAND});

  registerBiome({0.45, 0.3}, {ICE_MOUNTAINS, DESERT_HILLS, FOREST_HILLS, TAIGA_HILLS,
                              SMALL_MOUNTAINS, JUNGLE_HILLS, BIRCH_FOREST_HILLS, COLD_TAIGA_HILLS, MEGA_TAIGA_HILLS,
                              MESA_PLATEAU_FOREST_MOUNTAINS, MESA_PLATEAU_MOUNTAINS
  });

  registerBiome({1.5, 0.025}, {SAVANNA_PLATEAU, MESA_PLATEAU_FOREST, MESA_PLATEAU});
  registerBiome({0.275, 0.25}, {DESERT_MOUNTAINS});
  registerBiome({0.525, 0.55}, {ICE_PLAINS_SPIKES});
  registerBiome({0.55, 0.5}, {BIRCH_FOREST_HILLS_MOUNTAINS});
  registerBiome({-0.1, 0.3}, {SWAMPLAND_MOUNTAINS});
  registerBiome({0.2, 0.4}, {JUNGLE_MOUNTAINS, JUNGLE_EDGE_MOUNTAINS, BIRCH_FOREST_MOUNTAINS, ROOFED_FOREST_MOUNTAINS});
  registerBiome({0.3, 0.4}, {TAIGA_MOUNTAINS, COLD_TAIGA_MOUNTAINS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS});
  registerBiome({0.1, 0.4}, {FLOWER_FOREST});
  registerBiome({0.4125, 1.325}, {SAVANNA_MOUNTAINS});
  registerBiome({1.1, 1.3125}, {SAVANNA_PLATEAU_MOUNTAINS});
}

}