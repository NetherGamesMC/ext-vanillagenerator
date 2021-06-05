#include <php.h>
#include "VanillaNoise.h"


int Utils::floor(double x) {
    int floored = (int) x;
    return x < floored ? floored - 1 : floored;
}

double Utils::lerp(double delta, double start, double end) {
    return start + delta * (end - start);
}

double Utils::grad(int hash, double x, double y, double z) {
    hash &= 15;
    double u = hash < 8 ? x : y;
    double v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;
    return ((hash & 1) == 0 ? u : -u) + ((hash & 2) == 0 ? v : -v);
}

double Utils::fade(double x) {
    return x * x * x * (x * (x * 6 - 15) + 10);
}

int Utils::floorSimplex(double x) {
    return x > 0 ? (int) x : (int) x - 1;
}

double Utils::dot(const int8_t g[3], double x, double y) {
    return g[0] * x + g[1] * y;
}

double Utils::dot(const int8_t g[3], double x, double y, double z) {
    return g[0] * x + g[1] * y + g[2] * z;
}

long Utils::floorLong(double x) {
    return x >= 0 ? (long) x : (long) x - 1;
}

//
// ScalableOctaves
//

void ScalableOctaves::setScale(double scale) {
    setXScale(scale);
    setYScale(scale);
    setZScale(scale);
}

double ScalableOctaves::getXScale() const {
    return xScale;
}

double ScalableOctaves::getYScale() const {
    return yScale;
}

double ScalableOctaves::getZScale() const {
    return zScale;
}

void ScalableOctaves::setXScale(double scaleX) {
    this->xScale = scaleX;
}

void ScalableOctaves::setYScale(double scaleY) {
    this->yScale = scaleY;
}

void ScalableOctaves::setZScale(double scaleZ) {
    this->zScale = scaleZ;
}

//
// SimplexNoise
//

SimplexNoise::SimplexNoise(Random &random) : PerlinNoise(random) {
    for (int i = 0; i < 512; i++) {
        permMod12[i] = permutations[i] % 12;
    }
}

double *SimplexNoise::get2dNoise(double *noise, double x, double z, int sizeX, int sizeY,
                                 double scaleX, double scaleY, double amplitude) {
    int index = 0;
    for (int i = 0; i < sizeY; i++) {
        double zin = offsetY + (z + i) * scaleY;
        for (int j = 0; j < sizeX; j++) {
            double xin = offsetX + (x + j) * scaleX;
            noise[index++] += simplex2D(xin, zin) * amplitude;
        }
    }

    return noise;
}

double *SimplexNoise::get3dNoise(double *noise, double x, double y, double z, int sizeX, int sizeY, int sizeZ,
                                 double scaleX, double scaleY, double scaleZ, double amplitude) {
    int index = 0;
    for (int i = 0; i < sizeZ; i++) {
        double zin = offsetZ + (z + i) * scaleZ;
        for (int j = 0; j < sizeX; j++) {
            double xin = offsetX + (x + j) * scaleX;
            for (int k = 0; k < sizeY; k++) {
                double yin = offsetY + (y + k) * scaleY;
                noise[index++] += simplex3D(xin, yin, zin) * amplitude;
            }
        }
    }
    return noise;
}

double SimplexNoise::simplex2D(double xin, double yin) {
    // Skew the input space to determine which simplex cell we're in
    double s = (xin + yin) * F2; // Hairy factor for 2D
    int i = Utils::floorSimplex(xin + s);
    int j = Utils::floorSimplex(yin + s);
    double t = (i + j) * G2;
    double dx0 = i - t; // Unskew the cell origin back to (x,y) space
    double dy0 = j - t;
    double x0 = xin - dx0; // The x,y distances from the cell origin
    double y0 = yin - dy0;

    // For the 2D case, the simplex shape is an equilateral triangle.

    // Determine which simplex we are in.
    int i1; // Offsets for second (middle) corner of simplex in (i,j) coords
    int j1;
    if (x0 > y0) {
        i1 = 1; // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        j1 = 0;
    } else {
        i1 = 0; // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        j1 = 1;
    }

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 + G22; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 + G22;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permMod12[ii + permutations[jj]];
    int gi1 = permMod12[ii + i1 + permutations[jj + j1]];
    int gi2 = permMod12[ii + 1 + permutations[jj + 1]];

    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    double n0;
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * Utils::dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
    }

    double t1 = 0.5 - x1 * x1 - y1 * y1;
    double n1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * Utils::dot(grad3[gi1], x1, y1);
    }

    double t2 = 0.5 - x2 * x2 - y2 * y2;
    double n2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * Utils::dot(grad3[gi2], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (n0 + n1 + n2);
}

double SimplexNoise::simplex3D(double xin, double yin, double zin) {
// Skew the input space to determine which simplex cell we're in
    double s = (xin + yin + zin) * F3; // Very nice and simple skew factor for 3D
    int i = Utils::floorSimplex(xin + s);
    int j = Utils::floorSimplex(yin + s);
    int k = Utils::floorSimplex(zin + s);
    double t = (i + j + k) * G3;
    double dx0 = i - t; // Unskew the cell origin back to (x,y,z) space
    double dy0 = j - t;
    double dz0 = k - t;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.

    int i1; // Offsets for second corner of simplex in (i,j,k) coords
    int j1;
    int k1;
    int i2; // Offsets for third corner of simplex in (i,j,k) coords
    int j2;
    int k2;

    double x0 = xin - dx0; // The x,y,z distances from the cell origin
    double y0 = yin - dy0;
    double z0 = zin - dz0;
    // Determine which simplex we are in
    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = 1; // X Y Z order
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0;
        } else if (x0 >= z0) {
            i1 = 1; // X Z Y order
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        } else {
            i1 = 0; // Z X Y order
            j1 = 0;
            k1 = 1;
            i2 = 1;
            j2 = 0;
            k2 = 1;
        }
    } else { // x0<y0
        if (y0 < z0) {
            i1 = 0; // Z Y X order
            j1 = 0;
            k1 = 1;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        } else if (x0 < z0) {
            i1 = 0; // Y Z X order
            j1 = 1;
            k1 = 0;
            i2 = 0;
            j2 = 1;
            k2 = 1;
        } else {
            i1 = 0; // Y X Z order
            j1 = 1;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0;
        }
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    double x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    double y1 = y0 - j1 + G3;
    double z1 = z0 - k1 + G3;
    double x2 = x0 - i2 + G32; // Offsets for third corner in (x,y,z) coords
    double y2 = y0 - j2 + G32;
    double z2 = z0 - k2 + G32;

    // Work out the hashed gradient indices of the four simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = permMod12[ii + permutations[jj + permutations[kk]]];
    int gi1 = permMod12[ii + i1 + permutations[jj + j1 + permutations[kk + k1]]];
    int gi2 = permMod12[ii + i2 + permutations[jj + j2 + permutations[kk + k2]]];
    int gi3 = permMod12[ii + 1 + permutations[jj + 1 + permutations[kk + 1]]];

    // Calculate the contribution from the four corners
    double t0 = 0.5 - x0 * x0 - y0 * y0 - z0 * z0;
    double n0; // Noise contributions from the four corners
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * Utils::dot(grad3[gi0], x0, y0, z0);
    }

    double t1 = 0.5 - x1 * x1 - y1 * y1 - z1 * z1;
    double n1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * Utils::dot(grad3[gi1], x1, y1, z1);
    }

    double t2 = 0.5 - x2 * x2 - y2 * y2 - z2 * z2;
    double n2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * Utils::dot(grad3[gi2], x2, y2, z2);
    }

    double x3 = x0 + G33; // Offsets for last corner in (x,y,z) coords
    double y3 = y0 + G33;
    double z3 = z0 + G33;
    double t3 = 0.5 - x3 * x3 - y3 * y3 - z3 * z3;
    double n3;
    if (t3 < 0) {
        n3 = 0.0;
    } else {
        t3 *= t3;
        n3 = t3 * t3 * Utils::dot(grad3[gi3], x3, y3, z3);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 32.0 * (n0 + n1 + n2 + n3);
}

double SimplexNoise::noise3d(double xin, double yin, double zin) {
    xin += offsetX;
    yin += offsetZ;
    if (xin == 0.0) {
        return simplex2D(xin, yin);
    }

    zin += offsetZ;
    return simplex3D(xin, yin, zin);
}

//
// BukkitSimplexOctaveGenerator
//

BukkitSimplexOctaveGenerator::BukkitSimplexOctaveGenerator(Random &random, int octaves) {
    for (int i = 0; i < octaves; i++) {
        auto simplexNoise = BukkitSimplexNoiseGenerator(random);

        simplexOctaves.push_back(simplexNoise);
    }
}

double BukkitSimplexOctaveGenerator::noise(double x, double y, double frequency, double amplitude, bool normalized) {
    double result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

    for (BukkitSimplexNoiseGenerator octave : simplexOctaves) {
        auto value = octave.simplex2D(x * freq, y * freq) * amp;

        result += value;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }

    if (normalized) {
        result /= max;
    }

    return result;
}

//
// BukkitSimplexNoiseGenerator
//

BukkitSimplexNoiseGenerator::BukkitSimplexNoiseGenerator(Random &random) {
    offsetX = random.nextFloat() * 256;
    offsetY = random.nextFloat() * 256;
    random.nextSignedInt(); // offsetZ, generates the next random number

    for (int i = 0; i < 256; ++i) {
        permutations[i] = random.nextBoundedInt(256);
    }

    for (int i = 0; i < 256; ++i) {
        int pos = (int) random.nextBoundedInt(256 - i) + i;
        int old = permutations[i];

        permutations[i] = permutations[pos];
        permutations[pos] = old;
        permutations[i + 256] = permutations[i];
    }

    random.nextSignedInt(); // offsetW, generates the next random number
}

double BukkitSimplexNoiseGenerator::simplex2D(double xin, double yin) {
    xin += offsetX;
    yin += offsetY;

    double n0, n1, n2; // Noise contributions from the three corners

    // Skew the input space to determine which simplex cell we're in
    double s = (xin + yin) * F2; // Hairy factor for 2D
    int i = Utils::floorSimplex(xin + s);
    int j = Utils::floorSimplex(yin + s);
    double t = (i + j) * G2;
    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = xin - X0; // The x,y distances from the cell origin
    double y0 = yin - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.

    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0) {
        i1 = 1;
        j1 = 0;
    } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else {
        i1 = 0;
        j1 = 1;
    } // upper triangle, YX order: (0,0)->(0,1)->(1,1)

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 + G22; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 + G22;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permutations[ii + permutations[jj]] % 12;
    int gi1 = permutations[ii + i1 + permutations[jj + j1]] % 12;
    int gi2 = permutations[ii + 1 + permutations[jj + 1]] % 12;

    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * Utils::dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
    }

    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * Utils::dot(grad3[gi1], x1, y1);
    }

    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * Utils::dot(grad3[gi2], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (n0 + n1 + n2);
}

//
// PerlinNoise
//

PerlinNoise::PerlinNoise(Random &random) {
    offsetX = random.nextFloat() * 256;
    offsetY = random.nextFloat() * 256;
    offsetZ = random.nextFloat() * 256;

    for (int i = 0; i < 256; i++) {
        permutations[i] = i;
    }

    for (int i = 0; i < 256; i++) {
        int pos = (int) random.nextBoundedInt(256 - i) + i;
        int old = permutations[i];
        permutations[i] = permutations[pos];
        permutations[pos] = old;
        permutations[i + 256] = permutations[i];
    }
}

double *PerlinNoise::getNoise(double *noise, double x, double y, double z, int sizeX, int sizeY,
                              int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
    if (sizeY == 1) {
        return get2dNoise(noise, x, z, sizeX, sizeZ, scaleX, scaleZ, amplitude);
    } else {
        return get3dNoise(noise, x, y, z, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, amplitude);
    }
}

double *PerlinNoise::get2dNoise(double *noise, double x, double z, int sizeX, int sizeZ,
                                double scaleX, double scaleZ, double amplitude) {
    int index = 0;
    for (int i = 0; i < sizeX; i++) {
        double dx = x + offsetX + i * scaleX;
        int floorX = Utils::floor(dx);
        int ix = floorX & 255;
        dx -= floorX;
        double fx = Utils::fade(dx);
        for (int j = 0; j < sizeZ; j++) {
            double dz = z + offsetZ + j * scaleZ;
            int floorZ = Utils::floor(dz);
            int iz = floorZ & 255;
            dz -= floorZ;
            double fz = Utils::fade(dz);
            // Hash coordinates of the square corners
            int a = permutations[ix];
            int aa = permutations[a] + iz;
            int b = permutations[ix + 1];
            int ba = permutations[b] + iz;
            double x1 = Utils::lerp(fx,
                                    Utils::grad(permutations[aa], dx, 0, dz),
                                    Utils::grad(permutations[ba], dx - 1, 0, dz));
            double x2 = Utils::lerp(fx,
                                    Utils::grad(permutations[aa + 1], dx, 0, dz - 1),
                                    Utils::grad(permutations[ba + 1], dx - 1, 0, dz - 1));
            noise[index++] += Utils::lerp(fz, x1, x2) * amplitude;
        }
    }

    return noise;
}

double *PerlinNoise::get3dNoise(double *noise, double x, double y, double z, int sizeX,
                                int sizeY, int sizeZ, double scaleX, double scaleY, double scaleZ, double amplitude) {
    int n = -1;
    double x1 = 0;
    double x2 = 0;
    double x3 = 0;
    double x4 = 0;
    int index = 0;
    for (int i = 0; i < sizeX; i++) {
        double dx = x + offsetX + (i * scaleX);
        int floorX = Utils::floor(dx);
        int ix = floorX & 255;
        dx -= floorX;
        double fx = Utils::fade(dx);
        for (int j = 0; j < sizeZ; j++) {
            double dz = z + offsetZ + j * scaleZ;
            int floorZ = Utils::floor(dz);
            int iz = floorZ & 255;
            dz -= floorZ;
            double fz = Utils::fade(dz);
            for (int k = 0; k < sizeY; k++) {
                double dy = y + offsetY + k * scaleY;
                int floorY = Utils::floor(dy);
                int iy = floorY & 255;
                dy -= floorY;
                double fy = Utils::fade(dy);
                if (k == 0 || iy != n) {
                    n = iy;
                    // Hash coordinates of the cube corners
                    int a = permutations[ix] + iy;
                    int aa = permutations[a] + iz;
                    int ab = permutations[a + 1] + iz;
                    int b = permutations[ix + 1] + iy;
                    int ba = permutations[b] + iz;
                    int bb = permutations[b + 1] + iz;
                    x1 = Utils::lerp(fx,
                                     Utils::grad(permutations[aa], dx, dy, dz),
                                     Utils::grad(permutations[ba], dx - 1, dy, dz));
                    x2 = Utils::lerp(fx,
                                     Utils::grad(permutations[ab], dx, dy - 1, dz),
                                     Utils::grad(permutations[bb], dx - 1, dy - 1, dz));
                    x3 = Utils::lerp(fx,
                                     Utils::grad(permutations[aa + 1], dx, dy, dz - 1),
                                     Utils::grad(permutations[ba + 1], dx - 1, dy, dz - 1));
                    x4 = Utils::lerp(fx,
                                     Utils::grad(permutations[ab + 1], dx, dy - 1, dz - 1),
                                     Utils::grad(permutations[bb + 1], dx - 1, dy - 1, dz - 1));
                }

                double y1 = Utils::lerp(fy, x1, x2);
                double y2 = Utils::lerp(fy, x3, x4);

                noise[index++] += Utils::lerp(fz, y1, y2) * amplitude;
            }
        }
    }

    return noise;
}

//
// PerlinOctaveGenerator
//

PerlinOctaveGenerator::PerlinOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv) {
    for (int i = 0; i < octavesNum; ++i) {
        auto perlin = PerlinNoise(random);

        perlinOctaves.push_back(perlin);
    }

    sizeX = size_xv;
    sizeY = size_yv;
    sizeZ = size_zv;
}

double *PerlinOctaveGenerator::getFractalBrownianMotion(double x, double y, double z, double lacunarity,
                                                        double persistence) {
    auto *noise = new double[getArraySize()];

    for (int i = 0; i < getArraySize(); i++) {
        noise[i] = 0.0;
    }

    double freq = 1;
    double amp = 1;

    x = x * xScale;
    y = y * yScale;
    z = z * zScale;

    // fBm
    // the noise have to be periodic over x and z axis: otherwise it can go crazy with high
    // input, leading to strange oddities in terrain generation like the old minecraft farland
    // symptoms.
    for (PerlinNoise octave : perlinOctaves) {
        double dx = x * freq;
        double dz = z * freq;
        // compute integer part
        long lx = Utils::floorLong(dx);
        long lz = Utils::floorLong(dz);
        // compute fractional part
        dx -= lx;
        dz -= lz;
        // wrap integer part to 0..16777216
        lx %= 16777216;
        lz %= 16777216;
        // add to fractional part
        dx += lx;
        dz += lz;

        double dy = y * freq;

        octave.getNoise(noise, dx, dy, dz, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, amp);

        freq *= lacunarity;
        amp *= persistence;
    }

    return noise;
}

int PerlinOctaveGenerator::getArraySize() const {
    return sizeX * sizeY * sizeZ;
}

int PerlinOctaveGenerator::getSizeY() const {
    return sizeY;
}

SimplexOctaveGenerator::SimplexOctaveGenerator(Random &random, int octavesNum, int size_xv, int size_yv, int size_zv) {
    for (int i = 0; i < octavesNum; ++i) {
        auto simplex = SimplexNoise(random);

        simplexOctaves.push_back(simplex);
    }

    sizeX = size_xv;
    sizeY = size_yv;
    sizeZ = size_zv;
}

double *SimplexOctaveGenerator::getFractalBrownianMotion(double x, double y, double z, double lacunarity,
                                                         double persistence) {
    auto *noise = new double[getArraySize()];

    for (int i = 0; i < getArraySize(); i++) {
        noise[i] = 0.0;
    }

    double freq = 1.0, amp = 1.0;

    for (SimplexNoise octave : simplexOctaves) {
        octave.getNoise(noise, x, y, z, sizeX, sizeY, sizeZ, xScale * freq, yScale * freq, zScale * freq, 0.55 / amp);
        freq *= lacunarity;
        amp *= persistence;
    }

    return noise;
}

int SimplexOctaveGenerator::getArraySize() const {
    return sizeX * sizeY * sizeZ;
}

double SimplexOctaveGenerator::noise(double x, double y, double z, double frequency, double amplitude,
                                     bool normalized) {
    double result = 0.0, amp = 1.0, freq = 1.0, max = 0.0;

    x *= getXScale();
    y *= getYScale();
    z *= getZScale();

    for (SimplexNoise octave : simplexOctaves) {
        auto value = octave.noise3d(x * freq, y * freq, z * freq) * amp;

        result += value;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }

    if (normalized) {
        result /= max;
    }

    return result;
}

int SimplexOctaveGenerator::getSizeX() const {
    return sizeX;
}

int SimplexOctaveGenerator::getSizeZ() const {
    return sizeZ;
}

