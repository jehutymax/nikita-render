//
// Created by Rafael Campos on 6/5/16.
//

#ifndef NIKITA_RENDER_FBMNOISE_H
#define NIKITA_RENDER_FBMNOISE_H

#include "../nikita.h"
#include "texture.h"
#include <random>

namespace nikita {

const int tableSize = 256;
const int tableMask = tableSize - 1;
const int seed = 224;

template<class Type> Type
linear_interp(const float f, const Type &s, const Type &t) {
    return (s + f * (t - s));
}

class LatticeNoise {
public:
    LatticeNoise();
    virtual float getNoiseValue(const Point &p) const = 0;
    virtual float getFractalSumValue(const Point &p) const;
    virtual float getTurbulenceValue(const Point &p) const;
    virtual float getFBMValue(const Point &p) const;
    virtual void setOctaves(float oct);
    virtual void setGain(float g);
    virtual void setLacunarity(float lac);
protected:
    static const unsigned char permutationTable[tableSize];
    float valueTable[tableSize];

private:
    float numberOctaves;
    float fractalSumMin;
    float fractalSumMax;
    float gain;
    float lacunarity;
    float fbmMin;
    float fbmMax;

    void initializeLookupTable(int seed);
};

class LinearNoise : public LatticeNoise
{
public:
    virtual float getNoiseValue(const Point &p) const;
};

typedef std::shared_ptr<LatticeNoise> LatticeNoisePtr;
typedef std::shared_ptr<LinearNoise> LinearNoisePtr;

class FBMTexture : public Texture
{
public:
    virtual Color getColor(const Point& p) const;
    void setNoiseMaker(const LatticeNoisePtr np);
    void setColor(const Color &color);
    void setBounds(float min, float max);

private:
    LatticeNoisePtr noise;
    Color color;
    float min;
    float max;
};

typedef std::shared_ptr<FBMTexture> FBMTexturePtr;
}


#endif //NIKITA_RENDER_FBMNOISE_H
