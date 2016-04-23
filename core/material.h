//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_MATERIAL_H
#define NIKITA_RENDER_MATERIAL_H

#include "color.h"
#include "bxdf.h"
#include "light.h"

namespace nikita
{
class Material
{
public:
    virtual BRDFPtr getAmbient() = 0;
    virtual BRDFPtr getDiffuse() = 0;
    virtual BRDFPtr getSpecular() = 0;
};

class Matte : public Material
{
public:
    Matte();
    void setKa(float k);
    void setKd(float k);
    void setCd(const Color &c);

    virtual BRDFPtr getAmbient();
    virtual BRDFPtr getDiffuse();
    virtual BRDFPtr getSpecular();

private:
    LambertianPtr ambient;
    LambertianPtr diffuse;
};

class Phong : public Material
{
public:
    Phong();
    void setKa(float k);
    void setKd(float k);
    void setKs(float k);
    void setExp(float e);
    void setCd(const Color &c);

    virtual BRDFPtr getAmbient();
    virtual BRDFPtr getDiffuse();
    virtual BRDFPtr getSpecular();
private:
    LambertianPtr ambient;
    LambertianPtr diffuse;
    GlossyPtr glossy;
};

typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Matte> MattePtr;
typedef std::shared_ptr<Phong> PhongPtr;

}

#endif //NIKITA_RENDER_MATERIAL_H
