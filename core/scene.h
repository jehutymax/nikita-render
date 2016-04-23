//
// Created by Rafael Campos on 4/3/16.
//

#ifndef NIKITA_RENDER_SCENE_H
#define NIKITA_RENDER_SCENE_H

#include <vector>
#include "shape.h"
#include "intersection.h"
#include "light.h"
#include "primitive.h"

namespace nikita
{
    class Scene
    {
    public:

        bool intersect(Ray &ray, IntersectionPtr &is) const;

        std::vector<GeoPrimPtr> objects;
        std::vector<LightPtr> lights;
        AmbientLightPtr ambientLight;
        Color backgroundColor;
    };

typedef std::shared_ptr<Scene> ScenePtr;

}

#endif //NIKITA_RENDER_SCENE_H
