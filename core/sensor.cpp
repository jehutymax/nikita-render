//
// Created by Rafael Campos on 4/6/16.
//

#include "sensor.h"

using nikita::Sensor;
using nikita::BWFilm;

Sensor::Sensor(int resX, int resY) : resolutionX(resX), resolutionY(resY)
{ }


// BWFilm
BWFilm::BWFilm(int resX, int resY, const std::string &filename) : file(filename), Sensor(resX, resY)
{
    image.resize(resolutionX, resolutionY);
}

void BWFilm::addSample(const CameraSample &sample, int color)
{
    image(sample.imageX, sample.imageY) += color;
    if(image(sample.imageX, sample.imageY) > 255)
        image(sample.imageX, sample.imageY) = 255;
}

void BWFilm::getImageExtent(int *startX, int *endX, int *startY, int *endY) const
{
    *startX = 0;
    *startY = 0;
    *endX = resolutionX - 1;
    *endY = resolutionY - 1;
}

void BWFilm::writeImage()
{
    ImageOutput *out = ImageOutput::create(file);
    if (!out)
        return;
    ImageSpec spec(resolutionX, resolutionY, 1, TypeDesc::UINT8);
    unsigned char pixels[resolutionX*resolutionY];
    for (int i = 0; i < resolutionX; i++)
        for (int j = 0; j < resolutionY; j++)
            pixels[i + resolutionY*j] = image(i,j);
    out->open(file, spec);
    out->write_image(TypeDesc::UINT8, pixels);
    out->close();

    ImageOutput::destroy(out);
}
