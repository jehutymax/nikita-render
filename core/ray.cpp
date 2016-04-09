//
// Created by Rafael Campos on 4/2/16.
//

#include "ray.h"

using nikita::Ray;

Ray::Ray()
    : origin(0, 0, 0),
      direction(0, 0, 0),
      tMin(0.0),
      tMax(Infinity),
      depth(0)
{}

Ray::Ray(const Point &o, const Vector &d, float tMin, float tMax = Infinity)
    : origin(o), direction(d), tMin(tMin),  tMax(tMax), depth(0)
{}

Point Ray::operator()(float t) const
{
    return (origin + direction * t);
}