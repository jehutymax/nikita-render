//
// Created by Rafael Campos on 6/4/16.
//

#ifndef NIKITA_RENDER_TEXTURE_H
#define NIKITA_RENDER_TEXTURE_H

#include "../nikita.h"
#include "../color.h"

namespace nikita
{
class Texture
{
public:
    virtual Color getColor(const Point &p) const = 0;
};

class ConstantColor : public Texture
{
public:
    ConstantColor(Color c);

    virtual Color getColor(const Point&) const;

private:
    Color color;
};

typedef std::shared_ptr<Texture> TexturePtr;

}

#endif //NIKITA_RENDER_TEXTURE_H
