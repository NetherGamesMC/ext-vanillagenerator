dnl config.m4 for extension extnoise

PHP_ARG_ENABLE(mcgenerator, whether to enable Minecraft world generator support,
[  --enable-mcgenerator    Enable Minecraft world generator features])

if test "$PHP_MCGENERATOR" != "no"; then
  dnl probably won't use this, but will keep just in case
  PHP_REQUIRE_CXX()

  dnl idk 
  PHP_NEW_EXTENSION(mcgenerator, mcgenerator.cpp lib/biomes/BiomeArray.cpp lib/biomes/BiomeClimate.cpp lib/biomes/BiomeHeightManager.cpp lib/chunk/Chunk.cpp lib/chunk/ChunkManager.cpp lib/generator/biomegrid/BiomeEdgeMapLayer.cpp lib/generator/biomegrid/BiomeMapLayer.cpp lib/generator/biomegrid/BiomeThinEdgeMapLayer.cpp lib/generator/biomegrid/BiomeVariationMapLayer.cpp lib/generator/biomegrid/DeepOceanMapLayer.cpp lib/generator/biomegrid/ErosionMapLayer.cpp lib/generator/biomegrid/MapLayer.cpp lib/generator/biomegrid/NoiseMapLayer.cpp lib/generator/biomegrid/RarePlainsMapLayer.cpp lib/generator/biomegrid/RiverMapLayer.cpp lib/generator/biomegrid/ShoreMapLayer.cpp lib/generator/biomegrid/SmoothMapLayer.cpp lib/generator/biomegrid/WhittakerMapLayer.cpp lib/generator/biomegrid/ZoomMapLayer.cpp lib/generator/decorators/DeadBushDecorator.cpp lib/generator/decorators/Decorator.cpp lib/generator/decorators/DoublePlantDecorator.cpp lib/generator/decorators/FlowerDecorator.cpp lib/generator/decorators/LakeDecorator.cpp lib/generator/decorators/MushroomDecorator.cpp lib/generator/decorators/SugarCaneDecorator.cpp lib/generator/decorators/TallGrassDecorator.cpp lib/generator/decorators/TreeDecorator.cpp lib/generator/ground/DirtAndStonePatchGroundGenerator.cpp lib/generator/ground/DirtPatchGroundGenerator.cpp lib/generator/ground/GravelPatchGroundGenerator.cpp lib/generator/ground/GroundGenerator.cpp lib/generator/ground/MesaGroundGenerator.cpp lib/generator/ground/StonePatchGroundGenerator.cpp lib/generator/nether/NetherBiomePopulator.cpp lib/generator/nether/NetherPopulator.cpp lib/generator/nether/decorators/FireDecorator.cpp lib/generator/nether/decorators/GlowstoneDecorator.cpp lib/generator/nether/decorators/MushroomDecorator.cpp lib/generator/nether/populators/NetherOrePopulator.cpp lib/generator/objects/DoubleTallPlant.cpp lib/generator/objects/Flower.cpp lib/generator/objects/GenericTree.cpp lib/generator/objects/Lake.cpp lib/generator/objects/OreVein.cpp lib/generator/objects/SugarCane.cpp lib/generator/objects/TallGrass.cpp lib/generator/objects/TerrainObjects.cpp lib/generator/overworld/OverworldGenerator.cpp lib/generator/overworld/OverworldPopulators.cpp lib/generator/populators/BiomePopulator.cpp lib/generator/populators/OrePopulator.cpp lib/noise/BukkitSimplexNoiseGenerator.cpp lib/noise/octaves/BukkitSimplexOctaveGenerator.cpp lib/noise/octaves/PerlinOctaveGenerator.cpp lib/noise/octaves/ScalableOctaves.cpp lib/noise/octaves/SimplexOctaveGenerator.cpp lib/noise/PerlinNoise.cpp lib/noise/SimplexNoise.cpp lib/objects/math/Math.cpp lib/objects/MinecraftBlock.cpp lib/objects/random/Random.cpp lib/objects/VanillaBiomeGrid.cpp src/Noise.cpp src/OverworldGeneratorImpl.cpp src/RandomImpl.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++14 -fno-strict-aliasing -DGSL_THROW_ON_CONTRACT_VIOLATION=1, yes)
  PHP_SUBST(MCGENERATOR_SHARED_LIBADD)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
  PHP_ADD_INCLUDE(ext/chunkutils2/gsl/include)
  PHP_ADD_INCLUDE(ext/chunkutils2)
  PHP_ADD_INCLUDE(ext/chunkutils2/src)
  PHP_ADD_INCLUDE(ext/morton/libmorton/libmorton/include)
fi