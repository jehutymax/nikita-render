//
// Created by Rafael Campos on 6/4/16.
//

#include "texture.h"

using nikita::ConstantColor;

ConstantColor::ConstantColor(Color c)
{
    this->color = c;
}

nikita::Color ConstantColor::getColor(const Point&) const
{
    return color;
}