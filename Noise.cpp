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
#include <string>

class Noise : public Php::Base {

private:
    FastNoiseLite noise;
    unsigned int _lastRuntime = 0;

public:
    Noise() = default;

    virtual ~Noise() = default;

    void initialize(Php::Parameters &params) {
        int seed = params[0];
        noise = FastNoiseLite(seed);
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    }

    Php::Value generate3d(Php::Parameters &params) {
        int x = params[0];
        int y = params[1];
        int z = params[2];
        int x_offset = params[3];
        int y_offset = params[4];
        int z_offset = params[5];
        Php::Array noiseData;
        int indx = 0;
        for (int xx = x_offset; xx < (x + x_offset); xx++) {
            for (int yx = y_offset; yx < (y + y_offset); yx++) {
                for (int zx = z_offset; zx < (z + z_offset); zx++) {
                    noiseData[indx] = noise.GetNoise((float) xx , (float) yx, (float) zx);
                }
            }
        }
        return noiseData;
    }

    Php::Value getNoise3d(Php::Parameters &params){
        int x = params[0];
        int y = params[1];
        int z = params[2];
        return noise.GetNoise((float) x, (float) y, (float) z);
    }

    Php::Value getNoise2d(Php::Parameters &params){
        int x = params[0];
        int z = params[1];
        return noise.GetNoise((float) x, (float) z);
    }

    Php::Value generate2d(Php::Parameters &params) {
        int x = params[0];
        int z = params[1];
        int x_offset = params[2];
        int z_offset = params[3];
        Php::Array noiseData;
        int indx = 0;
        for (int xx = x_offset; xx < (x + x_offset); xx++) {
            for (int zx = z_offset; zx < (z + z_offset); zx++) {
                noiseData[indx++] = noise.GetNoise((float) xx, (float) zx);
            }
        }
        return noiseData;
    }

    void setNoiseType(Php::Parameters &params) {
        int type = params[0];
        FastNoiseLite::NoiseType fType;
        switch (type) {
            case 0:
                fType = FastNoiseLite::NoiseType_OpenSimplex2;
                break;
            case 1:
                fType = FastNoiseLite::NoiseType_OpenSimplex2S;
                break;
            case 2:
                fType = FastNoiseLite::NoiseType_Cellular;
                break;
            case 3:
                fType = FastNoiseLite::NoiseType_Perlin;
                break;
            case 4:
                fType = FastNoiseLite::NoiseType_ValueCubic;
                break;
            case 5:
                fType = FastNoiseLite::NoiseType_Value;
                break;
            default:
                std::string err = std::to_string(type);
                throw Php::Exception("Unknown NoiseType: " + err);
        }
        noise.SetNoiseType(fType);
    }

    void setRotationType3D(Php::Parameters &params) {
        int type = params[0];
        FastNoiseLite::RotationType3D fType;
        switch (type) {
            case 0:
                fType = FastNoiseLite::RotationType3D_None;
                break;
            case 1:
                fType = FastNoiseLite::RotationType3D_ImproveXYPlanes;
                break;
            case 2:
                fType = FastNoiseLite::RotationType3D_ImproveXZPlanes;
                break;
            default:
                std::string err = std::to_string(type);
                throw Php::Exception("Unknown RotationType3D: " + err);
        }
        noise.SetRotationType3D(fType);
    }

    void setDomainWarpType(Php::Parameters &params) {
        int type = params[0];
        FastNoiseLite::DomainWarpType fType;
        switch (type) {
            case 0:
                fType = FastNoiseLite::DomainWarpType_OpenSimplex2;
                break;
            case 1:
                fType = FastNoiseLite::DomainWarpType_OpenSimplex2Reduced;
                break;
            case 2:
                fType = FastNoiseLite::DomainWarpType_BasicGrid;
                break;
            default:
                std::string err = std::to_string(type);
                throw Php::Exception("Unknown DomainWarpType: " + err);
        }
        noise.SetDomainWarpType(fType);
    }

    void setCellularReturnType(Php::Parameters &params) {
        int type = params[0];
        FastNoiseLite::CellularReturnType fType;
        switch (type) {
            case 0:
                fType = FastNoiseLite::CellularReturnType::CellularReturnType_CellValue;
                break;
            case 1:
                fType = FastNoiseLite::CellularReturnType::CellularReturnType_Distance;
                break;
            case 2:
                fType = FastNoiseLite::CellularReturnType::CellularReturnType_Distance2;
                break;
            case 3:
                fType = FastNoiseLite::CellularReturnType_Distance2Add;
                break;
            case 4:
                fType = FastNoiseLite::CellularReturnType_Distance2Sub;
                break;
            case 5:
                fType = FastNoiseLite::CellularReturnType_Distance2Mul;
                break;
            case 6:
                fType = FastNoiseLite::CellularReturnType_Distance2Div;
                break;
            default:
                std::string err = std::to_string(type);
                throw Php::Exception("Unknown CellularReturnType: " + err);
        }
        noise.SetCellularReturnType(fType);
    }

    void setCellularDistanceFunction(Php::Parameters &params) {
        int function = params[0];
        FastNoiseLite::CellularDistanceFunction fType;
        switch (function) {
            case 0:
                fType = FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean;
                break;
            case 1:
                fType = FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_EuclideanSq;
                break;
            case 2:
                fType = FastNoiseLite::CellularDistanceFunction_Manhattan;
                break;
            case 3:
                fType = FastNoiseLite::CellularDistanceFunction_Hybrid;
                break;
            default:
                std::string err = std::to_string(function);
                throw Php::Exception("Unknown CellularDistanceFunction: " + err);
        }
        noise.SetCellularDistanceFunction(fType);
    }

    void setFractalType(Php::Parameters &params) {
        int type = params[0];
        FastNoiseLite::FractalType fType;
        switch (type) {
            case 0:
                fType = FastNoiseLite::FractalType::FractalType_None;
                break;
            case 1:
                fType = FastNoiseLite::FractalType::FractalType_FBm;
                break;
            case 2:
                fType = FastNoiseLite::FractalType::FractalType_Ridged;
                break;
            case 3:
                fType = FastNoiseLite::FractalType::FractalType_PingPong;
                break;
            case 4:
                fType = FastNoiseLite::FractalType::FractalType_DomainWarpProgressive;
                break;
            case 5:
                fType = FastNoiseLite::FractalType::FractalType_DomainWarpIndependent;
                break;
            default:
                std::string err = std::to_string(type);
                throw Php::Exception("Unknown FractalType: " + err);
        }
        noise.SetFractalType(fType);
    }

    void setFrequency(Php::Parameters &params) {
        double frequency = params[0];
        noise.SetFrequency(frequency);
    }

    void setSeed(Php::Parameters &params) {
        int seed = params[0];
        noise.SetSeed(seed);
    }

    void setFractalOctaves(Php::Parameters &params) {
        int octaves = params[0];
        noise.SetFractalOctaves(octaves);
    }

    void setFractalGain(Php::Parameters &params) {
        double gain = params[0];
        noise.SetFractalGain(gain);
    }

    void setFractalLacunarity(Php::Parameters &params) {
        double lacunarity = params[0];
        noise.SetFractalLacunarity(lacunarity);
    }

    void setFractalWeightedStrength(Php::Parameters &params) {
        double strength = params[0];
        noise.SetFractalWeightedStrength(strength);
    }

    void setFractalPingPongStrength(Php::Parameters &params) {
        double strength = params[0];
        noise.SetFractalPingPongStrength(strength);
    }

    void setCellularJitter(Php::Parameters &params) {
        double cellularJitter = params[0];
        noise.SetCellularJitter(cellularJitter);
    }

    void setDomainWarpAmp(Php::Parameters &params) {
        double warpAmp = params[0];
        noise.SetDomainWarpAmp(warpAmp);
    }
};
