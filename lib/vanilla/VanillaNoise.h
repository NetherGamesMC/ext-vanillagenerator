#ifndef VANILLA_NOISE_EXT
#define VANILLA_NOISE_EXT

#include <vector>
#include <lib/pocketmine/Random.h>

#define SQRT_3      1.7320508075688772
#define F2          0.5 * (SQRT_3 - 1)
#define G2          (3 - SQRT_3) / 6
#define G22         G2 * 2.0 - 1
#define F3          1.0 / 3.0
#define G3          1.0 / 6.0
#define G32         G3 * 2.0
#define G33         G3 * 3.0 - 1.0

class Utils {
public:
    static int floor(double x);

    static double lerp(double delta, double start, double end);

    static double grad(int i, double d, double d1, double d2);

    static double fade(double x);

    static int floorSimplex(double x);

    static long floorLong(double x);

    static double dot(const int8_t g[3], double x, double y);

    static double dot(const int8_t g[3], double x, double y, double z);
};

class PerlinNoise {
public:
    explicit PerlinNoise(Random &random);

    double *getNoise(double *noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                     double scaleX, double scaleY, double scaleZ, double amplitude);

protected:
    uint8_t permutations[512] = {};

    double offsetX;
    double offsetY;
    double offsetZ;

    virtual double *get2dNoise(double *noise, double x, double z, int sizeX, int sizeZ,
                               double scaleX, double scaleZ, double amplitude);

    virtual double *get3dNoise(double *noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                               double scaleX, double scaleY, double scaleZ, double amplitude);
};

class ScalableOctaves {
public:
    void setScale(double scale);

    double getXScale() const;

    double getYScale() const;

    double getZScale() const;

    void setXScale(double scaleX);

    void setYScale(double scaleY);

    void setZScale(double scaleZ);

protected:
    double xScale = 1.0;
    double yScale = 1.0;
    double zScale = 1.0;
};

class PerlinOctaveGenerator : public ScalableOctaves {
public:
    explicit PerlinOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv);

    double *getFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

    int getArraySize() const;

    int getSizeY() const;

private:
    std::vector<PerlinNoise> perlinOctaves;

    int sizeX;
    int sizeY;
    int sizeZ;
};

class SimplexNoise : public PerlinNoise {
public:
    explicit SimplexNoise(Random &random);

    double noise3d(double d, double d1, double d2);

protected:
    int8_t grad3[12][3] = {
            {1,  1,  0},
            {-1, 1,  0},
            {1,  -1, 0},
            {-1, -1, 0},
            {1,  0,  1},
            {-1, 0,  1},
            {1,  0,  -1},
            {-1, 0,  -1},
            {0,  1,  1},
            {0,  -1, 1},
            {0,  1,  -1},
            {0,  -1, -1}
    };

    uint8_t permMod12[512] = {};

    double *get2dNoise(double *noise, double x, double z, int sizeX, int sizeZ,
                       double scaleX, double scaleZ, double amplitude) override;

    double *get3dNoise(double *noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ, double scaleX,
                       double scaleY, double scaleZ, double amplitude) override;

private:
    double simplex2D(double xin, double yin);

    double simplex3D(double xin, double yin, double zin);
};

// THIS IMPLEMENTATION IS THE SAME AS SIMPLEX NOISE

class BukkitSimplexNoiseGenerator {
public:

    explicit BukkitSimplexNoiseGenerator(Random &random);

    double simplex2D(double xin, double yin);

private:
    int8_t grad3[12][3] = {
            {1,  1,  0},
            {-1, 1,  0},
            {1,  -1, 0},
            {-1, -1, 0},
            {1,  0,  1},
            {-1, 0,  1},
            {1,  0,  -1},
            {-1, 0,  -1},
            {0,  1,  1},
            {0,  -1, 1},
            {0,  1,  -1},
            {0,  -1, -1}
    };

    uint8_t permutations[512] = {};

    double offsetX;
    double offsetY;
};

class BukkitSimplexOctaveGenerator {
public:
    explicit BukkitSimplexOctaveGenerator(Random &random, int octaves);

    double noise(double x, double y, double frequency, double amplitude, bool normalized);

private:
    std::vector<BukkitSimplexNoiseGenerator> simplexOctaves;
};

// THIS IMPLEMENTATION IS THE SAME AS SIMPLEX NOISE

class SimplexOctaveGenerator : public ScalableOctaves {
public:
    explicit SimplexOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv);

    double *getFractalBrownianMotion(double x, double y, double z, double lacunarity, double persistence);

    int getSizeX() const;

    int getSizeZ() const;

    int getArraySize() const;

    double noise(double x, double y, double z, double frequency, double amplitude, bool normalized = false);

private:
    std::vector<SimplexNoise> simplexOctaves;

    int sizeX;
    int sizeY;
    int sizeZ;
};

#endif
