#include "MathUtil.h"

float MCB::MathUtil::Clamp(float var, float min, float max)
{
    if (var < min)
    {
        return min;
    }

    if (var > max)
    {
        return max;
    }
    return var;
}
