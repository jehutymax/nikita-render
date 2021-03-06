//
// Created by Rafael Campos on 3/28/16.
//

#ifndef NIKITA_RENDER_SPHERE_H
#define NIKITA_RENDER_SPHERE_H

#include "shape.h"

namespace nikita
{
class Sphere : public Shape
{
public:
    Sphere(const TransformPtr obj2World, const TransformPtr world2Obj,
           float radius);
    Sphere(const TransformPtr obj2World, const TransformPtr world2Obj,
        float radius, float z0, float z1, float phi); // support for partial spheres.

    BoundingBox objectBound() const; // implemented, tested, in use.

    bool isIntersectable();
    bool intersect(const Ray &ray, float *t, IntersectionPtr ip) const;
    bool intersectP(const Ray &ray, float *t) const;

private:
    float radius;
    float phiMax;
    float zMin, zMax;
    float thetaMin, thetaMax; // derived information

    bool calculateIntersection(const Ray &ray, float *t, float k = 0) const;
};
}

#endif //NIKITA_RENDER_SPHERE_H
