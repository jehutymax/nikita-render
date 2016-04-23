//
// Created by Rafael Campos on 4/16/16.
//

#include "light.h"

using nikita::PointLight;
using nikita::AmbientLight;

PointLight::PointLight(const TransformPtr &light2World, int nSamples) : Light(light2World, nSamples)
{
    this->lightPosition = light2World->applyP(Point(0,0,0));
    this->lightColor = Color::white();
    this->lightIntensity = 1.0;
}

Vector PointLight::getDirection(const Point &p)
{
    return (this->lightPosition - p).normalized();
}

nikita::Color PointLight::getRadiance()
{
    return lightIntensity * lightColor;
}

nikita::Color PointLight::getPower()
{
    return lightColor;
}

void PointLight::setIntensity(float intensity)
{
    this->lightIntensity = MathUtils::clamp(intensity, 0.0f, 1.0f);
}

void PointLight::setColor(const Color &color)
{
    this->lightColor = color;
}

AmbientLight::AmbientLight(const TransformPtr t, int)
    : Light(t, 1.0),
      lightIntensity(0.3),
      lightColor(Color::white())
{ }

Vector AmbientLight::getDirection(const Point &p)
{
    return Vector(0,0,0);
}

nikita::Color AmbientLight::getRadiance()
{
    return lightIntensity * lightColor;
}

nikita::Color AmbientLight::getPower()
{
    return lightColor;
}

void AmbientLight::setIntensity(float intensity)
{
    this->lightIntensity = MathUtils::clamp(intensity, 0.0f, 1.0f);
}

void AmbientLight::setColor(const Color &color)
{
    this->lightColor = color;
}