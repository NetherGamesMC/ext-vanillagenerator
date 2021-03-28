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
PHPCPP_EXPORT void *get_module() {
    static Php::Extension extension("noisegen", "1.0");
    Php::Class <SimplexNoise> noise("SimplexNoise");
    noise.method<&SimplexNoise::initialize>("initialize", {
        Php::ByVal("seed", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::free>("free");
    noise.method<&SimplexNoise::generate>("generate");
    noise.method<&SimplexNoise::setCellularDistanceFunction>("setCellularDistanceFunction", {
        Php::ByVal("distanceFunction", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalWeightedStrength>("setFractalWeightedStrength", {
        Php::ByVal("strength", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalPingPongStrength>("setFractalPingPongStrength", {
        Php::ByVal("strength", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalLacunarity>("setFractalLacunarity", {
        Php::ByVal("lacunarity", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setCellularJitter>("setCellularJitter", {
            Php::ByVal("jitter", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setDomainWarpAmp>("setDomainWarpAmp", {
            Php::ByVal("warpAmp", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalGain>("setFractalGain", {
            Php::ByVal("gain", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalOctaves>("setFractalOctaves", {
            Php::ByVal("octaves", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setSeed>("setSeed", {
            Php::ByVal("seed", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setFrequency>("setFrequency", {
            Php::ByVal("frequency", Php::Type::Float)
    });
    noise.method<&SimplexNoise::setFractalType>("setFractalType", {
            Php::ByVal("type", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setCellularReturnType>("setCellularReturnType", {
            Php::ByVal("returnType", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setDomainWarpType>("setDomainWarpType", {
            Php::ByVal("warpType", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setRotationType3D>("setRotationType3D", {
            Php::ByVal("rotationType", Php::Type::Numeric)
    });
    noise.method<&SimplexNoise::setNoiseType>("setNoiseType", {
            Php::ByVal("noiseType", Php::Type::Numeric)
    });
    extension.add(std::move(noise));
    return extension;
}
}


