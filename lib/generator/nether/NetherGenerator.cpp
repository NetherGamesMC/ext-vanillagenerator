#include <lib/objects/constants/BiomeList.h>
#include "NetherGenerator.h"

#define COORDINATE_SCALE 684.412
#define HEIGHT_SCALE 2053.236
#define HEIGHT_NOISE_SCALE_X 100.0
#define HEIGHT_NOISE_SCALE_Z 100.0
#define DETAIL_NOISE_SCALE_X 80.0
#define DETAIL_NOISE_SCALE_Y 60.0
#define DETAIL_NOISE_SCALE_Z 80.0
#define SURFACE_SCALE 0.0625

NetherGenerator::NetherGenerator(int_fast64_t seed)
    : random_(seed), octaveRandom_(seed), octaves_(
    {
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 1, 5),
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 17, 5),
        PerlinOctaveGenerator(octaveRandom_, 16, 5, 17, 5),
        PerlinOctaveGenerator(octaveRandom_, 8, 5, 17, 5),
        SimplexOctaveGenerator(octaveRandom_, 4, 16, 16, 1),
        PerlinOctaveGenerator(octaveRandom_, 4, 16, 16, 1),
        PerlinOctaveGenerator(octaveRandom_, 4, 16, 1, 16)
    }) {

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

  octaves_.soulSand.SetXScale(SURFACE_SCALE / 2.0);
  octaves_.soulSand.SetYScale(SURFACE_SCALE / 2.0);

  octaves_.gravel.SetXScale(SURFACE_SCALE / 2.0);
  octaves_.gravel.SetZScale(SURFACE_SCALE / 2.0);
}

void NetherGenerator::GenerateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  GenerateChunkData(world, chunkX, chunkZ);
}

void NetherGenerator::PopulateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  populator.Populate(world, random_, chunkX, chunkZ);
}

int_fast32_t NetherGenerator::DensityHash(int_fast32_t i, int_fast32_t j, int_fast32_t k) {
  return (k << 6) | (j << 3) | i;
}

void NetherGenerator::GenerateChunkData(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  GenerateRawTerrain(world, chunkX, chunkZ);

  int_fast32_t cx = chunkX << 4;
  int_fast32_t cz = chunkZ << 4;

  auto surface = octaves_.surface.GetFractalBrownianMotion(cx, 0.0, cz, 0.5, 2.0);
  auto soulSand = octaves_.soulSand.GetFractalBrownianMotion(cx, 0.0, cz, 0.5, 2.0);
  auto gravel = octaves_.gravel.GetFractalBrownianMotion(cx, 0.0, 0.0, cz, 2.0);
  Chunk *chunk = world.GetChunk(chunkX, chunkZ);

  for (int_fast32_t x = 0; x < 16; ++x) {
    for (int_fast32_t z = 0; z < 16; ++z) {
      chunk->GetBiomeArray().Set(x, z, HELL);
      GenerateTerrainColumn(world, cx + x, cz + z, surface[x | z << 4], soulSand[x | z << 4], gravel[x | z << 4]);
    }
  }
}

void NetherGenerator::GenerateRawTerrain(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ) {
  auto density = GenerateTerrainDensity(chunkX << 2, chunkZ << 2);

  Chunk *chunk = world.GetChunk(chunkX, chunkZ);
  for (int i = 0; i < 5 - 1; i++) {
    for (int j = 0; j < 5 - 1; j++) {
      for (int k = 0; k < 17 - 1; k++) {
        double d1 = density[DensityHash(i, j, k)];
        double d2 = density[DensityHash(i + 1, j, k)];
        double d3 = density[DensityHash(i, j + 1, k)];
        double d4 = density[DensityHash(i + 1, j + 1, k)];
        double d5 = (density[DensityHash(i, j, k + 1)] - d1) / 8;
        double d6 = (density[DensityHash(i + 1, j, k + 1)] - d2) / 8;
        double d7 = (density[DensityHash(i, j + 1, k + 1)] - d3) / 8;
        double d8 = (density[DensityHash(i + 1, j + 1, k + 1)] - d4) / 8;

        for (int l = 0; l < 8; l++) {
          double d9 = d1;
          double d10 = d3;
          int_fast32_t yPos = l + (k << 3);
          int_fast32_t yBlockPos = yPos & 0xf;
          NormalBlockArrayContainer *subChunk = chunk->GetSubChunk(yPos >> 4);

          for (int m = 0; m < 4; m++) {
            double dens = d9;
            for (int n = 0; n < 4; n++) {
              // any density higher than 0 is ground, any density lower or equal
              // to 0 is air (or lava if under the lava level).
              if (dens > 0) {
                subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), NETHERRACK.GetFullId());
              } else if (l + (k << 3) < 32) {
                subChunk->set(m + (i << 2), yBlockPos, n + (j << 2), STILL_LAVA.GetFullId());
              }
              // interpolation along z
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

void NetherGenerator::GenerateTerrainColumn(ChunkManager &world,
                                            int_fast32_t x,
                                            int_fast32_t z,
                                            double surfaceNoise,
                                            double soulSandNoise,
                                            double gravelNoise) {
  MinecraftBlock topMat = NETHERRACK;
  MinecraftBlock groundMat = NETHERRACK;

  bool soulSand = soulSandNoise + random_.NextFloat() * 0.2 > 0;
  bool gravel = gravelNoise + random_.NextFloat() * 0.2 > 0;

  int surfaceHeight = (int) (surfaceNoise / 3.0 + 3.0 + random_.NextFloat() * 0.25);
  int deep = -1;
  Chunk *chunk = world.GetChunk(x >> 4, z >> 4);

  auto chunkBlockX = static_cast<int_fast8_t>(x & 0x0f);
  auto chunkBlockZ = static_cast<int_fast8_t>(z & 0x0f);

  for (int_fast16_t y = 127; y >= 0; y--) {
    if (y <= random_.NextInt(5) || y >= 127 - random_.NextInt(5)) {
      chunk->SetFullBlock(chunkBlockX, y, chunkBlockZ, BEDROCK.GetFullId());
      continue;
    }

    MinecraftBlock mat = chunk->GetFullBlock(chunkBlockX, y, chunkBlockZ);
    if (mat == AIR.GetFullId()) {
      deep = -1;
    } else if (mat == NETHERRACK.GetFullId()) {
      if (deep == -1) {
        if (surfaceHeight <= 0) {
          topMat = AIR.GetFullId();
          groundMat = NETHERRACK.GetFullId();
        } else if (y >= 60 && y <= 65) {
          topMat = NETHERRACK.GetFullId();
          groundMat = NETHERRACK.GetFullId();
          if (gravel) {
            topMat = GRAVEL.GetFullId();
            groundMat = NETHERRACK.GetFullId();
          }
          if (soulSand) {
            topMat = SOUL_SAND.GetFullId();
            groundMat = SOUL_SAND.GetFullId();
          }
        }

        deep = surfaceHeight;
        if (y >= 63) {
          chunk->SetFullBlock(chunkBlockX, y, chunkBlockZ, topMat.GetFullId());
        } else {
          chunk->SetFullBlock(chunkBlockX, y, chunkBlockZ, groundMat.GetFullId());
        }
      } else if (deep > 0) {
        deep--;
        chunk->SetFullBlock(chunkBlockX, y, chunkBlockZ, groundMat.GetFullId());
      }
    }
  }
}

TerrainDensity NetherGenerator::GenerateTerrainDensity(int_fast32_t x, int_fast32_t z) {
  TerrainDensity density;

  auto heightNoise = octaves_.height.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughnessNoise = octaves_.roughness.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto roughnessNoise2 = octaves_.roughness2.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);
  auto detailNoise = octaves_.detail.GetFractalBrownianMotion(x, 0, z, 0.5, 2.0);

  double nv[17];
  for (int i = 0; i < 17; i++) {
    nv[i] = cos(i * M_PI * 6.0 / 17.0) * 2.0;
    double nh = i > 17 / 2 ? 17 - 1 - i : i;
    if (nh < 4.0) {
      nh = 4.0 - nh;
      nv[i] -= nh * nh * nh * 10.0;
    }
  }

  int index = 0;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 17; k++) {
        double noiseR = roughnessNoise[index] / 512.0;
        double noiseR2 = roughnessNoise2[index] / 512.0;
        double noiseD = (detailNoise[index] / 10.0 + 1.0) / 2.0;
        double nh = nv[k];
        // linear interpolation
        double dens = noiseD < 0 ? noiseR : noiseD > 1 ? noiseR2 : noiseR + (noiseR2 - noiseR) * noiseD;
        dens -= nh;
        index++;
        if (k > 13) {
          double lowering = (k - 13) / 3.0;
          dens = dens * (1.0 - lowering) + lowering * -10.0;
        }
        density[DensityHash(i, j, k)] = dens;
      }
    }
  }

  return density;
}
