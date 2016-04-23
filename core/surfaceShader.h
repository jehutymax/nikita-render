//
// Created by Rafael Campos on 4/19/16.
//

#ifndef NIKITA_RENDER_SURFACESHADER_H
#define NIKITA_RENDER_SURFACESHADER_H

#include "nikita.h"
#include "color.h"
#include "scene.h"
#include "sampler.h"

namespace nikita
{
class SurfaceShader
{
public:
    virtual Color Li(const ScenePtr scene, const IntersectionPtr ip, const Sample &sample) const = 0;
};

class SimpleShader : public SurfaceShader
{
public:
    virtual Color Li(const ScenePtr scene, const IntersectionPtr ip, const Sample &sample) const;
};

typedef std::shared_ptr<SurfaceShader> ShaderPtr;

}

#endif //NIKITA_RENDER_SURFACESHADER_H
