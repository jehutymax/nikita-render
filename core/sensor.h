//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_SENSOR_H
#define NIKITA_RENDER_SENSOR_H

#include "nikita.h"
#include "color.h"
#include "sampler.h"

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

namespace nikita
{
class Sensor
{
public:
    Sensor(int resX, int resY);

    virtual void addSample(const CameraSample& sample, const Color &color) {};
    virtual void getImageExtent(int *startX, int *endX, int *startY, int *endY) const {};
    virtual void writeImage(){};
    virtual void writeImage(const std::vector<Color> &samples) {}

    const int resolutionX;
    const int resolutionY;
};

class BWFilm : public Sensor
{
public:
    BWFilm(int resX, int resY, const std::string &filename);

    void addSample(const CameraSample& sample, int color);
    void getImageExtent(int *startX, int *endX, int *startY, int *endY) const;
    void writeImage();

private:
    std::string file;
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> image;

};

class RGBFilm: public Sensor
{
public:
    RGBFilm(int resX, int resY, const std::string &filename);

    virtual void addSample(const CameraSample& sample, const Color &color);
    virtual void getImageExtent(int *startX, int *endX, int *startY, int *endY) const;
    virtual void writeImage(const std::vector<Color> &samples);

private:
    std::string file;
    void averagePixels(const std::vector<nikita::Color> &supersampled, std::vector<nikita::Color> &result);
    static inline int toByte(float f);
};

int RGBFilm::toByte(float f)
{
    return floorf(f == 1.0 ? 255 : f * 256.0);
}

}

typedef std::shared_ptr<nikita::Sensor> SensorPtr;

#endif //NIKITA_RENDER_SENSOR_H
