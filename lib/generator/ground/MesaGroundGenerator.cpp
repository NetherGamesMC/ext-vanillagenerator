#include "MesaGroundGenerator.h"
#include <algorithm>

typedef MesaGroundGenerator GroundGen;

void GroundGen::GenerateTerrainColumn(SimpleChunkManager &world,
                                      Random &random,
                                      int_fast64_t x,
                                      int_fast64_t z,
                                      int biome,
                                      double surface_noise) {
  Initialize(random.getSeed());

  int seaLevel = 64;

  MinecraftBlock topMat = top_material;
  MinecraftBlock groundMat = ground_material;

  int surfaceHeight = FuncMax((int) (surface_noise / 3.0 + 3.0 + random.nextFloat() * 0.25), 1);
  bool colored = cos(surface_noise / 3.0 * M_PI) <= 0;
  double bryceCanyonHeight = 0;
  if (type_ == BRYCE) {
    int_fast64_t noiseX = (x & 0xFFFFFFF0) + (z & 0xF);
    int_fast64_t noiseZ = (z & 0xFFFFFFF0) + (x & 0xF);
    double noiseCanyonHeight = FuncMin(abs(surface_noise), canyon_height_noise_->noise(noiseX, noiseZ, 0.5, 2.0, 0.0));
    if (noiseCanyonHeight > 0) {
      double heightScale = abs(canyon_scale_noise_->noise(noiseX, noiseZ, 0.5, 2.0, 0.0));
      bryceCanyonHeight = pow(noiseCanyonHeight, 2) * 2.5;
      double maxHeight = ceil(50 * heightScale) + 14;
      if (bryceCanyonHeight > maxHeight) {
        bryceCanyonHeight = maxHeight;
      }

      bryceCanyonHeight += seaLevel;
    }
  }

  int_fast64_t chunk_x = x;
  int_fast64_t chunk_z = z;

  int deep = -1;
  bool ground_set = false;

  MinecraftBlock grass = GRASS;
  MinecraftBlock coarse_dirt = COARSE_DIRT;

  for (int y = 255; y >= 0; --y) {
    if (y < (int) bryceCanyonHeight && world.getBlockAt(x, y, z) == AIR) {
      world.setBlockAt(x, y, z, STONE);
    }
    if (y <= random.nextInt(5)) {
      world.setBlockAt(x, y, z, BEDROCK);
    } else {
      auto mat_id = world.getBlockAt(x, y, z).getId();
      if (mat_id == AIR.getId()) {
        deep = -1;
      } else if (mat_id == STONE.getId()) {
        if (deep == -1) {
          ground_set = false;
          if (y >= seaLevel - 5 && y <= seaLevel) {
            groundMat = ground_material;
          }

          deep = surfaceHeight + FuncMax(0, y - seaLevel - 1);
          if (y >= seaLevel - 2) {
            if (type_ == FOREST_TYPE && y > seaLevel + 22 + (surfaceHeight << 1)) {
              topMat = colored ? grass : coarse_dirt;
              world.setBlockAt(x, y, z, topMat);
            } else if (y > seaLevel + 2 + surfaceHeight) {
              auto color = colour_layer_[(y + (int) round(
                  color_noise_->noise(chunk_x, chunk_z, 0, 0.5, 2.0, false) * 2.0)) % 64];
              SetColoredGroundLayer(world, x, y, z, y < seaLevel || y > 128 ? 1 : (colored ? color : -1));
            } else {
              world.setBlockAt(x, y, z, top_material);
              ground_set = true;
            }
          } else {
            world.setBlockAt(x, y, z, groundMat);
          }
        } else if (deep > 0) {
          --deep;
          if (ground_set) {
            world.setBlockAt(x, y, z, ground_material);
          } else {
            auto color = colour_layer_[(y + (int) round(
                color_noise_->noise(chunk_x, chunk_z, 0, 0.5, 2.0, false) * 2.0)) % 64];
            SetColoredGroundLayer(world, x, y, z, color);
          }
        }
      }
    }
  }
}

void GroundGen::Initialize(int_fast64_t seed) {
  if (seed != seed_ || random_ == nullptr || color_noise_ == nullptr || canyon_scale_noise_ == nullptr
      || canyon_height_noise_ == nullptr) {

    delete random_;
    delete color_noise_;
    delete canyon_scale_noise_;
    delete canyon_height_noise_;

    seed_ = seed;

    random_ = new Random(seed);
    color_noise_ = new SimplexOctaveGenerator(*random_, 1, 0, 0, 0);
    color_noise_->setScale(1 / 512.0);
    InitializeColorLayers(*random_);

    canyon_height_noise_ = new SimplexOctaveGenerator(*random_, 4, 0, 0, 0);
    canyon_height_noise_->setScale(1 / 4.0);
    canyon_scale_noise_ = new SimplexOctaveGenerator(*random_, 1, 0, 0, 0);
    canyon_scale_noise_->setScale(1 / 512.0);
  }
}

void GroundGen::InitializeColorLayers(Random &random) {
  colour_layer_.fill(-1);

  int i = 0;
  while (i < colour_layer_.size()) {
    i += static_cast<int>(random.nextInt(5) + 1);
    if (i < colour_layer_.size()) {
      colour_layer_[i++] = 1; // orange
    }
  }

  SetRandomLayerColor(random, 2, 1, 4);  // yellow
  SetRandomLayerColor(random, 2, 2, 12); // brown
  SetRandomLayerColor(random, 2, 1, 14); // red

  int j = 0;
  for (i = 0; i < random.nextInt(3) + 3; i++) {
    j += static_cast<int>(random.nextInt(16)) + 4;
    if (j >= colour_layer_.size()) {
      break;
    }

    if (random.nextInt(2) == 0 || j < colour_layer_.size() - 1 && random.nextInt(2) == 0) {
      colour_layer_[j - 1] = 8; // light gray
    } else {
      colour_layer_[j] = 0; // white
    }
  }
}

void GroundGen::SetRandomLayerColor(Random &random, int min_layer_count, int min_layer_height, int color) {
  for (int i = 0; i < random.nextInt(4) + min_layer_count; i++) {
    int_fast64_t j = random.nextInt(64);

    int k = 0;
    while (k < random.nextInt(3) + min_layer_height && j < 64) {
      colour_layer_[j++] = color;
      k++;
    }
  }
}

void GroundGen::SetColoredGroundLayer(SimpleChunkManager &world,
                                      int_fast64_t x,
                                      int_fast32_t y,
                                      int_fast64_t z,
                                      int color) {
  if (color >= 0 && color <= 15) {
    world.setBlockAt(x, y, z, MinecraftBlock(STAINED_CLAY.getId(), color));
  } else {
    world.setBlockAt(x, y, z, HARDENED_CLAY);
  }
}

void MesaGroundGenerator::Clean() {
  delete color_noise_;
  delete canyon_height_noise_;
  delete canyon_scale_noise_;
  delete random_;
}
