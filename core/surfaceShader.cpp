//
// Created by Rafael Campos on 4/19/16.
//

#include "surfaceShader.h"

using nikita::SimpleShader;

nikita::Color SimpleShader::Li(const ScenePtr scene, const IntersectionPtr ip, const Sample &sample) const
{
    Vector wo = -(ip->ray.direction);
    Color L = ip->prim->material->getAmbient()->rho(wo) * scene->ambientLight->getRadiance();

    for (int i = 0; i < scene->lights.size(); ++i) {
        Vector wi = scene->lights[i]->getDirection(ip->hitPoint);
        float dp = ip->normal.dot(wi);

        if (dp > 0.f) {
            Color totalContribution;
            if (ip->prim->material->getDiffuse())
                totalContribution += ip->prim->material->getDiffuse()->f(wi, wo, ip->normal);
            if (ip->prim->material->getSpecular())
                totalContribution += ip->prim->material->getSpecular()->f(wi, wo, ip->normal);
            Color lightRadiance = scene->lights[i]->getRadiance();
            L += totalContribution * lightRadiance * dp;
        }
    }

    return L;
}