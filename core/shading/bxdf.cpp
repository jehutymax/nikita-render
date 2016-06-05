//
// Created by Rafael Campos on 4/19/16.
//

#include "bxdf.h"

using nikita::BxDF;
using nikita::Lambertian;
using nikita::PositionLambertian;
using nikita::Glossy;
using nikita::Mirror;
using nikita::Color;
using nikita::PerfectTransmitter;


Color Lambertian::f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    return sample_f(wi, wo, n);
}

Color Lambertian::sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    return (kd * InvPi * cd);
}

Color Lambertian::rho(const Vector &wo, const Point &p) const
{
    return (kd * cd);
}

void Lambertian::setKd(float k)
{
    kd = MathUtils::clamp(k, 0.f, 1.f);
}

void Lambertian::setCd(const Color &c)
{
    cd = c;
}

Color PositionLambertian::f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    return sample_f(wi, wo, n);
}

Color PositionLambertian::sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    return (kd * InvPi * cd->getColor(p));
}

Color PositionLambertian::rho(const Vector &wo, const Point &p) const
{
    return (kd * cd->getColor(p));
}

void PositionLambertian::setKd(float k)
{
    kd = MathUtils::clamp(k, 0.f, 1.f);
}

void PositionLambertian::setCd(const TexturePtr &c)
{
    cd = c;
}

Color Glossy::f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    Color L;
    float dp = n.dot(wi);
    Vector r(-wi + 2.0 * n * dp);
    float dpo = r.dot(wo);

    if (dpo > 0.f)
        L = (k * powf(dpo, exp)) * c;

    return L;
}


void Glossy::setColor(const Color &c)
{
    this->c = c;
}

void Glossy::setExp(float e)
{
    exp = e;
}

void Glossy::setK(float k)
{
    this->k = k;
}

Color Mirror::sample_f(Vector &wi, const Vector &wo, const Normal &n, const Point &p) const
{
    float dp = n.dot(wo);
    wi = -wo + 2.0f * n * dp;

    float dp2 = n.dot(wi);

    return ((this->k * this->c) / dp2);
}

void Mirror::setK(float k)
{
    this->k = k;
}

void Mirror::setColor(const Color &c)
{
    this->c = c;
}

bool PerfectTransmitter::totalInternalReflection(const Vector &wo, const Normal &n, const Point &p) const
{
    float cosTheta_i = n.dot(wo);
    float eta = ior;

    if (cosTheta_i < 0.f)
        eta = 1.f / eta;

    float num = 1.f - (cosTheta_i * cosTheta_i);
    float denom = eta * eta;

    return (1.f - num/denom < 0.f);

}

Color PerfectTransmitter::sample_f(Vector &wt, const Vector &wo, const Normal &n, const Point &p) const
{
    Normal normal(n);
    float cosTheta1 = normal.dot(wo);
    float eta = this->ior;

    if (cosTheta1 < 0.f)
    {
        cosTheta1 = -cosTheta1;
        normal = -normal;
        eta = 1.f / eta;
    }

    float t = 1.f - ((1.f - cosTheta1 * cosTheta1) / (eta * eta));
    float cosTheta2 = sqrtf(t);

    wt = -wo / eta - (cosTheta2 - cosTheta1 / eta) * normal;

    return (this->k / (eta * eta) * Color::white() / fabsf(normal.dot(wt)));
}

void PerfectTransmitter::setK(float k)
{
    this->k = MathUtils::clamp(k, 0.f, 1.f);
}

void PerfectTransmitter::setIor(float i)
{
    this->ior = i;
}