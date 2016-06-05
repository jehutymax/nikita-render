//
// Created by Rafael Campos on 6/4/16.
//

#ifndef NIKITA_RENDER_IMAGETEXTURE_H
#define NIKITA_RENDER_IMAGETEXTURE_H

#include "../nikita.h"
#include "texture.h"
#include "textureMapping.h"
#include "../../util/image.h"

namespace nikita
{
class ImageTexture : public Texture
{
public:
    virtual Color getColor(const Point &p) const;
    void setImage(const ImagePtr img);
    void setMapping(const TextureMappingPtr texmap);

private:
    int resX;
    int resY;

    ImagePtr image;
    TextureMappingPtr map;

};

typedef std::shared_ptr<ImageTexture> ImageTexturePtr;

}


#endif //NIKITA_RENDER_IMAGETEXTURE_H
