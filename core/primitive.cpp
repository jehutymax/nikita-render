//
// Created by Rafael Campos on 4/19/16.
//

#include "primitive.h"

using nikita::Primitive;
using nikita::GeoPrim;
using nikita::NonAcceleratedCollection;



GeoPrim::GeoPrim(ShapePtr shape, MaterialPtr material)
    : shape(shape), material(material)
{
}

bool GeoPrim::isIntersectable() const
{
    return shape->isIntersectable();
}

bool GeoPrim::fullyDivide(std::vector<PrimPtr> &divided) const
{
    std::vector<PrimPtr> stack;
    PrimPtr pp = std::const_pointer_cast<GeoPrim>(getPtr());
    stack.push_back(pp);

    while(stack.size())
    {
        PrimPtr prim = stack.back();
        stack.pop_back();
        if (prim->isIntersectable())
            divided.push_back(prim);
        else
            prim->divide(divided);
    }

    return true;
}

bool GeoPrim::divide(std::vector<PrimPtr> &divided) const
{
    std::vector<ShapePtr> dividedShapes;
    this->shape->divide(dividedShapes);

    if (dividedShapes.empty())
        return false;

    for (int i = 0; i < dividedShapes.size(); ++i) {
        divided.push_back(std::make_shared<GeoPrim>(dividedShapes[i], this->material));
    }
    return true;
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
        isect->ray = ray;
    }

    return hit;
}

bool GeoPrim::intersectP(Ray &ray, float maxDist)
{
    float t = 0;
    bool hit = false;
    if (shape->isIntersectable())
        hit = shape->intersectP(ray, &t);
    else
    {
        std::vector<ShapePtr> subshapes;
        shape->divide(subshapes);
        for (int l = 0; l < subshapes.size(); ++l) {
            hit = subshapes[l]->intersectP(ray, &t);
            if (hit && t < maxDist)
                break;
        }
    }

    if (hit)
        ray.tMax = t;

    return hit;
}

std::shared_ptr<GeoPrim> GeoPrim::getPtr()
{
    return shared_from_this();
}

std::shared_ptr<const GeoPrim> GeoPrim::getPtr() const
{
    return shared_from_this();
}

nikita::BoundingBox GeoPrim::objectBound() const
{
    return this->shape->objectBound();
}

nikita::BoundingBox GeoPrim::worldBound() const
{
    return this->shape->worldBound();
}

////////// NonAcceleratedCollection
NonAcceleratedCollection::NonAcceleratedCollection(const std::vector<GeoPrimPtr> &geo)
    : objects(geo)
{
}

bool NonAcceleratedCollection::isIntersectable() const
{
    return false; // not to be called
}

bool NonAcceleratedCollection::divide(std::vector<PrimPtr> &divided) const
{
    return fullyDivide(divided);
}

bool NonAcceleratedCollection::fullyDivide(std::vector<PrimPtr> &divided) const
{
    return false; // shouldn't be called
}

bool NonAcceleratedCollection::intersect(Ray &ray, IntersectionPtr isect)
{
    bool hit = false;
    for (int k = 0; k < objects.size(); k++)
        hit |=  objects[k]->intersect(ray, isect);

    return hit;
}

bool NonAcceleratedCollection::intersectP(Ray &ray, float maxDist)
{
    bool hit = false;
    for (int k = 0; k < objects.size(); k++) {
        hit = objects[k]->intersectP(ray, maxDist);
        if (hit)
            break;
    }

    return hit;
}