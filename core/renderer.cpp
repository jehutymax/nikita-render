//
// Created by Rafael Campos on 4/6/16.
//

#include "renderer.h"

using nikita::Renderer;
using nikita::SimpleRenderer;

// SimpleRenderer
SimpleRenderer::SimpleRenderer(CameraPtr camera)
    : camera(camera)
{ }

bool SimpleRenderer::runIntersectionTest(const Ray &r, const CameraSample &s, const ShapePtr shape)
{
    float t = 0;
    bool hit = shape->intersect(r, &t);
    if (hit)
        camera->film->addSample(s, 250);
    else
        camera->film->addSample(s, 0);
}

void SimpleRenderer::render(const Scene &scene)
{
    CpuTimer t("Shooting rays");
    // get samples
    for (int i = 0; i < camera->film->resolutionX; ++i)
    {
        for (int j = 0; j < camera->film->resolutionY; ++j)
        {
            CameraSample s;
            s.imageX = i;
            s.imageY = j;
            Ray ray;
            camera->generateRay(s, &ray);

            for (int k = 0; k < scene.objects.size(); k++)
            {
                float t = 0, epsilon = 0;
                Ray r = ray;
                if (scene.objects[k]->isIntersectable())
                {
                    runIntersectionTest(ray, s, scene.objects[k]);
                }
                else
                {
                    std::vector<ShapePtr> subshapes;
                    scene.objects[k]->divide(subshapes);

                    for (int l = 0; l < subshapes.size(); ++l) {
                        runIntersectionTest(ray, s, subshapes[l]);
                    }

                }
            }
        }
    }
    t.stop();
    camera->film->writeImage();
}