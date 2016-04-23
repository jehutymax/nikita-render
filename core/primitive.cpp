//
// Created by Rafael Campos on 4/19/16.
//

#include "primitive.h"

using nikita::GeoPrim;

GeoPrim::GeoPrim(ShapePtr shape, MaterialPtr material)
    : shape(shape), material(material)
{
}

bool GeoPrim::isIntersectable() const
{
    return shape->isIntersectable();
}

bool GeoPrim::divide(std::vector<ShapePtr> &divided) const
{
    return this->shape->divide(divided);
}

bool GeoPrim::intersect(Ray &ray, IntersectionPtr isect)
{
    float t = 0;
    bool hit = false;

    if (shape->isIntersectable())
        hit = shape->intersect(ray, &t, isect);
    else
    {
        std::vector<ShapePtr> subshapes;
        shape->divide(subshapes);
        for (int l = 0; l < subshapes.size(); ++l) {
            hit |= subshapes[l]->intersect(ray, &t, isect);
        }
    }
    if (hit) {
        ray.tMax = t;
        // finish completing the Intersection object.
        isect->prim = getPtr();
        isect->rayDepth = ray.depth;
        isect->ray = ray;
    }

    return hit;
}

std::shared_ptr<GeoPrim> GeoPrim::getPtr()
{
    return shared_from_this();
}