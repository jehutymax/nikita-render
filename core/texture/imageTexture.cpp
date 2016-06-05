//
// Created by Rafael Campos on 6/4/16.
//

#include "imageTexture.h"

using nikita::ImageTexture;

nikita::Color ImageTexture::getColor(const Point &p) const
{
    int row = 0;
    int col = 0;

    if (map)
    {
        map->getTexelCoords(p, resX, resY, row, col);
    }

    return image->getColor(row, col);
}

void ImageTexture::setImage(const ImagePtr img)
{
    this->image = img;
    this->resX = img->x;
    this->resY = img->y;
}

void ImageTexture::setMapping(const TextureMappingPtr texmap)
{
    this->map = texmap;
}