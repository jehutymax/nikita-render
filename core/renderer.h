//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_RENDERER_H
#define NIKITA_RENDER_RENDERER_H

#include "nikita.h"
#include "scene.h"
#include "camera.h"
#include "../util/cpuTimer.h"

namespace nikita
{
class Renderer
{
public:
    virtual void render(const Scene& scene) = 0;
//    virtual float incidentRadiance();
//
};


class SimpleRenderer : public Renderer
{
public:
    SimpleRenderer(CameraPtr camera);
    void render(const Scene& scene);
private:
    CameraPtr camera;

    bool runIntersectionTest(const Ray &r, const CameraSample &s, const ShapePtr shape);
};

}

#endif //NIKITA_RENDER_RENDERER_H
