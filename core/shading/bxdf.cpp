//
// Created by Rafael Campos on 4/19/16.
//

#include "bxdf.h"

using nikita::BxDF;
using nikita::Lambertian;
using nikita::Glossy;
using nikita::Mirror;
using nikita::Color;

Color Lambertian::f(Vector &wi, const Vector &wo, const Normal &n) const
{
    return sample_f(wi, wo, n);
}

Color Lambertian::sample_f(Vector &wi, const Vector &wo, const Normal &n) const
{
    return (kd * InvPi * cd);
}

Color Lambertian::rho(const Vector &wo) const
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

Color Glossy::f(Vector &wi, const Vector &wo, const Normal &n) const
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

Color Mirror::sample_f(Vector &wi, const Vector &wo, const Normal &n) const
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