#ifndef EXT_VANILLAGENERATOR_NETHERGENERATOR_H
#define EXT_VANILLAGENERATOR_NETHERGENERATOR_H

#include <lib/generator/overworld/OverworldGenerator.h>
#include <lib/generator/nether/populators/NetherPopulator.h>

struct NetherWorldOctaves {
  PerlinOctaveGenerator height;
  PerlinOctaveGenerator roughness;
  PerlinOctaveGenerator roughness2;
  PerlinOctaveGenerator detail;
  SimplexOctaveGenerator surface;
  PerlinOctaveGenerator soulSand;
  PerlinOctaveGenerator gravel;
};

class NetherGenerator {
 public:
  explicit NetherGenerator(int_fast64_t seed);

  /**
   *  @brief Terrain generation of 1 chunk area
   *  @param world    The world chunk manager class
   *  @param chunk_x  The chunk x coordinates
   *  @param chunk_z  The chunk z coordinates
   *
   *  This will generate a terrain of 16x16 blocks wide, this function should only
   *  generate its terrain within chunk coordinates radius.
   */
  void GenerateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ);

  /**
   *  @brief Terrain population of 3x3 chunks wide area
   *  @param world    The world chunk manager class
   *  @param chunk_x  The chunk x coordinates
   *  @param chunk_z  The chunk z coordinates
   *
   *  This will try to populate chunk for the given coordinates. It can however
   *  generate blocks outside the given chunk coordinates bound but no further than
   *  1 neighbouring chunk.
   */
  void PopulateChunk(ChunkManager &world, int_fast32_t chunkX, int_fast32_t chunkZ);
 private:
  TerrainDensity GenerateTerrainDensity(int_fast32_t x, int_fast32_t z);

  void GenerateChunkData(ChunkManager &world, int_fast32_t x, int_fast32_t z);
  void GenerateRawTerrain(ChunkManager &world, int_fast32_t x, int_fast32_t z);
  void GenerateTerrainColumn(ChunkManager &world, int_fast32_t x, int_fast32_t z, double surfaceNoise, double soulSandNoise, double gravelNoise);

  static int_fast32_t DensityHash(int_fast32_t i, int_fast32_t j, int_fast32_t k);

  Random random_;
  Random octaveRandom_;
  NetherWorldOctaves octaves_;
  NetherPopulator populator;
};

#endif //EXT_VANILLAGENERATOR_NETHERGENERATOR_H
