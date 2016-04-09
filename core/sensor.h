//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_SENSOR_H
#define NIKITA_RENDER_SENSOR_H

#include "nikita.h"
#include "sampler.h"

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

namespace nikita
{
class Sensor
{
public:
    Sensor(int resX, int resY);

    virtual void addSample(const CameraSample& sample, int color) = 0;
    virtual void getImageExtent(int *startX, int *endX, int *startY, int *endY) const = 0;
    virtual void writeImage() = 0;


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
}

typedef std::shared_ptr<nikita::Sensor> SensorPtr;

#endif //NIKITA_RENDER_SENSOR_H
