//
// Created by Rafael Campos on 4/2/16.
//

#ifndef NIKITA_RENDER_RAY_H
#define NIKITA_RENDER_RAY_H

#include "nikita.h"

namespace nikita
{
class Ray
{
public:
    Ray();
    Ray(const Point& origin, const Vector& direction, float tMin, float tMax);

    Point operator()(float t) const;

    // key members
    Point origin;
    Vector direction;
    int depth;
    float tMin, tMax;
};
}

#endif //NIKITA_RENDER_RAY_H
