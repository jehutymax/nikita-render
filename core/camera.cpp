//
// Created by Rafael Campos on 4/6/16.
//

#include "camera.h"

using nikita::Camera;
using nikita::ProjectiveCamera;
using nikita::PerspectiveCamera;

Camera::Camera(const Transform &camera2World, SensorPtr film) : cameraToWorld(camera2World), film(film)
{ }

ProjectiveCamera::ProjectiveCamera(const Transform &camera2World,
                                   const Transform &projection,
                                   const float *screenDimensions,
                                   SensorPtr film)
                                   : Camera (camera2World, film)
{
    cameraToScreen = projection;

    Vector s(float(film->resolutionX), float(film->resolutionY), 1.0f);
    Vector s1(1.0f / (screenDimensions[1] - screenDimensions[0]),
        1.0f / (screenDimensions[2] - screenDimensions[3]), 1.0f);
    Vector s2(-screenDimensions[0], -screenDimensions[3], 0.f);

    screenToImage = Transform::getScale(s) * Transform::getScale(s1) * Transform::getTranslation(s2);
    imageToScreen = Transform(screenToImage.getInv());

    imageToCamera = Transform(cameraToScreen.getInv() * imageToScreen.get());
}



// Perspective Camera

PerspectiveCamera::PerspectiveCamera(const Transform &camera2World,
                                     const float *screenDimensions,
                                     float fieldOfView,
                                     SensorPtr film)
                                    : ProjectiveCamera(camera2World,
                                                       Transform::getPerspective(fieldOfView, 1e-2f, 1000.f),
                                                       screenDimensions,
                                                       film)
{ }

float PerspectiveCamera::generateRay(const CameraSample &camSample, Ray *ray) const
{
    Point o(0, 0, 0); // all rays are generated in the Camera space origin.

    Point pImage(camSample.imageX, camSample.imageY, 0);
    Point pCamera = imageToCamera.applyP(pImage);
    *ray = Ray(o, pCamera.normalized(), 0.0f, Infinity);

    // camera space -> world space
    *ray = (cameraToWorld)(*ray);
    return 0;
}