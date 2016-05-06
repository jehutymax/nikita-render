//
// Created by Rafael Campos on 4/3/16.
//

#include "scene.h"

using nikita::Scene;

bool Scene::intersect(Ray &ray, IntersectionPtr &is) const
{
    return primitive->intersect(ray, is);
}


