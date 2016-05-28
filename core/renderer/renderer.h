//
// Created by Rafael Campos on 4/6/16.
//

#ifndef NIKITA_RENDER_RENDERER_H
#define NIKITA_RENDER_RENDERER_H

#include "../nikita.h"
#include "../scene.h"
#include "../camera.h"
#include "../../util/cpuTimer.h"
#include "../shading/surfaceShader.h"

#include <unordered_map>

namespace nikita
{

struct ImageCoord
{
    ImageCoord()
        : x(0.f), y(0.f)
    { }
    ImageCoord(float x, float y)
        : x(x), y(y)
    { }
    float x;
    float y;

    ImageCoord operator+(const ImageCoord &other)
    {
        return ImageCoord(this->x + other.x, this->y + other.y);
    }

    ImageCoord operator/(const float div)
    {
        return ImageCoord(this->x / div, this->y / div);
    }

    bool operator==(const ImageCoord &other) const {
        return this->x == other.x && this->y == other.y;
    }
};
}

namespace std
{
template<>
struct hash<nikita::ImageCoord>{
    size_t operator()(const nikita::ImageCoord &ic) const {
        return std::hash<float>()(ic.x) ^ std::hash<float>()(ic.y);
    }
};
}


namespace nikita {
class Renderer
{
public:
    virtual void render(const ScenePtr scene) = 0;
    virtual Color shootRay(const ScenePtr scene, Ray &ray, int depth = 0) = 0;

};


class SimpleRenderer : public Renderer
{
public:
    SimpleRenderer(CameraPtr camera);
    virtual void render(const ScenePtr scene);
    virtual Color shootRay(const ScenePtr scene, Ray &ray, int depth = 0);
private:
    CameraPtr camera;
    SamplerPtr sampler;
    ShaderPtr shader;
};

class SuperSamplerRenderer : public std::enable_shared_from_this<SuperSamplerRenderer>, public Renderer
{
public:
    SuperSamplerRenderer(CameraPtr camera);
    virtual void render(const ScenePtr scene);
    virtual Color shootRay(const ScenePtr scene, Ray &ray, int depth = 0);
private:
    CameraPtr camera;
    SamplerPtr sampler;
    ShaderPtr shader;
    ScenePtr scene;
    int maxDepth;
    float threshold;
    int maxBounces;

    // Pixel or subpixel:
    // a---b
    // |   |
    // d---c
    Color processSquare(ImageCoord a, ImageCoord b, ImageCoord c, ImageCoord d, int depth = 0);
    int processSquareNoShading(ImageCoord a, ImageCoord b, ImageCoord c, ImageCoord d, int depth = 0);
    Color processRay(CameraSample s);

    std::unordered_map<ImageCoord, Color> computedRays;
    bool pixelExists(const ImageCoord &c, Color &color);
    void savePixel(ImageCoord &c, Color &color);
};
}


#endif //NIKITA_RENDER_RENDERER_H
