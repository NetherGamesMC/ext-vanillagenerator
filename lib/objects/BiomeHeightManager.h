#ifndef EXT_NOISELIB_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_
#define EXT_NOISELIB_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_

struct BiomeHeight {
  double height = 0.0;
  double scale = 0.0;
};

namespace BiomeHeightManager {

BiomeHeight get(int biome);

void init();

}

#endif //EXT_NOISELIB_LIB_OBJECTS_BIOMEHEIGHTMANAGER_H_
