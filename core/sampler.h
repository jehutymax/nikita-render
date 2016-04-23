//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_SAMPLER_H
#define NIKITA_RENDER_SAMPLER_H

#include <memory>
namespace nikita
{

// forward definition of Sample,
// so it can be used by Sampler.
struct Sample;

class Sampler
{
public:
    Sampler(int x0, int x1, int y0, int y1, int spp);

    bool next(Sample &sample);
    // members
    const int startX, endX, startY, endY;
    const int samplesPerPixel;

private:
    int currentX;
    int currentY;
};

typedef std::shared_ptr<Sampler> SamplerPtr;

struct CameraSample
{
    CameraSample() {}
    CameraSample(int x, int y)
        : imageX(x), imageY(y)
    {}
    float imageX, imageY;
};

struct Sample : public CameraSample
{
};
}

#endif //NIKITA_RENDER_SAMPLER_H
