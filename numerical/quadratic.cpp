#include "quadratic.h"

bool numerical::quadratic::solve(float a, float b, float c, float *t0, float *t1)
{
    float discriminant = (b * b) - (4.0f * a * c);

    if (discriminant < 0)
        return false;

    // calculate the roots
    float discrSqRt = sqrt(discriminant);
    float q = -0.5f * (b + (((b < 0) ? (-1.f) : (1.f)) * discrSqRt));
    *t0 = q / a;
    *t1 = c / q;
    if (*t0 > *t1)
        std::swap(*t0, *t1);

    return true;
}