//
// Created by Rafael Campos on 4/6/16.
//

#include "renderer.h"

using nikita::Renderer;
using nikita::SimpleRenderer;

// SimpleRenderer
SimpleRenderer::SimpleRenderer(CameraPtr camera)
    : camera(camera),
      sampler(std::make_shared<Sampler>(0, camera->film->resolutionX, 0, camera->film->resolutionY, /* spp */ 1)),
      shader(std::make_shared<SimpleShader>())
{ }

void SimpleRenderer::render(const ScenePtr scene)
{
    CpuTimer t("Shooting rays");
    std::vector<Color> result(camera->film->resolutionX * camera->film->resolutionY);

    // get samples
    Sample sample;
    while(sampler->next(sample))
    {
        CameraSample s(sample.imageX, sample.imageY);
//        int vectorIndex = sampler->getFlatImageCoordinate();
        int vectorIndex = sample.imageY * camera->film->resolutionX + sample.imageX;
        IntersectionPtr ip = std::make_shared<Intersection>();
        Ray ray;
        camera->generateRay(s, &ray);

        if (scene->intersect(ray, ip))
        {
            // shade the object that was hit
            // hit information is maintained in the Intersection object,
            // returned by the intersect method of the geometry.
            result[vectorIndex] = shader->Li(scene, ip, sample);
        }
        else
        {
            // deal with rays that didn't hit anything
            result[vectorIndex] = scene->backgroundColor;

        }
    }
    t.stop();
    camera->film->writeImage(result);
}