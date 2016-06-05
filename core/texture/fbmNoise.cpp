//
// Created by Rafael Campos on 6/5/16.
//

#include "fbmNoise.h"

#define PERM(x) LatticeNoise::permutationTable[(x)&tableMask]
#define INDEX(ix, iy, iz) PERM((ix) + PERM((iy) + PERM(iz)))

using nikita::LatticeNoise;
using nikita::LinearNoise;
using nikita::FBMTexture;

// Table taken from Darwyn Peachey in Ebert et al. (2003), page 70.
const unsigned char LatticeNoise::permutationTable[tableSize] = {
    225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
    5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
    169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
    248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
    36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
    170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
    162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
    53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
    18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
    106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
    26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
    12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
    184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
    236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
    48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
    137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
};

LatticeNoise::LatticeNoise() :
    numberOctaves(1),
    lacunarity(2.f),
    gain(0.5)
{
    initializeLookupTable(seed);
    fbmMax = (1.f - std::powf(gain, numberOctaves)) / (1.f - gain);
    fbmMin = -fbmMax;
}

void LatticeNoise::initializeLookupTable(int seed)
{
    std::random_device rd;
    std::mt19937 engine(rd());

    std::uniform_real_distribution<> dist(-1.0, 1.0);
    for (int i = 0; i < tableSize; ++i) {
        valueTable[i] = float(dist(engine));
    }
}

float LatticeNoise::getFractalSumValue(const Point &p) const
{
    float amplitude = 1.f;
    float frequency = 1.f;
    float fractalSum = 0.f;

    for (int i = 0; i < numberOctaves; ++i) {
        fractalSum += amplitude * getNoiseValue(frequency * p);
        amplitude *= 0.5f;
        frequency *= 2.f;
    }

    fractalSum = (fractalSum - fractalSumMin) / (fractalSumMax - fractalSumMin);
    return fractalSum;
}

float LatticeNoise::getTurbulenceValue(const Point &p) const
{
    float amplitude = 1.f;
    float frequency = 1.f;
    float turbulence = 0.f;

    for (int i = 0; i < numberOctaves; ++i) {
        turbulence += amplitude * std::fabsf(getNoiseValue(frequency * p));
        amplitude *= 0.5f;
        frequency *= 2.f;
    }

    turbulence /= fractalSumMax;
    return turbulence;
}

float LatticeNoise::getFBMValue(const Point &p) const
{
    float amplitude = 1.f;
    float frequency = 1.f;
    float fbm = 0.f;

    for (int i = 0; i < numberOctaves; ++i) {
        fbm += amplitude * getNoiseValue(frequency * p);
        amplitude *= gain;
        frequency *= lacunarity;
    }

    fbm = (fbm - fbmMin) / (fbmMax - fbmMin);
    return fbm;
}

void LatticeNoise::setOctaves(float oct)
{
    this->numberOctaves = oct;
}

void LatticeNoise::setGain(float g)
{
    this->gain = g;
}

void LatticeNoise::setLacunarity(float lac)
{
    this->lacunarity = lac;
}

float LinearNoise::getNoiseValue(const Point &p) const
{
    int ix, iy, iz;
    float d[2][2][2];
    Vector f;
    Vector x;
    float x3;
    Vector y;

    ix = std::floor(p(0));
    f(0) = p(0) - ix;

    iy = std::floor(p(1));
    f(1) = p(1) - iy;

    iz = std::floor(p(2));
    f(2) = p(2) - iz;

    for (int k = 0; k <= 1; ++k)
        for (int j = 0; j <= 1; ++j)
            for (int i = 0; i <= 1; ++i)
                d[k][j][i] = valueTable[INDEX(ix + i, iy + j, iz + k)];

    x(0) = linear_interp(f(0), d[0][0][0], d[0][0][1]);
    x(1) = linear_interp(f(0), d[0][1][0], d[0][1][1]);
    x(2) = linear_interp(f(0), d[1][0][0], d[1][0][1]);
    x3 = linear_interp(f(0), d[1][1][0], d[1][1][1]);
    y(0) = linear_interp(f(1), x(0), x(1));
    y(1) = linear_interp(f(1), x(2), x3);
    y(2) = linear_interp(f(2), y(0), y(1));

    return y(2);
}

nikita::Color FBMTexture::getColor(const Point &p) const
{
    float value = noise->getFBMValue(p);
    value = min + (max - min) * value;
    return (value * color);
}

void FBMTexture::setNoiseMaker(const LatticeNoisePtr np)
{
    this->noise = np;
}

void FBMTexture::setColor(const Color &color)
{
    this->color = color;
}

void FBMTexture::setBounds(float min, float max)
{
    this->min = std::min(min, max);
    this->max = std::max(min, max);
}