//
// Created by Rafael Campos on 6/4/16.
//

#include "image.h"

using nikita::Image;

Image::Image(std::string filename)
{
    openImage(filename);
}

void Image::openImage(std::string path)
{
    ImageInput *in = ImageInput::open(path);
    if (!in)
    {
        std::cerr << "Image couldn't be read: " << path << std::endl;
        return;
    }

    const ImageSpec &spec = in->spec();
    x = spec.width;
    y = spec.height;
    channels = spec.nchannels;
    pixels = std::vector<unsigned char>(size_t(x * y * channels));
    in->read_image(TypeDesc::UINT8, &pixels[0]);
    in->close();
    ImageInput::destroy(in);
}

// i = row, j = col
nikita::Color Image::getColor(int i, int j)
{
    // for now, assume channels = 3
    if (channels != 3)
    {
        std::cerr << "Only RGB images are supported." << std::endl;
        return Color::red();
    }

    int k = (i * x + j) * 3;
    Color result(pixels[k]/255.f, pixels[k+1]/255.f, pixels[k+2]/255.f);

    return result;
}