//
// Created by Rafael Campos on 4/19/16.
//

#include "bxdf.h"

using nikita::BxDF;
using nikita::Lambertian;
using nikita::Glossy;
using nikita::Color;

Color Lambertian::f(const Vector &wi, const Vector &wo, const Normal &n) const
{
    return sample_f(wi, wo);
}

Color Lambertian::sample_f(const Vector &wi, const Vector &wo) const
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

Color Glossy::f(const Vector &wi, const Vector &wo, const Normal &n) const
{
    Color L;
    float dp = n.dot(wi);
    Vector r(-wi + 2.0 * n * dp);
    float dpo = r.dot(wo);

    if (dpo > 0.f)
        L = (k * powf(dpo, exp)) * c;

    return L;
}

Color Glossy::sample_f(const Vector &wi, const Vector &wo) const
{
    return Color::black(); // not implemented.
}

Color Glossy::rho(const Vector &wo) const
{
    return Color::black();
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