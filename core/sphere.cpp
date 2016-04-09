//
// Created by Rafael Campos on 3/28/16.
//

#include "sphere.h"
#include "../numerical/quadratic.h"

using nikita::Sphere;

Sphere::Sphere(const TransformPtr obj2World, const TransformPtr world2Obj, float radius)
    : radius(radius), zMin(-radius), zMax(radius), phiMax(MathUtils::toRadians(360.0f)), Shape(obj2World, world2Obj)
{
    thetaMin = acosf(MathUtils::clamp(zMin/radius, -1.0, 1.0));
    thetaMax = acosf(MathUtils::clamp(zMax/radius, -1.0, 1.0));
}

nikita::Sphere::Sphere(const TransformPtr obj2World,
                       const TransformPtr world2Obj,
                       float rad,
                       float z0,
                       float z1,
                       float phi) : Shape(obj2World, world2Obj)
{
    radius = rad;
    zMin = MathUtils::clamp(std::min(z0, z1), -radius, radius);
    zMax = MathUtils::clamp(std::max(z0, z1), -radius, radius);
    phiMax = MathUtils::toRadians(MathUtils::clamp(phi, 0.0, 360.0));
    thetaMin = acosf(MathUtils::clamp(zMin/radius, -1.0, 1.0));
    thetaMax = acosf(MathUtils::clamp(zMax/radius, -1.0, 1.0));
}

nikita::BoundingBox nikita::Sphere::objectBound() const
{
    Point a(-radius, -radius, zMin);
    Point b(radius, radius, zMax);
    return BoundingBox::BoundingBox(a, b);
}

bool Sphere::isIntersectable()
{
    return true;
}

bool Sphere::intersect(const Ray &r, float *t) const
{
    nikita::Ray ray;
    //transform incoming ray to object-space
    ray = (*worldToObject)(r);
    float a = ray.direction.squaredNorm();
    float b = 2.0f * (ray.direction(0) * ray.origin(0) +
                     ray.direction(1) * ray.origin(1) +
                     ray.direction(2) * ray.origin(2));
    float c = ray.origin.squaredNorm() - (this->radius * this->radius);

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
    //std::cout << x0 <<  ", " << x1 <<  std::endl;
    *t = hit;
    return true;

}