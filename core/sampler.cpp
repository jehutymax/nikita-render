//
// Created by Rafael Campos on 4/6/16.
//

#include "sampler.h"

using nikita::Sampler;

Sampler::Sampler(int x0, int x1, int y0, int y1, int spp)
    : startX(x0), endX(x1), startY(y0), endY(y1), samplesPerPixel(spp), currentX(0), currentY(0)
{ }

bool Sampler::next(Sample &sample)
{
    if (currentX + 1 == endX && currentY == endY)
        return false;

    if (currentY == endY)
    {
        currentY = 0;
        currentX++;
    }

    sample.imageX = currentX;
    sample.imageY = currentY++;

    return true;
}