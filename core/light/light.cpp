//
// Created by Rafael Campos on 4/16/16.
//

#include "light.h"

using nikita::Light;
using nikita::PointLight;
using nikita::SpotLight;
using nikita::AmbientLight;

bool Light::castsShadow()
{
    return this->shadow;
}

PointLight::PointLight(const TransformPtr &light2World, int nSamples) : Light(light2World, nSamples, true)
{
    this->lightPosition = light2World->applyP(Point(0,0,0));
    this->lightColor = Color::white();
    this->lightIntensity = 1.0;
}

Vector PointLight::getDirection(const Point &p)
{
    return ((this->lightPosition - p).normalized());
}

nikita::Color PointLight::getRadiance(const Point &p)
{
    return lightIntensity * lightColor;
}

void PointLight::setIntensity(float intensity)
{
    this->lightIntensity = MathUtils::clamp(intensity, 0.0f, 1.0f);
}

void PointLight::setColor(const Color &color)
{
    this->lightColor = color;
}

bool PointLight::testVisibility(Ray &ray, PrimPtr primGroup) const
{
    float distance = (this->lightPosition - ray.origin).norm();
    return primGroup->intersectP(ray, distance);
}

AmbientLight::AmbientLight(const TransformPtr t, int)
    : Light(t, 1.0, false),
      lightIntensity(0.3),
      lightColor(Color::white())
{ }

Vector AmbientLight::getDirection(const Point &p)
{
    return Vector(0,0,0);
}

nikita::Color AmbientLight::getRadiance(const Point &p)
{
    return lightIntensity * lightColor;
}

void AmbientLight::setIntensity(float intensity)
{
    this->lightIntensity = MathUtils::clamp(intensity, 0.0f, 1.0f);
}

void AmbientLight::setColor(const Color &color)
{
    this->lightColor = color;
}

bool AmbientLight::testVisibility(Ray &ray, PrimPtr primGroup) const
{
    return false;
}

// SpotLight
SpotLight::SpotLight(const TransformPtr &light2World,
                     const Color &color,
                     float intensity,
                     float width,
                     float decay)
    :   lightPosition(light2World->applyP(Point(0,0,0))),
        lightColor(color),
        lightIntensity(intensity),
        Light(light2World, 1, true)
{
    cosTotalWidth = cosf(MathUtils::toRadians(width));
    cosDecayBegin = cosf(MathUtils::toRadians(decay));
}

nikita::Color SpotLight::getRadiance(const Point &p)
{
    Vector wi = (lightPosition - p).normalized();
    float result = lightIntensity * evaluateDecay(wi) / (lightPosition - p).squaredNorm();
    return lightColor * result;
}

float SpotLight::evaluateDecay(const Vector &w) const
{
    TransformPtr worldToLight = std::make_shared<Transform>(lightToWorld->getInv());
    Vector wl = (worldToLight->applyV(w)).normalized();
    float cosTheta = wl(2);

    if (cosTheta < cosTotalWidth)
        return 0.f;
    if (cosTheta > cosDecayBegin)
        return 1.f;

    // evaluate the decay inside the cone
    float d = (cosTheta - cosTotalWidth) / (cosDecayBegin - cosTotalWidth);
    return (d*d*d*d); // d^4
}

void SpotLight::setColor(const Color &color)
{
    this->lightColor = color;
}

void SpotLight::setIntensity(float intensity)
{
    this->lightIntensity = intensity;
}

bool SpotLight::testVisibility(Ray &ray, PrimPtr primGroup) const
{
    float distance = (this->lightPosition - ray.origin).norm();
    return primGroup->intersectP(ray, distance);
}

Vector SpotLight::getDirection(const Point &p)
{
    return ((this->lightPosition - p).normalized());
}