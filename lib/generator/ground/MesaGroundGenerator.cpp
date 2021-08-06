#include "MesaGroundGenerator.h"
#include <algorithm>

typedef MesaGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(ChunkManager &world,
                                      Random &random,
                                      int_fast32_t x,
                                      int_fast32_t z,
                                      int biome,
                                      double surface_noise) {
  Initialize(random.GetSeed());

  int seaLevel = 64;

  MinecraftBlock topMat = top_material;
  MinecraftBlock groundMat = ground_material;

  int surfaceHeight = FuncMax((int) (surface_noise / 3.0 + 3.0 + random.NextFloat() * 0.25), 1);
  bool colored = cos(surface_noise / 3.0 * M_PI) <= 0;
  double bryceCanyonHeight = 0;
  if (type_ == BRYCE) {
    int_fast32_t noiseX = (x & 0xFFFFFFF0) + (z & 0xF);
    int_fast32_t noiseZ = (z & 0xFFFFFFF0) + (x & 0xF);
    double noiseCanyonHeight = FuncMin(abs(surface_noise), canyon_height_noise_->Noise(noiseX, noiseZ, 0, 0.5, 2.0, false));
    if (noiseCanyonHeight > 0) {
      double heightScale = abs(canyon_scale_noise_->Noise(noiseX, noiseZ, 0, 0.5, 2.0, false));
      bryceCanyonHeight = pow(noiseCanyonHeight, 2) * 2.5;
      double maxHeight = ceil(50 * heightScale) + 14;
      if (bryceCanyonHeight > maxHeight) {
        bryceCanyonHeight = maxHeight;
      }

      bryceCanyonHeight += seaLevel;
    }
  }

  int_fast32_t chunk_x = x;
  int_fast32_t chunk_z = z;

  int deep = -1;
  bool ground_set = false;

  MinecraftBlock grass = GRASS;
  MinecraftBlock coarse_dirt = COARSE_DIRT;

  for (int y = 255; y >= 0; --y) {
    auto highest_block_at = world.GetHighestBlockAt(x, z);
    if (y < (int) bryceCanyonHeight && world.GetBlockAt(x, y, z) == AIR && !(highest_block_at == WATER || highest_block_at == STILL_WATER)) { // Make sure we are dealing with something that isn't water.
      world.SetBlockAt(x, y, z, STONE);
    }
    if (y <= random.NextInt(5)) {
      world.SetBlockAt(x, y, z, BEDROCK);
    } else {
      auto mat_id = world.GetBlockAt(x, y, z).GetId();
      if (mat_id == AIR.GetId()) {
        deep = -1;
      } else if (mat_id == STONE.GetId()) {
        if (deep == -1) {
          ground_set = false;
          if (y >= seaLevel - 5 && y <= seaLevel) {
            groundMat = ground_material;
          }

          deep = surfaceHeight + FuncMax(0, y - seaLevel - 1);
          if (y >= seaLevel - 2) {
            if (type_ == FOREST_TYPE && y > seaLevel + 22 + (surfaceHeight << 1)) {
              topMat = colored ? grass : coarse_dirt;
              world.SetBlockAt(x, y, z, topMat);
            } else if (y > seaLevel + 2 + surfaceHeight) {
              auto color = colour_layer_[(y + (int) round(
                  color_noise_->Noise(chunk_x, chunk_z, 0, 0.5, 2.0, false) * 2.0)) % 64];
              SetColoredGroundLayer(world, x, y, z, y < seaLevel || y > 128 ? 1 : (colored ? color : -1));
            } else {
              world.SetBlockAt(x, y, z, top_material);
              ground_set = true;
            }
          } else {
            world.SetBlockAt(x, y, z, groundMat);
          }
        } else if (deep > 0) {
          --deep;
          if (ground_set) {
            world.SetBlockAt(x, y, z, ground_material);
          } else {
            auto color = colour_layer_[(y + (int) round(
                color_noise_->Noise(chunk_x, chunk_z, 0, 0.5, 2.0, false) * 2.0)) % 64];
            SetColoredGroundLayer(world, x, y, z, color);
          }
        }
      }
    }
  }
}

void GroundGen::Initialize(int_fast32_t seed) {
  if (seed != seed_ || random_ == nullptr || color_noise_ == nullptr || canyon_scale_noise_ == nullptr
      || canyon_height_noise_ == nullptr) {

    delete random_;
    delete color_noise_;
    delete canyon_scale_noise_;
    delete canyon_height_noise_;

    seed_ = seed;

    random_ = new Random(seed);
    color_noise_ = new SimplexOctaveGenerator(*random_, 1, 0, 0, 0);
    color_noise_->SetScale(1 / 512.0);
    InitializeColorLayers(*random_);

    canyon_height_noise_ = new SimplexOctaveGenerator(*random_, 4, 0, 0, 0);
    canyon_height_noise_->SetScale(1 / 4.0);
    canyon_scale_noise_ = new SimplexOctaveGenerator(*random_, 1, 0, 0, 0);
    canyon_scale_noise_->SetScale(1 / 512.0);
  }
}

void GroundGen::InitializeColorLayers(Random &random) {
  colour_layer_.fill(-1);

  int i = 0;
  while (i < colour_layer_.size()) {
    i += static_cast<int>(random.NextInt(5) + 1);
    if (i < colour_layer_.size()) {
      colour_layer_[i++] = 1; // orange
    }
  }

  SetRandomLayerColor(random, 2, 1, 4);  // yellow
  SetRandomLayerColor(random, 2, 2, 12); // brown
  SetRandomLayerColor(random, 2, 1, 14); // red

  int j = 0;
  for (i = 0; i < random.NextInt(3) + 3; i++) {
    j += static_cast<int>(random.NextInt(16)) + 4;
    if (j >= colour_layer_.size()) {
      break;
    }

    if (random.NextInt(2) == 0 || j < colour_layer_.size() - 1 && random.NextInt(2) == 0) {
      colour_layer_[j - 1] = 8; // light gray
    } else {
      colour_layer_[j] = 0; // white
    }
  }
}

void GroundGen::SetRandomLayerColor(Random &random, int min_layer_count, int min_layer_height, int color) {
  for (int i = 0; i < random.NextInt(4) + min_layer_count; i++) {
    int_fast32_t j = random.NextInt(64);

    int k = 0;
    while (k < random.NextInt(3) + min_layer_height && j < 64) {
      colour_layer_[j++] = color;
      k++;
    }
  }
}

void GroundGen::SetColoredGroundLayer(ChunkManager &world,
                                      int_fast32_t x,
                                      int_fast32_t y,
                                      int_fast32_t z,
                                      int color) {
  if (color >= 0 && color <= 15) {
    world.SetBlockAt(x, y, z, MinecraftBlock(STAINED_CLAY.GetId(), color));
  } else {
    world.SetBlockAt(x, y, z, HARDENED_CLAY);
  }
}

MesaGroundGenerator::~MesaGroundGenerator() {
  delete color_noise_;
  delete canyon_height_noise_;
  delete canyon_scale_noise_;
  delete random_;
}
