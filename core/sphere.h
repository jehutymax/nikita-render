//
// Created by Rafael Campos on 3/28/16.
//

#ifndef NIKITA_RENDER_SPHERE_H
#define NIKITA_RENDER_SPHERE_H

#include <math.h>

#include "nikita.h"
#include "shape.h"
#include "BoundingBox.h"
#include "../util/mathUtils.h"

namespace nikita
{
class Sphere : public Shape
{
public:
    Sphere(const TransformPtr obj2World, const TransformPtr world2Obj,
           float radius);
    Sphere(const TransformPtr obj2World, const TransformPtr world2Obj,
        float radius, float z0, float z1, float phi);

    BoundingBox objectBound() const; // to come...

    bool isIntersectable();
    bool intersect(const Ray &ray, float *t) const;

private:
    float radius;
    float phiMax;
    float zMin, zMax;
    float thetaMin, thetaMax; // derived information
};
}

#endif //NIKITA_RENDER_SPHERE_H
