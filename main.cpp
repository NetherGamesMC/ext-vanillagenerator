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

#include "Noise.cpp"

#endif

extern "C" {
PHPCPP_EXPORT void *get_module() {
    static Php::Extension extension("noisegen", "1.0");
    Php::Class <Noise> noise("Noise");
    noise.method<&Noise::initialize>("initialize", {
            Php::ByVal("seed", Php::Type::Numeric)
    });
    noise.method<&Noise::generate2d>("generate2d", {
            Php::ByVal("x", Php::Type::Numeric),
            Php::ByVal("y", Php::Type::Numeric),
            Php::ByVal("x_offset", Php::Type::Numeric),
            Php::ByVal("z_offset", Php::Type::Numeric),
    });

    noise.method<&Noise::generate3d>("generate3d", {
        Php::ByVal("x", Php::Type::Numeric),
        Php::ByVal("y", Php::Type::Numeric),
        Php::ByVal("z", Php::Type::Numeric),
        Php::ByVal("x_offset", Php::Type::Numeric),
        Php::ByVal("y_offset", Php::Type::Numeric),
        Php::ByVal("z_offset", Php::Type::Numeric),
    });

    noise.method<&Noise::getNoise2d>("getNoise2d", {
        Php::ByVal("x", Php::Type::Numeric),
        Php::ByVal("z", Php::Type::Numeric)
    });

    noise.method<&Noise::getNoise3d>("getNoise3d", {
        Php::ByVal("x", Php::Type::Numeric),
        Php::ByVal("y", Php::Type::Numeric),
        Php::ByVal("z", Php::Type::Numeric),
    })
    
    noise.method<&Noise::setCellularDistanceFunction>("setCellularDistanceFunction", {
            Php::ByVal("distanceFunction", Php::Type::Float)
    });
    noise.method<&Noise::setFractalWeightedStrength>("setFractalWeightedStrength", {
            Php::ByVal("strength", Php::Type::Float)
    });
    noise.method<&Noise::setFractalPingPongStrength>("setFractalPingPongStrength", {
            Php::ByVal("strength", Php::Type::Float)
    });
    noise.method<&Noise::setFractalLacunarity>("setFractalLacunarity", {
            Php::ByVal("lacunarity", Php::Type::Float)
    });
    noise.method<&Noise::setCellularJitter>("setCellularJitter", {
            Php::ByVal("jitter", Php::Type::Float)
    });
    noise.method<&Noise::setDomainWarpAmp>("setDomainWarpAmp", {
            Php::ByVal("warpAmp", Php::Type::Float)
    });
    noise.method<&Noise::setFractalGain>("setFractalGain", {
            Php::ByVal("gain", Php::Type::Float)
    });
    noise.method<&Noise::setFractalOctaves>("setFractalOctaves", {
            Php::ByVal("octaves", Php::Type::Numeric)
    });
    noise.method<&Noise::setSeed>("setSeed", {
            Php::ByVal("seed", Php::Type::Numeric)
    });
    noise.method<&Noise::setFrequency>("setFrequency", {
            Php::ByVal("frequency", Php::Type::Float)
    });
    noise.method<&Noise::setFractalType>("setFractalType", {
            Php::ByVal("type", Php::Type::Numeric)
    });
    noise.method<&Noise::setCellularReturnType>("setCellularReturnType", {
            Php::ByVal("returnType", Php::Type::Numeric)
    });
    noise.method<&Noise::setDomainWarpType>("setDomainWarpType", {
            Php::ByVal("warpType", Php::Type::Numeric)
    });
    noise.method<&Noise::setRotationType3D>("setRotationType3D", {
            Php::ByVal("rotationType", Php::Type::Numeric)
    });
    noise.method<&Noise::setNoiseType>("setNoiseType", {
            Php::ByVal("noiseType", Php::Type::Numeric)
    });

    // Constant registration

    // NoiseTypes
    extension.add(Php::Constant("NOISE_TYPE_OPENSIMPLEX2", 0));
    extension.add(Php::Constant("NOISE_TYPE_OPENSIMPLEX2S", 1));
    extension.add(Php::Constant("NOISE_TYPE_CELLULAR", 2));
    extension.add(Php::Constant("NOISE_TYPE_PERLIN", 3));
    extension.add(Php::Constant("NOISE_TYPE_VALUE_CUBIC", 4));
    extension.add(Php::Constant("NOISE_TYPE_VALUE", 5));

    //  Rotation3DTypes
    extension.add(Php::Constant("ROTATION3D_TYPE_NONE", 0));
    extension.add(Php::Constant("ROTATION3D_TYPE_IMPROVE_XY", 1));
    extension.add(Php::Constant("ROTATION3D_TYPE_IMPROVE_XZ", 2));

    // DomainWarpTypes
    extension.add(Php::Constant("WARP_TYPE_OPENSIMPLEX2", 0));
    extension.add(Php::Constant("WARP_TYPE_OPENSIMPLEX2REDUCED", 1));
    extension.add(Php::Constant("WARP_TYPE_BASICGRID", 2));

    // CellularReturnTypes
    extension.add(Php::Constant("CELLULAR_RETURN_CELLVALUE", 0));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE", 1));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE2", 2));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE2ADD", 3));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE2SUB", 4));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE2MUL", 5));
    extension.add(Php::Constant("CELLULAR_RETURN_DISTANCE2DIV", 6));

    // CellularDistanceFunction
    extension.add(Php::Constant("CELLULAR_FUNCTION_EUCLIDEAN", 0));
    extension.add(Php::Constant("CELLULAR_FUNCTION_EUCLIDEANSQ", 1));
    extension.add(Php::Constant("CELLULAR_FUNCTION_MANHATTAN", 2));
    extension.add(Php::Constant("CELLULAR_FUNCTION_HYBRID", 3));

    // FractalTypes
    extension.add(Php::Constant("FRACTAL_TYPE_NONE", 0));
    extension.add(Php::Constant("FRACTAL_TYPE_FBM", 1));
    extension.add(Php::Constant("FRACTAL_TYPE_RIDGED", 2));
    extension.add(Php::Constant("FRACTAL_TYPE_PINGPONG", 3));
    extension.add(Php::Constant("FRACTAL_TYPE_DOMAINWARP_PROGRESSIVE", 4));
    extension.add(Php::Constant("FRACTAL_TYPE_DOMAINWARP_INDEPENDENT", 5));

    extension.add(std::move(noise));
    return extension;
}
}


