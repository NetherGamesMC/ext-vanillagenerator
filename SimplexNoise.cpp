#ifndef PHP_CPP_INCL
#define PHP_CPP_INCL
#include <phpcpp.h>
#endif

#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED
#include "FastNoiseLite.h"
#endif

#include <chrono>
#include <iostream>
class SimplexNoise : public Php::Base
{

private:
  Php::Array noiseData;
  FastNoiseLite noise;
  unsigned int _lastRuntime = 0;

public:
  SimplexNoise() = default;
  virtual ~SimplexNoise() = default;

  void free() {
    delete noiseData;
  }

  void initialize(int seed = 1137){
     noise = &FastNoiseLite(seed);
     noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  }

  void setFrequency(float frequency){
    noise.setFrequency(frequency);
  }
  
  void setSeed(int seed){
    noise.setSeed(seed);
  }

  void setFractalType(int type){
    auto fType;
    switch(type){
      case 0:
	fType = FractalType::FractalType_None;
      break;
      case 1:

  Php::Value generate(Php::Parameters &params){
    auto tempTime = std::chrono::high_resolution_clock::now();
    int x = params[0];
    int y = params[1];
    int indx= 0;
    for(int yx = 0; yx < y; yx++){
      for(int xx = 0; xx < x; xx++) {
        noiseData[indx++] = noise.GetNoise((float) yx, (float) xx);
      }
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto time = now - tempTime;
    Php::out << std::chrono::duration_cast<std::chrono::nanoseconds>(time).count() << std::endl;
    return noiseData;
  }

};
