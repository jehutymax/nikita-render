//
// Created by Rafael Campos on 4/6/16.
//

#include "sensor.h"

using nikita::Sensor;
using nikita::BWFilm;
using nikita::RGBFilm;

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

// RGBFilm
RGBFilm::RGBFilm(int resX, int resY, const std::string &filename) : file(filename), Sensor(resX, resY)
{
}

void RGBFilm::addSample(const CameraSample &sample, const Color &c)
{

}

void RGBFilm::averagePixels(const std::vector<nikita::Color> &supersampled, std::vector<nikita::Color> &result)
{
    result.clear();
    int resX = resolutionX / 2;
    int resY = resolutionY / 2;
    result.resize(resX * resY);
    for (int i = 0; i < resX; ++i)
        for (int j = 0; j < resY; ++j) {
            int offset_x = 2*i;
            int offset_y = 2*j;
            result[j + i * resX] =
                      supersampled[offset_x * resolutionX + offset_y]/4 +
                      supersampled[offset_x * resolutionX + offset_y + 1]/4 +
                      supersampled[(offset_x + 1) * resolutionX + offset_y]/4 +
                      supersampled[(offset_x + 1) * resolutionX + offset_y + 1]/4;
        }
};

void RGBFilm::getImageExtent(int *startX, int *endX, int *startY, int *endY) const
{
    *startX = 0;
    *startY = 0;
    *endX = resolutionX - 1;
    *endY = resolutionY - 1;
}

void RGBFilm::writeImage(const std::vector<nikita::Color> &samples)
{
    ImageOutput *out = ImageOutput::create(file);
    if (!out)
        return;

    std::vector<nikita::Color> averaged;
    averagePixels(samples, averaged);

    int resX = resolutionX / 2;
    int resY = resolutionY / 2;

    ImageSpec spec(resX, resY, 3, TypeDesc::UINT8);
    std::vector<unsigned char> pixels;
    for (int i = 0; i < resX; i++)
        for (int j = 0; j < resY; j++) {
            int a = RGBFilm::toByte(averaged[i * resX + j].get(0));
            int b = RGBFilm::toByte(averaged[i * resX + j].get(1));
            int c = RGBFilm::toByte(averaged[i * resX + j].get(2));
            pixels.push_back(a);
            pixels.push_back(b);
            pixels.push_back(c);
        }
    out->open(file, spec);
    out->write_image(TypeDesc::UINT8, &pixels[0]);
    out->close();

    ImageOutput::destroy(out);
}
