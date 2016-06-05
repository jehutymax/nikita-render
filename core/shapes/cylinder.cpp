//
// Created by Rafael Campos on 5/31/16.
//

#include "cylinder.h"

using nikita::Cylinder;

Cylinder::Cylinder(const TransformPtr obj2World,
                   const TransformPtr world2Obj,
                   float radius,
                   float z0,
                   float z1)
    : radius(radius),
      zMin(std::min(z0, z1)),
      zMax(std::max(z0, z1)),
      phiMax(MathUtils::toRadians(360.0f)),
      Shape(obj2World, world2Obj)
{
}

Cylinder::Cylinder(const TransformPtr obj2World,
                   const TransformPtr world2Obj,
                   float radius,
                   float z0,
                   float z1,
                   float sweep)
    : radius(radius),
      zMin(std::min(z0, z1)),
      zMax(std::max(z0, z1)),
      phiMax(MathUtils::toRadians(MathUtils::clamp(sweep, 0.f, 360.f))),
      Shape(obj2World, world2Obj)
{ }

bool Cylinder::isIntersectable()
{
    return true;
}

nikita::BoundingBox Cylinder::objectBound() const
{
    Point a(-radius, -radius, zMin);
    Point b(radius, radius, zMax);
    return BoundingBox(a, b);
}

bool Cylinder::intersect(const Ray &r, float *t, IntersectionPtr ip) const
{
    nikita::Ray ray;
    //transform incoming ray to object-space
    ray = (*worldToObject)(r);

    bool hit = calculateIntersection(r, t, 0.f);

    if (hit) {
        // fill Intersection object
        ip->hit = true;
        ip->hitPoint = r(*t);
        ip->normal = (ray.origin + (*t) * ray.direction) / this->radius;
    }

    return hit;
}

bool Cylinder::intersectP(const Ray &ray, float *t) const
{
    return calculateIntersection(ray, t, 1e-2f);
}

bool Cylinder::calculateIntersection(const Ray &r, float *t, float kEpsilon) const
{
    nikita::Ray ray;
    //transform incoming ray to object-space
    ray = (*worldToObject)(r);
    float a = ray.direction(0) * ray.direction(0) + ray.direction(1) * ray.direction(1);
    float b = 2.0f * (ray.direction(0) * ray.origin(0) +
        ray.direction(1) * ray.origin(1));
    float c = (ray.origin(0) * ray.origin(0)) + (ray.origin(1) * ray.origin(1)) - (this->radius * this->radius);

    float x0 = 0, x1 = 0;
    if (!numerical::quadratic::solve(a, b, c, &x0, &x1))
        return false;

    if (x0 > ray.tMax || x1 < ray.tMin)
        return false;

    float hit = x0;
    if (x0 < ray.tMin)
    {
        hit = x1;
        if (hit > ray.tMax)
            return false;
    }

    Point phi_t = ray(hit);
    if (phi_t(0) == 0.0f && phi_t(1) == 0.0f)
        phi_t(0) = 1e-5f * this->radius; // avoid division by zero in the arc tangent of y/x

    float phi = atan2f(phi_t(1), phi_t(0));
    if (phi < 0.0f)
        phi += 2.0f * Pi;

    if ((phi_t(2) < zMin) || (phi_t(2) > zMax) || (phi > phiMax))
    {
        if (hit == x1)
            return false;

        if (x1 > r.tMax)
            return false;

        hit = x1;

        Point phi_t = ray(hit);
        if (phi_t(0) == 0.0f && phi_t(1) == 0.0f)
            phi_t(0) = 1e-5f * this->radius; // avoid division by zero in the arc tangent of y/x

        float phi = atan2f(phi_t(1), phi_t(0));
        if (phi < 0.0f)
            phi += 2.0f * Pi;

        if ((phi_t(2) < zMin) || (phi_t(2) > zMax) || (phi > phiMax))
            return false;
    }

    if (hit > kEpsilon) {
        *t = hit;
        return true;
    }
    else
        return false;
}