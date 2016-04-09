//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_CAMERA_H
#define NIKITA_RENDER_CAMERA_H

#include "nikita.h"
#include "transform.h"
#include "sensor.h"
#include "sampler.h"

namespace nikita
{
class Camera
{
public:
    Camera(const Transform& camera2World, SensorPtr film);

    virtual float generateRay(const CameraSample& camSample, Ray *ray) const = 0;

    // Members
    Transform cameraToWorld;
    SensorPtr film;
};

class ProjectiveCamera : public Camera
{
public:
       ProjectiveCamera(const Transform& camera2World,
                        const Transform &projection,
                        const float screenDimensions[4],
                        SensorPtr film);


protected:
    Transform cameraToScreen, imageToCamera;
    Transform screenToImage, imageToScreen;
};

class PerspectiveCamera : public ProjectiveCamera
{
public:
    PerspectiveCamera(const Transform& camera2World,
                     const float screenDimensions[4],
                     float fieldOfView,
                     SensorPtr film);


    float generateRay(const CameraSample &camSample, Ray *ray) const;
};

typedef std::shared_ptr<Camera> CameraPtr;

}

#endif //NIKITA_RENDER_CAMERA_H
