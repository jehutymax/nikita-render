//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_SAMPLER_H
#define NIKITA_RENDER_SAMPLER_H

namespace nikita
{
class Sampler
{
    Sampler(int x0, int x1, int y0, int y1, int spp);

    // members
    const int startX, endX, startY, endY;
    const int samplesPerPixel;
};

struct CameraSample
{
    float imageX, imageY;
};

struct Sample : public CameraSample
{

};
}

#endif //NIKITA_RENDER_SAMPLER_H
