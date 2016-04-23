//
// Created by Rafael Campos on 4/3/16.
//

#include "scene.h"

using nikita::Scene;

bool Scene::intersect(Ray &ray, IntersectionPtr &is) const
{
    bool hit = false;
    for (int k = 0; k < objects.size(); k++)
        hit |=  objects[k]->intersect(ray, is);

    return hit;
}


