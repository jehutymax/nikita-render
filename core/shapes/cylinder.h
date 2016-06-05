//
// Created by Rafael Campos on 5/31/16.
//

#ifndef NIKITA_RENDER_CYLINDER_H
#define NIKITA_RENDER_CYLINDER_H

#include "shape.h"

namespace nikita
{
class Cylinder : public Shape
{
public:
    Cylinder(const TransformPtr obj2World, const TransformPtr world2Obj,
             float radius, float z0, float z1);
    Cylinder(const TransformPtr obj2World, const TransformPtr world2Obj,
             float radius, float z0, float z1, float sweep); // support for 'pie-like' cylinders

    BoundingBox objectBound() const; // implemented, tested, in use.

    bool isIntersectable();
    bool intersect(const Ray &ray, float *t, IntersectionPtr ip) const;
    bool intersectP(const Ray &ray, float *t) const;
private:
    float radius;
    float zMin;
    float zMax;
    float phiMax; // cylinder's sweep, from 0 to 2pi radians


    bool calculateIntersection(const Ray &r, float *t, float k) const;
};

typedef std::shared_ptr<Cylinder> CylinderPtr;

}

#endif //NIKITA_RENDER_CYLINDER_H
