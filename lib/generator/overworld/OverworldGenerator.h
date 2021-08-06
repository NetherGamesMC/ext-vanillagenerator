#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_

#include <lib/noise/octaves/PerlinOctaveGenerator.h>
#include <lib/noise/octaves/SimplexOctaveGenerator.h>
#include <lib/chunk/ChunkManager.h>
#include <lib/generator/populators/Populator.h>
#include <lib/objects/VanillaBiomeGrid.h>
#include <lib/generator/ground/GroundGenerator.h>
#include "OverworldPopulators.h"

typedef std::map<int, double> TerrainDensity;

struct WorldOctaves {
  PerlinOctaveGenerator height;
  PerlinOctaveGenerator roughness;
  PerlinOctaveGenerator roughness2;
  PerlinOctaveGenerator detail;
  SimplexOctaveGenerator surface;
};

class OverworldGenerator {
 public:
  Random random_; // Will be used for terrain population.

  /**
   *  @brief The constructor for Overworld generator.
   *  @param seed     A pseudo-random number generator seed
   */
  explicit OverworldGenerator(int_fast64_t seed, bool isUHC);

  /**
   *  @brief Perform series of memory garbage collection.
   */
  ~OverworldGenerator();

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

  void GenerateChunkData(ChunkManager &world, int_fast32_t x, int_fast32_t z, const VanillaBiomeGrid &biome);
  void GenerateRawTerrain(ChunkManager &world, int_fast32_t x, int_fast32_t z);

  static int DensityHash(int i, int j, int k);
  static int ElevationWeightHash(int x, int z);

  bool isUHC_;

  GridBiome::MapLayerPair mapLayer_;

  Random octaveRandom_;  // This is used for octaves generation, used internally
  WorldOctaves octaves_;

  GroundGenerator defaultGenerator;
  OverworldPopulator populators;

  std::map<int, double> elevationWeight_;
  std::map<std::vector<int>, std::shared_ptr<GroundGenerator>> groundMap_;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OVERWORLDGENERATOR_H_
