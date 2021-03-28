#include <phpcpp.h>
#include <string>
#include <phpcpp.h>
#include <iostream>
#ifndef PERLIN_INCLUDED
#define PERLIN_INCLUDED
#include "FastNoiseLite.h"
#endif

#ifndef NOISE_INC
#define NOISE_INC
#include "SimplexNoise.cpp"
#endif

extern "C" {
    PHPCPP_EXPORT void *get_module() 
    {
	static Php::Extension extension("noisegen", "1.0");
	Php::Class<SimplexNoise> noise("SimplexNoise");
	noise.method<&SimplexNoise::initialize>("initialize");
	noise.method<&SimplexNoise::free>("free");
	noise.method<&SimplexNoise::generate>("generate");
	extension.add(std::move(noise));
        return extension;
    }
}


