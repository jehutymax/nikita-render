//
// Created by Rafael Campos on 3/28/16.
//

#ifndef NIKITA_RENDER_MATHUTILS_H
#define NIKITA_RENDER_MATHUTILS_H

#include "../core/nikita.h"

namespace nikita
{
class MathUtils
{
public:
    static constexpr float epsilon = 0.0001;
    inline static float clamp(float value, float low, float high);
    inline static int clamp(int value, int low, int high);

    inline static float toDegrees(float radians);
    inline static float toRadians(float degrees);

    inline static bool almostEqual(float ref, float value);

};
}

inline float nikita::MathUtils::clamp(float value, float low, float high)
{
    if (value <= low)
        return low;
    if (value >= high)
        return high;
    return value;
}
inline int nikita::MathUtils::clamp(int value, int low, int high)
{
    if (value <= low)
        return low;
    if (value >= high)
        return high;
    return value;
}
inline float nikita::MathUtils::toDegrees(float radians)
{
    return (180.0/Pi) * radians;
}
inline float nikita::MathUtils::toRadians(float degrees)
{
    return (Pi/180.0) * degrees;
}
inline bool nikita::MathUtils::almostEqual(float ref, float value)
{
    if (value >= (ref - epsilon) && value <= (ref + epsilon))
        return true;
    else return false;
}

#endif //NIKITA_RENDER_MATHUTILS_H
