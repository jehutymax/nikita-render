//
// Created by Rafael Campos on 4/6/16.
//

//#define RENDER_RAY_SAMPLE

#include "renderer.h"

using nikita::Renderer;
using nikita::SimpleRenderer;
using nikita::SuperSamplerRenderer;

float counter = 0;

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
        int vectorIndex = int(sample.imageY * camera->film->resolutionX + sample.imageX);
        Ray ray;
        camera->generateRay(s, &ray);
        result[vectorIndex] = shootRay(scene, ray);


    }
    t.stop();
    camera->film->writeImage(result);
//    camera->film->writeAveragedImage(result);
}

nikita::Color SimpleRenderer::shootRay(const ScenePtr scene, Ray &ray, int)
{
    IntersectionPtr ip = std::make_shared<Intersection>();
    if (scene->intersect(ray, ip))
    {
        // shade the object that was hit
        // hit information is maintained in the Intersection object,
        // returned by the intersect method of the geometry.
        return shader->Li(scene, ip);
    }
    else
    {
        // deal with rays that didn't hit anything
        return scene->backgroundColor;
    }
}

// SuperSamplerRenderer
SuperSamplerRenderer::SuperSamplerRenderer(CameraPtr camera)
    : camera(camera),
      sampler(std::make_shared<Sampler>(0, camera->film->resolutionX, 0, camera->film->resolutionY, /* spp */ 1)),
      shader(std::make_shared<TransparentShader>()),
      maxDepth(3), threshold(0.001f), maxBounces(5)
{ }

void SuperSamplerRenderer::render(const ScenePtr scene)
{
    CpuTimer t("Shooting rays with super-sampling");
    std::vector<Color> result(camera->film->resolutionX * camera->film->resolutionY);
    this->scene = scene;

    // get samples
    Sample sample;

    // Commented-out sections below refer to the rendering of the ray heatmap for adaptive supersampling.
    int min = 999;
    int max = 0;
    int count = 0;
    int acc = 0;
    std::vector<int> cc(20);
    while(sampler->next(sample))
    {
        int vectorIndex = sample.imageY * camera->film->resolutionX + sample.imageX;

#ifndef RENDER_RAY_SAMPLE
        result[vectorIndex] = processSquare(
            ImageCoord(sample.imageX-0.5, sample.imageY+0.5),
            ImageCoord(sample.imageX+0.5, sample.imageY+0.5),
            ImageCoord(sample.imageX+0.5, sample.imageY-0.5),
            ImageCoord(sample.imageX-0.5, sample.imageY-0.5)
        );
#else
        int sampleCount = processSquareNoShading(
            ImageCoord(sample.imageX-0.5, sample.imageY+0.5),
            ImageCoord(sample.imageX+0.5, sample.imageY+0.5),
            ImageCoord(sample.imageX+0.5, sample.imageY-0.5),
            ImageCoord(sample.imageX-0.5, sample.imageY-0.5)
        );
        if (sampleCount == 1)
            result[vectorIndex] = Color(0.2f, 0.2f, 0.2f);
        else {
            float c = sampleCount / 17.f;
            result[vectorIndex] = Color(c, c, c);
        }
        min = std::min(min, sampleCount);
        max = std::max(max, sampleCount);
        count++;
        acc += sampleCount;
        cc[sampleCount]++;
    }
    std::cout << "min: " << min << ", max: " << max << std::endl;
    std::cout << "avg: " << (float)acc/count << ", count: " << count << std::endl;
    for (int i = 0; i < cc.size(); ++i) {
        std::cout << i << ": " << cc[i] << std::endl;
#endif

    }
    t.stop();
    std::cout << "Number of primary rays: " << counter << std::endl;
    camera->film->writeImage(result);
}

nikita::Color SuperSamplerRenderer::processSquare(ImageCoord a, ImageCoord b, ImageCoord c, ImageCoord d, int depth)
{
    Color color_a;
    if (!pixelExists(a, color_a)) {
        CameraSample cs_a(a.x, a.y);
        color_a = processRay(cs_a);
        savePixel(a, color_a);
    }

    Color color_b;
    if (!pixelExists(b, color_b)) {
        CameraSample cs_b(b.x, b.y);
        color_b = processRay(cs_b);
        savePixel(b, color_b);
    }

    Color color_c;
    if (!pixelExists(c, color_c)) {
        CameraSample cs_c(c.x, c.y);
        color_c = processRay(cs_c);
        savePixel(c, color_c);
    }

    Color color_d;
    if (!pixelExists(d, color_d)) {
        CameraSample cs_d(d.x, d.y);
        color_d = processRay(cs_d);
        savePixel(d, color_d);
    }

    // decide if colors are within tolerance or if we're done with the recursion
    bool stop = color_a.isSimilarTo(color_b, threshold);
    stop &= color_a.isSimilarTo(color_d, threshold);
    stop &= color_c.isSimilarTo(color_b, threshold);
    stop &= color_c.isSimilarTo(color_d, threshold);

    if (
            ((depth + 1) == maxDepth) || stop

        ) {
        // if they are
        return (color_a * 0.25 + color_b * 0.25 + color_c * 0.25 + color_d * 0.25);
    }
    else {
        // and if they're not
        ImageCoord center((a.x + b.x) / 2, (a.y + d.y) / 2);
        float newDepth = depth + 1;
        return (0.25 * processSquare(a, (a + b) / 2, center, (a + d) / 2, newDepth) +
            0.25 * processSquare((a + b) / 2, b, (b + c) / 2, center, newDepth) +
            0.25 * processSquare(center, (b + c) / 2, c, (c + d) / 2, newDepth) +
            0.25 * processSquare((a + d) / 2, center, (c + d) / 2, d, newDepth));
    }
}

int SuperSamplerRenderer::processSquareNoShading(ImageCoord a, ImageCoord b, ImageCoord c, ImageCoord d, int depth)
{
    Color color_a;
    if (!pixelExists(a, color_a)) {
        CameraSample cs_a(a.x, a.y);
        color_a = processRay(cs_a);
        savePixel(a, color_a);
    }

    Color color_b;
    if (!pixelExists(b, color_b)) {
        CameraSample cs_b(b.x, b.y);
        color_b = processRay(cs_b);
        savePixel(b, color_b);
    }

    Color color_c;
    if (!pixelExists(c, color_c)) {
        CameraSample cs_c(c.x, c.y);
        color_c = processRay(cs_c);
        savePixel(c, color_c);
    }

    Color color_d;
    if (!pixelExists(d, color_d)) {
        CameraSample cs_d(d.x, d.y);
        color_d = processRay(cs_d);
        savePixel(d, color_d);
    }

    bool stop = color_a.isSimilarTo(color_b, threshold);
    stop &= color_a.isSimilarTo(color_d, threshold);
    stop &= color_c.isSimilarTo(color_b, threshold);
    stop &= color_c.isSimilarTo(color_d, threshold);

    // decide if colors are within tolerance or if we're done with the recursion
    if (
        ((depth + 1) == maxDepth) || stop

        ) {
        // if they are
        return 1;
    }
    else {
        // and if they're not
        ImageCoord center((a.x + b.x) / 2.f, (a.y + d.y) / 2.f);
        float newDepth = depth + 1;
        return (1 +
            processSquareNoShading(a, (a + b) / 2.f, center, (a + d) / 2.f, newDepth) +
            processSquareNoShading((a + b) / 2.f, b, (b + c) / 2.f, center, newDepth) +
            processSquareNoShading(center, (b + c) / 2.f, c, (c + d) / 2.f, newDepth) +
            processSquareNoShading((a + d) / 2.f, center, (c + d) / 2.f, d, newDepth));
    }
}

nikita::Color SuperSamplerRenderer::processRay(CameraSample s)
{
    Color result;
    Ray ray;
    camera->generateRay(s, &ray);
    result = shootRay(scene, ray);
    counter++;
    return result;
}

bool SuperSamplerRenderer::pixelExists(const ImageCoord &c, Color &color)
{
    auto search = computedRays.find(c);
    if (search != computedRays.end()) {
        color = search->second;
        return true;
    }
    else return false;
}

void SuperSamplerRenderer::savePixel(ImageCoord &c, Color &color)
{
    computedRays.insert(std::make_pair(c, color));
}

nikita::Color SuperSamplerRenderer::shootRay(const ScenePtr scene, Ray &ray, int depth)
{
    if (depth > maxBounces)
        return Color::black();

    IntersectionPtr ip = std::make_shared<Intersection>();
    ip->renderer = shared_from_this();
    ip->rayDepth = depth;

    if (scene->intersect(ray, ip))
        return shader->Li(scene, ip);
    else
        return scene->backgroundColor;
}