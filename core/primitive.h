//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_PRIMITIVE_H
#define NIKITA_RENDER_PRIMITIVE_H

#include "nikita.h"
#include "ray.h"
#include "intersection.h"
#include "shape.h"
#include "material.h"

namespace nikita
{
class Shape;
typedef std::shared_ptr<Shape> ShapePtr;

class GeoPrim : public std::enable_shared_from_this<GeoPrim>
{
public:
    GeoPrim() {};
    GeoPrim(ShapePtr shape, MaterialPtr material);
    virtual bool isIntersectable() const;
    virtual bool divide(std::vector<ShapePtr> &divided) const;
    virtual bool intersect(Ray &ray, IntersectionPtr isect);

    ShapePtr shape;
    MaterialPtr material;

private:
    std::shared_ptr<GeoPrim> getPtr();
};

typedef std::shared_ptr<GeoPrim> GeoPrimPtr;
}

#endif //NIKITA_RENDER_PRIMITIVE_H
