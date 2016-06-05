//
// Created by Rafael Campos on 4/2/16.
//

#ifndef NIKITA_RENDER_QUADRATIC_H
#define NIKITA_RENDER_QUADRATIC_H

#include <cmath>
#include <utility> // for swap

namespace numerical
{
    class quadratic
    {
    public:
        static bool solve(float a, float b, float c, float *t0, float *t1);
    };
}

#endif //NIKITA_RENDER_QUADRATIC_H
