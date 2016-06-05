//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_BXDF_H
#define NIKITA_RENDER_BXDF_H

#include "../nikita.h"
#include "../color.h"
#include "../sampler.h"
#include "../../util/mathUtils.h"
#include "../texture/texture.h"

namespace nikita
{
class BxDF
{
public:
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const {return Color::black(); }
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const {return Color::black(); }
    virtual Color rho(const Vector &wo, const Point &p = Point(0,0,0)) const {return Color::black(); }
    virtual bool totalInternalReflection(const Vector &wo, const Normal &n) const {return false; }

protected:
    SamplerPtr sampler;
};

class Lambertian : public BxDF
{
public:
    Lambertian() : BxDF() {}
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    virtual Color rho(const Vector &wo, const Point &p = Point(0,0,0)) const;

    void setKd(float k);
    void setCd(const Color &c);
private:
    float kd; // diffuse reflection coefficient
    Color cd; // diffuse color
};

class PositionLambertian : public BxDF
{
public:
    PositionLambertian() : BxDF() {}
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    virtual Color rho(const Vector &wo, const Point &p = Point(0,0,0)) const;

    void setKd(float k);
    void setCd(const TexturePtr &c);
private:
    float kd; // diffuse reflection coefficient
    TexturePtr cd; // diffuse color
};

class Glossy : public BxDF
{
public:
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;

    void setK(float k);
    void setExp(float e);
    void setColor(const Color &c);
private:
    float k;
    Color c;
    float exp;
};

class Mirror : public BxDF{
public:
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;

    void setK(float k);
    void setColor(const Color &c);
private:
    float k;
    Color c;
};

class PerfectTransmitter : public BxDF
{
public:
    virtual bool totalInternalReflection(const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p = Point(0,0,0)) const;
    void setK(float k);
    void setIor(float i);

private:
    float k;
    float ior;
};

typedef std::shared_ptr<BxDF> BRDFPtr;
typedef std::shared_ptr<Lambertian> LambertianPtr;
typedef std::shared_ptr<PositionLambertian> PositionLambertianPtr;
typedef std::shared_ptr<Glossy> GlossyPtr;
typedef std::shared_ptr<Mirror> MirrorPtr;
typedef std::shared_ptr<PerfectTransmitter> PerfectTransmitterPtr;

}

#endif //NIKITA_RENDER_BXDF_H
