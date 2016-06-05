//
// Created by Rafael Campos on 4/19/16.
//

#include "material.h"

using nikita::Material;
using nikita::Matte;
using nikita::PositionMatte;
using nikita::Phong;
using nikita::Reflective;
using nikita::Transparent;

nikita::BRDFPtr Material::getReflective()
{
    return nullptr;
}

nikita::BRDFPtr Material::getTransparent()
{
    return nullptr;
}

Matte::Matte() : Material()
{
    ambient = std::make_shared<Lambertian>();
    diffuse = std::make_shared<Lambertian>();
    setKa(0.25);
    setKd(0.6);
    setCd(Color::green());
}

void Matte::setKa(float k)
{
    ambient->setKd(k);
}

void Matte::setKd(float k)
{
    diffuse->setKd(k);
}

void Matte::setCd(const Color &c)
{
    ambient->setCd(c);
    diffuse->setCd(c);
}

nikita::BRDFPtr Matte::getAmbient()
{
    return ambient;
}

nikita::BRDFPtr Matte::getDiffuse()
{
    return diffuse;
}

nikita::BRDFPtr Matte::getSpecular()
{
    return nullptr;
}

PositionMatte::PositionMatte() : Material()
{
    ambient = std::make_shared<PositionLambertian>();
    diffuse = std::make_shared<PositionLambertian>();
    setKa(0.25);
    setKd(0.6);
}

void PositionMatte::setKa(float k)
{
    ambient->setKd(k);
}

void PositionMatte::setKd(float k)
{
    diffuse->setKd(k);
}

void PositionMatte::setCd(const TexturePtr &c)
{
    ambient->setCd(c);
    diffuse->setCd(c);
}

nikita::BRDFPtr PositionMatte::getAmbient()
{
    return ambient;
}

nikita::BRDFPtr PositionMatte::getDiffuse()
{
    return diffuse;
}

nikita::BRDFPtr PositionMatte::getSpecular()
{
    return nullptr;
}

Phong::Phong() : Material()
{
    ambient = std::make_shared<Lambertian>();
    diffuse = std::make_shared<Lambertian>();
    glossy = std::make_shared<Glossy>();
    setKa(0.25);
    setKd(0.6);
    setKs(0.5);
    setExp(6);
    setCd(Color::green());
}

void Phong::setKa(float k)
{
    ambient->setKd(k);
}

void Phong::setKd(float k)
{
    diffuse->setKd(k);
}

void Phong::setKs(float k)
{
    glossy->setK(k);
}

void Phong::setCd(const Color &c)
{
    ambient->setCd(c);
    diffuse->setCd(c);
    glossy->setColor(c);
}

void Phong::setExp(float e)
{
    glossy->setExp(e);
}

nikita::BRDFPtr Phong::getAmbient()
{
    return ambient;
}

nikita::BRDFPtr Phong::getDiffuse()
{
    return diffuse;
}

nikita::BRDFPtr Phong::getSpecular()
{
    return glossy;
}

Reflective::Reflective() : Phong()
{
    mirror = std::make_shared<Mirror>();
    setKr(0.25);
    setCd(Color::blue());
}

void Reflective::setKr(float k)
{
    this->mirror->setK(k);
}

void Reflective::setCd(const Color &c)
{
    Phong::setCd(c);
    this->mirror->setColor(c);
}

nikita::BRDFPtr Reflective::getReflective()
{
    return mirror;
}

Transparent::Transparent() : Phong()
{
    mirror = std::make_shared<Mirror>();
    transmitter = std::make_shared<PerfectTransmitter>();
    setKt(0.4);
}

void Transparent::setKt(float k)
{
    mirror->setK(1.f - k);
    transmitter->setK(k);
}

void Transparent::setCd(const Color &c)
{
    Phong::setCd(c);
    mirror->setColor(c);
}

nikita::BRDFPtr Transparent::getReflective()
{
    return mirror;
}

nikita::BRDFPtr Transparent::getTransparent()
{
    return transmitter;
}

void Transparent::setIor(float i)
{
    transmitter->setIor(i);
}