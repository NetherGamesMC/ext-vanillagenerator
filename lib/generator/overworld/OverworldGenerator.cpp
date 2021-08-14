#include "OverworldGenerator.h"

#include <algorithm>
#include <lib/biomes/BiomeHeightManager.h>
#include <lib/objects/constants/BiomeList.h>
#include <lib/biomes/BiomeClimate.h>

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
    : random_(seed), octaveRandom_(seed), mapLayer_(GridBiome::Initialize(seed, isUHC)), isUHC_(isUHC), octaves_(
    {
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 1, 5),
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 33, 5),
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 33, 5),
        PerlinOctaveGenerator(octaveRandom_, 8, 5, 33, 5),
        SimplexOctaveGenerator(octaveRandom_, 4, 16, 1, 16)
    }) {

  BiomeClimate::Init(isUHC_);
  BiomeHeightManager::Init(isUHC_);

  octaves_.height.SetXScale(HEIGHT_NOISE_SCALE_X);
  octaves_.height.SetZScale(HEIGHT_NOISE_SCALE_Z);

  octaves_.roughness.SetXScale(COORDINATE_SCALE);
  octaves_.roughness.SetYScale(HEIGHT_SCALE);
  octaves_.roughness.SetZScale(COORDINATE_SCALE);

  octaves_.roughness2.SetXScale(COORDINATE_SCALE);
  octaves_.roughness2.SetYScale(HEIGHT_SCALE);
  octaves_.roughness2.SetZScale(COORDINATE_SCALE);

  octaves_.detail.SetXScale(COORDINATE_SCALE / DETAIL_NOISE_SCALE_X);
  octaves_.detail.SetYScale(HEIGHT_SCALE / DETAIL_NOISE_SCALE_Y);
  octaves_.detail.SetZScale(COORDINATE_SCALE / DETAIL_NOISE_SCALE_Z);

  octaves_.surface.SetScale(SURFACE_SCALE);

  groundMap_.insert({{BEACH, COLD_BEACH, DESERT, DESERT_HILLS, DESERT_MOUNTAINS}, sandyGroundGenerator_});
  groundMap_.insert({{STONE_BEACH}, rockyGroundGenerator_});
  groundMap_.insert({{ICE_PLAINS_SPIKES}, snowyGroundGenerator_});
  groundMap_.insert({{MUSHROOM_ISLAND, MUSHROOM_SHORE}, mycelGroundGenerator_});
  groundMap_.insert({{EXTREME_HILLS}, stonePatchGroundGenerator_});
  groundMap_.insert({{EXTREME_HILLS_MOUNTAINS, EXTREME_HILLS_PLUS_MOUNTAINS}, gravelPatchGroundGenerator_});
  groundMap_.insert({{SAVANNA_MOUNTAINS, SAVANNA_PLATEAU_MOUNTAINS}, dirtAndStonePatchGroundGenerator_});
  if (!isUHC_) {
    groundMap_.insert({{MEGA_TAIGA, MEGA_TAIGA_HILLS, MEGA_SPRUCE_TAIGA, MEGA_SPRUCE_TAIGA_HILLS}, dirtPatchGroundGenerator_});
  }
  groundMap_.insert({{MESA, MESA_PLATEAU, MESA_PLATEAU_FOREST}, defaultMesaGroundGenerator_});
  groundMap_.insert({{MESA_BRYCE}, bryceMesaGroundGenerator_});
  groundMap_.insert({{MESA_PLATEAU_FOREST, MESA_PLATEAU_FOREST_MOUNTAINS}, forestMesaGroundGenerator_});

  // fill a 5x5 array with values that acts as elevation weight on chunk neighboring,
  // this can be viewed as a parabolic field: the center gets the more weight, and the
  // weight decreases as distance increases from the center. This is applied on the
  // lower scale biome grid.
  for (int_fast32_t x = 0; x < 5; ++x) {
    for (int_fast32_t z = 0; z < 5; ++z) {
      double sqX = x - 2;
      sqX *= sqX;
      double sqZ = z - 2;
      sqZ *= sqZ;

      double value = 10.0 / sqrt(sqX + sqZ + 0.2);

      elevationWeight_.insert({ElevationWeightHash(x, z), value});
    }
  }
}

void OverworldGenerator::GenerateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  GridBiome::BiomeGrid read = mapLayer_.highResolution->GenerateValues(chunkX * 16, chunkZ * 16, 16, 16);

  GenerateChunkData(world, chunkX, chunkZ, VanillaBiomeGrid(read));

  caveGenerator.Generate(world, random_, chunkX, chunkZ, world.GetChunk(chunkX, chunkZ));
}

void OverworldGenerator::PopulateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  populators_.Populate(world, random_, chunkX, chunkZ);
}

OverworldGenerator::~OverworldGenerator() {
  elevationWeight_.clear();
  groundMap_.clear();

  mapLayer_.highResolution.reset();
  mapLayer_.lowResolution.reset();
}

void OverworldGenerator::GenerateChunkData(ChunkManager &world,
                                           int_fast32_t chunkX,
                                           int_fast32_t chunkZ,
                                           const VanillaBiomeGrid &biome) {
  GenerateRawTerrain(world, chunkX, chunkZ);

  int_fast32_t cx = chunkX << 4;
  int_fast32_t cz = chunkZ << 4;

  auto octaveGenerator = octaves_.surface;
  auto sizeX = octaveGenerator.GetSizeX();
  auto sizeZ = octaveGenerator.GetSizeZ();

  auto surfaceNoise = octaveGenerator.GetFractalBrownianMotion(cx, 0.0, cz, 0.5, 0.5);

  auto chunk = world.GetChunk(chunkX, chunkZ);

  int_fast32_t id;
  for (int_fast32_t x = 0; x < sizeX; ++x) {
    for (int_fast32_t z = 0; z < sizeZ; ++z) {
      id = biome.GetBiome(x, z);

      if (isUHC_ && (id == 0 || id == 6 || id == 10 || (id >= 21 && id <= 24) || (id >= 32 && id <= 33) || id == 134
          || id == 149 || id == 151 || id == 160 || id == 161)) {
        id = 132;
      }

      chunk->GetBiomeArray().Set(x, z, id);

      bool found = false;
      for (const auto &mappings : groundMap_) {
        auto biomes = mappings.first;
        if (std::find(biomes.begin(), biomes.end(), id) != biomes.end()) {
          mappings.second.GenerateTerrainColumn(world, random_, cx + x, cz + z, id, surfaceNoise[x | z << 4]);

          found = true;
          break;
        }
      }

      if (!found) {
        defaultGenerator_.GenerateTerrainColumn(world, random_, cx + x, cz + z, id, surfaceNoise[x | z << 4]);
      }
    }
  }
}

int_fast32_t OverworldGenerator::ElevationWeightHash(int_fast32_t x, int_fast32_t z) {
  return (x << 3) | z;
}

int_fast32_t OverworldGenerator::DensityHash(int_fast32_t i, int_fast32_t j, int_fast32_t k) {
  return (k << 6) | (j << 3) | i;
}

TerrainDensity OverworldGenerator::GenerateTerrainDensity(int_fast32_t x, int_fast32_t z) {
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
  auto biomeGrid = mapLayer_.lowResolution->GenerateValues(x, z, 10, 10);

  auto heightNoise = octaves_.height.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughnessNoise = octaves_.roughness.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughnessNoise2 = octaves_.roughness2.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto detailNoise = octaves_.detail.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);

  int_fast32_t index = 0;
  int_fast32_t indexHeight = 0;

  for (int_fast32_t i = 0; i < 5; ++i) {
    for (int_fast32_t j = 0; j < 5; ++j) {
      double avgHeightScale = 0.0;
      double avgHeightBase = 0.0;
      double totalWeight = 0.0;

      int_fast32_t biome = biomeGrid[i + 2 + (j + 2) * 10];
      BiomeHeightManager biomeHeight = BiomeHeightManager::Get(biome);
      // Sampling an average height base and scale by visiting the neighborhood
      // of the current biomegrid column.
      for (int_fast32_t m = 0; m < 5; ++m) {
        for (int_fast32_t n = 0; n < 5; ++n) {
          int_fast32_t nearBiome = biomeGrid[i + m + (j + n) * 10];
          BiomeHeightManager nearBiomeHeight = BiomeHeightManager::Get(nearBiome);

          double heightBase = BIOME_HEIGHT_OFFSET + nearBiomeHeight.height * BIOME_HEIGHT_WEIGHT;
          double heightScale = BIOME_SCALE_OFFSET + nearBiomeHeight.scale * BIOME_SCALE_WEIGHT;

          double weight = elevationWeight_[ElevationWeightHash(m, n)] / (heightBase + 2.0);
          if (nearBiomeHeight.height > biomeHeight.height) {
            weight *= 0.5;
          }

          avgHeightScale += heightScale * weight;
          avgHeightBase += heightBase * weight;
          totalWeight += weight;
        }
      }

      avgHeightScale /= totalWeight;
      avgHeightBase /= totalWeight;
      avgHeightScale = avgHeightScale * 0.9 + 0.1;
      avgHeightBase = (avgHeightBase * 4.0 - 1.0) / 8.0;

      double noiseH = heightNoise[indexHeight++] / 8000.0;
      if (noiseH < 0) {
        noiseH = -noiseH * 0.3;
      }

      noiseH = noiseH * 3.0 - 2.0;
      if (noiseH < 0) {
        noiseH = fmax(noiseH * 0.5, -1.0) / 1.4 * 0.5;
      } else {
        noiseH = fmin(noiseH, 1.0) / 8.0;
      }

      noiseH = (noiseH * 0.2 + avgHeightBase) * BASE_SIZE / 8.0 * 4.0 + BASE_SIZE;
      for (int_fast32_t k = 0; k < 33; ++k) {
        // density should be lower and lower as we climb up, this gets a height value to
        // subtract from the noise.
        double nh = (k - noiseH) * STRETCH_Y * 128.0 / 256.0 / avgHeightScale;
        if (nh < 0.0) {
          nh *= 4.0;
        }

        double noiseR = roughnessNoise[index] / 512.0;
        double noiseR2 = roughnessNoise2[index] / 512.0;
        double noiseD = (detailNoise[index] / 10.0 + 1.0) / 2.0;

        // linear interpolation
        double dens = noiseD < 0 ? noiseR : (noiseD > 1 ? noiseR2 : noiseR + (noiseR2 - noiseR) * noiseD);
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

void OverworldGenerator::GenerateRawTerrain(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  auto density = GenerateTerrainDensity(chunkX, chunkZ);

  int_fast32_t seaLevel = 64;

  // Terrain densities are sampled at different resolutions (1/4x on x,z and 1/8x on y by default)
  // so it's needed to re-scale it. Linear interpolation is used to fill in the gaps.

  int_fast32_t fill = abs(DENSITY_FILL_MODE);
  int_fast32_t seaFill = DENSITY_FILL_SEA_MODE;
  double densityOffset = DENSITY_FILL_OFFSET;

  auto stillWater = STILL_WATER.GetFullId();
  auto water = WATER.GetFullId();
  auto stone = STONE.GetFullId();

  auto chunk = world.GetChunk(chunkX, chunkZ);
  for (int_fast32_t i = 0; i < 5 - 1; ++i) {
    for (int_fast32_t j = 0; j < 5 - 1; ++j) {
      for (int_fast32_t k = 0; k < 33 - 1; ++k) {
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

        for (int_fast32_t l = 0; l < 8; ++l) {
          double d9 = d1;
          double d10 = d3;

          int_fast32_t yPos = l + (k << 3);
          int_fast32_t yBlockPos = yPos & 0xf;
          NormalBlockArrayContainer *subChunk = chunk->GetSubChunk(yPos >> 4);
          for (int_fast32_t m = 0; m < 4; ++m) {
            double dens = d9;
            for (int_fast32_t n = 0; n < 4; ++n) {
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
                subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), water);
              } else if (fill == 2 || fill == 9 || fill == 12 || fill == 15) {
                subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), stone);
              }

              if ((dens > densityOffset && fill > -1) || (dens <= densityOffset && fill < 0)) {
                if (fill == 0 || fill == 3 || fill == 6 || fill == 9 || fill == 12) {
                  subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), stone);
                } else if (fill == 2 || fill == 7 || fill == 10 || fill == 16) {
                  subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), stillWater);
                }
              } else if ((yPos < seaLevel - 1 && seaFill == 0) || (yPos >= seaLevel - 1 && seaFill == 1)) {
                if (fill == 0 || fill == 3 || fill == 7 || fill == 10 || fill == 13) {
                  subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), stillWater);
                } else if (fill == 1 || fill == 6 || fill == 9 || fill == 15) {
                  subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), stone);
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