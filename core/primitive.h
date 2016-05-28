//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_PRIMITIVE_H
#define NIKITA_RENDER_PRIMITIVE_H

#include "nikita.h"
#include "ray.h"
#include "shading/intersection.h"
#include "shapes/shape.h"
#include "material.h"

namespace nikita
{
class Primitive
{
public:
    Primitive (){};
    virtual ~Primitive(){};
    virtual bool isIntersectable() const = 0;
    virtual bool divide(std::vector<std::shared_ptr<Primitive>> &divided) const = 0;
    virtual bool fullyDivide(std::vector<std::shared_ptr<Primitive>> &divided) const {return false;};
    virtual bool intersect(Ray &ray, IntersectionPtr isect) = 0;
    virtual bool intersectP(Ray &ray, float maxDist) = 0;

    virtual BoundingBox objectBound() const {return BoundingBox(); };
    virtual BoundingBox worldBound() const {return BoundingBox(); };

};

typedef std::shared_ptr<Primitive> PrimPtr;

class GeoPrim : public std::enable_shared_from_this<GeoPrim>, public Primitive
{
public:
    GeoPrim() {};
    GeoPrim(ShapePtr shape, MaterialPtr material);
    virtual bool isIntersectable() const;
    virtual bool divide(std::vector<PrimPtr> &divided) const;
    virtual bool fullyDivide(std::vector<PrimPtr> &divided) const;
    virtual bool intersect(Ray &ray, IntersectionPtr isect);
    virtual bool intersectP(Ray &ray, float maxDist);

    virtual BoundingBox objectBound() const;
    virtual BoundingBox worldBound() const;

    ShapePtr shape;
    MaterialPtr material;

private:
    std::shared_ptr<GeoPrim> getPtr();
    std::shared_ptr<const GeoPrim> getPtr() const;
};

typedef std::shared_ptr<GeoPrim> GeoPrimPtr;

class NonAcceleratedCollection : public Primitive
{
public:
    NonAcceleratedCollection() {};
    NonAcceleratedCollection(const std::vector<GeoPrimPtr> &geo);
    virtual bool isIntersectable() const;
    virtual bool divide(std::vector<PrimPtr> &divided) const;
    virtual bool fullyDivide(std::vector<PrimPtr> &divided) const;
    virtual bool intersect(Ray &ray, IntersectionPtr isect);
    virtual bool intersectP(Ray &ray, float maxDist);

    std::vector<GeoPrimPtr> objects;
};
}

#endif //NIKITA_RENDER_PRIMITIVE_H
