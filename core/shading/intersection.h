//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_INTERSECTION_H
#define NIKITA_RENDER_INTERSECTION_H

#include "../nikita.h"
#include "../ray.h"
#include "../primitive.h"

namespace nikita
{
class Intersection
{
public:

    Intersection();
    Intersection(const Intersection &i);

    bool hit;               // does this represent a hit?
    GeoPrimPtr prim;        // primitive containing the hit object
    Point hitPoint;         // hit point in world coordinates
    Point localHitPoint;    // hit point in object coordinates
    Normal normal;          // normal at hit point
    Ray ray;                // required for specular highlights
    int rayDepth;           // used for recursion
    Vector direction;       // necessary for area lights
    RendererPtr renderer;   // pointer to the renderer for reflective shading
};

typedef std::shared_ptr<Intersection> IntersectionPtr;

}

#endif //NIKITA_RENDER_INTERSECTION_H
