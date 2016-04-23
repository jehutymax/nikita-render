//
// Created by Rafael Campos on 4/19/16.
//

#include "intersection.h"

nikita::Intersection::Intersection()
    : hit(false),
      prim(nullptr),
      rayDepth(0)
{ }

nikita::Intersection::Intersection(const Intersection &i)
    : hit(i.hit),
      prim(i.prim),
      hitPoint(i.hitPoint),
      normal(i.normal),
      ray(i.ray),
      rayDepth(i.rayDepth),
      direction(i.direction)
{ }