//
// Created by Rafael Campos on 4/16/16.
//

#ifndef NIKITA_RENDER_COLOR_H
#define NIKITA_RENDER_COLOR_H

#include "nikita.h"

namespace nikita
{
class Color
{
public:
    Color();
    Color(float red, float green, float blue);
    Color(const float rgb[3]);
    Color(const RGB &rgb);
    Color(const Color &color);

    Color operator-(const Color &other) const;
    Color operator+(const Color &other) const;
    Color operator+=(const Color &other);
    Color operator* (const Color &other) const;
    Color operator*= (const Color &other);
    Color operator* (const float scalar);
    inline friend Color operator*(const float scalar, const Color &c);
    Color operator*= (float scalar);
    Color operator/ (float scalar) const;
    Color operator/= (float scalar);

    float distance(const Color &other);
    bool isSimilarTo(const Color &other, float tolerance);

    RGB get() const;
    float get(int) const;

    static Color pow(const Color &base, float power);
    static Color red();
    static Color green();
    static Color blue();
    static Color white();
    static Color black();

private:
    RGB rgb;
    void set(const RGB &values);
};

Color operator*(float scalar, const Color &c)
{
    return Color(scalar * c.rgb);
}

}

#endif //NIKITA_RENDER_COLOR_H
