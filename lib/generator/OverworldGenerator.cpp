#include "OverworldGenerator.h"

#include <algorithm>
#include <memory>
#include <lib/objects/BiomeHeightManager.h>
#include <lib/pocketmine/BiomeList.h>
#include <lib/generator/ground/SandyGroundGenerator.h>
#include <lib/generator/ground/RockyGroundGenerator.h>
#include <lib/generator/ground/MycelGroundGenerator.h>
#include <lib/generator/ground/StonePatchGroundGenerator.h>
#include <lib/generator/ground/GravelPatchGroundGenerator.h>
#include <lib/generator/ground/DirtAndStonePatchGroundGenerator.h>
#include <lib/generator/ground/DirtPatchGroundGenerator.h>
#include <lib/generator/ground/MesaGroundGenerator.h>
#include <lib/generator/ground/SnowyGroundGenerator.h>
#include <lib/objects/Biome.h>

#define COORDINATE_SCALE 684.412
#define HEIGHT_SCALE 684.412
#define HEIGHT_NOISE_SCALE_X 200.0
#define HEIGHT_NOISE_SCALE_Z 200.0
#define DETAIL_NOISE_SCALE_X 80.0
#define DETAIL_NOISE_SCALE_Y 160.0
#define DETAIL_NOISE_SCALE_Z 80.0
#define SURFACE_SCALE 0.0625
#define BASE_SIZE 8.5
#define STRETCH_Y 12.0
#define BIOME_HEIGHT_OFFSET 0.0
#define BIOME_HEIGHT_WEIGHT 1.0
#define BIOME_SCALE_OFFSET 0.0
#define BIOME_SCALE_WEIGHT 1.0
#define DENSITY_FILL_MODE 0
#define DENSITY_FILL_SEA_MODE 0
#define DENSITY_FILL_OFFSET 0.0

OverworldGenerator::OverworldGenerator(int_fast64_t seed, bool isUHC)
    : random_(seed), octave_random_(seed), map_layer_(GridBiome::initialize(seed, isUHC)), is_uhc_(isUHC) {

  // Initialize octaves
  octaves_ = new WorldOctaves{
      PerlinOctaveGenerator(octave_random_, 16, 5, 1, 5),
      PerlinOctaveGenerator(octave_random_, 16, 5, 33, 5),
      PerlinOctaveGenerator(octave_random_, 16, 5, 33, 5),
      PerlinOctaveGenerator(octave_random_, 8, 5, 33, 5),
      SimplexOctaveGenerator(octave_random_, 4, 16, 1, 16)
  };

  octaves_->height.setXScale(HEIGHT_NOISE_SCALE_X);
  octaves_->height.setZScale(HEIGHT_NOISE_SCALE_Z);

  octaves_->roughness.setXScale(COORDINATE_SCALE);
  octaves_->roughness.setYScale(HEIGHT_SCALE);
  octaves_->roughness.setZScale(COORDINATE_SCALE);

  octaves_->roughness_2.setXScale(COORDINATE_SCALE);
  octaves_->roughness_2.setYScale(HEIGHT_SCALE);
  octaves_->roughness_2.setZScale(COORDINATE_SCALE);

  octaves_->detail.setXScale(COORDINATE_SCALE / DETAIL_NOISE_SCALE_X);
  octaves_->detail.setYScale(HEIGHT_SCALE / DETAIL_NOISE_SCALE_Y);
  octaves_->detail.setZScale(COORDINATE_SCALE / DETAIL_NOISE_SCALE_Z);

  octaves_->surface.setScale(SURFACE_SCALE);

  ground_map_.insert({{BEACH, COLD_BEACH, DESERT, DESERT_HILLS, DESERT_MOUNTAINS},
                      std::shared_ptr<GroundGenerator>(new SandyGroundGenerator())});
  ground_map_.insert({{STONE_BEACH}, std::shared_ptr<GroundGenerator>(new RockyGroundGenerator())});
  ground_map_.insert({{ICE_PLAINS_SPIKES}, std::shared_ptr<GroundGenerator>(new SnowyGroundGenerator())});
  ground_map_.insert({{MUSHROOM_ISLAND, MUSHROOM_SHORE}, std::shared_ptr<GroundGenerator>(new MycelGroundGenerator())});
  ground_map_.insert({{EXTREME_HILLS}, std::shared_ptr<GroundGenerator>(new StonePatchGroundGenerator())});
  ground_map_.insert({{EXTREME_HILLS_MOUNTAINS, EXTREME_HILLS_PLUS_MOUNTAINS},
                      std::shared_ptr<GroundGenerator>(new GravelPatchGroundGenerator())});
  ground_map_.insert({{SAVANNA_MOUNTAINS, SAVANNA_PLATEAU_MOUNTAINS},
                      std::shared_ptr<GroundGenerator>(new DirtAndStonePatchGroundGenerator())});
  ground_map_.insert({{MEGA_TAIGA, MEGA_TAIGA_HILLS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS},
                      std::shared_ptr<GroundGenerator>(new DirtPatchGroundGenerator())});
  ground_map_.insert({{MESA, MESA_PLATEAU, MESA_PLATEAU_FOREST},
                      std::shared_ptr<GroundGenerator>(new MesaGroundGenerator())});
  ground_map_.insert({{MESA_BRYCE}, std::shared_ptr<GroundGenerator>(new MesaGroundGenerator(MesaType::BRYCE))});
  ground_map_.insert({{MESA_PLATEAU_FOREST, MESA_PLATEAU_FOREST_MOUNTAINS},
                      std::shared_ptr<GroundGenerator>(new MesaGroundGenerator(MesaType::FOREST_TYPE))});

  // fill a 5x5 array with values that acts as elevation weight on chunk neighboring,
  // this can be viewed as a parabolic field: the center gets the more weight, and the
  // weight decreases as distance increases from the center. This is applied on the
  // lower scale biome grid.
  for (int x = 0; x < 5; ++x) {
    for (int z = 0; z < 5; ++z) {
      double sq_x = x - 2;
      sq_x *= sq_x;
      double sq_z = z - 2;
      sq_z *= sq_z;

      double value = 10.0 / sqrt(sq_x + sq_z + 0.2);

      elevation_weight_.insert({ElevationWeightHash(x, z), value});
    }
  }

  populators.push_back(std::shared_ptr<Populator>(new OverworldPopulator()));
}

void OverworldGenerator::GenerateChunk(SimpleChunkManager &world, int_fast64_t chunk_x, int_fast64_t chunk_z) {
  Biome::init(is_uhc_);
  BiomeHeightManager::init(is_uhc_);

  GridBiome::BiomeGrid read = map_layer_.high_resolution->GenerateValues(chunk_x * 16, chunk_z * 16, 16, 16);

  GenerateChunkData(world, chunk_x, chunk_z, VanillaBiomeGrid(read));
}

void OverworldGenerator::PopulateChunk(SimpleChunkManager &world, int_fast64_t chunk_x, int_fast64_t chunk_z) {
  for (auto &x : populators) {
    x->Populate(world, random_, chunk_x, chunk_z);
  }
}

OverworldGenerator::~OverworldGenerator() {
  elevation_weight_.clear();
  populators.clear();
  ground_map_.clear();

  map_layer_.high_resolution.reset();
  map_layer_.low_resolution.reset();

  Biome::clean();
  BiomeHeightManager::clean();

  delete octaves_;
}

void OverworldGenerator::GenerateChunkData(SimpleChunkManager &world,
                                           int_fast64_t chunk_x,
                                           int_fast64_t chunk_z,
                                           const VanillaBiomeGrid &biome) {
  GenerateRawTerrain(world, chunk_x, chunk_z);

  int_fast64_t cx = chunk_x << 4;
  int_fast64_t cz = chunk_z << 4;

  auto octave_generator = octaves_->surface;
  auto size_x = octave_generator.getSizeX();
  auto size_z = octave_generator.getSizeZ();

  auto surface_noise = octave_generator.getFractalBrownianMotion(cx, 0.0, cz, 0.5, 0.5);

  auto chunk = world.getChunk(chunk_x, chunk_z);

  int id;
  for (int x = 0; x < size_x; ++x) {
    for (int z = 0; z < size_z; ++z) {
      id = biome.getBiome(x, z);

      if (is_uhc_ && (id == 0 || id == 6 || id == 10 || (id >= 21 && id <= 24) || (id >= 32 && id <= 33) || id == 134
          || id == 149 || id == 151 || id == 160 || id == 161)) {
        id = 132;
      }

      chunk->getBiomeArray()->set(x, z, id);

      bool found = false;
      for (const auto &mappings : ground_map_) {
        auto biomes = mappings.first;
        if (std::find(biomes.begin(), biomes.end(), id) != biomes.end()) {
          mappings.second->GenerateTerrainColumn(world, random_, cx + x, cz + z, id, surface_noise[x | z << 4]);

          found = true;
          break;
        }
      }

      if (!found) {
        default_generator.GenerateTerrainColumn(world, random_, cx + x, cz + z, id, surface_noise[x | z << 4]);
      }
    }
  }
}

int OverworldGenerator::ElevationWeightHash(int x, int z) {
  return (x << 3) | z;
}

int OverworldGenerator::DensityHash(int i, int j, int k) {
  return (k << 6) | (j << 3) | i;
}

TerrainDensity OverworldGenerator::GenerateTerrainDensity(int_fast64_t x, int_fast64_t z) {
  TerrainDensity density;

  // Scaling chunk x and z coordinates (4x, see below)
  x <<= 2;
  z <<= 2;

  // Get biome grid data at lower res (scaled 4x, at this scale a chunk is 4x4 columns of
  // the biome grid),
  // we are loosing biome detail but saving huge amount of computation.
  // We need 1 chunk (4 columns) + 1 column for later needed outer edges (1 column) and at
  // least 2 columns
  // on each side to be able to cover every value.
  // 4 + 1 + 2 + 2 = 9 columns but the biomegrid generator needs a multiple of 2 so we ask
  // 10 columns wide
  // to the biomegrid generator.
  // This gives a total of 81 biome grid columns to work with, and this includes the chunk
  // neighborhood.
  auto biomeGrid = map_layer_.low_resolution->GenerateValues(x, z, 10, 10);

  auto height_noise = octaves_->height.getFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughness_noise = octaves_->roughness.getFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughness_noise_2 = octaves_->roughness_2.getFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto detail_noise = octaves_->detail.getFractalBrownianMotion(x, 0, z, 0.5, 2.0);

  int index = 0;
  int index_height = 0;

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      double avg_height_scale = 0.0;
      double avg_height_base = 0.0;
      double total_weight = 0.0;

      int biome = biomeGrid[i + 2 + (j + 2) * 10];
      BiomeHeight biome_height = BiomeHeightManager::get(biome);
      // Sampling an average height base and scale by visiting the neighborhood
      // of the current biomegrid column.
      for (int m = 0; m < 5; ++m) {
        for (int n = 0; n < 5; ++n) {
          int near_biome = biomeGrid[i + m + (j + n) * 10];
          BiomeHeight near_biome_height = BiomeHeightManager::get(near_biome);

          double height_base = BIOME_HEIGHT_OFFSET + near_biome_height.height * BIOME_HEIGHT_WEIGHT;
          double height_scale = BIOME_SCALE_OFFSET + near_biome_height.scale * BIOME_SCALE_WEIGHT;

          double weight = elevation_weight_[ElevationWeightHash(m, n)] / (height_base + 2.0);
          if (near_biome_height.height > biome_height.height) {
            weight *= 0.5;
          }

          avg_height_scale += height_scale * weight;
          avg_height_base += height_base * weight;
          total_weight += weight;
        }
      }

      avg_height_scale /= total_weight;
      avg_height_base /= total_weight;
      avg_height_scale = avg_height_scale * 0.9 + 0.1;
      avg_height_base = (avg_height_base * 4.0 - 1.0) / 8.0;

      double noise_h = height_noise[index_height++] / 8000.0;
      if (noise_h < 0) {
        noise_h = -noise_h * 0.3;
      }

      noise_h = noise_h * 3.0 - 2.0;
      if (noise_h < 0) {
        noise_h = fmax(noise_h * 0.5, -1.0) / 1.4 * 0.5;
      } else {
        noise_h = fmin(noise_h, 1.0) / 8.0;
      }

      noise_h = (noise_h * 0.2 + avg_height_base) * BASE_SIZE / 8.0 * 4.0 + BASE_SIZE;
      for (int k = 0; k < 33; ++k) {
        // density should be lower and lower as we climb up, this gets a height value to
        // subtract from the noise.
        double nh = (k - noise_h) * STRETCH_Y * 128.0 / 256.0 / avg_height_scale;
        if (nh < 0.0) {
          nh *= 4.0;
        }

        double noise_r = roughness_noise[index] / 512.0;
        double noise_r_2 = roughness_noise_2[index] / 512.0;
        double noise_d = (detail_noise[index] / 10.0 + 1.0) / 2.0;

        // linear interpolation
        double dens = noise_d < 0 ? noise_r : (noise_d > 1 ? noise_r_2 : noise_r + (noise_r_2 - noise_r) * noise_d);
        dens -= nh;
        ++index;
        if (k > 29) {
          double lowering = (k - 29) / 3.0;
          // linear interpolation
          dens = dens * (1.0 - lowering) + -10.0 * lowering;
        }
        density[DensityHash(i, j, k)] = dens;
      }
    }
  }
  return density;
}

void OverworldGenerator::GenerateRawTerrain(SimpleChunkManager &world, int_fast64_t chunk_x, int_fast64_t chunk_z) {
  auto density = GenerateTerrainDensity(chunk_x, chunk_z);

  int sea_level = 64;

  // Terrain densities are sampled at different resolutions (1/4x on x,z and 1/8x on y by default)
  // so it's needed to re-scale it. Linear interpolation is used to fill in the gaps.

  int fill = abs(DENSITY_FILL_MODE);
  int sea_fill = DENSITY_FILL_SEA_MODE;
  double density_offset = DENSITY_FILL_OFFSET;

  auto still_water = STILL_WATER.getFullId();
  auto water = WATER.getFullId();
  auto stone = STONE.getFullId();

  auto chunk = world.getChunk(chunk_x, chunk_z);
  for (int i = 0; i < 5 - 1; ++i) {
    for (int j = 0; j < 5 - 1; ++j) {
      for (int k = 0; k < 33 - 1; ++k) {
        // 2x2 grid
        double d1 = density[DensityHash(i, j, k)];
        double d2 = density[DensityHash(i + 1, j, k)];
        double d3 = density[DensityHash(i, j + 1, k)];
        double d4 = density[DensityHash(i + 1, j + 1, k)];
        // 2x2 grid (row above)
        double d5 = (density[DensityHash(i, j, k + 1)] - d1) / 8;
        double d6 = (density[DensityHash(i + 1, j, k + 1)] - d2) / 8;
        double d7 = (density[DensityHash(i, j + 1, k + 1)] - d3) / 8;
        double d8 = (density[DensityHash(i + 1, j + 1, k + 1)] - d4) / 8;

        for (int l = 0; l < 8; ++l) {
          double d9 = d1;
          double d10 = d3;

          int y_pos = l + (k << 3);
          int y_block_pos = y_pos & 0xf;
          NormalBlockArrayContainer *sub_chunk = chunk->getSubChunk(y_pos >> 4);
          for (int m = 0; m < 4; ++m) {
            double dens = d9;
            for (int n = 0; n < 4; ++n) {
              // any density higher than density offset is ground, any density
              // lower or equal to the density offset is air
              // (or water if under the sea level).
              // this can be flipped if the mode is negative, so lower or equal
              // to is ground, and higher is air/water
              // and, then data can be shifted by afill the order is air by
              // default, ground, then water. they can shift places
              // within each if statement
              // the target is density_offset + 0, since the default target is
              // 0, so don't get too confused by the naming :)
              if (fill == 1 || fill == 10 || fill == 13 || fill == 16) {
                sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), water);
              } else if (fill == 2 || fill == 9 || fill == 12 || fill == 15) {
                sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), stone);
              }

              if ((dens > density_offset && fill > -1) || (dens <= density_offset && fill < 0)) {
                if (fill == 0 || fill == 3 || fill == 6 || fill == 9 || fill == 12) {
                  sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), stone);
                } else if (fill == 2 || fill == 7 || fill == 10 || fill == 16) {
                  sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), still_water);
                }
              } else if ((y_pos < sea_level - 1 && sea_fill == 0) || (y_pos >= sea_level - 1 && sea_fill == 1)) {
                if (fill == 0 || fill == 3 || fill == 7 || fill == 10 || fill == 13) {
                  sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), still_water);
                } else if (fill == 1 || fill == 6 || fill == 9 || fill == 15) {
                  sub_chunk->set(m + (i << 2), y_block_pos, n + (j << 2), stone);
                }
              }

              dens += (d10 - d9) / 4;
            }

            // interpolation along x
            d9 += (d2 - d1) / 4;

            // interpolate along z
            d10 += (d4 - d3) / 4;
          }

          // interpolation along y
          d1 += d5;
          d3 += d7;
          d2 += d6;
          d4 += d8;
        }
      }
    }
  }
}