#ifndef EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_
#define EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_

class BiomeHeightManager {
 public:
  BiomeHeightManager(double dHeight = 0.0, double dScale = 0.0) : height(dHeight), scale(dScale) {}

  double height;
  double scale;

  static std::map<uint_fast8_t, BiomeHeightManager> heights;
  static BiomeHeightManager defaultHeight;

  static void Init(bool isUHC);

  static BiomeHeightManager Get(int_fast32_t biome);

  static void RegisterBiome(BiomeHeightManager climate, const std::vector<uint_fast8_t> &biomeIds);
};

#endif //EXT_VANILLAGENERATOR_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_
