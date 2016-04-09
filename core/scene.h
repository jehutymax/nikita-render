//
// Created by Rafael Campos on 4/3/16.
//

#ifndef NIKITA_RENDER_SCENE_H
#define NIKITA_RENDER_SCENE_H

#include <vector>
#include "shape.h"
namespace nikita
{
    class Scene
    {
    public:
        std::vector<ShapePtr> objects;
    };

typedef std::shared_ptr<Scene> ScenePtr;

}

#endif //NIKITA_RENDER_SCENE_H
