//
// Created by Rafael Campos on 6/4/16.
//

#ifndef NIKITA_RENDER_TEXTUREMAPPING_H
#define NIKITA_RENDER_TEXTUREMAPPING_H

#include "../nikita.h"

namespace nikita
{
class TextureMapping
{
public:
    virtual void getTexelCoords(const Point &hitPoint,
                                const int resX,
                                const int resY,
                                int &row,
                                int &col) const = 0;



};

class Spherical : public TextureMapping
{
public:
    virtual void getTexelCoords(const Point &hitPoint,
                                const int resX,
                                const int resY,
                                int &row,
                                int &col) const;

private:
    float theta(const Point &p) const;
    float phi(const Point &p) const;

};

typedef std::shared_ptr<TextureMapping> TextureMappingPtr;

}


#endif //NIKITA_RENDER_TEXTUREMAPPING_H
