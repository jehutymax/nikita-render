//
// Created by Rafael Campos on 4/16/16.
//

#include "color.h"
#include "../util/mathUtils.h"

using nikita::Color;

Color::Color()
{
    rgb.setZero();
}

Color::Color(float red, float green, float blue)
    : rgb(red, green, blue)
{
    set(rgb);
}

Color::Color(const float *rgb) : rgb(rgb[0], rgb[1], rgb[2])
{
    set(this->rgb);
}

Color::Color(const RGB &rgb)
{
    set(rgb);
}

Color::Color(const Color &color)
{
    set(color.rgb);
}

Color Color::operator-(const Color &other) const
{
    return Color(this->rgb - other.rgb);
}

Color Color::operator+(const Color &other) const
{
    return Color(this->rgb + other.rgb);
}

Color Color::operator+=(const Color &other)
{
    set(this->rgb + other.rgb);
    return *this;
}

Color Color::operator*(const Color &other) const
{
    return Color(this->rgb * other.rgb);
}

Color Color::operator*(const float scalar)
{
    return Color(scalar * this->rgb);
}

Color Color::operator*=(const Color &other)
{
    set(this->rgb * other.rgb);
    return *this;
}

Color Color::operator*=(float scalar)
{
    set(scalar * this->rgb);
    return *this;
}

Color Color::operator/(float scalar) const
{
    return Color(this->rgb / scalar);
}

Color Color::operator/=(float scalar)
{
    set(this->rgb / scalar);
    return *this;
}

float Color::distance(const Color &other)
{
    float deltaR = std::abs(this->rgb(0) - other.rgb(0));
    float deltaG = std::abs(this->rgb(1) - other.rgb(1));
    float deltaB = std::abs(this->rgb(2) - other.rgb(2));

    Vector euclideanDistance(deltaR, deltaG, deltaB);
//    if (euclideanDistance.norm() != 0.0)
//        std::cout << euclideanDistance.norm() << std::endl;

return euclideanDistance.norm();
}

bool Color::isSimilarTo(const Color &other, float tolerance)
{
    float deltaR = std::abs(this->rgb(0) - other.rgb(0));
    float deltaG = std::abs(this->rgb(1) - other.rgb(1));
    float deltaB = std::abs(this->rgb(2) - other.rgb(2));

    return ((deltaR < tolerance) &&
            (deltaG < tolerance) &&
            (deltaB < tolerance));
}

Color Color::red()
{
    return Color(1.0f, 0.0f, 0.0f);
}

Color Color::green()
{
    return Color(0.0f, 1.0f, 0.0f);
}

Color Color::blue()
{
    return Color(0.0f, 0.0f, 1.0f);
}

Color Color::white()
{
    return Color(1.0f, 1.0f, 1.0f);
}

Color Color::black()
{
    return Color();
}

RGB Color::get() const
{
    return this->rgb;
}

float Color::get(int channel) const
{
    if (channel < 0 || channel > 2)
        return 0;
    else
        return this->rgb(channel);
}

void Color::set(const RGB &values)
{
    rgb(0) = MathUtils::clamp(values(0), 0.0f, 1.0f);
    rgb(1) = MathUtils::clamp(values(1), 0.0f, 1.0f);
    rgb(2) = MathUtils::clamp(values(2), 0.0f, 1.0f);
}


