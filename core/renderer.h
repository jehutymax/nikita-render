//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_RENDERER_H
#define NIKITA_RENDER_RENDERER_H

#include "nikita.h"
#include "scene.h"
#include "camera.h"
#include "../util/cpuTimer.h"
#include "surfaceShader.h"

namespace nikita
{
class Renderer
{
public:
    virtual void render(const ScenePtr scene) = 0;
//    virtual float incidentRadiance();
//
};


class SimpleRenderer : public Renderer
{
public:
    SimpleRenderer(CameraPtr camera);
    virtual void render(const ScenePtr scene);
private:
    CameraPtr camera;
    SamplerPtr sampler;
    ShaderPtr shader;
};

}

#endif //NIKITA_RENDER_RENDERER_H
