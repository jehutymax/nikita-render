//
// Created by Rafael Campos on 3/26/16.
//

#ifndef NIKITA_RENDER_SHAPE_H
#define NIKITA_RENDER_SHAPE_H

#include "nikita.h"
#include "transform.h"
#include "ray.h"
#include "intersection.h"
#include "accelerator/BoundingBox.h"

namespace nikita
{
class Shape
{
public:
    Shape(const TransformPtr objectToWorld, const TransformPtr worldToObject)
        : objectToWorld(objectToWorld), worldToObject(worldToObject) {}
    virtual ~Shape() {}

    virtual bool isIntersectable()
    {
        return false;
    }

    virtual bool divide(std::vector<std::shared_ptr<Shape>> &divided)
    {
        return false;
    }

    virtual bool fullyDivide(std::vector<std::shared_ptr<Shape>> &divided) {return false;}

    virtual bool intersect(const Ray &ray, float *t, IntersectionPtr ip) const
    {
        return false;
    }

    virtual BoundingBox objectBound() const = 0;

    virtual BoundingBox worldBound() const
    {
        return (*objectToWorld)(objectBound());
    }

    const TransformPtr objectToWorld, worldToObject;
};

typedef std::shared_ptr<Shape> ShapePtr;
}

#endif //NIKITA_RENDER_SHAPE_H
