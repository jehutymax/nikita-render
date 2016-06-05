//
// Created by Rafael Campos on 6/4/16.
//

#ifndef NIKITA_RENDER_IMAGE_H
#define NIKITA_RENDER_IMAGE_H

#include <OpenImageIO/imageio.h>
#include "../core/color.h"
OIIO_NAMESPACE_USING


namespace nikita
{
class Image
{
public:
    Image(std::string filename);
    Color getColor(int i, int j);

    int x;
    int y;
    int channels;

private:
    std::vector<unsigned char> pixels;
    void openImage(std::string path);

};

typedef std::shared_ptr<Image> ImagePtr;

}

#endif //NIKITA_RENDER_IMAGE_H
