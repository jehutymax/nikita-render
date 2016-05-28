//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_BXDF_H
#define NIKITA_RENDER_BXDF_H

#include "../nikita.h"
#include "../color.h"
#include "../sampler.h"
#include "../../util/mathUtils.h"

namespace nikita
{
class BxDF
{
public:
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n) const {return Color::black(); };
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n) const {return Color::black(); };
    virtual Color rho(const Vector &wo) const {return Color::black(); };

protected:
    SamplerPtr sampler;
};

class Lambertian : public BxDF
{
public:
    Lambertian() : BxDF() {}
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n) const;
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n) const;
    virtual Color rho(const Vector &wo) const;

    void setKd(float k);
    void setCd(const Color &c);
private:
    float kd; // diffuse reflection coefficient
    Color cd; // diffuse color
};

class Glossy : public BxDF
{
public:
    virtual Color f(Vector &wi, const Vector &wo, const Normal &n) const;

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
    virtual Color sample_f(Vector &wi, const Vector &wo, const Normal &n) const;

    void setK(float k);
    void setColor(const Color &c);
private:
    float k;
    Color c;
};

typedef std::shared_ptr<BxDF> BRDFPtr;
typedef std::shared_ptr<Lambertian> LambertianPtr;
typedef std::shared_ptr<Glossy> GlossyPtr;
typedef std::shared_ptr<Mirror> MirrorPtr;

}

#endif //NIKITA_RENDER_BXDF_H
