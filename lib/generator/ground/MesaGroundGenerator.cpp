#include "MesaGroundGenerator.h"
#include <algorithm>
#include <lib/objects/math/Math.h>

typedef MesaGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world, Random &random, int_fast32_t x, int_fast32_t z, int biome, double surfaceNoise) {
  Initialize(random.GetSeed());

  int seaLevel = 64;

  MinecraftBlock topMat = topMaterial;
  MinecraftBlock groundMat = groundMaterial;

  int surfaceHeight = Math::Max((int) (surfaceNoise / 3.0 + 3.0 + random.NextFloat() * 0.25), 1);
  bool colored = cos(surfaceNoise / 3.0 * M_PI) <= 0;
  double bryceCanyonHeight = 0;
  if (type_ == BRYCE) {
    int_fast32_t noiseX = (x & 0xFFFFFFF0) + (z & 0xF);
    int_fast32_t noiseZ = (z & 0xFFFFFFF0) + (x & 0xF);
    double noiseCanyonHeight = Math::Max(abs(surfaceNoise), canyonHeightNoise_.Noise(noiseX, noiseZ, 0, 0.5, 2.0, false));
    if (noiseCanyonHeight > 0) {
      double heightScale = abs(canyonScaleNoise_.Noise(noiseX, noiseZ, 0, 0.5, 2.0, false));
      bryceCanyonHeight = pow(noiseCanyonHeight, 2) * 2.5;
      double maxHeight = ceil(50 * heightScale) + 14;
      if (bryceCanyonHeight > maxHeight) {
        bryceCanyonHeight = maxHeight;
      }

      bryceCanyonHeight += seaLevel;
    }
  }

  int_fast32_t chunkX = x;
  int_fast32_t chunkZ = z;

  int deep = -1;
  bool groundSet = false;

  MinecraftBlock grass = GRASS;
  MinecraftBlock coarseDirt = COARSE_DIRT;

  for (int y = 255; y >= 0; --y) {
    auto highest_block_at = world.GetHighestBlockAt(x, z);
    if (y < (int) bryceCanyonHeight && world.GetBlockAt(x, y, z) == AIR && !(highest_block_at == WATER || highest_block_at == STILL_WATER)) { // Make sure we are dealing with something that isn't water.
      world.SetBlockAt(x, y, z, STONE);
    }
    if (y <= random.NextInt(5)) {
      world.SetBlockAt(x, y, z, BEDROCK);
    } else {
      auto matId = world.GetBlockAt(x, y, z).GetId();
      if (matId == AIR.GetId()) {
        deep = -1;
      } else if (matId == STONE.GetId()) {
        if (deep == -1) {
          groundSet = false;
          if (y >= seaLevel - 5 && y <= seaLevel) {
            groundMat = groundMaterial;
          }

          deep = surfaceHeight + Math::Max(0, y - seaLevel - 1);
          if (y >= seaLevel - 2) {
            if (type_ == FOREST_TYPE && y > seaLevel + 22 + (surfaceHeight << 1)) {
              topMat = colored ? grass : coarseDirt;
              world.SetBlockAt(x, y, z, topMat);
            } else if (y > seaLevel + 2 + surfaceHeight) {
              auto color = colourLayer_[(y + (int) round(
                  colorNoise_.Noise(chunkX, chunkZ, 0, 0.5, 2.0, false) * 2.0)) % 64];
              SetColoredGroundLayer(world, x, y, z, y < seaLevel || y > 128 ? 1 : (colored ? color : -1));
            } else {
              world.SetBlockAt(x, y, z, topMaterial);
              groundSet = true;
            }
          } else {
            world.SetBlockAt(x, y, z, groundMat);
          }
        } else if (deep > 0) {
          --deep;
          if (groundSet) {
            world.SetBlockAt(x, y, z, groundMaterial);
          } else {
            auto color = colourLayer_[(y + (int) round(colorNoise_.Noise(chunkX, chunkZ, 0, 0.5, 2.0, false) * 2.0)) % 64];
            SetColoredGroundLayer(world, x, y, z, color);
          }
        }
      }
    }
  }
}

MesaGroundGenerator::MesaGroundGenerator(MesaType type) : type_(type) {
  colourLayer_.fill(-1); // Hardened clay

  topMaterial = RED_SAND;
  groundMaterial = MinecraftBlock(STAINED_CLAY.GetId(), 1); // Orange block

  colorNoise_.SetScale(1 / 512.0);
  canyonHeightNoise_.SetScale(1 / 4.0);
  canyonScaleNoise_.SetScale(1 / 512.0);
}

void GroundGen::Initialize(int_fast64_t seed) {
  if (seed != seed_) {
    seed_ = seed;
    random_.SetSeed(seed);

    InitializeColorLayers(random_);
  }
}

void GroundGen::InitializeColorLayers(Random &random) {
  colourLayer_.fill(-1);

  int i = 0;
  while (i < colourLayer_.size()) {
    i += static_cast<int>(random.NextInt(5) + 1);
    if (i < colourLayer_.size()) {
      colourLayer_[i++] = 1; // orange
    }
  }

  SetRandomLayerColor(random, 2, 1, 4);  // yellow
  SetRandomLayerColor(random, 2, 2, 12); // brown
  SetRandomLayerColor(random, 2, 1, 14); // red

  int j = 0;
  for (i = 0; i < random.NextInt(3) + 3; i++) {
    j += static_cast<int>(random.NextInt(16)) + 4;
    if (j >= colourLayer_.size()) {
      break;
    }

    if (random.NextInt(2) == 0 || j < colourLayer_.size() - 1 && random.NextInt(2) == 0) {
      colourLayer_[j - 1] = 8; // light gray
    } else {
      colourLayer_[j] = 0; // white
    }
  }
}

void GroundGen::SetRandomLayerColor(Random &random, int minLayerCount, int minLayerHeight, int color) {
  for (int i = 0; i < random.NextInt(4) + minLayerCount; i++) {
    int_fast32_t j = random.NextInt(64);

    int k = 0;
    while (k < random.NextInt(3) + minLayerHeight && j < 64) {
      colourLayer_[j++] = color;
      k++;
    }
  }
}

void GroundGen::SetColoredGroundLayer(ChunkManager &world, int_fast32_t x, int_fast32_t y, int_fast32_t z, int color) {
  if (color >= 0 && color <= 15) {
    world.SetBlockAt(x, y, z, MinecraftBlock(STAINED_CLAY.GetId(), color));
  } else {
    world.SetBlockAt(x, y, z, HARDENED_CLAY);
  }
}