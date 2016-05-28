//
// Created by Rafael Campos on 4/19/16.
//

#include "surfaceShader.h"
#include "../renderer/renderer.h"

using nikita::SimpleShader;
using nikita::ReflectiveShader;


nikita::Color SimpleShader::Li(const ScenePtr scene, const IntersectionPtr ip) const
{
    Vector wo = -(ip->ray.direction);
    Color L = ip->prim->material->getAmbient()->rho(wo) * scene->ambientLight->getRadiance();

    for (int i = 0; i < scene->lights.size(); ++i) {
        Vector wi = scene->lights[i]->getDirection(ip->hitPoint);
        float dp = ip->normal.dot(wi);

        if (dp != 0.f) {
            bool inShadow;
            if (scene->lights[i]->castsShadow()) {
                // create shadow ray
                Ray shadowRay(ip->hitPoint, wi, 0.f);
                // check if current hit point is in shadow
                inShadow = scene->lights[i]->testVisibility(shadowRay, scene->primitive);
            }

            Color totalContribution;
            if (!inShadow) {
                if (ip->prim->material->getDiffuse())
                    totalContribution += ip->prim->material->getDiffuse()->f(wi, wo, ip->normal);
                if (ip->prim->material->getSpecular())
                    totalContribution += ip->prim->material->getSpecular()->f(wi, wo, ip->normal);

                Color lightRadiance = scene->lights[i]->getRadiance();
                L += totalContribution * lightRadiance * dp;
            }
        }
    }
    return L;
}

nikita::Color ReflectiveShader::Li(const ScenePtr scene, const IntersectionPtr ip) const
{
    // first, account for the direct illumination using the simple shader
    Color L = SimpleShader::Li(scene, ip);

    Vector wo = -(ip->ray.direction);
    Vector wi;
    Color fr;
    if (ip->prim->material->getReflective()) {
        fr = ip->prim->material->getReflective()->sample_f(wi, wo, ip->normal);
        Ray reflectedRay(ip->hitPoint, wi, 0.f);
        float dp = ip->normal.dot(wi);
        if (dp > 0.f) {
            Color reflection = ip->renderer->shootRay(scene, reflectedRay, ip->rayDepth + 1);
            L += fr * reflection * dp;
        }
    }
    return L;
}
