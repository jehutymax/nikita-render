//
// Created by Rafael Campos on 6/4/16.
//

#include "textureMapping.h"
#include "../../util/mathUtils.h"

using nikita::Spherical;

void Spherical::getTexelCoords(const Point &hitPoint, const int resX, const int resY, int &row, int &col) const
{
    // transform the point to texture space

    // calculate theta and phi
    float theta = this->theta(hitPoint);
    float phi = this->phi(hitPoint);

    // perform the mapping from theta, phi to u, v
    float u = phi * (1.f/(2.f*Pi));
    float v = 1 - (theta * (1.f/Pi));

    // map u, v to texel coords
    col = int((resX - 1) * u);
    row = int((resY - 1) * v);
}

float Spherical::theta(const Point &p) const
{
    return acosf(MathUtils::clamp(p(1), -1.f, 1.f));
//    return acosf(p(1));
}

float Spherical::phi(const Point &p) const
{
    float phi = atan2f(p(0), p(2));
    return ((phi < 0.f) ? phi + 2.f*Pi : phi);
}