//
// Created by Rafael Campos on 4/3/16.
//

#ifndef NIKITA_RENDER_SCENE_H
#define NIKITA_RENDER_SCENE_H

#include <vector>
#include "shapes/shape.h"
#include "shading/intersection.h"
#include "light/light.h"
#include "primitive.h"

namespace nikita
{
    class Scene
    {
    public:

        bool intersect(Ray &ray, IntersectionPtr &is) const;
        bool intersectP(Ray &ray, float maxDist) const;

        PrimPtr primitive;
        std::vector<LightPtr> lights;
        AmbientLightPtr ambientLight;
        Color backgroundColor;
    };

typedef std::shared_ptr<Scene> ScenePtr;

}

#endif //NIKITA_RENDER_SCENE_H
