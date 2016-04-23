//
// Created by Rafael Campos on 4/16/16.
//

#ifndef NIKITA_RENDER_LIGHT_H
#define NIKITA_RENDER_LIGHT_H

#include "nikita.h"
#include "transform.h"
#include "color.h"

namespace nikita
{
class Light
{
public:
    Light(const TransformPtr &light2World, int nSamples) : lightToWorld(light2World), numberSamples(nSamples)
    {}
    virtual void setColor(const Color &color) = 0;
    virtual void setIntensity(float intensity) = 0;
    virtual Vector getDirection(const Point &p) = 0;
    virtual Color getRadiance() = 0;
    virtual Color getPower() = 0;

protected:
    TransformPtr lightToWorld;
    int numberSamples;
};

class PointLight : public Light
{
public:
    PointLight(const TransformPtr &light2World, int nSamples);
    virtual Vector getDirection(const Point &p);
    virtual Color getRadiance();
    virtual Color getPower();
    virtual void setIntensity(float intensity);
    virtual void setColor(const Color &color);
private:
    Point lightPosition;
    Color lightColor;
    float lightIntensity;
};

class AmbientLight : public Light
{
public:
    AmbientLight(const TransformPtr, int);
    virtual Vector getDirection(const Point &p);
    virtual Color getRadiance();
    virtual Color getPower();
    virtual void setIntensity(float intensity);
    virtual void setColor(const Color &color);
private:
    Color lightColor;
    float lightIntensity;
};

typedef std::shared_ptr<Light> LightPtr;
typedef std::shared_ptr<PointLight> PointLightPtr;
typedef std::shared_ptr<AmbientLight> AmbientLightPtr;

}

#endif //NIKITA_RENDER_LIGHT_H
